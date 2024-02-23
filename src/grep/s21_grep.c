#include "s21_grep.h"

int main(int argc, char* argv[]) {
  flags_t Flags = {0};
  lines_t Patterns = {0, NULL};
  lines_t Files = {0, NULL};

  ArgvParsing(argc, argv, &Flags, &Patterns, &Files);
  if (Patterns.num && Files.num) {
    MyGrep(&Flags, &Patterns, &Files);
  }

  ClearLines(&Patterns);
  ClearLines(&Files);

  return 0;
}

void MyGrep(flags_t* Flags, lines_t* Patterns, lines_t* Files) {
  regex_t compiled_mask;

  int cflags = REG_EXTENDED | REG_NEWLINE;
  if (Flags->i) cflags |= REG_ICASE;

  char* mask = BuildMask(Patterns);
  int is_compiled = 0;
  if (mask) {
    is_compiled = !regcomp(&compiled_mask, mask, cflags);
    free(mask);
  }

  FILE* fp = NULL;
  for (int k = Flags->shift; k < Files->num && is_compiled; k++) {
    fp = fopen(Files->lines[k], "r");
    if (fp) {
      GrepFile(Flags, &compiled_mask, Files->lines[k], fp);
      fclose(fp);
    } else {
      if (!Flags->s)
        fprintf(stderr, "%s: No such file or directory", Files->lines[k]);
    }
  }
  if (is_compiled) regfree(&compiled_mask);
}

void GrepFile(flags_t* Flags, regex_t* mask_p, char* filename, FILE* fp) {
  int match_lines = 0;
  int line_num = 0;

  int all_is_ok = 1;
  while (all_is_ok) {
    char new_str[STR_MAX] = {0};
    if (fgets(new_str, STR_MAX, fp)) {
      line_num++;
      size_t nmatch = 1;
      regmatch_t pmatch[1] = {0};
      int match = regexec(mask_p, new_str, nmatch, pmatch, 0) == Flags->v;
      match_lines += match;
      if (match && !Flags->c && !Flags->l) {
        if (!Flags->h) printf("%s:", filename);
        if (Flags->n) printf("%d:", line_num);
        if (!Flags->o) {
          printf("%s", new_str);
          if (new_str[strlen(new_str) - 1] != '\n') printf("\n");
        } else {
          char* substring = new_str;
          do {
            PrintSubstring(pmatch[0].rm_so, pmatch[0].rm_eo, substring);
            substring += pmatch[0].rm_eo;
          } while (!regexec(mask_p, substring, nmatch, pmatch, 0));
        }
      }
      if (Flags->l && match_lines) all_is_ok = 0;
    } else {
      all_is_ok = 0;
    }
  }
  if (Flags->c) {
    if (!Flags->h) printf("%s:", filename);
    printf("%d\n", match_lines);
  }
  if (Flags->l && match_lines) {
    printf("%s\n", filename);
  }
}

void PrintSubstring(int start, int end, char* str) {
  for (int k = start; k < end; k++) printf("%c", str[k]);
  printf("\n");
}

char* BuildMask(lines_t* Patterns) {
  char* mask = NULL;
  size_t mask_len = 0;
  for (int k = 0; k < Patterns->num; k++) {
    mask = realloc(mask,
                   (mask_len + strlen(Patterns->lines[k]) + 2) * sizeof(char));
    if (mask) {
      strcpy(mask + mask_len, Patterns->lines[k]);
      strcat(mask, "|");
      mask_len = strlen(mask);
    } else {
      k = Patterns->num;
      free(mask);
      mask = NULL;
    }
  }
  if (mask) mask[strlen(mask) - 1] = 0;
  return mask;
}

void ArgvParsing(int argc, char* argv[], flags_t* Flags, lines_t* Patterns,
                 lines_t* Files) {
  int new_opt = 0;
  while (optind < argc) {
    new_opt = getopt(argc, argv, "ivclhnsoe:f:");
    switch (new_opt) {
      case 'i':
        Flags->i = 1;
        break;
      case 'v':
        Flags->v = 1;
        break;
      case 'c':
        Flags->c = 1;
        break;
      case 'l':
        Flags->l = 1;
        break;
      case 'h':
        Flags->h = 1;
        break;
      case 'n':
        Flags->n = 1;
        break;
      case 's':
        Flags->s = 1;
        break;
      case 'o':
        Flags->o = 1;
        break;
      case 'e':
        AddLine(Patterns, optarg);
        break;
      case 'f':
        PatternsFromFile(Patterns, optarg);
        break;
      case -1:
        AddLine(Files, argv[optind++]);
        break;
    }
  }
  if (!Patterns->num && Files->num) {
    AddLine(Patterns, Files->lines[0]);
    Flags->shift = 1;
  }
  if (Files->num - Flags->shift == 1) Flags->h = 1;
  if (Flags->c || Flags->l || Flags->v) Flags->o = 0;
}

void PatternsFromFile(lines_t* Dest, char* filename) {
  FILE* fp = NULL;
  fp = fopen(filename, "r");
  char new_str[STR_MAX] = {0};
  if (fp) {
    while (fgets(new_str, STR_MAX, fp)) {
      if (new_str[strlen(new_str) - 1] == '\n')
        new_str[strlen(new_str) - 1] = 0;
      if (strlen(new_str)) AddLine(Dest, new_str);
      memset(new_str, 0, STR_MAX);
    }
    fclose(fp);
  }
}

void AddLine(lines_t* Dest, char* new_line) {
  Dest->lines = realloc(Dest->lines, (Dest->num + 1) * sizeof(char*));
  if (Dest->lines) {
    Dest->lines[Dest->num] = NULL;
    Dest->lines[Dest->num] = calloc(strlen(new_line) + 1, sizeof(char));
    if (Dest->lines[Dest->num]) {
      strcpy(Dest->lines[Dest->num], new_line);
      Dest->num++;
    }
  }
}

void ClearLines(lines_t* Lines) {
  for (int k = 0; k < Lines->num; k++) free(Lines->lines[k]);
  free(Lines->lines);
}