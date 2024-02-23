#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>

typedef struct {
  int b;
  int n;
  int e;
  int s;
  int t;
  int v;
} flags_t;

void Parsing(int argc, char* argv[], flags_t* Flags);
void MyCat(int argc, char* argv[], flags_t* Flags);
void CatFile(FILE* fp, flags_t* Flags);
void PrintingSymbol(int symbol, flags_t* Flags);

#endif
