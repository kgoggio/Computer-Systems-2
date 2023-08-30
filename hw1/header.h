/*-------------------------------------------------------------------------*
 *---									---*
 *---		header.h						---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a					Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define		MAX_LINE	256

#define		RANGE_LOWEST	0

#define		RANGE_HIGHEST	32767

#define		MIN_NUM_NUMBERS	0

#define		MAX_NUM_NUMBERS	0x40000000

void countWithList(int);
void countWithTree(int);
int getNextNumber();