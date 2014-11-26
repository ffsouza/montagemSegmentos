//============================================================================
// Name        : montagemFragmentos.cpp
// Author      : Felipe Faria de Souza
// Version     :
// Copyright   : Felipe Faria de Souza
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
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

void processFragments(Fragment* head);

void compareFragments(Fragment* fragment1, Fragment* fragment2, int overlap,
		float seqError);

void processFragments(Fragment* fragmentsSequence);

void processFragmentSequence(Fragment* sequence);

long* distanceMatrix;
long matrixN;
long* initializeDistanceMatrix(long n);

int main(int argc, char** argv) {
	clearParams();
	loadParams(argc, argv);

	char* input = getParamInput();

	if (input == NULL) {
		perror("ERROR");
		return 0;
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
	distanceMatrix = initializeDistanceMatrix(n);

	printf("Iniciando processamento\n");
	processFragments(head);

	//processDistancesMatrix();

	//printOutput

	//Free memory
	Fragment* node = head;
	Fragment* aux;
	while (node != NULL) {
		aux = node->next;
		free(node);
		node = aux;
	}
	free(distanceMatrix);
	return 0;
}

long getMatrixPosition(long x, long y);

long* initializeDistanceMatrix(long n) {
	matrixN = n;
	return (long*) malloc(sizeof(long) * n * n);
}

long getMatrixPosition(long x, bool reverse, long y, bool reverse2) {
	return x * matrixN + y + (reverse2 ? 1 : 0);
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

void processFragments(Fragment* head) {
	Fragment* currentFrag = head;

	while (currentFrag != NULL) {
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
	if (bestOverlap > 0) {
		printf(
				"bestOverlap:%d\nSeqA(%ld) reverse %s: %s\nSeqB(%ld) reverse: %s: %s\n",
				bestOverlap, fragment1->id, reverse1 ? "true" : "false", seq1,
				fragment2->id, reverse2 ? "true" : "false", seq2);
	}
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

	dist = calculateDistance(fragment1, true, fragment2, false, overlap,
			seqError);
	distanceMatrix[getMatrixPosition(id1, true, id2, false)] = dist;

	dist = calculateDistance(fragment2, false, fragment1, false, overlap,
			seqError);
	distanceMatrix[getMatrixPosition(id2, false, id1, false)] = dist;

	dist = calculateDistance(fragment2, true, fragment1, false, overlap,
			seqError);
	distanceMatrix[getMatrixPosition(id2, true, id1, false)] = dist;

}
