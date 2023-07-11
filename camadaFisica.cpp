#include <bits/stdc++.h>
#include "camadaAplicacao.hpp"
#include "camadaEnlace.hpp"
#include "camadaFisica.hpp"
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

namespace CamadaFisica {

    int tipoCodificacao;
    long double porcentagemErro;

    namespace Transmissora {

        // Funcao para codificar o quadro
        void chamada(vector<bool> quadro) {
            vector<bool> fluxoBrutoDeBits;

            if (tipoCodificacao == 1) { // Codificacao Binaria 
                fluxoBrutoDeBits = codificacaoBinaria(quadro);
            }
            else if (tipoCodificacao == 2) { // Codificacao Manchester 
                fluxoBrutoDeBits = codificacaoManchester(quadro);
            }
            else if (tipoCodificacao == 3) { // Codificacao Bipolar 
                fluxoBrutoDeBits = codificacaoBipolar(quadro);
            }

            // transmite a mensagem codificada
            CamadaFisica::meioDeComunicacao(fluxoBrutoDeBits);
        }

        // Funcao de Codificacao Binaria (NRZI-M)
        vector<bool> codificacaoBinaria(vector<bool> quadro) {
            // dois sinais (0 == V-, 1 == V+)

            int tamanho = (int) quadro.size();
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

            CamadaAplicacao::imprimirSinal(quadro, 1, "Quadro a ser codificado com a codificacao Binaria (NRZI-M)");

            return onda;
        }

        // Funcao de Codificacao Manchester (IEEE convention)
        vector<bool> codificacaoManchester(vector<bool> quadro) {
            // dois sinais (0 == V-, 1 == V+)

            int tamanho = (int) quadro.size();
            vector<bool> clock(tamanho*2, 0), onda(tamanho*2, 0);

            // inicializa o clock
            for(int i=0; i<tamanho; i++) 
                clock[2*i+1] = 1;

            // realiza o xor do sinal com o clock caso o bit seja 1
            for(int i=0; i<tamanho; i++) {
                onda[2*i] = clock[2*i] ^ quadro[i];
                onda[2*i+1] = clock[2*i+1] ^ quadro[i];
            }

            CamadaAplicacao::imprimirSinal(quadro, 1, "Quadro a ser codificado com a codificacao Manchester (IEEE)");
            return onda;
        }

        // Funcao de Codificacao Bipolar (AMI)
        vector<bool> codificacaoBipolar(vector<bool> quadro) {
            // tres sinais (0 == Terra, 1 == V+, 2 == V-)

            int tamanho = (int) quadro.size();

            // flag booleano que cria o comportamento alternado do bipolar
            bool polaridade = 1; // 1 == V+, 0 == V-

            vector<bool> onda(tamanho*2, 0);

            for(int i=0; i<tamanho; i++) {
                // sinal com bit 1 (V = V+ ou V-)
                if (quadro[i]) {
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
                // sinal com bit 0 (V = 0)
                else {
                    onda[2*i] = 0;
                    onda[2*i+1] = 0;
                }
            }

            CamadaAplicacao::imprimirSinal(quadro, 1, "Quadro a ser codificado com a codificacao Bipolar (AMI)");
            return onda;
        }

    }

    // Funcao que simula a transferencia de bits pelo meio fisico
    void meioDeComunicacao(vector<bool> fluxoBrutoDeBits) {

        vector<bool> fluxoBrutoDeBitsPontoA, fluxoBrutoDeBitsPontoB;

        fluxoBrutoDeBitsPontoA = fluxoBrutoDeBits;
        CamadaAplicacao::imprimirSinal(fluxoBrutoDeBitsPontoA, 2, "Fluxo de bits do meio de comunicacao (enviado)");

        // Simula a propagação do sinal no meio físico
        // do transmissor A para o Receptor B
        int erros = 0, count = 0;
        for(bool bit : fluxoBrutoDeBitsPontoA) {
            unsigned int random = rng() % 100000;
            count++;
            
            if (random+1 <= porcentagemErro*1000) {
                fluxoBrutoDeBitsPontoB.push_back(!bit);
                erros++;
            }
            else 
                fluxoBrutoDeBitsPontoB.push_back(bit);
        }

        cout << "Houve " << erros << " inversao(oes) devido a transmissao! " << '\n' << endl;
        cout << "De um Total de: " << count << " bits enviados na transmissao! " << '\n' << endl;

        CamadaAplicacao::imprimirSinal(fluxoBrutoDeBitsPontoB, 2, "Fluxo de bits do meio de comunicacao (recebido)");

        // chama a funcao que recebe o trem de bits
        CamadaFisica::Receptora::chamada(fluxoBrutoDeBitsPontoB);
    }

    namespace Receptora {
       
        // Funcao que decodifica a mensagem codificada (Camada Fisica)
        void chamada(vector<bool> onda) {
            vector<bool> fluxoBrutoDeBits;

            if (tipoCodificacao == 1) { // Codificacao Binaria
                fluxoBrutoDeBits = decodificacaoBinaria(onda);
            }
            else if (tipoCodificacao == 2) { // Codificacao Manchester
                fluxoBrutoDeBits = decodificacaoManchester(onda);
            }
            else if (tipoCodificacao == 3) { // Codificacao Bipolar
                fluxoBrutoDeBits = decodificacaoBipolar(onda);
            }

            CamadaEnlace::Receptora::chamada(fluxoBrutoDeBits);
        } 

        // Funcao de Decodificacao Binaria (NRZI-M)
        vector<bool> decodificacaoBinaria(vector<bool> onda) {
            // dois sinais (0 == V-, 1 == V+)

            int tamanho = (int) onda.size();
            vector<bool> quadro(tamanho/2, 0);

            for(int i=0; i<tamanho/2; i++) {
                // há transição (bit 1)
                if (onda[2*i] ^ onda[2*i+1]) { 
                    quadro[i] = 1;
                }
                // não há transição (bit 0)
                else { 
                    quadro[i] = 0;
                }
            }

            CamadaAplicacao::imprimirSinal(quadro, 1, "Quadro gerado a partir da decodificacao Binaria (NRZI-M)");
            return quadro;
        }

        // Funcao de Decodificacao Manchester
        vector<bool> decodificacaoManchester(vector<bool> onda) {
            // dois sinais (0 == V-, 1 == V+)

            int tamanho = (int) onda.size();
            vector<bool> quadro(tamanho/2, 0);

            for(int i=0; i<tamanho/2; i++) {
                // houve inversao pelo xor (bit 1)
                if (onda[2*i] == 1 and onda[2*i+1] == 0) { 
                    quadro[i] = 1;
                }
                // nao houve inversao pelo xor (bit 0)
                else { 
                    quadro[i] = 0;
                }
            }

            CamadaAplicacao::imprimirSinal(quadro, 1, "Quadro gerado a partir da decodificacao Manchester (IEEE)");
            return quadro;
        }

        // Funcao de Decodificacao Bipolar
        vector<bool> decodificacaoBipolar(vector<bool> onda) {
            // tres sinais (0 == Terra, 1 == V+, 2 == V-)

            int tamanho = (int) onda.size();
            vector<bool> quadro(tamanho/2, 0);

            for(int i=0; i<tamanho/2; i++) {
                // Sinal sem polariade -> Bit 0
                if (onda[2*i] == 0 and onda[2*i+1] == 0) { 
                    quadro[i] = 0;
                }
                // Sinal com polariade -> Bit 1
                else { 
                    quadro[i] = 1;
                }
            }

            CamadaAplicacao::imprimirSinal(quadro, 1, "Quadro gerado a partir da decodificacao Bipolar (AMI)");
            return quadro;
        }

    }

}   
