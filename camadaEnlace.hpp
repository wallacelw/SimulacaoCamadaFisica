#ifndef CAMADAENLACE_HPP
#define CAMADAENLACE_HPP

#include <bits/stdc++.h>
using namespace std;

namespace CamadaEnlace {

    extern int tipoEnquadramento;
    extern int tipoTratamento;
    namespace Transmissora {

        void chamada(vector<bool> quadro);
        vector<bool> tratamentoErro(vector<bool> quadro);
        vector<bool> paridade(vector<bool> quadro);
        vector<bool> crc(vector<bool> quadro);
        vector<bool> hamming(vector<bool> quadro);
        vector<bool> enquadramento(vector<bool> quadro);
        vector<bool> contagemDeCaracteres(vector<bool> quadro);
        vector<bool> insercaoDeBytes(vector<bool> quadro);

    }

    namespace Receptora {

        void chamada(vector<bool> quadro);
        vector<bool> tratamentoErro(vector<bool> quadro);
        vector<bool> paridade(vector<bool> quadro);
        vector<bool> crc(vector<bool> quadro);
        vector<bool> hamming(vector<bool> quadro);
        vector<bool> desenquadramento(vector<bool> quadro);
        vector<bool> contagemDeCaracteres(vector<bool> quadro);
        vector<bool> insercaoDeBytes(vector<bool> quadro);

    }

}

#endif