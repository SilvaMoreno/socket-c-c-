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
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

//#include "funcoes/usermanager.h"
#include "funcoes/struct.h"
#include "funcoes/gettime.h"
#include "funcoes/interface.h"
#include "funcoes/message.h"
#include "funcoes/kbhit.h"
#include "funcoes/voomanager.h"
#include "funcoes/pedidomanager.h"
#include "funcoes/adminmanager.h"

#define MAXDATASIZE 500
#define MAXALLOC 500

std::string getUserName();

int main(int argc, char const *argv[]) {

  system("clear");

  if (argc != 3) {
    std::cerr << "Execução correta ./client ip porta." << '\n';
    exit(0);
  }

  //tipo de dados representa IDs de processo
  pid_t pid;
  std::string c = argv[2];
  int porta = atoi(c.c_str());
  char username[50], senha[50], *messageChat, *dados, *dadosCadastro;
  int localSocket, conecao;
  struct sockaddr_in remoto;
  int opt, opt1, opt2, flow, numBytes, chat;
  char bufferKey, *bufferSend, *bufferRcv, *_bufferRcv;
  char senhaAntiga[50], usuario[50];

  struct messageList messages;

  std::string portaadress = argv[2];
  std::string ipadress = argv[1];

  //Alocação de memória
	bufferRcv = (char *) calloc (MAXALLOC, sizeof(char));
	_bufferRcv = (char *) calloc (MAXALLOC, sizeof(char));
	bufferSend = (char *) calloc (MAXALLOC, sizeof(char));
  dados = (char *) calloc (MAXALLOC, sizeof(char));
  dadosCadastro = (char *) calloc (MAXALLOC, sizeof(char));
  messageChat = (char *) calloc (MAXALLOC, sizeof(char));

  //criar socket
  localSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (localSocket == -1) {
    std::cerr << "Erro ao criar socket" << '\n';
  }

  //configurar socket para a coneção
  remoto.sin_family = AF_INET;
  remoto.sin_port = htons(porta);
  remoto.sin_addr.s_addr = inet_addr(argv[1]);
  bzero(&(remoto.sin_zero), 8);

  //coneção ao host remoto
  conecao = connect(localSocket, (struct sockaddr *)&remoto, sizeof(remoto));
  if (conecao == -1) {
      std::cerr << "Erro ao criar a coneção" << '\n';
  }

//  printAnimation();

  do {
    printHeader("Sistema Voo");
    printMenuOptions();
    std::cin >> opt;

    //menu principal
    switch (opt) {
      case 1:{
            flow = 0;
            while (!flow) {

                printHeader("Login no sistema");
                std::cout << "nome usuario: ";
                std::cin >> username;
                std::cout << "senha: ";
                std::cin >> senha;

                strcpy(dados, "");
                strcat(dados, username);
                strcat(dados, "-");
                strcat(dados, senha);

                bufferSend = genarateMessage(dados, LOGIN, 0);
                send(localSocket, bufferSend, MAXDATASIZE, 0);

                numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                bufferRcv[numBytes] = '\0';
                strcpy(_bufferRcv, bufferRcv);

                if (checkMessage(_bufferRcv, 0) == CONNECTED) {
                  printAlert("Coneção feita com sucesso");
                  strcpy(usuario, username);
                  //savePortaCliente(usuario, ipadress, portaadress);
                  sleep(1);
                  flow = 1;
                }else if (checkMessage(_bufferRcv, 0) == INVALIDUSERNAME) {
                  printAlert("Nome utilizador não existe");
                  sleep(3);
                  flow = 0;
                }else if (checkMessage(_bufferRcv, 0) == INVALIDPASSWORD) {
                  printAlert("Senha Incorreta");
                  sleep(3);
                  flow = 0;
                }

            }

MENUCLIENTE:do {
              printHeader("Menu do Cliente");
              printSecondMenuOptionsCliente();
              std::cin >> opt2;

              switch (opt2) {
                case 1:{//fazer pedido de uma passagem
                  bufferSend = genarateMessage("", LISTAVOO, 0);
                  send(localSocket, bufferSend, MAXDATASIZE, 0);

                  numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                  bufferRcv[numBytes] = '\0';
                  strcpy(_bufferRcv, bufferRcv);

                  strcpy(dadosCadastro, "");
                  strcpy(dadosCadastro, _bufferRcv);

                  printHeader("Todos os Voos");
                  std::cout << "                                                  atualizado em  " << getHourMinutesSeconds();


                  while (checkMessage(_bufferRcv, 0) != FINISHED) {

                  //  std::cout << "conteudo original ==>> " << dadosCadastro << '\n';
                    printVooList(dadosCadastro);

                    numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                    bufferRcv[numBytes] = '\0';
                    strcpy(_bufferRcv, bufferRcv);

                    strcpy(dadosCadastro, _bufferRcv);
                  }
                  std::cout << "------------------------------- FIM DA LISTA -----------------------------------\n";
                  waitKey();
                  char voo[5], quant[3];
                  std::cout << "Qual Voo pretendes pegar?" << '\n';
                  std::cout << "\n\t 0 => cancelar" << '\n';
                  std::cout << "\t Numero de voo para escolher" << '\n';
                  std::cin >> voo;
                  if (strcmp(voo, "0") == 0) {
                    break;
                  }
                  std::cout << "Quantidade: ";
                  std::cin >> quant;
                  std::cout << "Usuario: " << usuario << " Voo: " << voo << "Quantidade: " << quant << '\n';

                  strcpy(dadosCadastro, "");
                  strcat(dadosCadastro, usuario);
                  strcat(dadosCadastro, "-");
                  strcat(dadosCadastro, voo);
                  strcat(dadosCadastro, "-");
                  strcat(dadosCadastro, quant);

                  bufferSend = genarateMessage(dadosCadastro, REGISTARPEDIDO, 0);
                  send(localSocket, bufferSend, MAXDATASIZE, 0);

                  numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                  bufferRcv[numBytes] = '\0';
                  strcpy(_bufferRcv, bufferRcv);

                    if(checkMessage(_bufferRcv, 0) == SUCESS) {
                      printAlert("Pedido feito com sucesso");
                      sleep(3);
                    }else{
                      printAlert("Erro. Pedido não registado");
                      sleep(3);
                    }
                  break;

                }
                case 2:{
                  //std::cout << "opt 2" << '\n';
                  bufferSend = genarateMessage(usuario, LISTAPEDIDO, 0);
                  send(localSocket, bufferSend, MAXDATASIZE, 0);

                  numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                  bufferRcv[numBytes] = '\0';
                  strcpy(_bufferRcv, bufferRcv);

                  strcpy(dadosCadastro, "");
                  strcpy(dadosCadastro, _bufferRcv);

                    printHeader("Meus Voos");

                    while (checkMessage(_bufferRcv, 0) != FINISHED ) {
                          std::cout << "\n------------------------------- " << "Voo " << dividir(dadosCadastro)[2] << "-------------------------------\n";
                          std::cout << "Origem: " << dividir(dadosCadastro)[3] << '\n';
                          std::cout << "Destino: " << dividir(dadosCadastro)[4] << '\n';
                          std::cout << "Data: " << dividir(dadosCadastro)[5] << '\n';
                          std::cout << "Hora: " << dividir(dadosCadastro)[6] << '\n';
                          std::cout << "Preço: " << ( atoi((dividir(dadosCadastro)[1]).c_str()) * atoi((dividir(dadosCadastro)[8]).c_str()) ) << '\n';

                          numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                          bufferRcv[numBytes] = '\0';
                          strcpy(_bufferRcv, bufferRcv);
                          strcpy(dadosCadastro, _bufferRcv);
                      }

                      std::cout << "------------------------------- FIM DA LISTA -----------------------------------\n";
                      sleep(1);

                      waitKey();
                  break;
                }
                case 3:{
                  std::cout << "opt 3" << '\n';

                  break;
                }
                case 4:{

                  printHeader("Conversas - Chat");
                  chat = 1;
                  while (chat) {
                    fflush(stdin);
                  //  while(!kbhit()){

                      strcpy(dadosCadastro, "");
                      strcat(dadosCadastro, usuario);

                      bufferSend = genarateMessage(dadosCadastro, UPDATECHAT,0);
                      send(localSocket, bufferSend, MAXDATASIZE, 0);


                       numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                       bufferRcv[numBytes] = '\0';
                       strcpy(_bufferRcv, bufferRcv);

                       //strcpy(dadosCadastro, "");
                       strcpy(dadosCadastro, _bufferRcv);
                        std::cout << "bufferRcv = " << _bufferRcv << '\n';
                       while(checkMessage(_bufferRcv,0) != FINISHED){

                         std::cout << dadosCadastro << '\n';

                         numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                         bufferRcv[numBytes] = '\0';
                         strcpy(_bufferRcv, bufferRcv);

                         strcpy(dadosCadastro, _bufferRcv);

                       }

                       if (checkMessage(_bufferRcv,0) != ERROR) {
                         std::cout << dadosCadastro << '\n';
                       }
                      //  usleep(500000);
                  //  }

                    //std::cout << "\t> mensagem\n\tl limpar\n\tv voltar" << '\n';
                    bufferKey = getchar();
                  //  std::cin >> bufferKey;
                    if (bufferKey == '>') {
                      std::cin.ignore();
                     VERIFIQUE:std::cout << "[" << getHourMinutes() << "] - Eu: ";
                      //std::cin >> messageChat;
                      scanf(" %[^\n]s", messageChat);
                      if (strlen(messageChat) < 32) {
                        std::cout << "minimo de caracteres 30" << '\n';
                        goto VERIFIQUE;
                      }else if (strlen(messageChat) > 250) {
                        std::cout << "Maximo de caracteres 250" << '\n';
                        goto VERIFIQUE;
                      }
                      std::cout << '\n';

                      strcpy(dadosCadastro, "");
                      strcat(dadosCadastro, usuario);
                      strcat(dadosCadastro, "-");
                      strcat(dadosCadastro, messageChat);

                      bufferSend = genarateMessage(dadosCadastro, MESSAGE,0);
											send(localSocket, bufferSend, MAXDATASIZE, 0);

                    } else if (bufferKey == 'v') {
                      chat = 0;
                    } else if (bufferKey == 'l') {
                      printHeader("Conversa - chat");
                    }else{
                      fflush(stdin);
                    }

                  }

                  break;
                }
                case 5:{
                  bufferSend = genarateMessage("", CLOSE, 0);
                  send(localSocket, bufferSend, MAXDATASIZE, 0);
                  break;
                }
              }
            } while(opt2 != 5);



        break;
      }
      case 2:{


        //criar conta de utilizador
        printHeader("Cadastro do Sistema de Voos");

          std::cin.ignore();

          struct User user;

          std::cout << "Nome Completo: ";
          std::cin.getline(user.name, 50);


          int flow = 0;

          LOOP:while (!flow) {

            std::cout << "Nome usuario: ";
            std::cin >> username;

            strcpy(dados, "");
            strcat(dados, username);
            std::cout << "User Name inserido = " << dados << '\n';

            bufferSend = genarateMessage(dados, VALIDUSERNAME, 0);
            send(localSocket, bufferSend, MAXDATASIZE, 0);

          //  std::cout << "bufferSend a ser enviado = " << bufferSend << '\n';

            numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
            bufferRcv[numBytes] = '\0';
            strcpy(_bufferRcv, bufferRcv);

            if (checkMessage(_bufferRcv, 0) == INVALIDUSERNAME) {
              std::cerr << "Nome de utilizador já esta em uso! tente um outro nome." << '\n';
              goto LOOP;
            }else if(checkMessage(_bufferRcv, 0) == VALIDUSERNAME) {
              strcpy(user.userName, username);
            strcpy(usuario, username);
              flow = 1;
            }

          }

          std::cout << "Senha: ";
          std::cin >> user.senha;

          std::cout << "Idade: ";
          std::cin >> user.idade;

          std::cout << "Email: ";
          std::cin >> user.email;

          std::cout << "Telefone: ";
          std::cin >> user.telefone;


          std::cout << user.name << " sen" << user.telefone << '\n';

          strcpy(dadosCadastro, "");
          strcat(dadosCadastro, user.name);
          strcat(dadosCadastro, "-");
          strcat(dadosCadastro, user.userName);
          strcat(dadosCadastro, "-");
          strcat(dadosCadastro, user.senha);
          strcat(dadosCadastro, "-");
          strcat(dadosCadastro, user.idade);
          strcat(dadosCadastro, "-");
          strcat(dadosCadastro, user.email);
          strcat(dadosCadastro, "-");
          strcat(dadosCadastro, user.telefone);

          bufferSend = genarateMessage(dadosCadastro, REGISTRAR, 0);
          send(localSocket, bufferSend, MAXDATASIZE, 0);

          numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
          bufferRcv[numBytes] = '\0';
          strcpy(_bufferRcv, bufferRcv);

          if (checkMessage(_bufferRcv, 0) == SUCESS) {
            printAlert("Conta criada com sucesso.");
            waitKey();
            goto MENUCLIENTE;
          }else if(checkMessage(_bufferRcv, 0) == ERROR) {
            printAlert("Erro conta não ciada");
            waitKey();
          }

          //std::cin.get();
        break;
      }
      SENHAINCORETO:case 3:{
        //para o Administrador

        char senha[20];

        printHeader("Administrador - Login");
        std::cout << "Senha: ";
        std::cin >> senha;

        strcpy(dadosCadastro, "");
        strcat(dadosCadastro, senha);

        bufferSend = genarateMessage(dadosCadastro, VALIDADMINPASSWORD, 0);
        send(localSocket, bufferSend, MAXDATASIZE, 0);

        numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
        bufferRcv[numBytes] = '\0';
        strcpy(_bufferRcv, bufferRcv);

        if (checkMessage(_bufferRcv, 0) == SUCESS) {
          std::cout << "Sucesso." << '\n';
          strcpy(usuario, "admin");
        }else if(checkMessage(_bufferRcv, 0) == INVALIDPASSWORD) {
          printAlert("Senha Incorreta");
          sleep(3);
          goto SENHAINCORETO;
        }
        strcpy(senhaAntiga, senha);

        do {
          printHeader("Sistema de Voo - Administração");
          printMenuOptionsAdmin();
          std::cin >> opt1;

          switch (opt1) {
            case 1:{//adicionar voos
                printHeader("Adicionar novo Voo");
                struct Voo voo;

                IGUAL:std::cout << "Origem: ";
                std::cin >> voo.origem;
                std::cout << "Destino: ";
                std::cin >> voo.destino;

                if (strcmp(voo.origem, voo.destino) == 0) {
                  std::cerr << "Não podes ter mesma origem e destino." << '\n';
                  goto IGUAL;
                }

                std::cout << "Data (dd/mm/yyyy): ";
                std::cin >> voo.data;

                std::cout << "Hora (HH:MM): ";
                std::cin >> voo.hora;

                std::cout << "Lugares: ";
                std::cin >> voo.quantidade;

                std::cout << "Preço: ";
                std::cin >> voo.preco;

                strcpy(dadosCadastro, "");
                strcat(dadosCadastro, voo.origem);
                strcat(dadosCadastro, "-");
                strcat(dadosCadastro, voo.destino);
                strcat(dadosCadastro, "-");
                strcat(dadosCadastro, voo.data);
                strcat(dadosCadastro, "-");
                strcat(dadosCadastro, voo.hora);
                strcat(dadosCadastro, "-");
                strcat(dadosCadastro, voo.quantidade);
                strcat(dadosCadastro, "-");
                strcat(dadosCadastro, voo.preco);

                bufferSend = genarateMessage(dadosCadastro, REGISTRARVOO, 0);
                send(localSocket, bufferSend, MAXDATASIZE, 0);

                numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                bufferRcv[numBytes] = '\0';
                strcpy(_bufferRcv, bufferRcv);

                if (checkMessage(_bufferRcv, 0) == SUCESS) {
                  printAlert("Voo adicionado com sucesso.");
                }else if(checkMessage(_bufferRcv, 0) == ERROR) {
                  printAlert("Erro. Voo não adicionado");
                }
                waitKey();

              break;
            }
            case 2:{//ver passagems
              bufferSend = genarateMessage("", LISTAVOO, 0);
              send(localSocket, bufferSend, MAXDATASIZE, 0);

              numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
              bufferRcv[numBytes] = '\0';
              strcpy(_bufferRcv, bufferRcv);

              strcpy(dadosCadastro, "");
              strcpy(dadosCadastro, _bufferRcv);

              printHeader("Todos os Voos");
              //std::cout << "                                                  atualizado em  " << getHourMinutesSeconds();


              while (checkMessage(_bufferRcv, 0) != FINISHED) {

              //  std::cout << "conteudo original ==>> " << dadosCadastro << '\n';
                printVooList(dadosCadastro);

                numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                bufferRcv[numBytes] = '\0';
                strcpy(_bufferRcv, bufferRcv);

                strcpy(dadosCadastro, _bufferRcv);
              }
              std::cout << "------------------------------- FIM DA LISTA -----------------------------------\n";
              waitKey();

              break;
            }
            case 3:{//Alterer conta
                std::cout << "Nova Senha: ";
                std::cin >> senha;

                strcpy(dadosCadastro, "");
                strcpy(dadosCadastro, senha);
                bufferSend = genarateMessage(dadosCadastro, VALIDADMINPASSWORD, 0);
                send(localSocket, bufferSend, MAXDATASIZE, 0);

                numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                bufferRcv[numBytes] = '\0';
                strcpy(_bufferRcv, bufferRcv);

                if (checkMessage(_bufferRcv, 0) == SUCESS) {
                  printAlert("Senha alterado com sucesso.");
                  sleep(3);
                }

              break;

            }
            case 4:{//chat
              printHeader("Conversas - Chat");
              chat = 1;
              while (chat) {
                fflush(stdin);
              //  while(!kbhit()){

                  strcpy(dadosCadastro, "");
                  strcat(dadosCadastro, usuario);

                  bufferSend = genarateMessage(dadosCadastro, UPDATECHAT,0);
                  send(localSocket, bufferSend, MAXDATASIZE, 0);


                   numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                   bufferRcv[numBytes] = '\0';
                   strcpy(_bufferRcv, bufferRcv);

                   //strcpy(dadosCadastro, "");
                   strcpy(dadosCadastro, _bufferRcv);
                    std::cout << "bufferRcv = " << _bufferRcv << '\n';
                   while(checkMessage(_bufferRcv,0) != FINISHED){

                     std::cout << dadosCadastro << '\n';

                     numBytes = recv(localSocket, bufferRcv, MAXDATASIZE, 0);
                     bufferRcv[numBytes] = '\0';
                     strcpy(_bufferRcv, bufferRcv);

                     strcpy(dadosCadastro, _bufferRcv);

                   }

                   if (checkMessage(_bufferRcv,0) != ERROR) {
                     std::cout << dadosCadastro << '\n';
                   }
                  //  usleep(500000);
              //  }

                //std::cout << "\t> mensagem\n\tl limpar\n\tv voltar" << '\n';
                bufferKey = getchar();
              //  std::cin >> bufferKey;
                if (bufferKey == '>') {
                  std::cin.ignore();
                 VERIFIC:std::cout << "[" << getHourMinutes() << "] - Eu: ";
                  //std::cin >> messageChat;
                  scanf(" %[^\n]s", messageChat);
                  if (strlen(messageChat) < 32) {
                    std::cout << "minimo de caracteres 30" << '\n';
                    goto VERIFIC;
                  }else if (strlen(messageChat) > 250) {
                    std::cout << "Maximo de caracteres 250" << '\n';
                    goto VERIFIC;
                  }
                  std::cout << '\n';

                  strcpy(dadosCadastro, "");
                  strcat(dadosCadastro, usuario);
                  strcat(dadosCadastro, "-");
                  strcat(dadosCadastro, messageChat);

                  bufferSend = genarateMessage(dadosCadastro, MESSAGE,0);
                  send(localSocket, bufferSend, MAXDATASIZE, 0);

                } else if (bufferKey == 'v') {
                  chat = 0;
                } else if (bufferKey == 'l') {
                  printHeader("Conversa - chat");
                }else{
                  fflush(stdin);
                }

              }
              break;
            }
            case 5:{//sair
              bufferSend = genarateMessage("", CLOSE, 0);
              send(localSocket, bufferSend, MAXDATASIZE, 0);
              break;
            }
          }
        } while(opt1 != 5);

        break;
      }
      case 4:{
          faetures();
          waitKey();
        break;
      }
      case 5:{//caso sair
          bufferSend = genarateMessage(usuario, CLOSE, 0);
					send(localSocket, bufferSend, MAXDATASIZE, 0);
					printAlert("Terminado a mas ver!");

					free(bufferSend);
					free(bufferRcv);
					free(_bufferRcv);
					return 0;
        break;
      }
    }

  } while(opt != 5);

  free(bufferSend);
  free(bufferRcv);
  free(_bufferRcv);

  return 0;
}
