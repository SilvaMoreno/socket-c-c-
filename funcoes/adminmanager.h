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
*/#ifndef  ADMINMANAGER_HPP

#define ADMINMANAGER_HPP

#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "../split/split.h"
#include "message.h"
#include "gettime.h"

int checkAdmin(std::string senha){
  std::ifstream afile("file/admin.txt", std::ios::in);
  if (afile.is_open()) {
    std::string line;
    std::getline(afile, line);

    if (dividir(line)[1].compare( senha ) == 0) {
        return SUCESS;
    }else{
      return INVALIDPASSWORD;
    }
    afile.close();
  }
  else {
    std::ofstream afile("file/admin.txt", std::ios::out);
    if (afile.is_open()) {
      afile << "senha-" << senha << "\n";
      afile.close();
      return SUCESS;
    }
  }
}

void savePortaCliente(std::string usuario, std::string ip, std::string porta, std::string estado) {
  std::string dados;
  std::ifstream inFile("file/log.txt", std::ios::in);
  if (inFile.is_open()) {
    std::string line;
    while (std::getline(inFile, line)) {
    //  std::cout << line << '\n';
    dados += line + "\n";
    }
    inFile.close();
  }
  else {
    std::cerr << "Ficheiro inixixtente.\n";
  }

  std::ofstream afile("file/log.txt", std::ios::out);
  if (afile.is_open()) {
    if (estado == "saida") {
      afile << dados << "Saiu [" << getNow() << "] - " << usuario << " : IP=" << ip << " Porta=" << porta;
    }else{
      afile << dados << "Entrou [" << getNow() << "] - " << usuario << " : IP=" << ip << " Porta=" << porta;

    }
    afile.close();
  }
}

#endif
