ARQUIVOS1 = camadaAplicacao.cpp camadaEnlace.cpp camadaFisica.cpp simulador.cpp
ARQUIVOS2 = camadaAplicacao.hpp camadaEnlace.hpp camadaFisica.hpp

all: final

final: $(ARQUIVOS1) $(Arquivos2)
	g++ -std=c++17 -o simulador.exe $(ARQUIVOS1) $(ARQUIVOS2)