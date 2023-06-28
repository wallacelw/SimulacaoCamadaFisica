#include <bits/stdc++.h>
using namespace std;

#include "camadaFisica.hpp"

// Interface de Interação com o usuário (Camada de Aplicacao)
void aplicacaoTransmissora() {

    // Define qual a codificação utilizada, sendo essa escolhida pelo usuário
    cout << "Escolha a codificacao que sera utilizada:" << endl;
    cout << "(1) Codificacao Binaria" << endl;
    cout << "(2) Codificacao Manchester" << endl;
    cout << "(3) Codificacao Bipolar" << endl;
    int tipoDeCodificacao; cin >> tipoDeCodificacao;
    
    // Recebe a mensagem a ser enviada
    string mensagem;
    cout << "Digite uma mensagem:" << endl;
    cin >> mensagem;

    // Chama a funcao para transmitir a mensagem
    chamadaDeAplicacaoTransmissora(mensagem, tipoDeCodificacao);
}

// Funcao para converter a mensagem a ser enviada em sua representação binária
void chamadaDeAplicacaoTransmissora(string mensagem, int tipoDeCodificacao) {

    // converte a mensagem em um vetor de bits
    // parte-se do presuposto que a mensagem contém apenas caracteres ascii
    // desse modo, cada caractere possui 8 bits
    // utiliza a representacao de little-endian
    vector<bool> quadro;
    for(char c : mensagem) {
        for(int i=0; i<8; i++) {
            if (c & (1 << i)) quadro.push_back(1);
            else quadro.push_back(0);
        }
    } 

    // chama a funcao para codificar o quadro (Camada Física)
    camadaFisicaTransmissora(quadro, tipoDeCodificacao);
}

// Funcao para codificar o quadro
void camadaFisicaTransmissora(vector<bool> quadro, int tipoDeCodificacao) {
    vector<bool> fluxoBrutoDeBits;

    if (tipoDeCodificacao == 1) { // Codificacao Binaria
        fluxoBrutoDeBits = camadaFisicaTransmissoraCodificacaoBinaria(quadro);
    }
    else if (tipoDeCodificacao == 2) { // Codificacao Manchester
        fluxoBrutoDeBits = camadaFisicaTransmissoraCodificacaoManchester(quadro);
    }
    else if (tipoDeCodificacao == 3) { // Codificacao Bipolar
        fluxoBrutoDeBits = camadaFisicaTransmissoraCodificacaoBipolar(quadro);
    }

    // transmite a mensagem codificada
    meioDeComunicacao(fluxoBrutoDeBits, tipoDeCodificacao);
}

// Funcao de Codificacao Binaria
vector<bool> camadaFisicaTransmissoraCodificacaoBinaria(vector<bool> quadro) {
    return quadro;
}

// Funcao de Codificacao Manchester
vector<bool> camadaFisicaTransmissoraCodificacaoManchester(vector<bool> quadro) {
    return quadro;
}

// Funcao de Codificacao Bipolar
vector<bool> camadaFisicaTransmissoraCodificacaoBipolar(vector<bool> quadro) {
    return quadro;
}

// Funcao que simula a transferencia de bits pelo meio fisico
void meioDeComunicacao(vector<bool> fluxoBrutoDeBits, int tipoDeCodificacao) {

    vector<bool> fluxoBrutoDeBitsPontoA, fluxoBrutoDeBitsPontoB;

    fluxoBrutoDeBitsPontoA = fluxoBrutoDeBits;

    for(bool bit : fluxoBrutoDeBitsPontoA) {
        fluxoBrutoDeBitsPontoB.push_back(bit);
    }

    // chama a funcao que recebe o trem de bits
    camadaFisicaReceptora(fluxoBrutoDeBitsPontoB, tipoDeCodificacao);
}

// Funcao que decodifica a mensagem codificada (Camada Fisica)
void camadaFisicaReceptora(vector<bool> quadro, int tipoDeDecodificacao) {
    vector<bool> fluxoBrutoDeBits;

    if (tipoDeDecodificacao == 1) { // Codificacao Binaria
        fluxoBrutoDeBits = camadaFisicaReceptoraDecodificacaoBinaria(quadro);
    }
    else if (tipoDeDecodificacao == 2) { // Codificacao Manchester
        fluxoBrutoDeBits = camadaFisicaReceptoraDecodificacaoManchester(quadro);
    }
    else if (tipoDeDecodificacao == 3) { // Codificacao Bipolar
        fluxoBrutoDeBits = camadaFisicaReceptoraDecodificacaoBipolar(quadro);
    }

    chamadaDeAplicacaoReceptora(fluxoBrutoDeBits);
}

// Funcao de Decodificacao Binaria
vector<bool> camadaFisicaReceptoraDecodificacaoBinaria(vector<bool> quadro) {
    return quadro;
}

// Funcao de Decodificacao Manchester
vector<bool> camadaFisicaReceptoraDecodificacaoManchester(vector<bool> quadro) {
    return quadro;
}

// Funcao de Decodificacao Bipolar
vector<bool> camadaFisicaReceptoraDecodificacaoBipolar(vector<bool> quadro) {
    return quadro;
}

// Funcao que converte a representacao binária da mensagem de volta para caracteres
void chamadaDeAplicacaoReceptora(vector<bool> quadro) {
    string mensagem;
    int caracteres = ((int) quadro.size()) / 8;
    for(int i=0; i<caracteres; i++) {
        char asci_code = 0;
        for(int j=0; j<8; j++) {
            if (quadro[i*8 + j])
                asci_code += (1 << j);
        }
        mensagem += asci_code;
    }
    aplicacaoReceptora(mensagem);
}

// Interface que mostra a mensagem recebida (Camada de Aplicacao)
void aplicacaoReceptora(string mensagem) {
    cout << "A mensagem decodificada e recebida:" << endl;
    cout << mensagem << endl;
}