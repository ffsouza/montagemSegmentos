/*
 * params.h
 *
 *  Created on: Aug 12, 2012
 *      Author: ffsouza
 */

#ifndef PARAMS_H_
#define PARAMS_H_

int loadParams(int argc, char **argv);
int checkIfState(char * value);
void treatParam(char * value);
void clearParams();

char* getParamOutput();
int getParamSequenceSize();
int getParamSequenceStdDeviation();
int getParamSequenceNumber();

#endif /* PARAMS_H_ */
