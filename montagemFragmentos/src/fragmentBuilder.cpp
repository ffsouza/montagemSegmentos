/*
 * fragmentBuilder.c
 *
 *  Created on: Nov 22, 2014
 *      Author: ffsouza
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fragment.hpp"

long readFragments(FILE* in, uint maxSize, Fragment** root) {
	char* buffer = (char*) malloc(sizeof(char) * (1 + maxSize));
	uint readSize;
	Fragment* head = NULL;

	long id = 0;
	if (fscanf(in, "%s", buffer)) {
		readSize = strlen(buffer);
		Fragment * fragment = buildFragment(id++, buffer, readSize);
		Fragment* node = head = fragment;

		while (fscanf(in, "%s", buffer) > 0) {
			readSize = strlen(buffer);
			if(readSize == 0){
				break;
			}
			fragment = buildFragment(id++, buffer, readSize);
			node->next = fragment;
			node = fragment;
		}

	}
	free(buffer);
	*root = head;
	return id;
}
