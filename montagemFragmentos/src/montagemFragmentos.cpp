//============================================================================
// Name        : montagemFragmentos.cpp
// Author      : Felipe Faria de Souza
// Version     :
// Copyright   : Felipe Faria de Souza
// Description : Hello World in C++, Ansi-style
//============================================================================

/**
 * To run use: E.g: motagemFragments -i ./input/n5000_t25_d5.dat-o ./output/result.out -ol 3 -e 0.2
 */

//Comment it to disable debug messages
//#define DEBUG_ENABLED
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "params.hpp"
#include "fragmentBuilder.hpp"
#include "fragment.hpp"

using namespace std;

float seqError;
float seqOverlap;

typedef struct t_SeqFragment {
	Fragment* fragment;
	int direction;
	int overlap;
} SeqFragment;

char** LIST_SEQUENCES;

void processFragments(Fragment* head);

void compareFragments(Fragment* fragment1, Fragment* fragment2, int overlap,
		float seqError);

void processFragments(Fragment* fragmentsSequence);

void processFragmentSequence(Fragment* sequence);

char* getSequence(long sequence, bool reversed);

long getMatrixPosition(long x, bool reverse1, long y, bool reverse2);

long* distanceMatrix;
long matrixN;
long* initializeDistanceMatrix(long n);

void printSequence(FILE *fout, int* bestSequence, long sequenceSize);

long processBestSequence(int** bestSequence, long* sequenceSize);

void freeFragmentsMemory(Fragment* head) {
	if (LIST_SEQUENCES != NULL) {
		free(LIST_SEQUENCES);
		LIST_SEQUENCES = NULL;
	}
	Fragment* node = head;
	Fragment* aux;
	while (node != NULL) {
		aux = node->next;
		free(node);
		node = aux;
	}
}

int main(int argc, char** argv) {
	double elapsedTime; //check ellapsed time
	struct timeval t_inicial, t_final, ti_distancias, tf_distancias,
			ti_processamento;

	clearParams();
	loadParams(argc, argv);

	char* input = getParamInput();

	if (input == NULL) {
		perror("ERROR");
		return 0;
	}
	char* outputFile = getParamOutput();
	FILE* foutput;
	if (outputFile == NULL) {
		foutput = stdout;
	} else {
		foutput = fopen(outputFile, "w+");
		if (foutput == NULL) {
			perror(
					"Não foi possivel abrir o arquivo de saída. Utilizando a saida padrão.");
			foutput = stdout;
		}
	}

	printf("Iniciando montagem de fragmentos: %s\n", input);

	seqError = getParamSequenceError();
	seqOverlap = getParamSequenceOverlap();

	FILE* fileInput = fopen(input, "r");
	Fragment* head;
	printf("leitura de fragmentos: %s\n", input);

	long n = readFragments(fileInput, 150, &head);
	fclose(fileInput);
	printf("Iniciando montagem de fragmentos: %s\n", input);

	gettimeofday(&t_inicial, NULL);
	distanceMatrix = initializeDistanceMatrix(n);
	if (distanceMatrix == NULL) {
		perror("Incapaz de alocar matrix de distancias. Erro: ");
		freeFragmentsMemory(head);
		return 1;
	}

	printf("Processando distancias\n");
	gettimeofday(&ti_distancias, NULL);
	LIST_SEQUENCES = (char**) malloc(sizeof(char*) * n);
	processFragments(head);
	gettimeofday(&tf_distancias, NULL);

	printf("Calculando melhor sequênciamento\n");
	gettimeofday(&ti_processamento, NULL);

	int *bestSequence;
	long sequenceSize = 0l;
	long bestValue = processBestSequence(&bestSequence, &sequenceSize);

	gettimeofday(&t_final, NULL);

	printf("Imprimindo Resultados:\n");

	fprintf(foutput, "Tamanho da melhor sequência: %ld\n", bestValue);

	printSequence(foutput, bestSequence, sequenceSize);

	//calculate the elapsed time
	elapsedTime = (tf_distancias.tv_sec - ti_distancias.tv_sec) * 1000000; // sec to ms
	elapsedTime += (tf_distancias.tv_usec - ti_distancias.tv_usec); // us to ms
	fprintf(foutput, "Tempo calculo das distancias\t\t\t%.1lf ms\n",
			elapsedTime);

	elapsedTime = (t_final.tv_sec - ti_processamento.tv_sec) * 1000000; // sec to ms
	elapsedTime += (t_final.tv_usec - ti_processamento.tv_usec); // us to ms
	fprintf(foutput, "Tempo processamento da sequência\t\t%.1lf ms\n",
			elapsedTime);

	elapsedTime = (t_final.tv_sec - ti_distancias.tv_sec) * 1000000; // sec to ms
	elapsedTime += (t_final.tv_usec - ti_distancias.tv_usec); // us to ms
	fprintf(foutput, "Tempo total de processamento\t\t\t%.1lf ms\n",
			elapsedTime);

	elapsedTime = (t_final.tv_sec - t_inicial.tv_sec) * 1000000; // sec to ms
	elapsedTime += (t_final.tv_usec - t_inicial.tv_usec); // us to ms
	fprintf(foutput, "Tempo total (incluindo leitura dos dados)\t%.1lf ms\n",
			elapsedTime);

	//Free memory
	freeFragmentsMemory(head);
	free(distanceMatrix);
	return 0;
}

long* initializeDistanceMatrix(long n) {
	matrixN = n;
	return (long*) malloc(sizeof(long) * 4 * n * n);
}

long getMatrixPosition(long x, bool reverse1, long y, bool reverse2) {
	//lin * matrixSize + col
	return (y + reverse2 ? 1 : 0) * (2 * matrixN) + 2 * x + (reverse1 ? 1 : 0);
}
void processFragmentSequence(Fragment* fragment) {
	float seqError = getParamSequenceError();
	int overlap = getParamSequenceOverlap();

	Fragment* currentFrag = fragment->next;
	while (currentFrag != NULL) {
		compareFragments(fragment, currentFrag, overlap, seqError);
		currentFrag = currentFrag->next;
	}
}

char* getSequence(long sequence, bool reversed) {
	char* seq = LIST_SEQUENCES[sequence];
	if (!reversed) {
		return seq;
	}
	int lenght = strlen(seq);
	char* rerveseSeq = (char*) malloc(sizeof(char) * lenght);
	int i, j;
	for (i = 0, j = lenght - 1; i < lenght; i++, j--) {
		rerveseSeq[j] = seq[i];
	}
	return rerveseSeq;
}

void processFragments(Fragment* head) {
	Fragment* currentFrag = head;

	while (currentFrag != NULL) {
		LIST_SEQUENCES[currentFrag->id] = currentFrag->sequence;
		processFragmentSequence(currentFrag);
		currentFrag = currentFrag->next;
	}
}

long calculateDistance(Fragment* fragment1, bool reverse1, Fragment* fragment2,
		bool reverse2, int minOverlap, float seqError) {
	int i, j;

	//Calcular distancias entre fragmentos
	char* seq1 = fragment1->sequence;
	char* seq2 = fragment2->sequence;
	int n1 = fragment1->sequenceSize;
	int n2 = fragment2->sequenceSize;
	int overlap = minOverlap;

	int hit, miss;
	int bestOverlap = 0;
	int inc1 = reverse1 ? -1 : +1;
	int inc2 = reverse2 ? -1 : +1;
	int iterations;
	while (overlap < n1 && overlap < n2) {
		hit = miss = 0;
		i = reverse1 ? overlap - 1 : n1 - overlap;
		j = reverse2 ? overlap - 1 : 0;
		iterations = overlap;
		for (; iterations > 0; iterations--, i += inc1, j += inc2) {
			seq1[i] == seq2[j] ? hit++ : miss++;
		}
		if (miss / ((float) overlap) <= seqError) {
			bestOverlap = overlap;
		}
		overlap++;
	}
#ifdef DEBUG_ENABLED
	printf(
			"bestOverlap:%d\nSeqA(%ld) reverse %s: %s\nSeqB(%ld) reverse: %s: %s\n",
			bestOverlap, fragment1->id, reverse1 ? "true" : "false", seq1,
			fragment2->id, reverse2 ? "true" : "false", seq2);
#endif
	return bestOverlap;
}

void compareFragments(Fragment* fragment1, Fragment* fragment2, int overlap,
		float seqError) {

	int id1 = fragment1->id;
	int id2 = fragment2->id;
	distanceMatrix[getMatrixPosition(id1, false, id1, false)] = 0;
	distanceMatrix[getMatrixPosition(id1, false, id1, true)] = 0;
	distanceMatrix[getMatrixPosition(id1, true, id1, false)] = 0;
	distanceMatrix[getMatrixPosition(id1, true, id1, true)] = 0;

	long dist = calculateDistance(fragment1, false, fragment2, false, overlap,
			seqError);
	distanceMatrix[getMatrixPosition(id1, false, id2, false)] = dist;
	distanceMatrix[getMatrixPosition(id2, true, id1, true)] = dist;

	dist = calculateDistance(fragment1, false, fragment2, true, overlap,
			seqError);
	distanceMatrix[getMatrixPosition(id1, false, id2, true)] = dist;
	distanceMatrix[getMatrixPosition(id2, false, id1, true)] = dist;

	dist = calculateDistance(fragment1, true, fragment2, false, overlap,
			seqError);
	distanceMatrix[getMatrixPosition(id1, true, id2, false)] = dist;
	distanceMatrix[getMatrixPosition(id2, true, id1, false)] = dist;

	dist = calculateDistance(fragment1, true, fragment2, true, overlap,
			seqError);
	distanceMatrix[getMatrixPosition(id1, true, id2, true)] = dist;
	distanceMatrix[getMatrixPosition(id2, false, id1, false)] = dist;
}

void printSequence(FILE *fout, int* bestSequence, long sequenceSize) {
	if (sequenceSize < 1) {
		fprintf(fout, "sequência vazia.");
		return;
	}
	fprintf(fout, "Sequência: \n");
	int current = bestSequence[0];
	int sequence = current / 2;
	bool invertida = current % 2 == 1;
	fprintf(fout, "seq: %d%s ", sequence, invertida ? " invertida\t" : "\t\t");
	fprintf(fout, "%s", getSequence(sequence, invertida));

	long i = 1;
	int next;
	int nextSequence;
	bool nextSeqInvertida;
	while (i < sequenceSize) {
		next = bestSequence[i];
		nextSequence = next / 2;
		nextSeqInvertida = next % 2 == 1;
		long coverage = distanceMatrix[getMatrixPosition(sequence, invertida,
				nextSequence, nextSeqInvertida)];
		fprintf(fout, "\n[%ld] :", coverage);
		fprintf(fout, " seq: %d%s", nextSequence,
				nextSeqInvertida ? " invertida\t" : "\t\t");
		fprintf(fout, "%s", getSequence(nextSequence, nextSeqInvertida));
		current = next;
		i++;
	}
	fprintf(fout, "\n");
}

long processBestSequence(int** bestSequence, long* sequenceSize) {
	int n = 5;
	int* seq = (int*) malloc(sizeof(int) * n);
	//valures ímpares representam sequências invertidas

	seq[0] = 1;
	seq[1] = 4;
	seq[2] = 7;
	seq[3] = 10;
	seq[4] = 15;

	int bestValue = 10;

	*bestSequence = seq;
	*sequenceSize = n;
	return bestValue;
}
