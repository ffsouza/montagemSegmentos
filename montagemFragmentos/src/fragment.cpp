/*
 * fragment.c
 *
 *  Created on: Nov 22, 2014
 *      Author: ffsouza
 */
#include <string.h>
#include <stdlib.h>
#include "fragment.h"

Fragment* buildFragment(long id, char* sequenceBuffer, uint n) {
	Fragment* fragment = (Fragment*) malloc(sizeof(Fragment));
	fragment-> id = id;
	fragment->sequence = (char*) malloc(sizeof(char) * (n+1));
	fragment->next = NULL;
	strncpy(fragment->sequence, sequenceBuffer, n);
	fragment->sequence[n] = '\0';
	return fragment;
}
