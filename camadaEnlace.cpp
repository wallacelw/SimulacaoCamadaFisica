#include <bits/stdc++.h>
#include "camadaAplicacao.hpp"
#include "camadaEnlace.hpp"
#include "camadaFisica.hpp"
using namespace std;

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

        vector<bool> insercaoDeBytes(vector<bool> quadro) {
            char flag = '~'; // = 0x7E = 0111 1110

            string quadroEnquadrado_str;

            // add flag 
            quadroEnquadrado_str.push_back(flag);

            for(int i=0; i<(int)quadro.size()/8; i++) {
                char c = 0;
                for(int j=0; j<8; j++) {
                    if (quadro[i*8 + j]) c += (1 << j);
                }
                // add ESC character before ESC and ~
                if (c == 0x1B or c == 0x7E) {
                    quadroEnquadrado_str.push_back(0x1B);
                }
                quadroEnquadrado_str.push_back(c);
            }

            // add flag 
            quadroEnquadrado_str.push_back(flag);

            vector<bool> quadroEnquadrado;

            for(char c : quadroEnquadrado_str) {
                for(int i=0; i<8; i++) {
                    if (c & (1 << i)) quadroEnquadrado.push_back(1);
                    else quadroEnquadrado.push_back(0);
                }
            } 

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

            string quadroDesenquadrado_str;
            bool lastWasESC = 0;

            for(int i=0; i<(int)quadroEnquadrado.size()/8; i++) {
                char c = 0;
                for(int j=0; j<8; j++) {
                    if (quadroEnquadrado[i*8 + j]) c += (1 << j);
                }

                // escaping
                if (lastWasESC) {
                    lastWasESC = 0;
                    quadroDesenquadrado_str.push_back(c);
                }
                // found ESC
                else if (c == 0x1B) {
                    lastWasESC = 1;
                }
                // found FLAG
                else if (c == 0x7E) {
                    continue;
                }
                else {
                    quadroDesenquadrado_str.push_back(c);
                }
            }

            vector<bool> quadroDesenquadrado;

            for(char c : quadroDesenquadrado_str) {
                for(int i=0; i<8; i++) {
                    if (c & (1 << i)) quadroDesenquadrado.push_back(1);
                    else quadroDesenquadrado.push_back(0);
                }
            } 

            return quadroDesenquadrado;

        }
    }

}