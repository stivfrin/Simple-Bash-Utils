#include "s21_cat.h"

int main(int argc, char* argv[]) {
  if (argc > 1) {
    flags_t Flags = {0};
    Parsing(argc, argv, &Flags);
    MyCat(argc, argv, &Flags);
  }
  return 0;
}

void Parsing(int argc, char* argv[], flags_t* Flags) {
  struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                  {"number", 0, NULL, 'n'},
                                  {"squeeze-blank", 0, NULL, 's'},
                                  {NULL, 0, NULL, 0}};
  for (int opt = getopt_long(argc, argv, "+beEnstTv", long_options, 0);
       opt != -1; opt = getopt_long(argc, argv, "+beEnstTv", long_options, 0)) {
    switch (opt) {
      case 'b':
        Flags->b = 1;
        break;
      case 'v':
        Flags->v = 1;
        break;
      case 'e':
        Flags->e = Flags->v = 1;
        break;
      case 'E':
        Flags->e = 1;
        break;
      case 'n':
        Flags->n = 1;
        break;
      case 's':
        Flags->s = 1;
        break;
      case 't':
        Flags->t = Flags->v = 1;
        break;
      case 'T':
        Flags->t = 1;
        break;
    }
  }
}

void MyCat(int argc, char* argv[], flags_t* Flags) {
  FILE* fp = NULL;
  for (int i = optind; i < argc; i++) {
    fp = fopen(argv[i], "r");
    if (fp) {
      CatFile(fp, Flags);
      fclose(fp);
    } else {
      fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[i]);
    }
  }
}

void CatFile(FILE* fp, flags_t* Flags) {
  int str_counter = 0;
  int pr_ch = '\n';
  int new_ch = fgetc(fp);

  while (new_ch != EOF) {
    if (Flags->s && pr_ch == '\n' && new_ch == '\n') {
      while (new_ch == '\n') new_ch = fgetc(fp);
      if (new_ch != EOF) fseek(fp, -1, SEEK_CUR);
      new_ch = '\n';
    }
    if (pr_ch == '\n')
      if ((Flags->n && !Flags->b) || (Flags->b && new_ch != '\n'))
        printf("%6d\t", ++str_counter);

    PrintingSymbol(new_ch, Flags);
    pr_ch = new_ch;
    new_ch = fgetc(fp);
  }
}

void PrintingSymbol(int symbol, flags_t* Flags) {
  int flag_to_print = 1;
  if (symbol == EOF) {
    flag_to_print = 0;
  }
  if (symbol == '\n' && Flags->e) {
    printf("$\n");
    flag_to_print = 0;
  }
  if (symbol == '\t' && Flags->t) {
    printf("^I");
    flag_to_print = 0;
  }
  if (Flags->v && flag_to_print) {
    if (symbol == 127) {
      printf("^?");
      flag_to_print = 0;
    }
    if (symbol < 32 && symbol != '\n' && symbol != '\t') {
      printf("^%c", symbol + 64);
      flag_to_print = 0;
    }
    if (symbol > 127 && symbol < 160) {
      printf("M-^%c", symbol - 64);
      flag_to_print = 0;
    }
  }
  if (flag_to_print) printf("%c", symbol);
}
