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
#include <signal.h>

#include "funcoes/message.h"
#include "funcoes/struct.h"
#include "split/split.h"
#include "funcoes/usermanager.h"
#include "funcoes/adminmanager.h"
#include "funcoes/voomanager.h"
#include "funcoes/pedidomanager.h"
#include "funcoes/messagemanager.h"

#define MAXDATASIZE 500
#define MAXALLOC 500
#define BACKLOG 5

void tempoEspera(int signum){
  std::cout << "Server a espera de coneção" << '\n';
}

int main(int argc, char const *argv[]) {

  system("clear");

  if (argc != 2) {
    std::cerr << "Execução correta ./server porta." << '\n';
    exit(0);
  }

  //signal(SIGALRM, tempoEspera);

  //tipo de dados representa IDs de processo
  pid_t pid;

  int fluxo, tipoRequisicao;
	char userName[50];
	char *bufferRcv, *_bufferRcv, *msgContent, *dados;
  struct listaVoo voos;

  std::string c = argv[1];
  int porta = atoi(c.c_str());

  struct sockaddr_in local;
	struct sockaddr_in remote;

  int localSocket, remotoSocket, structSize;

  //criar socket
  localSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (localSocket == -1) {
    std::cerr << "Erro ao criar socket" << '\n';
  }

  //configurar socket para a coneção
  local.sin_family = AF_INET;
  local.sin_port = htons(porta);
  local.sin_addr.s_addr = INADDR_ANY;
  memset(local.sin_zero,0,8);

  //Associar a porta
  if ( bind(localSocket, (struct sockaddr *)&local, sizeof(local)) == -1 ) {
      std::cerr << "Erro ao criar bind" << '\n';
  }

  //Inicio da escuta
  if (listen(localSocket, 5) == -1) {
    std::cerr << "Erro ao escutar a coneção" << '\n';
  }

  //structSize = sizeof(struct sockaddr_in);
  structSize = sizeof(struct sockaddr_in);
  alarm(30);
  while (1) {
    //Aguardar pedido de coneção

    remotoSocket = accept(localSocket, (struct sockaddr *)&remote, (socklen_t *)&structSize);
    alarm(0);
    //criar um novo processo apos aceitar a coneção
    pid = fork();
    if (pid == 0) {
        //Alocar memorei necessaria
        bufferRcv = (char*) calloc(MAXALLOC,sizeof(char));
        _bufferRcv = (char*) calloc(MAXALLOC,sizeof(char));
        msgContent = (char*) calloc(MAXALLOC,sizeof(char));
        dados = (char*) calloc(MAXALLOC,sizeof(char));

   SAIR:fluxo = 0;
        while (!fluxo) {
          recv(remotoSocket, bufferRcv, MAXDATASIZE, 0);
          strcpy(_bufferRcv, bufferRcv);

          std::cout << "conteudo original ==>> " << _bufferRcv << '\n';
          strcpy(dados, _bufferRcv);

          std::string nome;
          std::string senha;

          tipoRequisicao = checkMessage(_bufferRcv, 1);

          switch (tipoRequisicao) {
            case LOGIN:{
              //pegar conteudo da mensagen
                  strcpy(_bufferRcv, bufferRcv);
                  msgContent = check(_bufferRcv);
                  bufferRcv[strlen(bufferRcv)] = '\0';

                  nome = dividir(dados)[1];
                  senha = dividir(dados)[2];

                  if (checkUserName(nome)) {
                    if (userLogin(nome, senha)) {
                      send(remotoSocket, genarateMessage("", CONNECTED, 1), MAXDATASIZE, 0);

                    //  std::cout << "ddddd===>>>>>>>>><<< " << inet_ntoa(remote.sin_addr) << '\n';
                      savePortaCliente(nome, inet_ntoa(remote.sin_addr) , c, "");
                      fluxo = 1;
                    }else{
                      send(remotoSocket, genarateMessage("", INVALIDPASSWORD, 1), MAXDATASIZE, 0);
                      fluxo = 0;
                    }
                  }else{
                    send(remotoSocket, genarateMessage("", INVALIDUSERNAME, 1), MAXDATASIZE, 0);
                    fluxo = 0;
                  }
              break;
            }/***************************************************************************************/
            case VALIDUSERNAME:{
              nome = dividir(dados)[1];

               if (checkUserName(nome)) {
                 send(remotoSocket, genarateMessage("", INVALIDUSERNAME, 1), MAXDATASIZE, 0);
                 fluxo = 0;
               }else{
                 send(remotoSocket, genarateMessage("", VALIDUSERNAME, 1), MAXDATASIZE, 0);
                 fluxo = 0;
               }
              break;
            }/*************************************************************************************/
            case REGISTRAR:{
              struct stringUser user;
              user.name = dividir(dados)[1];
              user.userName = dividir(dados)[2];
              user.senha = dividir(dados)[3];
              user.idade = dividir(dados)[4];
              user.email = dividir(dados)[5];
              user.telefone = dividir(dados)[6];

              //std::cout << user.name << "-" << user.userName << "-" << user.senha << "-" << user.idade << "-" << user.email << "-" << user.telefone << '\n';

              if (addUser(user)) {
                send(remotoSocket, genarateMessage("", SUCESS, 1), MAXDATASIZE, 0);

                savePortaCliente(user.userName, inet_ntoa(remote.sin_addr) , c, "");
                fluxo = 1;
              }else{
                send(remotoSocket, genarateMessage("", ERROR, 1), MAXDATASIZE, 0);
                fluxo = 0;
              }
              break;
            }/***********************************************************************************/
            case VALIDADMINPASSWORD:{
               std::string senha = dividir(dados)[1];
                int res = checkAdmin(senha);
                if (res == SUCESS) {
                  send(remotoSocket, genarateMessage("", SUCESS, 1), MAXDATASIZE, 0);
                  savePortaCliente("admin", inet_ntoa(remote.sin_addr) , c, "");
                  fluxo = 1;
                } else if (res == INVALIDPASSWORD) {
                  send(remotoSocket, genarateMessage("", INVALIDPASSWORD, 1), MAXDATASIZE, 0);
                  fluxo = 0;
                }
              break;
            }/************************************************************************************/
            case CLOSE:{
              std::cout << "close" << '\n';
              savePortaCliente(dividir(dados)[1], inet_ntoa(remote.sin_addr) , c, "saida");
              //std::cout << "entrou no if close" << '\n';
              close(remotoSocket);

      				free(bufferRcv);
      				free(_bufferRcv);
      				free(msgContent);
      				return 0;

              break;
            }
          }

        }//fim while com primeiro menu

        fluxo = 0;
        while (!fluxo) {

          recv(remotoSocket, bufferRcv, MAXDATASIZE, 0);
          strcpy(_bufferRcv, bufferRcv);

          std::cout << "conteudo original ==>> " << _bufferRcv << '\n';
          strcpy(dados, _bufferRcv);

          tipoRequisicao = checkMessage(_bufferRcv, 1);

          switch (tipoRequisicao) {
            case REGISTRARVOO:{
              struct stringVoo voo;

              voo.origem = dividir(dados)[1];
              voo.destino = dividir(dados)[2];
              voo.data = dividir(dados)[3];
              voo.hora = dividir(dados)[4];
              voo.quantidade = dividir(dados)[5];
              voo.preco = dividir(dados)[6];

              if(addVoo(voo)){
                send(remotoSocket, genarateMessage("", SUCESS, 1), MAXDATASIZE, 0);
                fluxo = 0;
              }else{
                send(remotoSocket, genarateMessage("", ERROR, 1), MAXDATASIZE, 0);
                fluxo = 0;
              }

              break;
            }
            case LISTAVOO:{

                std::ifstream afile("file/voos.txt", std::ios::in);
                if (afile.is_open()) {
                  std::string line;
                  while (std::getline(afile, line)) {
                    strcpy(dados, "");
                    char *inf = new char[line.length() + 1];
                    memcpy(inf, line.c_str(), line.length() + 1);

                    send(remotoSocket, genarateMessage(inf, SUCESS, 1), MAXDATASIZE, 0);
                    fluxo = 0;
                  }
                  afile.close();
                }
                else {
                  std::cerr << "Filheiro inacessivel\n";
                }

                send(remotoSocket, genarateMessage("", FINISHED, 1), MAXDATASIZE, 0);

              break;
            }
            case VALIDADMINPASSWORD:{
              std::string newSenha = dividir(dados)[1];

                std::ofstream afile("file/admin.txt", std::ios::out);
                if (afile.is_open()) {
                  afile << "senha-" << newSenha  << "\n";
                  afile.close();
                }
                send(remotoSocket, genarateMessage("", SUCESS, 1), MAXDATASIZE, 0);
              break;
            }
            case REGISTARPEDIDO:{
              std::string usuario = dividir(dados)[1];
              if(addPedido( dividir(dados)[1], dividir(dados)[2], dividir(dados)[3]) ){
                send(remotoSocket, genarateMessage("", SUCESS, 1), MAXDATASIZE, 0);
              }else{
                  send(remotoSocket, genarateMessage("", ERROR, 1), MAXDATASIZE, 0);
              }

              break;
            }
            case LISTAPEDIDO:{
              std::string user = dividir(dados)[1];

              std::string dado, idVoo, pedido;

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
                          pedido = dividir(line)[2] + "-" + lineVoo;

                          strcpy(dados, "");
                          char *inf = new char[pedido.length() + 1];
                          memcpy(inf, pedido.c_str(), pedido.length() + 1);

                          send(remotoSocket, genarateMessage(inf, SUCESS, 1), MAXDATASIZE, 0);

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
                send(remotoSocket, genarateMessage("", FINISHED, 1), MAXDATASIZE, 0);

              }
              else {
                std::cerr << "Ficheiro pedido inacessivel.\n";
              }

              if ( pedido.compare("") == 0) {
                send(remotoSocket, genarateMessage("", ERROR, 1), MAXDATASIZE, 0);

              }
              fluxo = 0;

              break;
            }
            case MESSAGE:{//PAra conversa char
              //struct userList lista = getuserList();
              char *usua = new char[(dividir(dados)[1]).length() + 1];
              memcpy(usua, (dividir(dados)[1]).c_str(), (dividir(dados)[1]).length() + 1);
              char *msg = new char[(dividir(dados)[2]).length() + 1];
              memcpy(msg, (dividir(dados)[2]).c_str(), (dividir(dados)[2]).length() + 1);
            //  std::cout << "usuario ««««»»»» " << usua << " mensa ««««»»»» " << msg << '\n';
              saveMessage(usua, msg);
            //  std::cout << "INf = " << dados << '\n';
              break;
            }
            case UPDATECHAT:{
              char *usua = new char[(dividir(dados)[1]).length() + 1];
              memcpy(usua, (dividir(dados)[1]).c_str(), (dividir(dados)[1]).length() + 1);

              char fileName[50];
              memset (fileName,'\0',49);

              strcpy(fileName,"file/mensagems/");
              strcat(fileName, usua);
              strcat(fileName,".txt");

              std::ifstream file(fileName, std::ios::in);
              if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                //  std::cout << line << '\n';
                  char *nome = new char[line.length() + 1];
                  memcpy(nome, line.c_str(), line.length() + 1);
                  send(remotoSocket, genarateMessage(nome, MESSAGE, 1), MAXDATASIZE, 0);
                }
                file.close();
              //  break;

              }
              else {
                std::cerr << "Unable to open file\n";
              }
            //  std::cerr << "sucessssssssssssssssssssssssoooooooooo" << '\n';


              send(remotoSocket, genarateMessage("", FINISHED, 1), MAXDATASIZE, 0);


              // std::ifstream afile(fileName, std::ios::in);
              // if (afile.is_open()) {
              //   std::string line;
              //   while (std::getline(afile, line)) {
              //
              //     char *nome = new char[line.length() + 1];
              //     memcpy(nome, line.c_str(), line.length() + 1);
              //
              //     send(remotoSocket,genarateMessage(nome, MESSAGE, 1),MAXDATASIZE,0);
              //
              //   }
              //   afile.close();
              //   send(remotoSocket,genarateMessage("",FINISHED,1),MAXDATASIZE,0);
              // }
              // else {
              //   std::cerr << "Unable to open file\n";
              //   send(remotoSocket,genarateMessage("Ficheiro não existe",ERROR,1),MAXDATASIZE,0);
              // }




              // int i = 0;
						  // struct messageList messages = getMessege(usua);
							// for(i = 0; i < messages.tamanho + 1; i++){
							// 	send(remotoSocket,genarateMessage(messages.mensagem[i], MESSAGE, 1),MAXDATASIZE,0);
              //   std::cerr << "ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd" << '\n';
							// }
							// send(remotoSocket,genarateMessage("",FINISHED,1),MAXDATASIZE,0);

              break;
            }
            case CLOSE:{
              goto SAIR;
              break;
            }
          }



        }



    }

  }

  return 0;
}
