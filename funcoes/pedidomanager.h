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
#ifndef  PEDIDOMANAGER_HPP
#define PEDIDOMANAGER_HPP

#include <fstream>

#define MAXALLOC 500

bool addPedido(std::string usuario, std::string voo, std::string quantidade){
  bool estado = false;
  std::string dados;
  std::ifstream inputFile("file/pedido.txt", std::ios::in);
  if (inputFile.is_open()) {
    std::string line;
    while (std::getline(inputFile, line)) {
      dados += line + "\n";
    }
    inputFile.close();
  }
  else {
    std::cerr << "Ficheiro pedido inacessivel.\n";
  }

  std::ofstream afile("file/pedido.txt", std::ios::out);
  if (afile.is_open()) {
    afile << dados << usuario << "-" << voo << "-" << quantidade;
    afile.close();
    estado = true;
  }
  return estado;
}


std::string getMeusPedidos(std::string user){

  std::string dado, pedido, idVoo;

  std::ifstream readFile("file/pedido.txt", std::ios::in);
  if (readFile.is_open()) {

    std::string line;

    while (std::getline(readFile, line)) {
      dado = dividir(line)[0];
      if (user.compare( dado ) == 0) {

        idVoo = dividir(line)[1];

        std::ifstream afile("file/voos.txt", std::ios::in);
        if (afile.is_open()) {
          std::string lineVoo;
          while (std::getline(afile, lineVoo)) {
            if( (dividir(lineVoo)[0]).compare( idVoo )  == 0 ){
              pedido += dividir(line)[2] + "-" + lineVoo + "\n";
            }
          }
          afile.close();
        }
        else {
          std::cerr << "Unable to open file\n";
        }


      }

    }
    readFile.close();
  }
  else {
    std::cerr << "Ficheiro pedido inacessivel.\n";
  }

  return pedido;
  //std::cout << "eeeeeeeeeeeeeeeeeeeeeeeeeeee\n\nPedidos\n" << pedido << '\n';
}

#endif
