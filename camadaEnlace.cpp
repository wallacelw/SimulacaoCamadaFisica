#include <bits/stdc++.h>
#include "camadaAplicacao.hpp"
#include "camadaEnlace.hpp"
#include "camadaFisica.hpp"
using namespace std;

namespace CamadaEnlace {

    int tipoEnquadramento;
    int tipoCodificacao;

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
            vector<bool> quadroEnquadrado = quadro;
            return quadroEnquadrado;
        }

        vector<bool> insercaoDeBytes(vector<bool> quadro) {
            vector<bool> quadroEnquadrado = quadro;
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
            vector<bool> quadroDesenquadrado = quadroEnquadrado;
            return quadroDesenquadrado;

        }

        vector<bool> insercaoDeBytes(vector<bool> quadroEnquadrado) {
            vector<bool> quadroDesenquadrado = quadroEnquadrado;
            return quadroDesenquadrado;

        }
    }

}