/*
 * params.c
 *
 *  Created on: Nov 22th, 2014
 *      Author: Felipe Faria de Souza

 * parametros
 *
 *			-o arquivo de saída
 *
 */
#include "params.hpp"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/* STATE FOR ARGUMENTS READING */

char* STATE_DEFAULT = NULL; /* ignore inputs  */

char* STATE_SEQUENCE_OVERLAP = "-ol";
char* STATE_SEQUENCE_ERROR = "-e";
char* STATE_INPUT = "-i";
char* STATE_OUTPUT = "-o";

/*------------------------------------------------------------------- */
/* variable to store the input parameters */
char* paramOutput;
char* paramInput;
float paramError;
int paramSeqOverlap;

char* readingState;

char* getParamOutput() {
	return paramOutput;
}

char* getParamInput() {
	return paramInput;
}

int getParamSequenceOverlap() {
	return paramSeqOverlap;
}

float getParamSequenceError() {
	return paramError;
}

/** Loads the each parameter from the arguments gotten from the user .
 * --ip
 * --interface
 * --requisition
 * Each other argument will be considered like the pcap file from the information will be read.
 **/
int loadParams(int argc, char **argv) {
	int i;
	clearParams();
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
	if (strcmp(value, STATE_INPUT) == 0) {
		readingState = STATE_INPUT;
		return 1;
	}
	if (strcmp(value, STATE_OUTPUT) == 0) {
		readingState = STATE_OUTPUT;
		return 1;
	}
	if (strcmp(value, STATE_SEQUENCE_ERROR) == 0) {
		readingState = STATE_SEQUENCE_ERROR;
		return 1;
	}

	if (strcmp(value, STATE_SEQUENCE_OVERLAP) == 0) {
		readingState = STATE_SEQUENCE_OVERLAP;
		return 1;
	}
	return 0;
}

void treatParam(char * value) {
	if (readingState == STATE_DEFAULT) {
		return;
	}
	if (readingState == STATE_INPUT) {
		paramInput = value;
	}
	if (readingState == STATE_OUTPUT) {
		paramOutput = value;
	}
	if (readingState == STATE_SEQUENCE_OVERLAP) {
		sscanf(value, "%d", &paramSeqOverlap);
	}
	if (readingState == STATE_SEQUENCE_ERROR) {
		sscanf(value, "%f", &paramError);
	}

	readingState = STATE_DEFAULT;
}

void clearParams() {
	paramSeqOverlap = 5;
	paramError = 0.0;
	paramInput = NULL;
	paramOutput = NULL;
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
