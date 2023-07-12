#include <bits/stdc++.h>
#include "camadaAplicacao.hpp"
#include "camadaEnlace.hpp"
#include "camadaFisica.hpp"
using namespace std;

namespace CamadaEnlace {

    int tipoEnquadramento;
    int tipoTratamento;

    namespace Transmissora {

        // Representa a chamada da Camada de Enlace
        void chamada(vector<bool> quadro) {
        
            CamadaAplicacao::imprimirSinal(quadro, 1, "Quadro Recebido da Camada de Aplicacao na Camada de Enlace:");

            // Acrescenta ao quadro o tratamento de erro escolhido
            vector<bool> quadroTratado = tratamentoErro(quadro);
            CamadaAplicacao::imprimirSinal(quadroTratado, 1, "Quadro Com Tratamento de Erro:");

            // Enquadra o quadro
            vector<bool> quadroEnquadrado = enquadramento(quadroTratado);
            CamadaAplicacao::imprimirSinal(quadroEnquadrado, 1, "Quadro Enquadrado:");

            // Chama a Camada Física para transmissao
            CamadaFisica::Transmissora::chamada(quadroEnquadrado);
        }

        // Funcao que escolhe o tratamento de erro devido
        vector<bool> tratamentoErro(vector<bool> quadro) {
            vector<bool> quadroTratado;

            if (tipoTratamento == 1) {
                quadroTratado = paridade(quadro);
            }
            else if (tipoTratamento == 2) {
                quadroTratado = crc(quadro);
            }
            else if (tipoTratamento == 3) {
                quadroTratado = hamming(quadro);
            }

            return quadroTratado;
        }

        // Acrescenta um Bit de Paridade Par ao fim do quadro
        vector<bool> paridade(vector<bool> quadro) {
            bool xorsum = 0;
            for(auto bit : quadro) xorsum ^= bit;
            quadro.push_back(xorsum);
            return quadro; 
        }   

        // Acrescenta 32 bits ao fim do quadro que representa o resto da divisão
        vector<bool> crc(vector<bool> quadro) {
            // polinomio gerador g(x) definido pelo CRC32-IEEE
            int g = 0x04C11DB7;

            vector<bool> copy = quadro;

            int tamanho = (int) quadro.size();
            for(int i=0; i<32; i++) copy.push_back(0);

            // calcula o resto
            for(int i=0; i<tamanho; i++) {
                if (!copy[i]) continue;

                copy[i] = 0; // xor com o termo x^32
                for(int j=0; j<32; j++) { // xor com os demais termos
                    if (g & (1 << j)) {
                        copy[i+32-j] = !copy[i+32-j];
                    }
                }
            }

            // readiciona o quadro original, depois de calcular o resto
            for(int i=0; i<tamanho; i++) {
                copy[i] = quadro[i];
            }

            return copy;
        }

        // Acrescenta bits de paridade nas posições que são potencias de 2
        vector<bool> hamming(vector<bool> quadro) {
            int tamanho = (int) quadro.size();

            // Calcula quantos bits de paridade são necessários
            int parityBits = 0;
            int pow = 1; // pow = 2 ^ parity bits
            while(pow < parityBits + tamanho + 1) {
                parityBits += 1;
                pow *= 2;
            }
            
            // Adiciona os bits de paridade nas devidas posições
            pow = 1;
            int j = 1;
            vector<bool> codigo;
            for(int i=0; i<tamanho; i++) {
                if (j == pow) {
                    codigo.push_back(0);
                    j += 1;
                    i -= 1;
                    pow *= 2;
                }
                else {
                    codigo.push_back(quadro[i]);
                    j += 1;
                }
            }

            // Calcula o valor dos bits de paridade
            int novoTamanho = (int) codigo.size();
            vector<bool> xorsum(parityBits, 0);
            for(int i=0; i<novoTamanho; i++) {
                for(int j=0; j<parityBits; j++) {
                    if ((i+1) & (1 << j)) {
                        xorsum[j] = xorsum[j] ^ codigo[i];
                    }
                }
            }   

            // Ajusta os valores dos bits de paridade com os valores calculados
            for(int j=0; j<parityBits; j++) 
                codigo[(1 << j) - 1] = xorsum[j];
            
            return codigo;
        }

        // Função que escolhe qual o método de enquadramento
        vector<bool> enquadramento(vector<bool> quadro) {

            vector<bool> quadroEnquadrado;

            if (tipoEnquadramento == 1) {
                quadroEnquadrado = contagemDeCaracteres(quadro);
            }
            else if (tipoEnquadramento == 2) {
                quadroEnquadrado = insercaoDeBytes(quadro);
            }

            return quadroEnquadrado;
        }

        // Simplesmente adicionar no cabeçalho 4 bytes 
        // que indicam o tamnho do dado sendo transmitido (sem incluir o cabeçalho)
        vector<bool> contagemDeCaracteres(vector<bool> quadro) {
            vector<bool> quadroEnquadrado;

            int tamanho = (int) quadro.size() / 8;

            // Adiciona Cabeçalho (4 bytes)
            for(int j=0; j<32; j++) {
                if (tamanho & (1 << j)) quadroEnquadrado.push_back(1);
                else quadroEnquadrado.push_back(0);
            }

            // Copia o quadro original
            for(auto bit : quadro) quadroEnquadrado.push_back(bit);

            return quadroEnquadrado;
        }

        // Utiliza a técnica de inserção de um byte de flag no começo e no fim do quadro 
        // Também utiliza a ténica de bit stuffing (adiciona um '0' quando tem cinco '1' consecutivos nos dados)
        // O valor do flag utilizado é 0x7E = 0111 1110; 
        vector<bool> insercaoDeBytes(vector<bool> quadro) {
            // Insere o flago no começo
            vector<bool> quadroEnquadrado = {0, 1, 1, 1, 1, 1, 1, 0};

            // Copia os dados originais do quadro, adicionando um '0'
            // sempre que encontrar cinco '1' consecutivos
            int counter = 0;
            for(auto bit : quadro) {
                if (counter == 5) {
                    counter = 0;
                    quadroEnquadrado.push_back(0);
                }

                if (bit) {
                    counter++;
                }
                else {
                    counter = 0;
                }

                quadroEnquadrado.push_back(bit);
            }

            // Insere o flag no fim
            vector<bool> flag = {0, 1, 1, 1, 1, 1, 1, 0};
            for(auto bit : flag) quadroEnquadrado.push_back(bit);

            return quadroEnquadrado;
        }
    }

    namespace Receptora {

        void chamada(vector<bool> quadroEnquadrado) {
            
            CamadaAplicacao::imprimirSinal(quadroEnquadrado, 1, "Quadro Recebido do Meio Fisico");

            vector<bool> quadroTratado = desenquadramento(quadroEnquadrado);
            CamadaAplicacao::imprimirSinal(quadroTratado, 1, "Quadro Desenquadrado");

            vector<bool> quadro = tratamentoErro(quadroTratado);
            CamadaAplicacao::imprimirSinal(quadro, 1, "Quadro enviado a Camada de Aplicacao");

            CamadaAplicacao::Receptora::chamada(quadro);
        }

        vector<bool> tratamentoErro(vector<bool> quadroTratado) {
            vector<bool> quadro;

            if (tipoTratamento == 1) {
                quadro = paridade(quadroTratado);
            }
            else if (tipoTratamento == 2) {
                quadro = crc(quadroTratado);
            }
            else if (tipoTratamento == 3) {
                quadro = hamming(quadroTratado);
            }

            return quadro;
        }

        // Bit de Paridade Par ao fim do quadro
        vector<bool> paridade(vector<bool> quadro) {
            bool xorsum = 0;
            for(auto bit : quadro) xorsum ^= bit;
            if (xorsum) {
                cout << "ERRO Detectado - Paridade deveria ser PAR" << endl;
            }
            else {
                cout << "Sem ERRO Detectado - Paridade calculada PAR" << endl;
            }
            quadro.pop_back();
            return quadro; 
        }   

        vector<bool> crc(vector<bool> copy) {
            int g = 0x04C11DB7;

            vector<bool> quadro = copy;

            for(int i=0; i<32; i++) quadro.pop_back();

            int tamanho = (int) quadro.size();

            // calcula o resto
            for(int i=0; i<tamanho; i++) {
                if (!copy[i]) continue;

                copy[i] = 0; // xor with x^32
                for(int j=0; j<32; j++) {
                    if (g & (1 << j)) {
                        copy[i+32-j] = !copy[i+32-j];
                    }
                }
            }

            bool error = 0;
            for(int i=0; i<32; i++) {
                if (copy[tamanho+32-i-1]) error = 1;
            }

            if (error) {
                cout << "ERRO Detectado - Resto != 0" << endl;
            }
            else {
                cout << "Sem ERRO Detectado - Resto == 0" << endl;
            }           

            return quadro;
        }

        vector<bool> hamming(vector<bool> quadro) {
            int tamanho = (int) quadro.size();

            int parityBits = 0;
            int pow = 1; // pow = 2 ^ parity bits
            while(pow < tamanho+1) {
                parityBits += 1;
                pow *= 2;
            }

            vector<bool> xorsum(parityBits, 0);
            for(int i=0; i<tamanho; i++) {
                for(int j=0; j<parityBits; j++) {
                    if ((i+1) & (1 << j)) {
                        xorsum[j] = xorsum[j] ^ quadro[i];
                    }
                }
            }
            
            int error = 0;
            for(int j=0; j<parityBits; j++) {
                if (xorsum[j]) error += (1 << j);
            }

            if (error) {
                cout << "Erro no bit {" << error << "} !" << endl;
                quadro[error-1] = !quadro[error-1];
            }
            else {
                cout << "Sem erro!" << endl;
            }
            
            vector<bool> codigo;
            pow = 1;
            for(int i=0; i<tamanho; i++) {
                if ((i+1) == pow) {
                    pow *= 2;
                    continue;
                }
                codigo.push_back(quadro[i]);
            }
            
            return codigo;
        }

        vector<bool> desenquadramento(vector<bool> quadroEnquadrado) {

            vector<bool> quadroDesenquadrado;

            if (tipoEnquadramento == 1) {
                quadroDesenquadrado = contagemDeCaracteres(quadroEnquadrado);
            }
            else if (tipoEnquadramento == 2) {
                quadroDesenquadrado = insercaoDeBytes(quadroEnquadrado);
            }

            return quadroDesenquadrado;
        }

        vector<bool> contagemDeCaracteres(vector<bool> quadroEnquadrado) {
            vector<bool> quadroDesenquadrado;

            int tamanho = (int) quadroEnquadrado.size();
            for(int i=32; i<tamanho; i++) {
                quadroDesenquadrado.push_back(quadroEnquadrado[i]);
            }

            return quadroDesenquadrado;

        }

        vector<bool> insercaoDeBytes(vector<bool> quadroEnquadrado) {

            vector<bool> quadroDesenquadrado;
            int counter = 0;
            bool foundFlag = 0;

            for(auto bit : quadroEnquadrado) {
                if (foundFlag) {
                    foundFlag = 0;
                    for(int i=0; i<7; i++) {
                        quadroDesenquadrado.pop_back();
                    }
                    continue;
                }

                if (bit) {
                    if (counter == 5) { // 6 consecutive '1'
                        counter = 0;
                        foundFlag = 1;
                    }
                    counter++;
                }
                else {
                    if (counter == 5) { // skip 0
                        counter = 0;
                        continue;
                    }
                    counter = 0;
                }

                quadroDesenquadrado.push_back(bit);
            }

            return quadroDesenquadrado;
        }
    }

}