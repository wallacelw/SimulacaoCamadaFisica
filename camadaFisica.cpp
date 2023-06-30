#include <bits/stdc++.h>
using namespace std;

#include "camadaFisica.hpp"

// Debugging
#define db(a) cerr << " [ " << #a << " = " << a << " ] " << endl;
#define debug(a...) cerr<<#a<<": ";for(auto b:a)cerr<<b<<" ";cerr<<endl;
template <typename... A> void dbg(A const&... a) { ((cerr << "{" << a << "} "), ...); cerr << endl; }

// "Interface Gráfica" com o usuário
void imprimirSinal(vector<bool> &sinal, int bits, string mensagem) {

    int tamanho = (int) sinal.size();

    cout << mensagem << endl;
    
    for(int i=0; i<tamanho/bits; i++) {
        for(int j=0; j<bits; j++)
            cout << sinal[bits*i+j];

        for(int j=0; j<3-bits; j++)
            cout << " ";
    }
    cout << endl << endl;
}

// Interface de Interação com o usuário (Camada de Aplicacao)
void aplicacaoTransmissora(int argc, char** argv) {

    int tipoDeCodificacao;
    string mensagem;

    if (argc == 3) {   // Define qual a codificação utilizada, sendo essa escolhida pela interface de linha de comando
        tipoDeCodificacao = atoi(argv[1]);
        mensagem = argv[2];
    }
    else {      // Define qual a codificação utilizada, sendo essa escolhida pelo usuário
        cout << "Escolha a codificacao que sera utilizada:" << endl;
        cout << "(1) Codificacao Binaria" << endl;
        cout << "(2) Codificacao Manchester" << endl;
        cout << "(3) Codificacao Bipolar" << endl;
        cin >> tipoDeCodificacao;
        cout << endl << "Digite uma mensagem:" << endl;
        cin >> mensagem;
        cout << endl;
    }
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

// Funcao de Codificacao Binaria (NRZI-M)
vector<bool> camadaFisicaTransmissoraCodificacaoBinaria(vector<bool> quadro) {
    // dois sinais (0 == V-, 1 == V+)
    int tamanho = quadro.size();
    vector<bool> onda(tamanho*2, 0);

    for(int i=0; i<tamanho; i++) {

        // mantendo o sinal 
        if (i > 0) { // o primeiro bit é sempre 0
            onda[2*i] = onda[2*i-1];
        }

        // invertendo o sinal caso seja um bit 1
        if (quadro[i] == 1) {
            onda[2*i+1] = !onda[2*i];
        }
        else {
            onda[2*i+1] = onda[2*i];
        }
    }

    imprimirSinal(quadro, 1, "Quadro a ser codificado com a codificacao Binaria (NRZI-M)");

    return onda;
}

// Funcao de Codificacao Manchester (IEEE convention)
vector<bool> camadaFisicaTransmissoraCodificacaoManchester(vector<bool> quadro) {
    // dois sinais (0 == V-, 1 == V+)
    int tamanho = quadro.size();
    vector<bool> clock(tamanho*2, 0), onda(tamanho*2, 0);

    for(int i=0; i<tamanho; i++) 
        clock[2*i+1] = 1;

    for(int i=0; i<tamanho; i++) {
        onda[2*i] = clock[2*i] ^ quadro[i];
        onda[2*i+1] = clock[2*i+1] ^ quadro[i];
    }

    imprimirSinal(quadro, 1, "Quadro a ser codificado com a codificacao Manchester (IEEE)");
    return onda;
}

// Funcao de Codificacao Bipolar (AMI)
vector<bool> camadaFisicaTransmissoraCodificacaoBipolar(vector<bool> quadro) {
    // tres sinais (0 == Terra, 1 == V+, 2 == V-)
    int tamanho = quadro.size();
    bool polaridade = 1; // 1 == V+, 0 == V-

    vector<bool> onda(tamanho*2, 0);

    for(int i=0; i<tamanho; i++) {
        if (quadro[i] == 1) {
            if (polaridade) {
                onda[2*i] = 0;
                onda[2*i+1] = 1;
            }
            else {
                onda[2*i] = 1;
                onda[2*i+1] = 0;
            }
            polaridade = !polaridade;
        }
        else {
            onda[2*i] = 0;
            onda[2*i+1] = 0;
        }
    }

    imprimirSinal(quadro, 1, "Quadro a ser codificado com a codificacao Bipolar (AMI)");
    return onda;
}


// Funcao que simula a transferencia de bits pelo meio fisico
void meioDeComunicacao(vector<bool> fluxoBrutoDeBits, int tipoDeCodificacao) {

    vector<bool> fluxoBrutoDeBitsPontoA, fluxoBrutoDeBitsPontoB;

    fluxoBrutoDeBitsPontoA = fluxoBrutoDeBits;

    for(bool bit : fluxoBrutoDeBitsPontoA) {
        fluxoBrutoDeBitsPontoB.push_back(bit);
    }

    imprimirSinal(fluxoBrutoDeBits, 2, "Fluxo de bits do meio de comunicacao");

    // chama a funcao que recebe o trem de bits
    camadaFisicaReceptora(fluxoBrutoDeBitsPontoB, tipoDeCodificacao);
}

// Funcao que decodifica a mensagem codificada (Camada Fisica)
void camadaFisicaReceptora(vector<bool> onda, int tipoDeDecodificacao) {
    vector<bool> fluxoBrutoDeBits;

    if (tipoDeDecodificacao == 1) { // Codificacao Binaria
        fluxoBrutoDeBits = camadaFisicaReceptoraDecodificacaoBinaria(onda);
    }
    else if (tipoDeDecodificacao == 2) { // Codificacao Manchester
        fluxoBrutoDeBits = camadaFisicaReceptoraDecodificacaoManchester(onda);
    }
    else if (tipoDeDecodificacao == 3) { // Codificacao Bipolar
        fluxoBrutoDeBits = camadaFisicaReceptoraDecodificacaoBipolar(onda);
    }

    chamadaDeAplicacaoReceptora(fluxoBrutoDeBits);
}

// Funcao de Decodificacao Binaria (NRZI-M)
vector<bool> camadaFisicaReceptoraDecodificacaoBinaria(vector<bool> onda) {
    // dois sinais (0 == V-, 1 == V+)
    int tamanho = onda.size();
    vector<bool> quadro(tamanho/2, 0);

    for(int i=0; i<tamanho/2; i++) {
        if (onda[2*i] ^ onda[2*i+1]) { // há transição
            quadro[i] = 1;
        }
        else { // não há transição
            quadro[i] = 0;
        }
    }

    imprimirSinal(quadro, 1, "Quadro gerado a partir da decodificacao Binaria (NRZI-M)");
    return quadro;
}

// Funcao de Decodificacao Manchester
vector<bool> camadaFisicaReceptoraDecodificacaoManchester(vector<bool> onda) {
    // dois sinais (0 == V-, 1 == V+)
    int tamanho = onda.size();
    vector<bool> quadro(tamanho/2, 0);

    for(int i=0; i<tamanho/2; i++) {
        if (onda[2*i] == 1 and onda[2*i+1] == 0) { // houve inversao pelo xor
            quadro[i] = 1;
        }
        else { // nao houve inversao pelo xor
            quadro[i] = 0;
        }
    }

    imprimirSinal(quadro, 1, "Quadro gerado a partir da decodificacao Manchester (IEEE)");
    return quadro;
}

// Funcao de Decodificacao Bipolar
vector<bool> camadaFisicaReceptoraDecodificacaoBipolar(vector<bool> onda) {
    // tres sinais (0 == Terra, 1 == V+, 2 == V-)
    int tamanho = onda.size();
    vector<bool> quadro(tamanho/2, 0);

    for(int i=0; i<tamanho/2; i++) {
        if (onda[2*i] == 0 and onda[2*i+1] == 0) { // Sinal sem polariade -> Bit 0
            quadro[i] = 0;
        }
        else { // Sinal com polariade -> Bit 1
            quadro[i] = 1;
        }
    }

    imprimirSinal(quadro, 1, "Quadro gerado a partir da decodificacao Bipolar (AMI)");
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