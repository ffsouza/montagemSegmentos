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
	int id;
	char* sequence;
	struct t_fragment* next;
} Fragment;

Fragment* buildFragment(int id, char* sequence, uint n);

#endif /* FRAGMENT_H_ */
