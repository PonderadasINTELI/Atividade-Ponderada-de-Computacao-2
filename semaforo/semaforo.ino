// Atividade ponderada de Computação 2 - Semáforo com dois modos de operação.
// Modo diurno: ciclo vermelho -> verde -> amarelo com o plano do RA A2026.1A.0366 (final 6).
// Modo noturno: amarelo piscando, ativado pelo LDR com histerese (dois limiares).

// ---------- Pinos ----------
const int PINO_LDR = 34;       // ADC1 (GPIO34): continua lendo com Wi-Fi ligado
const int PINO_VERMELHO = 25;
const int PINO_AMARELO = 26;
const int PINO_VERDE = 27;

// ---------- Plano do RA (último dígito 6) ----------
const unsigned long T_VERMELHO = 11000; // 5 s + 6
const unsigned long T_VERDE = 6000;     // 3 s + floor(6 / 2)
const unsigned long T_AMARELO = 2000;   // 2 s para todos

// ---------- Calibração: PREENCHER com as medições da bancada ----------
// LEITURA_SOBE_COM_LUZ: true se a lanterna dá leitura MAIOR que a sombra (LDR do lado do 3V3).
const bool LEITURA_SOBE_COM_LUZ = true;
// Os dois limiares ficam entre a faixa de sombra e a de ambiente, com folga maior que o ruído medido.
// LIMIAR_ENTRADA fica mais perto da sombra; LIMIAR_SAIDA mais perto do ambiente.
// Ex. (leitura sobe com a luz): sombra ~800, ambiente ~2000 -> ENTRADA 1100, SAIDA 1500.
const int LIMIAR_ENTRADA = 0; // a luz cai além dele -> entra no modo noturno
const int LIMIAR_SAIDA = 0;   // a luz volta além dele -> sai do modo noturno

const unsigned long INTERVALO_LDR = 200;   // lê o LDR a cada 200 ms
const unsigned long MEIO_PERIODO_PISCA = 500; // amarelo noturno pisca a 1 Hz

enum Fase { VERMELHO, VERDE, AMARELO };
Fase fase = VERMELHO;
bool noturno = false;
unsigned long inicioFase = 0;
unsigned long ultimaLeitura = 0;
unsigned long trocasDeModo = 0;

// true se a leitura l indica MENOS luz que o limiar lim, respeitando o sentido do divisor.
bool maisEscuroQue(int l, int lim) {
  return LEITURA_SOBE_COM_LUZ ? l < lim : l > lim;
}

void acender(bool vermelho, bool amarelo, bool verde) {
  digitalWrite(PINO_VERMELHO, vermelho);
  digitalWrite(PINO_AMARELO, amarelo);
  digitalWrite(PINO_VERDE, verde);
}

void entrarFase(Fase f, unsigned long agora) {
  fase = f;
  inicioFase = agora;
  acender(f == VERMELHO, f == AMARELO, f == VERDE);
}

// Histerese: entre os dois limiares nada muda, então o ruído perto de um limiar não troca o modo.
void atualizarModo(unsigned long agora) {
  int leitura = analogRead(PINO_LDR);
  bool antes = noturno;
  if (!noturno && maisEscuroQue(leitura, LIMIAR_ENTRADA)) noturno = true;
  else if (noturno && maisEscuroQue(LIMIAR_SAIDA, leitura)) noturno = false; // leitura mais clara que a saída

  Serial.print(leitura);
  Serial.print(",");
  Serial.println(noturno ? "noturno" : "diurno");

  if (noturno != antes) {
    trocasDeModo++;
    Serial.print("# troca de modo #");
    Serial.print(trocasDeModo);
    Serial.println(noturno ? " -> noturno" : " -> diurno");
    if (!noturno) entrarFase(VERMELHO, agora); // volta ao diurno sempre pelo vermelho
  }
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // 0 a 4095
  pinMode(PINO_VERMELHO, OUTPUT);
  pinMode(PINO_AMARELO, OUTPUT);
  pinMode(PINO_VERDE, OUTPUT);
  if (LIMIAR_ENTRADA == 0 && LIMIAR_SAIDA == 0)
    Serial.println("# AVISO: limiares nao calibrados, preencha LIMIAR_ENTRADA e LIMIAR_SAIDA");
  Serial.println("leitura,modo");
  entrarFase(VERMELHO, millis());
}

void loop() {
  unsigned long agora = millis();

  if (agora - ultimaLeitura >= INTERVALO_LDR) {
    ultimaLeitura = agora;
    atualizarModo(agora);
  }

  if (noturno) {
    acender(false, (agora / MEIO_PERIODO_PISCA) % 2 == 0, false);
    return;
  }

  unsigned long decorrido = agora - inicioFase;
  if (fase == VERMELHO && decorrido >= T_VERMELHO) entrarFase(VERDE, agora);
  else if (fase == VERDE && decorrido >= T_VERDE) entrarFase(AMARELO, agora);
  else if (fase == AMARELO && decorrido >= T_AMARELO) entrarFase(VERMELHO, agora);
}
