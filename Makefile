
simulador.exe: simulador.o camadaFisica.o
	g++ simulador.o camadaFisica.o -o simulador.exe

simulador.o: simulador.cpp
	g++ -c simulador.cpp

camadaFisica.o: camadaFisica.cpp
	g++ -c camadaFisica.cpp

clean:
	rm -rf *.o simulador.exe
	