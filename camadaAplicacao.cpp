#include <bits/stdc++.h>
#include "camadaAplicacao.hpp"
#include "camadaEnlace.hpp"
#include "camadaFisica.hpp"

using namespace std;

namespace CamadaAplicacao {
    
    int tipoEnquadramento;
    int tipoCodificacao;
    int tipoTratamento;
    long double porcentagemErro;
    
    namespace Transmissora {

        // Interface de Interação com o usuário (Camada de Aplicacao)
        void aplicacao() {

            // Define qual a codificação utilizada, sendo esta escolhida pelo usuário
            cout << "Escolha a codificacao:" << endl;
            cout << "(1) Codificacao Binaria (NRZI-M)" << endl;
            cout << "(2) Codificacao Manchester (IEEE)" << endl;
            cout << "(3) Codificacao Bipolar (AMI)" << endl;

            cin >> tipoCodificacao;
            CamadaFisica::tipoCodificacao = tipoCodificacao;

             // Define qual o enquadramento utilizado, sendo este escolhido pelo usuário
            cout << "Escolha o enquadramento:" << endl;
            cout << "(1) Contagem de Caracteres" << endl;
            cout << "(2) Insercao de Bytes (Bit Stuffing)" << endl;

            cin >> tipoEnquadramento;
            CamadaEnlace::tipoEnquadramento = tipoEnquadramento;

            // Define qual o tratamento de erro utilizado
            cout << "Escolha o metodo de tratamento de erro:" << endl;
            cout << "(1) Deteccao - Bit de Paridade Par" << endl;
            cout << "(2) Deteccao - CRC (CRC32 - IEEE)" << endl;
            cout << "(3) Correcao - Codigo de Hamming (Par)" << endl;

            cin >> tipoTratamento;
            CamadaEnlace::tipoTratamento = tipoTratamento;

            // Define a porcentagem de erro do meio Físico
            cout << "Escolha a porcentagem de erro do meio fisico [0, 100]:" << endl;
            cout << "(Aceita-se uma precisao de ate 3 casas decimais)" << endl;

            cin >> porcentagemErro;
            CamadaFisica::porcentagemErro = porcentagemErro;

            // Recebe a mensagem a ser enviada
            cout << endl << "Digite uma mensagem: (pode conter espaços)" << endl;
            cin >> ws;
            string mensagem; getline(cin, mensagem);
            cout << endl;

            // Chama a funcao para transmitir a mensagem
            CamadaAplicacao::Transmissora::chamada(mensagem);
        }

        // Funcao para converter a mensagem a ser enviada em sua representação binária
        void chamada(string mensagem) {
            // converte a mensagem em um vetor de bits
            // parte-se do presuposto que a mensagem contém apenas caracteres ascii
            // desse modo, cada caractere possui exatamente 8 bits
            // utiliza a representacao little-endian
            vector<bool> quadro;
            for(char c : mensagem) {
                for(int i=0; i<8; i++) {
                    if (c & (1 << i)) quadro.push_back(1);
                    else quadro.push_back(0);
                }
            } 

            // chama a funcao para codificar o quadro (Camada Enlace)
            CamadaEnlace::Transmissora::chamada(quadro);
        }

    }

    namespace Receptora {
        
        // Funcao que converte a representacao binária da mensagem de volta para caracteres
        void chamada(vector<bool> quadro) {
            string mensagem;
            int caracteres = ((int) quadro.size()) / 8;

            // Analogamente a conversão de ascii para binário,
            // analisa-se 8 bits em little endian 
            // para definir o caractere correspondente
            for(int i=0; i<caracteres; i++) {
                char asci_code = 0;
                for(int j=0; j<8; j++) {
                    if (quadro[i*8 + j])
                        asci_code += (1 << j);
                }
                mensagem += asci_code;
            }

            // chama a aplicação receptora
            CamadaAplicacao::Receptora::aplicacao(mensagem);
        }

        // Interface que mostra a mensagem recebida (Camada de Aplicacao)
        void aplicacao(string mensagem) {
            cout << "A mensagem decodificada, desenquadrada, tratada e recebida:" << endl;
            cout << mensagem << endl;
        }
    }
    
    // "Interface Gráfica" com o usuário
    void imprimirSinal(vector<bool> &sinal, int bits, string mensagem) {
        // imprime a mensagem passada como argumento
        // e imprime o sinal com o devido espaçamento
        // definido pela variável "bits"

        int tamanho = (int) sinal.size();
        cout << mensagem << endl;
        
        for(int i=0; i<tamanho/bits; i++) {
            // imprime os bits do sinal
            for(int j=0; j<bits; j++)
                cout << sinal[bits*i+j];

            // imprime o espaçamento devido
            for(int j=0; j<3-bits; j++)
                cout << " ";
        }
        cout << endl << endl;
    }

}