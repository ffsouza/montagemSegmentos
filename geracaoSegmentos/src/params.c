/*
 * params.c
 *
 *  Created on: Aug , 2014
 *      Author: ffsouza

 * parametros
 *          -n numero de sequencias
 *			-t tamanho da
 *			-d desvio do tamanho
 *			-o arquivo de saída
 *
 */
#include "params.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/* STATE FOR ARGUMENTS READING */

const int STATE_DEFAULT = -1; /* ignore inputs  */
const int STATE_SEQUENCE_SIZE = 0;
const int STATE_SEQUENCE_STD_DEVIATION = 1;
const int STATE_NUMBER_SEQUENCES = 2;

const int STATE_OUTPUT = 7;

/*------------------------------------------------------------------- */
/* variable to store the input parameters */
char* paramOutput;
int paramSequenceSize;
int paramSequenceStdDeviation;
int paramSequenceNumber;

int readingState;

char* getParamOutput() {
	return paramOutput;
}
int getParamSequenceSize() {
	return paramSequenceSize;
}
int getParamSequenceStdDeviation() {
	return paramSequenceStdDeviation;
}

int getParamSequenceNumber() {
	return paramSequenceNumber;
}

/** Loads the each parameter from the arguments gotten from the user .
 * --ip
 * --interface
 * --requisition
 * Each other argument will be considered like the pcap file from the information will be read.
 **/
int loadParams(int argc, char **argv) {
	int i;
	if (argc < 1) {
		return EXIT_FAILURE;
	}
	readingState = STATE_DEFAULT;
	for (i = 0; i < argc; i++) {
		if (checkIfState(argv[i]) == 1) {
			continue;
		}
		treatParam(argv[i]);
	}
	return EXIT_SUCCESS;
}

/** Check the current parameter from the arguments gotten from the user .
 */
int checkIfState(char * value) {
	if (strcmp(value, "-t\0") == 0) {
		readingState = STATE_SEQUENCE_SIZE;
		return 1;
	}
	if (strcmp(value, "-n\0") == 0) {
		readingState = STATE_NUMBER_SEQUENCES;
		return 1;
	}
	if (strcmp(value, "-d\0") == 0) {
		readingState = STATE_SEQUENCE_STD_DEVIATION;
		return 1;
	}

	if (strcmp(value, "-o\0") == 0) {
		readingState = STATE_OUTPUT;
		return 1;
	}
	return 0;
}

void treatParam(char * value) {
	if (readingState == STATE_DEFAULT) {
		return;
	}
	if (readingState == STATE_NUMBER_SEQUENCES) {
		sscanf(value, "%d", &paramSequenceNumber);
	}
	if (readingState == STATE_OUTPUT) {
		paramOutput = value;
	}
	if (readingState == STATE_SEQUENCE_SIZE) {
		sscanf(value, "%d", &paramSequenceSize);
	}
	if (readingState == STATE_SEQUENCE_STD_DEVIATION) {
		sscanf(value, "%d", &paramSequenceStdDeviation);
	}
	readingState = STATE_DEFAULT;
}

void clearParams() {
	paramSequenceSize = 10;
	paramSequenceStdDeviation = 0;
	paramSequenceNumber = 1;
	readingState = STATE_DEFAULT;
}

void strprintf(char* var, char* string, char* param) {
	int lenght = strlen(var);
	const char* v = string;
	if (param == NULL) {
		sprintf(var + (sizeof(char*) * lenght), v, NULL);
	} else {
		sprintf(var + lenght, v, param);
	}
}

int cmpvalue(char* v, int startV, char* check, int sizeCheck) {
	int i = 0;
	for (; i < sizeCheck; i++) {
		if (v[startV + i] != check[i]) {
			return 0;
		}
	}
	return 1;
}
