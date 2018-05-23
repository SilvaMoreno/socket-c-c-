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
#ifndef GETTIME_HPP
#define GETTIME_HPP



#include <stdio.h>
#include <time.h>

char* getHourMinutesSeconds(){
	char *result = (char*) calloc(20,sizeof(char));
	struct tm *local;
	time_t t;

	t= time(NULL);
	local=localtime(&t);

	int hour=local->tm_hour;
	int min=local->tm_min;
	int sec=local->tm_sec;

	sprintf(result,"%d:%d:%d", hour, min, sec);


	return result;
	free(result);
}

char* getNow(){
	char *result = (char*) calloc(20,sizeof(char));
	struct tm *local;
	time_t t;

	t= time(NULL);
	local=localtime(&t);

	int ano = local->tm_year;
	int mes = local->tm_mon;
	int dia = local->tm_mday;

	int hour=local->tm_hour;
	int min=local->tm_min;
	int sec=local->tm_sec;

	sprintf(result,"%d-%d-%d %d:%d:%d", dia, mes, ano, hour, min, sec);

	return result;
	free(result);
}

char* getHourMinutes(){
	char *result = (char*) calloc(20,sizeof(char));
	struct tm *local;
	time_t t;

	t= time(NULL);
	local=localtime(&t);

	int hour=local->tm_hour;
	int min=local->tm_min;

	sprintf(result,"%d:%d", hour, min);

	return result;
	free(result);
}

#endif
