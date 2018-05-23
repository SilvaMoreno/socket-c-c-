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
#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum tag{
	LOGIN,
	INVALIDUSERNAME,
  INVALIDPASSWORD,
	VALIDUSERNAME,
	REGISTRAR,
	REGISTRARVOO,
	REGISTARPEDIDO,
	SUCESS,
	UPDATECHAT,
	ERROR,
	VALIDADMINPASSWORD,
	CONNECTED,
	MESSAGE,
	FINISHED,
	LISTAVOO,
	LISTAPEDIDO,
	CLOSE
};

char* genarateMessage(char *value, int type, int isServer){
  char *result, tag[20], content[300];

  result = (char *) calloc((strlen(value)+20), sizeof(char));

  switch (type) {
    case LOGIN:
      strcpy(tag, "login-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

    case INVALIDUSERNAME:
      strcpy(tag, "invalidUserName-");
      strcpy(content, value);
			//std::cout << "invalidUserName value = " << value << " \nConteudo " << content << '\n';
      strcat(result, tag);
      strcat(result, content);
			//std::cout << "2 -invalidUserName value = " << value << " \nConteudo " << content << '\n';
    break;

		case VALIDUSERNAME:
      strcpy(tag, "validUserName-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

		case UPDATECHAT:
      strcpy(tag, "updatechat-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

    case INVALIDPASSWORD:
      strcpy(tag, "invalidpassword-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

    case CONNECTED:
      strcpy(tag, "connected-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

		case LISTAVOO:
      strcpy(tag, "listavoo-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

		case LISTAPEDIDO:
      strcpy(tag, "listapedido-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

    case MESSAGE:
      strcpy(tag, "message-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

    case FINISHED:
      strcpy(tag, "finished-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

		case REGISTRAR:
      strcpy(tag, "registar-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

		case REGISTARPEDIDO:
      strcpy(tag, "registarpedido-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

		case REGISTRARVOO:
      strcpy(tag, "registarvoo-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

		case SUCESS:
      strcpy(tag, "sucess-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

		case ERROR:
      strcpy(tag, "error-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

		case VALIDADMINPASSWORD:
      strcpy(tag, "validadimpassword-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;

    case CLOSE:
      strcpy(tag, "close-");
      strcpy(content, value);
      strcat(result, tag);
      strcat(result, content);
    break;
  }

  if (isServer) {
    std::cout << "Mensagem (requisição resposta): " << result << '\n';
    fflush(stdout);
  }


	//std::cout << "Result = " << result << '\n';
	return result;
  free(result);
}

int checkMessage(char *bufferRcv, int isServer){
  if (isServer) {
  	std::cout << "Mensagem (requisição recebida): " << bufferRcv << '\n';
    fflush(stdout);
  }

  char *tag;
  tag = strtok(bufferRcv, "-");

  if(strcmp(tag, "login") == 0){
		return LOGIN;
	}if(strcmp(tag, "invalidUserName") == 0){
		return INVALIDUSERNAME;
	}if(strcmp(tag, "invalidpassword") == 0){
		return INVALIDPASSWORD;
	}if(strcmp(tag, "connected") == 0){
		return CONNECTED;
	}if(strcmp(tag, "message") == 0){
		return MESSAGE;
	}if(strcmp(tag, "updatechat") == 0){
		return UPDATECHAT;
	}if(strcmp(tag, "finished") == 0){
		return FINISHED;
	}if(strcmp(tag, "close") == 0){
		return CLOSE;
	}if(strcmp(tag, "validUserName") == 0){
		return VALIDUSERNAME;
	}if(strcmp(tag, "registar") == 0){
		return REGISTRAR;
	}if(strcmp(tag, "registarpedido") == 0){
		return REGISTARPEDIDO;
	}if(strcmp(tag, "registarvoo") == 0){
		return REGISTRARVOO;
	}if(strcmp(tag, "sucess") == 0){
		return SUCESS;
	}if(strcmp(tag, "listavoo") == 0){
		return LISTAVOO;
	}if(strcmp(tag, "listapedido") == 0){
		return LISTAPEDIDO;
	}if(strcmp(tag, "error") == 0){
		return ERROR;
	}if(strcmp(tag, "validadimpassword") == 0){
		return VALIDADMINPASSWORD;
	}

	return 0;
}

char* check(char *bufferRcv){
  char *token;

  token = strtok(bufferRcv, "-");
  token = strtok(NULL, "-");

  return token;
}


#endif
