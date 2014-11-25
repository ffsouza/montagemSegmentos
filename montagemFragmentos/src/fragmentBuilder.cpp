/*
 * fragmentBuilder.c
 *
 *  Created on: Nov 22, 2014
 *      Author: ffsouza
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fragment.h"

Fragment* readFragments(FILE* in, uint maxSize) {
	long n = 0;
	char* buffer = (char*) malloc(sizeof(char) * (1 + maxSize));
	uint readSize;
	Fragment* head = NULL;
	Fragment* node = NULL;
	int id = 0;
	if ( fscanf(in, "%s", buffer)) {
		readSize = strlen(buffer);
		Fragment * fragment = buildFragment(id++, buffer, readSize);
		head = fragment;
		node = head;
		while (fscanf(in, "%s", buffer)) {
			readSize = strlen(buffer);
			Fragment * fragment = buildFragment(id++, buffer, readSize);
			node->next = fragment;
			node = fragment;
		}

	}
	return head;
}
