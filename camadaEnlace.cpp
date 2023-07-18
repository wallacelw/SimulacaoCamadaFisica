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
            int k = 1;
            vector<bool> codigo;
            for(int i=0; i<tamanho; i++) {
                if (k == pow) {
                    codigo.push_back(0);
                    k += 1;
                    i -= 1;
                    pow *= 2;
                }
                else {
                    codigo.push_back(quadro[i]);
                    k += 1;
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

        // Representa o recebimento do quadro pela Camada de Enlace Receptora
        void chamada(vector<bool> quadroEnquadrado) {
            
            CamadaAplicacao::imprimirSinal(quadroEnquadrado, 1, "Quadro com Tratamento de Erros Recebido do Meio Fisico:");

            // Desenquadra o quadro
            vector<bool> quadroTratado = desenquadramento(quadroEnquadrado);
            CamadaAplicacao::imprimirSinal(quadroTratado, 1, "Quadro Desenquadrado:");

            // Trata o erro, removendo os bits adicionais
            vector<bool> quadro = tratamentoErro(quadroTratado);
            CamadaAplicacao::imprimirSinal(quadro, 1, "Quadro depois do Tratamento de Erros:");
            
            // Passa o quadro para a Camada de Aplicacao
            CamadaAplicacao::Receptora::chamada(quadro);
        }

        // Função que define como deve interpretar o tratamento dos erros
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

        // Averigua o Bit de Paridade Par ao fim do quadro e o remove
        vector<bool> paridade(vector<bool> quadro) {
            // Calcula a paridade do sinal
            bool xorsum = 0;
            for(auto bit : quadro) xorsum ^= bit;

            // Imprime a mensagem de erro
            if (xorsum) {
                cout << "ERRO - Paridade deveria ser PAR \n" << endl;
            }
            else {
                cout << "SEM ERRO - Paridade calculada é PAR \n" << endl;
            }
            
            // Retira o bit de paridade
            quadro.pop_back();

            return quadro; 
        }   

        // Calcula o resto da divisão polinomial e verifica se é 0
        // Remove os 32 bits adicionais no fim do quadro
        vector<bool> crc(vector<bool> copy) {
            // polinomio gerador do CRC32-IEEE
            int g = 0x04C11DB7;

            // Gera o quadro original, sem os 32 bits adicionais
            vector<bool> quadro = copy;
            for(int i=0; i<32; i++) quadro.pop_back();

            int tamanho = (int) quadro.size();

            // Calcula o resto
            for(int i=0; i<tamanho; i++) {
                if (!copy[i]) continue;

                copy[i] = 0;
                for(int j=0; j<32; j++) {
                    if (g & (1 << j)) {
                        copy[i+32-j] = !copy[i+32-j];
                    }
                }
            }

            // Verifica se houve erro 
            bool error = 0;
            for(int i=0; i<32; i++) {
                if (copy[tamanho+32-i-1]) error = 1;
            }

            // Imprime a mensagem de Erro
            if (error) {
                cout << "ERRO - Resto != 0 \n" << endl;
            }
            else {
                cout << "SEM ERRO - Resto == 0 \n" << endl;
            }           

            return quadro;
        }

        // Recalcula os bits de paridade e verifica se condiz
        // Também remove os bits de paridade
        vector<bool> hamming(vector<bool> quadro) {
            int tamanho = (int) quadro.size();

            // Calcula a quantidade de bits de paridade
            int parityBits = 0;
            int pow = 1; // pow = 2 ^ parity bits
            while(pow < tamanho+1) {
                parityBits += 1;
                pow *= 2;
            }

            // Calcula todos os valores de paridade
            vector<bool> xorsum(parityBits, 0);
            for(int i=0; i<tamanho; i++) {
                for(int j=0; j<parityBits; j++) {
                    if ((i+1) & (1 << j)) {
                        xorsum[j] = xorsum[j] ^ quadro[i];
                    }
                }
            }
            
            // Verifica se há erros 
            int error = 0;
            for(int j=0; j<parityBits; j++) {
                if (xorsum[j]) error += (1 << j);
            }

            // Assumindo que pode haver erro em um único bit,
            // Imprime o bit que teve inversão e o corrige
            if (error) {
                cout << "ERRO - inversao no bit {" << error << "} \n" << endl;
                quadro[error-1] = !quadro[error-1];
            }
            else {
                cout << "SEM ERRO - todos os bits de paridade estao condizentes \n" << endl;
            }
            
            // Remove os bits de paridade
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

        // Função que escolhe o método de desenquadramento
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

        // Remove o cabeçalho de 4 bytes
        vector<bool> contagemDeCaracteres(vector<bool> quadroEnquadrado) {
            vector<bool> quadroDesenquadrado;

            // Copia os bits do quadro a partir do 33º bit
            int tamanho = (int) quadroEnquadrado.size();
            for(int i=32; i<tamanho; i++) {
                quadroDesenquadrado.push_back(quadroEnquadrado[i]);
            }

            return quadroDesenquadrado;

        }
        
        // Remove as flag e os bits inseridos pelo bit stuffing
        vector<bool> insercaoDeBytes(vector<bool> quadroEnquadrado) {

            vector<bool> quadroDesenquadrado;
            int counter = 0;
            bool foundFlag = 0;

            for(auto bit : quadroEnquadrado) {

                // Detectou uma flag, remove os 7 primeiros bits
                // E pula o 8º
                if (foundFlag) {
                    foundFlag = 0;
                    for(int i=0; i<7; i++)
                        quadroDesenquadrado.pop_back(); 
                    continue;
                }

                // Bit 1
                if (bit) {
                    // 6º '1' consecutivo
                    if (counter == 5) {
                        counter = 0;
                        foundFlag = 1;
                    }
                    else counter++;
                }

                // Bit 0
                else {
                    counter = 0;
                    // Achou um bit '0' adicional, remove-o pulando ele
                    if (counter == 5) { 
                        continue;
                    }
                }

                // Acrescenta o bit
                quadroDesenquadrado.push_back(bit);
            }

            return quadroDesenquadrado;
        }
    }

}