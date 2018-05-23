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
#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "struct.h"
#include "../split/split.h"


bool addUser(struct stringUser u) {
  bool estado = false;
  std::string dados;

  std::ifstream afileIn("file/usuario.txt", std::ios::in);
  if (afileIn.is_open()) {
    std::string line;
    while (std::getline(afileIn, line)) {
      //std::cout << line << '\n';
      dados += line + "\n";
    }
    afileIn.close();

  }
  else {
    std::cerr << "Impóssivel abrir o ficlheiro\n";
    estado = false;
  }

std::ofstream afileOut("file/usuario.txt", std::ios::out);
  if (afileOut.is_open()) {
      afileOut << dados << u.name << "-" << u.userName << "-" << u.senha << "-" << u.idade << "-" << u.email << "-" << u.telefone << '\n';
      afileOut.close();
      estado = true;
  }
  else {
    std::cerr << "Imposivel abrir o ficlheiro\n";
    estado = false;
  }

  return estado;
}

bool checkUserName(std::string userName){
  bool estado = false;
  std::ifstream afile("file/usuario.txt", std::ios::in);
  if (afile.is_open()) {
    std::string line;
    while (std::getline(afile, line)) {
        std::string user = dividir(line)[1];
        if (user.compare(userName) == 0) {
          estado = true;//existe nome
        }
    }
    afile.close();
  }
  else {
    std::cerr << "Unable to open file\n";
  }
  return estado;
}

bool userLogin(std::string userName, std::string senha){
  bool estado = false;
  std::ifstream afile("file/usuario.txt", std::ios::in);
  if (afile.is_open()) {
    std::string line;
    while (std::getline(afile, line)) {
        std::string user = dividir(line)[1];
        if (user.compare(userName) == 0) {
          user = dividir(line)[2];
          if (user.compare(senha) == 0) {
              estado = true;
          }
        }
    }
    afile.close();
  }
  else {
    std::cerr << "Unable to open file\n";
  }
  return estado;
}

#endif
