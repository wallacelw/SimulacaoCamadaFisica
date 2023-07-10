#ifndef CAMADAAPLICACAO_HPP
#define CAMADAAPLICACAO_HPP

#include <bits/stdc++.h>
using namespace std;

namespace CamadaAplicacao {

    extern int tipoEnquadramento;
    extern int tipoCodificacao;

    namespace Transmissora {
        void aplicacao();
        void chamada(string mensagem);
    }

    namespace Receptora {
        void chamada(vector<bool> quadro);
        void aplicacao(string mensagem);
    }

    void imprimirSinal(vector<bool> &sinal, int bits, string mensagem);
}

#endif