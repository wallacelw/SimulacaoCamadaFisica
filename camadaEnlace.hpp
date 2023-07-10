#ifndef CAMADAENLACE_HPP
#define CAMADAENLACE_HPP

#include <bits/stdc++.h>
using namespace std;

namespace CamadaEnlace {

    extern int tipoCodificacao;
    extern int tipoEnquadramento;

    namespace Transmissora {

        void chamada(vector<bool> quadro);
        vector<bool> enquadramento(vector<bool> quadro);
        vector<bool> contagemDeCaracteres(vector<bool> quadro);
        vector<bool> insercaoDeBytes(vector<bool> quadro);

    }

    namespace Receptora {

        void chamada(vector<bool> quadro);
        vector<bool> desenquadramento(vector<bool> quadro);
        vector<bool> contagemDeCaracteres(vector<bool> quadro);
        vector<bool> insercaoDeBytes(vector<bool> quadro);

    }

}

#endif