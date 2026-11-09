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

#ifdef SIMULACAO_WOKWI
// ---------- Só para o simulador Wokwi (NÃO são medições, não vão para o relatório) ----------
// O módulo LDR do Wokwi tem o LDR do lado do GND: a leitura DESCE com a luz.
// Pela fórmula do Wokwi: ~50 lux dá leitura ~2500 e ~100 lux dá ~2050.
const bool LEITURA_SOBE_COM_LUZ = false;
const int LIMIAR_ENTRADA = 2500; // abaixo de ~50 lux -> modo noturno
const int LIMIAR_SAIDA = 2000;   // acima de ~100 lux -> modo diurno
#else
// ---------- Calibração (bancada, LDR + 47 kΩ, calibracao_ldr/saida-monitor-serial.txt) ----------
// sombra 0–31 (média 16), ambiente 1129–1338 (média 1228), lanterna 4004–4027 (média 4015).
// A lanterna dá leitura MAIOR que a sombra: LDR do lado do 3V3.
const bool LEITURA_SOBE_COM_LUZ = true;
// Entrada bem acima do máximo da sombra; saída abaixo do mínimo do ambiente.
// Folga de 400, ~6x o desvio do ambiente (67,7), cobre o ruído e a subida lenta do LDR.
const int LIMIAR_ENTRADA = 400; // leitura cai abaixo dele -> entra no modo noturno
const int LIMIAR_SAIDA = 800;   // leitura sobe acima dele -> sai do modo noturno
#endif

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
