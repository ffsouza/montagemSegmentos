/*
 ============================================================================
 Name        : geracaoSegmentos.c
 Author      : Felipe Faria de Souza
 Version     :
 Copyright   : Unicamp Students @2014
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "params.h"

void addSequence(FILE* fout, int size, int stdDeviation, char*buffer);

char* str_random(char* baseData, int nBaseData, int sequenceSize, char* buffer);
int main(int argc, char** argv) {
	puts("iniciando geração de fragmentos...");
	/*check command line arguments */
	clearParams();
	if (loadParams(argc, argv) != EXIT_SUCCESS) {
		fprintf(stderr,
				"Usage e.g.: lab01 -o output.dat -n number_of_sequences -s size_of_sequences -d size_standard_deviation\n");
		return EXIT_FAILURE;
	}

	char* output = getParamOutput();

	int numberOfSequences = getParamSequenceNumber();
	int size = getParamSequenceSize();
	int stdDeviation = getParamSequenceStdDeviation();

	FILE* fout;
	if (output != NULL) {
		fout = fopen(output, "w+");
	} else {
		fout = stdout;
	}
	int i;
	char* buffer = (char*) malloc(sizeof(char) * (size + stdDeviation + 1));
	for (i = 0; i < numberOfSequences; i++) {
		addSequence(fout, size, stdDeviation, buffer);
	}
	free(buffer);
	if (output != NULL) {
		fclose(fout);
	}
	puts("geração de executada...");

	return EXIT_SUCCESS;
}

void addSequence(FILE* fout, int size, int stdDeviation, char* buffer) {
	if (stdDeviation > 0)
		size = size + random() % (2 * stdDeviation) - stdDeviation;
	str_random("ACTG", 4, size, buffer);
	buffer[size] = '\0';
	fprintf(fout, "%s\n", buffer);
}

char* str_random(char* baseData, int nBaseData, int sequenceSize, char* buffer) {
	int j;
	for (j = 0; j < sequenceSize; j++) {
		buffer[j] = baseData[random() % nBaseData];
	}
}
