/*
  Universidade de Cabo Verde
  1º Projeto de Sistema Distribuidos
  3º Ano 2º Semestre
  Programado por:
    Elton Moreno
    Kelton Cabral
    Nataniel Pina

  Docente:
    Eloy Tavares
*/
#ifndef  VOOMANAGER_HPP
#define VOOMANAGER_HPP

#include <fstream>

#include "gettime.h"
#include "../split/split.h"
#include "interface.h"

#define MAXALLOC 500

bool addVoo(struct stringVoo voo){
  std::string dados;
  bool estado = true;
  int id = 1;

  std::ifstream inFile("file/voos.txt", std::ios::in);
  if (inFile.is_open()) {
    std::string line;
    while (std::getline(inFile, line)) {
      dados += line + '\n';
      id++;
    }
    inFile.close();
  }
  else {
    std::cerr << "Ficheiro não existe.\n";
    estado = false;
  }

  std::ofstream afile("file/voos.txt", std::ios::out);
  if (afile.is_open()) {
    afile << dados << id << "-" << voo.origem << "-" << voo.destino << "-" << voo.data << "-" << voo.hora << "-" << voo.quantidade  << "-" << voo.preco;
    afile.close();
    estado = true;
  }else{
    std::cerr << "Erro ao gravar dados." << '\n';
    estado = false;
  }

  return estado;
}


void printVooList(char *dado){

    std::cout << "\n------------------------------- " << dividir(dado)[1] << "-------------------------------\n";
    std::cout << "Origem: " << dividir(dado)[2] << '\n';
    std::cout << "Destino: " << dividir(dado)[3] << '\n';
    std::cout << "Data: " << dividir(dado)[4] << '\n';
    std::cout << "Hora: " << dividir(dado)[5] << '\n';
    std::cout << "Quantidade: " << dividir(dado)[6] << '\n';
    std::cout << "Preço: " << dividir(dado)[7] << '\n';
}


#endif
