/*
 * fragment.c
 *
 *  Created on: Nov 22, 2014
 *      Author: ffsouza
 */
#include <string.h>
#include <stdlib.h>
#include "fragment.hpp"

Fragment* buildFragment(long id, char* sequenceBuffer, uint n) {
	Fragment* fragment = (Fragment*) malloc(sizeof(Fragment));
	fragment-> id = id;
	fragment->sequence = (char*) malloc(sizeof(char) * n);
	fragment->next = NULL;
	strncpy(fragment->sequence, sequenceBuffer, n);
	fragment->sequenceSize = n;
	return fragment;
}
