# Relatório — Semáforo com dois modos de operação

> Preencha os campos `___`. Os dados de calibração precisam vir da medição real do LDR do kit (o barema dá zero para valores redondos sem medição).
> A página da atividade também gera um RELATORIO.md em "Baixar RELATORIO.md"; se preferir, substitua este arquivo pelo dela.

## Identificação

| Campo | Valor |
|---|---|
| Grupo | ___ |
| Turma | ___ |
| Integrantes | ___ |

## 1. Contextualizar (antes de medir)

- **Transições espúrias toleráveis por minuto:** ___
- **Por quê:** ___
- **Limiar previsto para o modo noturno (0 a 4095):** ___
- **De onde veio a previsão:** ___
- **O que o grupo delega à IA:** ___
- **Horário em que a previsão foi registrada:** ___

## 2. Consultar

- **Ferramenta de IA:** ___
- **Pergunta, exatamente como foi feita:** ___
- **Estratégia sugerida:** ___
- **Nome da técnica:** ___
- **Limiar sugerido pela IA (se houver):** ___

## 3. Confrontar (bancada)

Divisor de tensão: LDR + resistor fixo de ___ Ω entre 3V3 e GND, nó no GPIO34 (ADC1). LDR do lado do ___ (3V3/GND), então a leitura ___ (sobe/desce) com a luz.

Medição com `calibracao_ldr/calibracao_ldr.ino` (30 leituras por condição, 115200 baud):

| Condição | n | Mínimo | Máximo | Média | Desvio |
|---|---|---|---|---|---|
| ambiente | ___ | ___ | ___ | ___ | ___ |
| sombra | ___ | ___ | ___ | ___ | ___ |
| lanterna | ___ | ___ | ___ | ___ | ___ |

![Leituras por condição](grafico-calibracao.png)

<details><summary>Saída bruta do Monitor Serial</summary>

```
cole aqui a saída completa
```
</details>

**Divergência entre o limiar previsto, o sugerido pela IA e as medições:** ___

## 4. Construir

Plano do RA (último dígito 6):

| Fase | Tempo |
|---|---|
| Vermelho | 5 + 6 = **11 s** |
| Verde | 3 + ⌊6/2⌋ = **6 s** |
| Amarelo | **2 s** |
| Ciclo | **19 s** |

- **Limiar de entrada no modo noturno:** ___
- **Limiar de saída do modo noturno:** ___
- **Folga entre os limiares:** ___ (maior que o ruído medido numa mesma condição, ___)
- **Como os limiares saíram das medições:** ___

Implementação: `semaforo/semaforo.ino`. No modo diurno, ciclo vermelho → verde → amarelo com os tempos acima. O LDR é lido a cada 200 ms sem travar o ciclo (`millis()`). O modo noturno (amarelo piscando a 1 Hz) entra quando a leitura passa do limiar de entrada para o lado escuro e só sai quando passa do limiar de saída para o lado claro. Entre os dois, o modo atual se mantém. Na volta ao diurno, o ciclo reinicia no vermelho. Cada troca de modo é impressa no Serial (`# troca de modo #N`).

## 5. Comprovar

- **Link do vídeo:** ___
- **Trocas de modo observadas no vídeo:** ___ (critério declarado: ___ por minuto)
- **Por que a histerese impede a oscilação:** ___
