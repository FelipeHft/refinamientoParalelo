/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>
#define PI 3.14159265
using namespace std;

struct coordenada{
    float x;
    float y;
};


void angulos(coordenada a, coordenada b, coordenada c, float &anguloA, float &anguloB, float &anguloC){
    coordenada AB, BC, CA;
    AB.x = b.x - a.x;
    AB.y = b.y - a.y;
    BC.x = c.x - b.x;
    BC.y = c.y - b.y;
    CA.x = a.x - c.x;
    CA.y = a.y - c.y;
    float moduloAB = sqrt((AB.x * AB.x) + (AB.y * AB.y));
    float moduloBC = sqrt((BC.x * BC.x) + (BC.y * BC.y));
    float moduloCA = sqrt((CA.x * CA.x) + (CA.y * CA.y));
    float escalarABCA = (AB.x * CA.x) + (AB.y * CA.y);
    float escalarABBC = (AB.x * BC.x) + (AB.y * BC.y);
    float escalarBCCA = (BC.x * CA.x) + (BC.y * CA.y);
 
    float anguloARad = acos (fabs(escalarABCA/(moduloAB * moduloCA)));
    float anguloBRad = acos (fabs(escalarABBC/(moduloAB * moduloBC)));
    float anguloCRad = acos (fabs(escalarBCCA/(moduloBC * moduloCA)));
    anguloA = (anguloARad * 180) / PI;
    anguloB = (anguloBRad * 180) / PI;
    anguloC = (anguloCRad * 180) / PI;
    
}

void lados(coordenada a, coordenada b, coordenada c, float &ladoAB, float &ladoBC, float &ladoCA){
    ladoAB = sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
    ladoBC = sqrt(pow((c.x - b.x), 2) + pow((c.y - b.y), 2));
    ladoCA = sqrt(pow((c.x - a.x), 2) + pow((c.y - a.y), 2));
}

void separar(string linea, string &n1, string &n2, string &n3){
    vector<string> nodos;
    size_t found,found2;

    //string cadena ="2132312314535647484 123312 2233124";
    found = linea.find(" ");
    found2 = linea.find(" " , found+1);
    nodos.push_back(linea.substr(0,found));
    nodos.push_back(linea.substr(found+1,found2-found-1));
    nodos.push_back(linea.substr(found2+1,linea.size()));
    n1 = nodos[0];
    n2 = nodos[1];
    n3 = nodos[2];
}

void leer(){
    string rutaEntradaMesh = "espiral.mesh";
    string rutaEntradaNode = "espiral.node";
    string lineaMesh;
    string lineaNode;
    coordenada coordNodo1, coordNodo2, coordNodo3;
    int nombre = 5001;
    int existe = 0;
    
    ifstream archivo_mesh;
    ifstream archivo_node;

    ofstream archivo_salida_node;
    
    float anguloPrueba = 40.35;
    
    archivo_mesh.open(rutaEntradaMesh, ios::in); //abriendo archivo modo lectura
    archivo_node.open(rutaEntradaNode, ios::in);
    archivo_salida_node.open("espiralAux.node", ios::out);
    
    if(archivo_mesh.fail() || archivo_node.fail() || archivo_salida_node.fail()){
        //si no se puede abrir el archivo o crear se termina el programa
        cout<<"Error con los archivos!"<<endl;
    }
    else{
        getline(archivo_mesh, lineaMesh);
        while(getline(archivo_mesh, lineaMesh)){ //recorre archivo txt  
            float anguloA, anguloB, anguloC;
            float ladoAB, ladoBC, ladoCA;
            string nodo1, nodo2, nodo3;
            separar(lineaMesh, nodo1, nodo2, nodo3);
            while(getline(archivo_node, lineaNode)){
                string nodo, valorX, valorY;
                separar(lineaNode, nodo, valorX, valorY);
                if(nodo1 == nodo){
                    coordNodo1.x = stof(valorX);
                    coordNodo1.y = stof(valorY);
                }
                if(nodo2 == nodo){
                    coordNodo2.x = stof(valorX);
                    coordNodo2.y = stof(valorY);
                }
                if(nodo3 == nodo){
                    coordNodo3.x = stof(valorX);
                    coordNodo3.y = stof(valorY);
                }
            }
            archivo_node.clear();
            archivo_node.seekg(0, ios::beg);
            angulos(coordNodo1, coordNodo2, coordNodo3, anguloA, anguloB, anguloC);
            if(anguloA <= anguloPrueba || anguloB <= anguloPrueba || anguloC <= anguloPrueba){
                coordenada nuevoNodo;
                string lineaSalidaNode;
                lados(coordNodo1, coordNodo2, coordNodo3, ladoAB, ladoBC, ladoCA);
                if(ladoAB >= ladoBC && ladoAB >= ladoCA){
                    nuevoNodo.x = (coordNodo1.x + coordNodo2.x) / 2;
                    nuevoNodo.y = (coordNodo1.y + coordNodo2.y) / 2;
                }
                if(ladoBC >= ladoAB && ladoBC >= ladoCA){
                    nuevoNodo.x = (coordNodo2.x + coordNodo3.x) / 2;
                    nuevoNodo.y = (coordNodo2.y + coordNodo3.y) / 2;
                }
                if(ladoCA >= ladoBC && ladoCA >= ladoAB){
                    nuevoNodo.x = (coordNodo3.x + coordNodo1.x) / 2;
                    nuevoNodo.y = (coordNodo3.y + coordNodo1.y) / 2;
                }
                while(getline(archivo_node, lineaNode)){//revisar si el nodo existe
                    string nodo, valorX, valorY;
                    separar(lineaNode, nodo, valorX, valorY);
                    if(valorX == to_string(nuevoNodo.x) && valorY == to_string(nuevoNodo.y)){
                        existe = 1;
                    }
                }
                archivo_node.clear();
                archivo_node.seekg(0, ios::beg);
                if(existe == 0){
                    lineaSalidaNode = to_string(nombre) +" "+ to_string(nuevoNodo.x) +" "+ to_string(nuevoNodo.y);
                    archivo_salida_node << lineaSalidaNode << "\n";
                    nombre++;
                }
                else{
                    existe = 0;
                }
            }
        }
    }
    archivo_mesh.close();
    archivo_node.close();
    archivo_salida_node.close();
}

void archivoFinalNode(){
    string rutaEntradaNode = "espiral.node";
    string rutaEntradaNodeAux = "espiralAux.node";
    string rutaSalida = "espiralFinal.node";
    string lineaNode;
    string lineaNodeAux;
    ifstream archivo_node;
    ifstream archivo_node_aux;
    ofstream archivo_salida_node;
    archivo_node.open(rutaEntradaNode, ios::in);
    archivo_node_aux.open(rutaEntradaNodeAux, ios::in);
    archivo_salida_node.open(rutaSalida, ios::out);
    
    if(archivo_node.fail() || archivo_node_aux.fail() || archivo_salida_node.fail()){
        //si no se puede abrir el archivo o crear se termina el programa
        cout<<"Error con los archivos!"<<endl;
    }
    else{
        while(getline(archivo_node, lineaNode)){
            archivo_salida_node << lineaNode << "\n";
        }
        while(getline(archivo_node_aux, lineaNodeAux)){
            archivo_salida_node << lineaNodeAux << "\n";
        }
    }
    
    archivo_node.close();
    archivo_node_aux.close();
    archivo_salida_node.close();
}

void conforme(){
    string rutaEntradaMesh = "espiral.mesh";
    string rutaEntradaNodeFinal = "espiralFinal.node";
    string rutaSalida = "espiralFinal.mesh";
    string lineaMesh;
    string lineaNode;
    coordenada coordNodo1, coordNodo2, coordNodo3;
    coordenada puntoMedioN1N2, puntoMedioN2N3, puntoMedioN3N1;
    ifstream archivo_mesh;
    ifstream archivo_node;
    ofstream archivo_salida_mesh;
    archivo_mesh.open(rutaEntradaMesh, ios::in);
    archivo_node.open(rutaEntradaNodeFinal, ios::in);
    archivo_salida_mesh.open(rutaSalida, ios::out);
    int c=0;
    int c1=0;
    int c2=0;
    int c3=0;
    
    if(archivo_mesh.fail() || archivo_node.fail() || archivo_salida_mesh.fail()){
        //si no se puede abrir el archivo o crear se termina el programa
        cout<<"Error con los archivos!"<<endl;
    }
    else{
        getline(archivo_mesh, lineaMesh);
        while(getline(archivo_mesh, lineaMesh)){
            string nodo1, nodo2, nodo3;
            separar(lineaMesh, nodo1, nodo2, nodo3);
            while(getline(archivo_node, lineaNode)){
                string nodo, valorX, valorY;
                separar(lineaNode, nodo, valorX, valorY);
                if(nodo1 == nodo){
                    coordNodo1.x = stof(valorX);
                    coordNodo1.y = stof(valorY);
                }
                if(nodo2 == nodo){
                    coordNodo2.x = stof(valorX);
                    coordNodo2.y = stof(valorY);
                }
                if(nodo3 == nodo){
                    coordNodo3.x = stof(valorX);
                    coordNodo3.y = stof(valorY);
                }
            }
            archivo_node.clear();
            archivo_node.seekg(0, ios::beg);
            puntoMedioN1N2.x = (coordNodo1.x + coordNodo2.x) / 2;
            puntoMedioN1N2.y = (coordNodo1.y + coordNodo2.y) / 2;
            puntoMedioN2N3.x = (coordNodo2.x + coordNodo3.x) / 2;
            puntoMedioN2N3.y = (coordNodo2.y + coordNodo3.y) / 2;
            puntoMedioN3N1.x = (coordNodo3.x + coordNodo1.x) / 2;
            puntoMedioN3N1.y = (coordNodo3.y + coordNodo1.y) / 2;
            while(getline(archivo_node, lineaNode)){
                string nodo, valor1, valor2;
                separar(lineaNode, nodo, valor1, valor2);
                if(to_string(puntoMedioN1N2.x) == valor1 && to_string(puntoMedioN1N2.y) == valor2 && c==0){
                    archivo_salida_mesh << nodo1 << " " << nodo << " " << nodo3 << "\n";
                    archivo_salida_mesh << nodo2 << " " << nodo << " " << nodo3 << "\n";
                    c++;
                }
                else if(to_string(puntoMedioN2N3.x) == valor1 && to_string(puntoMedioN2N3.y) == valor2 && c1==0){
                    archivo_salida_mesh << nodo2 << " " << nodo << " " << nodo1 << "\n";
                    archivo_salida_mesh << nodo3 << " " << nodo << " " << nodo1 << "\n";
                    c1++;
                }
                else if(to_string(puntoMedioN3N1.x) == valor1 && to_string(puntoMedioN3N1.y) == valor2 && c2==0){
                    archivo_salida_mesh << nodo3 << " " << nodo << " " << nodo2 << "\n";
                    archivo_salida_mesh << nodo1 << " " << nodo << " " << nodo2 << "\n";
                    c2++;
                }
                else{
                    if (c3==0)
                    archivo_salida_mesh << nodo1 << " " << nodo2 << " " << nodo3 << "\n";
                    c3++;
                }
            }
            c,c1,c2,c3=0;
            archivo_node.clear();
            archivo_node.seekg(0, ios::beg);
        }
    }
    archivo_mesh.close();
    archivo_node.close();
    archivo_salida_mesh.close();   
}