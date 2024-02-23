#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAX 2000

typedef struct grep_flags {
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
  int shift;
} flags_t;

typedef struct patterns_and_filenames {
  int num;
  char **lines;
} lines_t;

void MyGrep(flags_t *Flags, lines_t *Patterns, lines_t *Files);
void GrepFile(flags_t *Flags, regex_t *mask_p, char *filename, FILE *fp);
char *BuildMask(lines_t *Patterns);
void PrintSubstring(int start, int end, char *str);

void ArgvParsing(int argc, char *argv[], flags_t *Flags, lines_t *Patterns,
                 lines_t *Files);
void PatternsFromFile(lines_t *Dest, char *filename);
void AddLine(lines_t *Dest, char *new_line);
void ClearLines(lines_t *Lines);

#endif