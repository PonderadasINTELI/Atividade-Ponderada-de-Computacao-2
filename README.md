# Atividade Ponderada de Computação 2 — Semáforo com dois modos

Semáforo no ESP32 que entra em modo noturno (amarelo piscando) quando o LDR detecta pouca luz, com histerese para não ficar alternando perto do limiar.

Enunciado: https://canaldoovidio.github.io/2026-2B-M04/ponderadas/semaforo-inteligente/index.html

## Circuito

| Componente | Ligação |
|---|---|
| LDR + resistor fixo (divisor) | 3V3 → LDR → nó → R fixo → GND (ou invertido); nó no **GPIO34** (ADC1) |
| LED vermelho | GPIO25 → resistor → LED → GND |
| LED amarelo | GPIO26 → resistor → LED → GND |
| LED verde | GPIO27 → resistor → LED → GND |

Use 3V3, não 5 V. Escolha o resistor fixo perto da resistência do LDR na luz ambiente.

## Passo a passo

1. Grave `calibracao_ldr/calibracao_ldr.ino`, abra o Monitor Serial a 115200 baud e digite `a`, `s` e `l` (ambiente, sombra, lanterna).
2. Cole a saída na página da atividade (Confrontar → Importar leituras) e baixe `grafico-calibracao.png`.
3. Em `semaforo/semaforo.ino`, preencha `LEITURA_SOBE_COM_LUZ`, `LIMIAR_ENTRADA` e `LIMIAR_SAIDA` com base na tabela.
4. Grave `semaforo/semaforo.ino` e grave o vídeo passando a luz devagar em torno do limiar.

## Entregas

- [ ] `semaforo/semaforo.ino` com os limiares preenchidos
- [ ] `RELATORIO.md` com a tabela de calibração
- [ ] `grafico-calibracao.png`
- [ ] Vídeo da transição no limiar (link no relatório)
- [ ] `ADR.md` de cada integrante
