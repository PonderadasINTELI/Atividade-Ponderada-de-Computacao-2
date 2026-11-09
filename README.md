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

Posição de cada peça na protoboard, furo a furo: [MONTAGEM.md](MONTAGEM.md).

## Gravar pelo VS Code

Usa o `arduino-cli` (já instalado com o core ESP32). `Ctrl+Shift+P` → **Tasks: Run Task**:

- **ESP32: listar portas** — rode com e sem o ESP32 no USB; a porta que aparece a mais é a dele.
- **ESP32: compilar** (`Ctrl+Shift+B`) — escolha o sketch.
- **ESP32: gravar** — escolha o sketch e digite a porta (ex.: `COM5`).
- **ESP32: monitor serial** — 115200 baud; digite `a`, `s` ou `l` + Enter no terminal. Feche (`Ctrl+C`) antes de gravar de novo.

## Simulação (Wokwi)

O circuito digital está em `diagram.json` (ESP32 DevKitC, módulo LDR, 3 LEDs com 220 Ω).

**No VS Code** (extensão Wokwi, precisa de licença gratuita, que você ativa uma vez pelo comando `Wokwi: Request a New License`):
1. Rode a tarefa **Wokwi: compilar para simulação**.
2. `Ctrl+Shift+P` → **Wokwi: Start Simulator**.
3. Clique no LDR e mova o controle de lux. Abaixo de ~50 lux entra o modo noturno; acima de ~100 lux, volta o diurno.

**No navegador:** em [wokwi.com/projects/new/esp32](https://wokwi.com/projects/new/esp32), cole o `semaforo.ino` no `sketch.ino` com `#define SIMULACAO_WOKWI` na primeira linha, e o `diagram.json` na aba de mesmo nome.

Na simulação, os limiares vêm de `SIMULACAO_WOKWI` no código, tirados da fórmula do Wokwi. **Não servem para a tabela de calibração**, que precisa vir do LDR real.

## Passo a passo

1. Grave `calibracao_ldr/calibracao_ldr.ino`, abra o Monitor Serial a 115200 baud e digite `a`, `s` e `l` (ambiente, sombra, lanterna).
2. Cole a saída na página da atividade (Confrontar → Importar leituras) e baixe `grafico-calibracao.png`.
3. Em `semaforo/semaforo.ino`, preencha `LEITURA_SOBE_COM_LUZ`, `LIMIAR_ENTRADA` e `LIMIAR_SAIDA` com base na tabela.
4. Grave `semaforo/semaforo.ino` e grave o vídeo passando a luz devagar em torno do limiar.

## Entregas

- [x] `semaforo/semaforo.ino` com os limiares preenchidos (entrada 400, saída 800)
- [x] `RELATORIO.md` com a tabela de calibração
- [x] `grafico-calibracao.png`
- [x] Vídeo da transição no limiar: https://youtube.com/shorts/3qd2LRfXiK0
- [ ] `ADR.md` de cada integrante (o deste repositório é o do Luiz)
