#ifndef CAMADAFISICA_HPP
#define CAMADAFISICA_HPP

#include <bits/stdc++.h>
using namespace std;

void aplicacaoTransmissora();
void chamadaDeAplicacaoTransmissora(string mensagem, int tipoDeCodificacao);
void camadaFisicaTransmissora(vector<bool> quadro, int tipoDeCodificacao);
vector<bool> camadaFisicaTransmissoraCodificacaoBinaria(vector<bool> quadro);
vector<bool> camadaFisicaTransmissoraCodificacaoManchester(vector<bool> quadro);
vector<bool> camadaFisicaTransmissoraCodificacaoBipolar(vector<bool> quadro);
void meioDeComunicacao(vector<bool> fluxoBrutoDeBits, int tipoDeCodificacao);
void camadaFisicaReceptora(vector<bool> quadro, int tipoDeDecodificacao);
vector<bool> camadaFisicaReceptoraDecodificacaoBinaria(vector<bool> quadro);
vector<bool> camadaFisicaReceptoraDecodificacaoManchester(vector<bool> quadro);
vector<bool> camadaFisicaReceptoraDecodificacaoBipolar(vector<bool> quadro);
void chamadaDeAplicacaoReceptora(vector<bool> quadro);
void aplicacaoReceptora(string mensagem);

#endif