/*
 * fragment.h
 *
 *  Created on: Nov 22, 2014
 *      Author: ffsouza
 */

#ifndef FRAGMENT_H_
#define FRAGMENT_H_

typedef unsigned int uint;

typedef struct t_fragment {
	long id;
	char* sequence;
	struct t_fragment* next;
} Fragment;

Fragment* buildFragment(long id, char* sequence, uint n);

#endif /* FRAGMENT_H_ */
