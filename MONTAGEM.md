# Manual de montagem na protoboard

Este manual usa a mesma bancada do protótipo do G01 (`g01/document/outros/reports/Sprint02/montagem-prototipo-fisico-luiz.md`): duas protoboards de 830 furos encaixadas pelo lado comprido, com o ESP32 deitado sobre o trilho do meio.

Pinos que o código usa (`semaforo/semaforo.ino`): **LDR → GPIO34**, **vermelho → GPIO25**, **amarelo → GPIO26**, **verde → GPIO27**.

## Material

| Qtde | Peça |
|---|---|
| 1 | ESP32-WROOM-32U (placa DevKitC V4, 19 pinos por lado) + cabo USB |
| 2 | Protoboards de 830 furos, encaixadas pelo lado comprido |
| 1 | LDR |
| 1 | Resistor de 47 kΩ (amarelo, violeta, laranja) para o LDR |
| 3 | Resistores de 220 Ω (vermelho, vermelho, marrom) para os LEDs |
| 3 | LEDs: vermelho, amarelo e verde |
| 13 | Jumpers |

## Como ler os furos

- `cima 30d` quer dizer: protoboard de cima, coluna 30, linha d.
- Nesta placa as letras são **invertidas**: a linha **j** fica em cima, perto do trilho de cima, e a linha **a** fica embaixo, perto da canaleta e do ESP32. Na ordem de cima para baixo: j, i, h, g, f | canaleta | e, d, c, b, a.
- A **coluna 1 é o primeiro furo da esquerda**. A numeração impressa na placa começa no segundo furo, então não conte por ela.
- Numa mesma coluna, os furos f–j (parte de cima) formam um ponto só, e os furos a–e (parte de baixo, do lado do ESP32) formam outro.
- Nesta protoboard os trilhos são **contínuos**: cada linha do trilho vai de ponta a ponta. Um jumper de GND e um de 3V3 bastam para a placa toda. Mesmo assim, confira com o multímetro, porque a protoboard do G01 tinha trilhos cortados no meio.

## Onde fica o ESP32

O ESP32 fica deitado sobre o trilho do meio, **com o USB à esquerda**, na mesma posição do protótipo do G01. A fileira de pinos que vai do 5V ao 3V3 entra em `cima 20a` a `cima 38a`. Com isso, cada pino fica numa coluna da placa de cima. Como as linhas b, c, d e e dessa coluna estão ligadas ao pino, os fios saem por elas.

| Coluna (placa de cima) | 20 | 21 | 22 | 23 | 24 | **25** | 26 | 27 | **28** | **29** | **30** | 31 | 32 | 33 | **34** | 35 | 36 | 37 | **38** |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Pino do ESP32 | 5V | CMD | SD3 | SD2 | 13 | **GND** | 12 | 14 | **27** | **26** | **25** | 33 | 32 | 35 | **34** | VN | VP | EN | **3V3** |

Os pinos em negrito são os usados. Para conferir antes de começar: no G01, o potenciômetro do GPIO34 saía de `cima 34e`, o LED do GPIO13 de `cima 24e` e o 3V3 de `cima 38e`.

**Não use a coluna 20, que é 5V.** O GPIO34 não aguenta 5 V.

## Passo a passo (com o USB DESCONECTADO)

Os LEDs ficam na parte de cima da protoboard de cima (linhas f a j), à **esquerda** do ESP32 (colunas 2 a 19). O LDR fica à **direita** (colunas 42 a 49). Todos usam o mesmo trilho de cima.

Use a linha azul (−) e a linha vermelha (+) do trilho **de cima** da placa de cima.

### 1. GND e 3V3 nos trilhos

| Jumper | De | Para | Para quê |
|---|---|---|---|
| J1 | `cima 25d` (GND) | trilho **−** de cima (qualquer coluna, por exemplo a 25) | GND dos LEDs e do LDR |
| J2 | `cima 38d` (3V3) | trilho **+** de cima (qualquer coluna, por exemplo a 38) | 3,3 V do LDR |

### 2. LEDs

Cada LED segue o mesmo caminho:

```
 GPIO ── resistor 220 Ω ── perna LONGA do LED │ perna CURTA ── trilho −
```

| LED | Jumper do GPIO | Resistor 220 Ω | Perna longa (+) | Perna curta (−) | Jumper para o − |
|---|---|---|---|---|---|
| Vermelho | `cima 30d` (GPIO25) → `cima 2j` | `cima 2i` ↔ `cima 6i` | `cima 6h` | `cima 7h` | `cima 7j` → − |
| Amarelo | `cima 29d` (GPIO26) → `cima 8j` | `cima 8i` ↔ `cima 12i` | `cima 12h` | `cima 13h` | `cima 13j` → − |
| Verde | `cima 28d` (GPIO27) → `cima 14j` | `cima 14i` ↔ `cima 18i` | `cima 18h` | `cima 19h` | `cima 19j` → − |

Na perna curta, a borda do LED é achatada.

### 3. LDR com divisor de tensão

```
 3V3 (trilho +) ── LDR ──┬── resistor 47 kΩ ── GND (trilho −)
                         │
                         └── GPIO34
```

| Peça | Perna 1 | Perna 2 |
|---|---|---|
| LDR (não tem lado certo) | `cima 42h` | `cima 45h` |
| Resistor 47 kΩ | `cima 45g` | `cima 49g` |
| Jumper | `cima 42j` | trilho **+** |
| Jumper | `cima 49j` | trilho **−** |
| Jumper | `cima 34d` (GPIO34) | `cima 45f` |

A coluna 45 é o ponto onde o LDR encontra o resistor, e é por ela que o GPIO34 lê a luz. Com o LDR do lado do 3V3, como aqui, a leitura **sobe** com a luz. Isso combina com o `LEITURA_SOBE_COM_LUZ = true` do código.

## Desenho (placa de cima, vista de cima, USB à esquerda)

```
 col:  2   6 7 8  12 13 14  18 19     25    28 29 30  34    38     42  45   49
 + ─────────────────────────────────────────────────────────────●──────────── 3V3 (J2)
 − ──────●─────●────────●───────────────────────────────────────────────●──── GND (J1)
 j   [25]│ [26]│  [27]  │                                       ●+    │
 i   └R220┤ └R220┤ └R220┤                                       │     │
 h      LED+−   LED+−   LED+−                                    └LDR─┤     │
 g                                                                    └R47k┘
 f                                                               [34]→f45
 ════════════════════════════════════ canaleta ═══════════════════════════════
 e..b                       GND(25)   27 26 25   34    3V3(38)   ← fios saem daqui
 a        ESP32 deitado:  5V ... 13 GND 12 14 27 26 25 33 32 35 34 VN VP EN 3V3
          (colunas 20 a 38, USB à esquerda)
```

## Antes de ligar o USB

- [ ] ESP32 com os pinos de 5V a 3V3 em `cima 20a` a `cima 38a`, firme nos furos
- [ ] J1 levando o GND do ESP32 até o trilho − de cima
- [ ] J2 levando o 3V3 até o trilho + de cima, e nada na coluna 20 (5V)
- [ ] Nenhum jumper ligando o trilho + direto no trilho −
- [ ] GPIO34 (`cima 34d`) indo para `cima 45f`, entre o LDR e o 47 kΩ
- [ ] Os três LEDs com a perna longa do lado do resistor

## Testar

1. Com o semáforo gravado, os LEDs devem acender na ordem: vermelho por 11 s, verde por 6 s e amarelo por 2 s.
   - Se nenhum acender, confira o J1.
   - Se só um LED não acender, gire esse LED.
2. Grave `calibracao_ldr` e digite `a`, `s` e `l` no monitor. Com sombra o número deve cair, e com a lanterna deve subir.
   - Se a leitura ficar travada em **4095**, falta o GND: confira o J1 e o jumper `cima 49j`.
   - Se ficar travada em **0**, falta o 3V3: confira o J2 e o jumper `cima 42j`.
