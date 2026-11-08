// Atividade ponderada de Computação 2 - Semáforo com dois modos de operação: calibração do LDR.
// Imprime a leitura bruta do conversor (0 a 4095) com o rótulo da condição.
// No Monitor Serial, digite a (ambiente), s (sombra) ou l (lanterna) e Enter.

const int PINO_LDR = 34; // ADC1: continua lendo com o Wi-Fi ligado
const unsigned long INTERVALO_MS = 200; // uma leitura a cada 200 ms
const int LEITURAS_POR_CONDICAO = 30; // 30 leituras = 6 s por condição

const char *condicao = "ambiente";
int contagem = LEITURAS_POR_CONDICAO; // começa parado, esperando a primeira letra

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // escala de 0 a 4095
  Serial.println("# digite a, s ou l e Enter para medir uma condicao");
  Serial.println("condicao,leitura");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'a') { condicao = "ambiente"; contagem = 0; }
    if (c == 's') { condicao = "sombra"; contagem = 0; }
    if (c == 'l') { condicao = "lanterna"; contagem = 0; }
  }
  if (contagem < LEITURAS_POR_CONDICAO) {
    int leitura = analogRead(PINO_LDR);
    Serial.print(condicao);
    Serial.print(",");
    Serial.println(leitura);
    contagem++;
    if (contagem == LEITURAS_POR_CONDICAO) Serial.println("# fim da condicao");
  }
  delay(INTERVALO_MS);
}
