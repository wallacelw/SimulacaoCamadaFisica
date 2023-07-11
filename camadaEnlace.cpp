#include <bits/stdc++.h>
#include "camadaAplicacao.hpp"
#include "camadaEnlace.hpp"
#include "camadaFisica.hpp"
using namespace std;

template <typename... A> void dbg(A const&... a) { ((cerr << "{" << a << "} "), ...); cerr << endl; }

namespace CamadaEnlace {

    int tipoEnquadramento;

    namespace Transmissora {

        void chamada(vector<bool> quadro) {

            vector<bool> quadroEnquadrado = enquadramento(quadro);

            CamadaFisica::Transmissora::chamada(quadroEnquadrado);
        }

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

            // add header (4 bytes)
            for(int j=0; j<32; j++) {
                if (tamanho & (1 << j)) quadroEnquadrado.push_back(1);
                else quadroEnquadrado.push_back(0);
            }

            for(auto bit : quadro) quadroEnquadrado.push_back(bit);

            return quadroEnquadrado;
        }
        // flag = 0x7E = 0111 1110; bit stuffing
        vector<bool> insercaoDeBytes(vector<bool> quadro) {
            vector<bool> quadroEnquadrado = {0, 1, 1, 1, 1, 1, 1, 0};

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

            vector<bool> flag = {0, 1, 1, 1, 1, 1, 1, 0};
            for(auto bit : flag) quadroEnquadrado.push_back(bit);

            return quadroEnquadrado;
        }
    }

    namespace Receptora {

        void chamada(vector<bool> quadroEnquadrado) {
            vector<bool> quadro = desenquadramento(quadroEnquadrado);

            CamadaAplicacao::Receptora::chamada(quadro);
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
            queue<bool> fila;
            int counter = 0;
            bool foundFlag = 0;

            for(auto bit : quadroEnquadrado) {
                if (foundFlag) {
                    foundFlag = 0;
                    fila = queue<bool>(); // reset
                    continue;
                }

                if (bit) {
                    if (counter == 5) {
                        foundFlag = 1;
                        continue;
                    }
                    counter++;
                }
                else {
                    if (counter == 5) {
                        // remove aditional 0
                        continue;
                    }
                    counter = 0;
                }

                fila.push(bit);
                if (fila.size() > 8) {
                    quadroDesenquadrado.push_back( fila.front() );
                    dbg(fila.front());
                    fila.pop();
                }
            }

            return quadroDesenquadrado;

        }
    }

}