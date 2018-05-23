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
#ifndef STRUCT_HPP
#define STRUCT_HPP

#include <string>

struct userList {
  int tamanho;
  char nome[100][100];
};

struct messageList {
  int tamanho;
  char mensagem[100][100];
};


struct stringUser {
  std::string name;
  std::string userName;
  std::string senha;
  std::string idade;
  std::string email;
  std::string telefone;
};

struct User {
  char name[50];
  char userName[50];
  char senha[50];
  char idade[5];
  char email[50];
  char telefone[20];
};

struct Voo {
  char origem[50];
  char destino[50];
  char data[15];
  char hora[8];
  char quantidade[5];
  char preco[10];
};

struct stringVoo {
  std::string origem;
  std::string destino;
  std::string data;
  std::string hora;
  std::string quantidade;
  std::string preco;
};

struct listaVoo{
	int tamanho;
  char id[3][100];
  char origem[50][100];
  char destino[50][100];
  char data[15][100];
  char hora[8][100];
  char quantidade[10][100];
  char preco[10][100];
};

#endif
