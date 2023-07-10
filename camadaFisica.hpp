#ifndef CAMADAFISICA_HPP
#define CAMADAFISICA_HPP

#include <bits/stdc++.h>
using namespace std;

#include <bits/stdc++.h>
#include "camadaAplicacao.hpp"
#include "camadaEnlace.hpp"
#include "camadaFisica.hpp"
using namespace std;


namespace CamadaFisica {

    extern int tipoCodificacao;

    namespace Transmissora {

        void chamada(vector<bool> quadro);
        vector<bool> codificacaoBinaria(vector<bool> quadro);
        vector<bool> codificacaoManchester(vector<bool> quadro);
        vector<bool> codificacaoBipolar(vector<bool> quadro);

    }

    void meioDeComunicacao(vector<bool> fluxoBrutoDeBits);

    namespace Receptora {
       
        void chamada(vector<bool> onda);
        vector<bool> decodificacaoBinaria(vector<bool> onda);
        vector<bool> decodificacaoManchester(vector<bool> onda);
        vector<bool> decodificacaoBipolar(vector<bool> onda);

    }

}   


#endif