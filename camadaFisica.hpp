#ifndef CAMADAFISICA_HPP
#define CAMADAFISICA_HPP

#include <bits/stdc++.h>
using namespace std;

void imprimirSinal(vector<bool> &sinal, int bits, string mensagem);
void aplicacaoTransmissora(int argc, char** argv);
void chamadaDeAplicacaoTransmissora(string mensagem, int tipoDeCodificacao);
void camadaFisicaTransmissora(vector<bool> quadro, int tipoDeCodificacao);
vector<bool> camadaFisicaTransmissoraCodificacaoBinaria(vector<bool> quadro);
vector<bool> camadaFisicaTransmissoraCodificacaoManchester(vector<bool> quadro);
vector<bool> camadaFisicaTransmissoraCodificacaoBipolar(vector<bool> quadro);
void meioDeComunicacao(vector<bool> fluxoBrutoDeBits, int tipoDeCodificacao);
void camadaFisicaReceptora(vector<bool> onda, int tipoDeDecodificacao);
vector<bool> camadaFisicaReceptoraDecodificacaoBinaria(vector<bool> onda);
vector<bool> camadaFisicaReceptoraDecodificacaoManchester(vector<bool> onda);
vector<bool> camadaFisicaReceptoraDecodificacaoBipolar(vector<bool> onda);
void chamadaDeAplicacaoReceptora(vector<bool> quadro);
void aplicacaoReceptora(string mensagem);

#endif