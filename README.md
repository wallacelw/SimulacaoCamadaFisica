# Simulacao de uma Camada Fisica

Trabalho 2 da disciplina de Teleinformática e Redes da UnB.

O projeto consiste em simular uma transmissão de uma mensagem da camada de Aplicação de um transmissor até um receptor. Para isso, durante a tranmissão, a mensagem em ascii é convertida em binário na camada de aplicação; recebe o tratamento de erros e o enquadramento na camada de enlace de dados; e, por fim, é codificada para ser trasmitida pela camada física. Analogamente, durante a recepção, o sinal detectado pela camada física é decodificado; em seguida, verifica se a mensagem recebida veio com erros e o desenquadra na camada de enlace; e, finalmente, reconverte o binário de volta na mensagem.

## Integrantes

200028880 - Wallace Ben Teng Lin Wu 

190024771 - Andressa de Araújo Pereira

200049682 - Pedro Brazil Suffert

## Compilação

```bash
g++ -std=c++17 -o simulador.exe camadaFisica.hpp camadaEnlace.hpp camadaAplicacao.hpp camadaFisica.cpp camadaEnlace.cpp camadaAplicacao.cpp simulador.cpp  
```

Implementou-se um arquivo makefile para facilitar a compilação:

```bash
make all
```

## Uso

```bash
./simulador.exe
```
