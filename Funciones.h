#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <fstream>


using namespace std;

struct coordenada{
    float x;
    float y;
};

void angulos(coordenada, coordenada, coordenada);
void lados(coordenada, coordenada, coordenada);
void separa(string, string &, string &, string &);
void leer();
void archivoFinalNode();
void conforme();

#endif /* FUNCIONES_H */

