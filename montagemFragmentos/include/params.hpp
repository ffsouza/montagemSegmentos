/*
 * params.h
 *
 *  Created on: Nov 22th, 2014
 *      Author: Felipe Faria de Souza
 */

#ifndef PARAMS_MONTAGEM_H_
#define PARAMS_MONTAGEM_H_

//Comment it to disable debug messages
#define DEBUG_ENABLED

int loadParams(int argc, char **argv);
int checkIfState(char * value);
void treatParam(char * value);
void clearParams();

char* getParamOutput();
char* getParamInput();
int getParamSequenceOverlap();
float getParamSequenceError();

#endif /* PARAMS_H_ */
