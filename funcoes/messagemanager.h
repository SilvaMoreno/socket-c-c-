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
#ifndef  MESSAGEMANAGER_HPP
#define MESSAGEMANAGER_HPP

struct userList getuserList(){
  int i = 0;
  struct userList usuario;

  usuario.tamanho = i;

  std::ifstream afile("file/usuario.txt", std::ios::in);
  if (afile.is_open()) {
    std::string line;
    while (std::getline(afile, line)) {
      std::string name = dividir(line)[1];
      char *nome = new char[name.length() + 1];
      memcpy(usuario.nome[i], name.c_str(), name.length() + 1);
      usuario.tamanho = i;
      i++;
    }
    afile.close();
  }
  else {
    std::cerr << "Ficheiro usaurio inexistente.\n";
  }

  return usuario;
}

void saveMessage(char *usuario, char *conteudo) {
  int i;
  struct userList lista = getuserList();
  char _conteudo[300];

  memset (_conteudo,'\0', 299);
  char *dado;
  char para[50];
  char fileName[50];
  char mensagemFinal[300];

  if (conteudo[0] == '@') {

    strcpy(_conteudo, conteudo);
  	memset (fileName,'\0',49);
  	memset (mensagemFinal,'\0',299);

    dado = strtok(_conteudo, " ");
    strcpy(para, dado);
  //  std::cerr << "Parrra 0000 " << para << '\n';
    for (i = 1; i < strlen(para); i++) {
      para[i-1] = para[i];

			if(i+1 == strlen(para)){
				para[i] = '\0';
			}
    }
//std::cerr << "Parrra 0000 " << para << '\n';
    dado = strtok(NULL,"\n");
		strcpy(conteudo, dado);
std::cerr << "msg 0000 " << conteudo << '\n';
    for (i = 0; i < lista.tamanho + 1; i++) {
        if (strcmp(lista.nome[i], usuario) != 0 && strcmp(lista.nome[i], para) == 0) {
          memset (fileName,'\0',49);
          memset (mensagemFinal,'\0',299);

          strcpy(fileName,"file/mensagems/");
  				strcat(fileName, lista.nome[i]);
  				strcat(fileName,".txt");

          strcat(mensagemFinal, "[");
  				strcat(mensagemFinal, getHourMinutes());
  				strcat(mensagemFinal, "]");
  				strcat(mensagemFinal, " - [Privado] - ");
  				strcat(mensagemFinal, usuario);
  				strcat(mensagemFinal, ": ");
  				strcat(mensagemFinal, conteudo);
  				strcat(mensagemFinal, "\n");

          std::string dadosExistente;
          std::ifstream afile(fileName, std::ios::in);
          if (afile.is_open()) {
            std::string line;
            while (std::getline(afile, line)) {
              //std::cout << line << '\n';
              dadosExistente += line + "\n";
            }
            afile.close();
          }
          else {
            std::cerr << "Ficheiro inexistente.\n";
          }

          std::ofstream outFile(fileName, std::ios::out);
          if (outFile.is_open()) {
            outFile << dadosExistente << mensagemFinal;
            outFile.close();
          }

        }
    }

  } else {

    for (i = 0; i < lista.tamanho + 1; i++) {
      if (strcmp(lista.nome[i], usuario) != 0) {
        memset (fileName,'\0',49);
  			memset (mensagemFinal,'\0',299);

  			strcpy(fileName,"file/mensagems/");
  			strcat(fileName, lista.nome[i]);
  			strcat(fileName,".txt");

  			strcat(mensagemFinal, "[");
  			strcat(mensagemFinal, getHourMinutes());
  			strcat(mensagemFinal, "] - ");

  			strcat(mensagemFinal, usuario);
  			strcat(mensagemFinal, ": ");
  			strcat(mensagemFinal, conteudo);
  			strcat(mensagemFinal, "\n");

        std::string dadosExistente;
        std::ifstream afile(fileName, std::ios::in);
        if (afile.is_open()) {
          std::string line;
          while (std::getline(afile, line)) {
            //std::cout << line << '\n';
            dadosExistente += line + "\n";
          }
          afile.close();
        }
        else {
          std::cerr << "Ficheiro inexistente.\n";
        }

        std::ofstream outFile(fileName, std::ios::out);
        if (outFile.is_open()) {
          outFile << dadosExistente << mensagemFinal;
          outFile.close();
        }

      }
    }
  }
}

struct messageList getMessege(char *usuario){
  char line[300];
	char _line[300];
	char fileName[50];
	memset (line,'\0',299);
	memset (_line,'\0',299);

  memset (fileName,'\0',49);

  struct messageList lista;

  int i = 0;
  lista.tamanho = i;

  strcpy(fileName,"file/mensagems/");
  strcat(fileName, usuario);
  strcat(fileName,".txt");

  std::ifstream afile(fileName, std::ios::in);
  if (afile.is_open()) {
    std::string line;
    while (std::getline(afile, line)) {
      char *nome = new char[line.length() + 1];
      memcpy(lista.mensagem[i], line.c_str(), line.length() + 1);
      i++;
    //  strcpy(_line, line);
    }
    afile.close();
  }
  else {
    std::cerr << "Unable to open file\n";
  }

  return lista;


}

#endif
