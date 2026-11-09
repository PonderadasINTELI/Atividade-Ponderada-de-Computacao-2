# ADR individual · Atividade ponderada de Computação 2 - Semáforo com dois modos de operação

**Data:** 08/11/2026
**Autor:** Luiz (luiz.cazelatto@sou.inteli.edu.br)
**Grupo:** Grupo 1

## Contexto

Modo noturno automático do semáforo, detectado por LDR. Calibração do grupo: média de 1.228 no ambiente, 16 na sombra e 4.015 com lanterna. Limiares adotados: entrada 400 e saída 800.

## Decisão

Trocar o resistor fixo do divisor de 10 kΩ por 47 kΩ, mantendo o LDR do lado do 3V3.

## Alternativa considerada

Manter o 10 kΩ e aumentar a luz do ambiente. Com 10 kΩ, a sala iluminada lia 0, igual à sombra, então não havia faixa entre as duas para colocar os limiares. Depender de mais luz deixaria a calibração presa a uma condição que não se repete. Também considerei inverter o LDR e o resistor, mas isso só inverte o sentido: a leitura iria para perto de 4095 e continuaria sem separar sala e sombra. Com 47 kΩ, a sala foi para ~1228 e a sombra para ~16, com espaço para os dois limiares.

## Sugestão de IA rejeitada ou corrigida

Os limiares sugeridos pelo Claude: entrada em 1500 e saída em 2000.

## Razão técnica

A IA supôs a luz ambiente perto do meio da escala, sem conhecer o divisor nem o LDR. Na medição, o ambiente ficou entre 1129 e 1338. Com entrada em 1500, o semáforo entraria em modo noturno com a sala acesa, e a saída em 2000 nunca seria alcançada. Mantive a técnica (histerese) e troquei os valores por 400 e 800, tirados da tabela de calibração.
