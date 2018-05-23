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
#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <iostream>
#include <string.h>
#include <unistd.h>

#include "gettime.h"

void printHeader(char * indentifier){
	int i;
	int size = strlen(indentifier);
	int padding = (78 - size)/2;
	system("clear");
	std::cout << "--------------------------------------------------------------------------------\n";

	for(i=0;i<padding; i++){
		std::cout << "#";
	}

  std::cout << " " << indentifier << " ";

	for(i=0;i<padding; i++){
		std::cout << "#";
	}

	std::cout << "                                        Último acesso: " << getHourMinutesSeconds() << std::endl;
  std::cout << "--------------------------------------------------------------------------------\n";

}

void printMenuOptions(){
	std::cout << '\n';
	std::cout << " 1 | Conectar-se" << '\n';
  std::cout << " 2 | Registar" << '\n';
  std::cout << " 3 | Administrador" << '\n';
	std::cout << " 4 | Funcionalidades" << "\n";
	std::cout << " 5 | Sair" << '\n';
	std::cout << '\n';
	std::cout << "Escolha sua opção: " << '\n';
}

void printSecondMenuOptionsCliente(){
  std::cout << '\n';
	std::cout << " 1 | Compara Passagens" << '\n';
	std::cout << " 2 | Ver Passagens" << "\n";
	std::cout << " 3 | Minha Conta" << '\n';
  std::cout << " 4 | Chat" << '\n';
  std::cout << " 5 | voltar" << '\n';
	std::cout << '\n';
	std::cout << "Escolha sua opção: " << '\n';
}

void printMenuOptionsAdmin() {
	std::cout << '\n';
	std::cout << " 1 | Adicionar Passagems" << '\n';
	std::cout << " 2 | Ver Passagens" << "\n";
	std::cout << " 3 | Alterar Conta" << '\n';
  std::cout << " 4 | Chat" << '\n';
  std::cout << " 5 | voltar" << '\n';
	std::cout << '\n';
	std::cout << "Escolha sua opção: " << '\n';
}


void printAlert(char *message){
	int i;
	int size = strlen(message);
	int padding = (76 - size)/2;

	std::cout << '\n';
  std::cout << '\n';

	for(i=0;i<padding; i++){

    std::cout << " ";
	}

  std::cout << " >> " << message << " << " << '\n';

	for(i=0;i<padding; i++){
		std::cout << " ";
	}
	std::cout << '\n';
}

void printMessageList(struct messageList messages){
	int i = 0;

	for(i=0; i < messages.tamanho; i++){
		std::cout << "> " << messages.mensagem[i] << '\n';
	}
}

void faetures(){
	printHeader("Funcionalidades");
	printAlert("Sistema de Voos versão beta é simples e intuitivo!");
	std::cout << '\n';
	std::cout << "Siga os passos:" << '\n';
	std::cout << "* Escolha a opçao conectar-se." << '\n';
  std::cout << "         *Informe o seu nome utilizador e a senha correta para ter acesso." << '\n';
	std::cout << "* Escolha a opção Registar-se." << '\n';
	std::cout << "         *Informe os dados pedidos para se cadastrar." << '\n';
	std::cout << "* Escolha a opção Administrador:" << '\n';
	std::cout << "         *Para adimistrar a agencia." << '\n';
	std::cout << "* Escolha Chat para converçar" << '\n';
	std::cout << "         *> [Enter] para enviar uma mensagem para todo mundo." << '\n';
	std::cout << "         *@nome para emviar mensagem para uma pessoa." << '\n';
	std::cout << "         *l [Enter] para limpar." << '\n';
	std::cout << "         *v [Enter] para sair do chat." << '\n';
}

#endif
