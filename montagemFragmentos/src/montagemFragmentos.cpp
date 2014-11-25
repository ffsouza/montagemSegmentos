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
#include "params.h"
#include "fragmentBuilder.h"
#include "fragment.h"

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

void processFragment(Fragment* sequence, Fragment* fragmentsSequence);

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
	printf("Iniciando montagem de fragmentos: %s\n",input);

	seqError = getParamSequenceError();
	seqOverlap = getParamSequenceOverlap();

	FILE* fileInput = fopen(input, "r");
	Fragment* head ;
	printf("leitura de fragmentos: %s\n",input);

	long n = readFragments(fileInput, 150, &head);
	printf("Iniciando montagem de fragmentos: %s\n",input);
	distanceMatrix = initializeDistanceMatrix(n);

	printf("Iniciando processamento\n");
	processFragments(head);

	//processDistancesMatrix();

	//printOutput

	//Free memory
	Fragment* node = head;
	Fragment* aux;
	while(node != NULL){
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

long getMatrixPosition(long x, long y) {
	return x * matrixN + y;
}
void processFragment(Fragment* fragment, Fragment* fragmentsSequence) {
	float seqError = getParamSequenceError();
	int overlap = getParamSequenceOverlap();

	Fragment* currentFrag = fragmentsSequence;
	while (currentFrag != NULL) {
		compareFragments(fragment, currentFrag, overlap, seqError);

		currentFrag = currentFrag->next;
	}
}

void processFragments(Fragment* head) {
	Fragment* currentFrag = head;

	while (currentFrag != NULL) {
		processFragment(currentFrag, currentFrag->next);
		currentFrag = head->next;
	}
}

long calculateDistance(Fragment* fragment1, bool reverse1, Fragment* fragment2, bool reverse2, int overlap,
		float seqError) {

	//TODO:
	//Calcular distancias entre fragmentos

	return random() % (overlap - (int) (overlap * seqError));
}

void compareFragments(Fragment* fragment1, Fragment* fragment2, int overlap,
		float seqError) {

	int id1 = fragment1->id;
	int id2 = fragment2->id;

	long dist = calculateDistance(fragment1, false, fragment2, false, overlap, seqError);
	distanceMatrix[getMatrixPosition(id1, id2)] = dist;
	dist = calculateDistance(fragment2, false, fragment1, false, overlap, seqError);
	distanceMatrix[getMatrixPosition(id2, id1)] = dist;

	dist = calculateDistance(fragment1, false, fragment2, true, overlap, seqError);
	distanceMatrix[getMatrixPosition(id1, id2) + 1] = dist;
	dist = calculateDistance(fragment2, false, fragment1, true, overlap, seqError);
	distanceMatrix[getMatrixPosition(id2, id1) + 1] = dist;

}
