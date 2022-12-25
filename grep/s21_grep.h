#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <regex.h>


typedef struct flags {
    int flag_e;
    int flag_i;
    int flag_v;
    int flag_c;
    int flag_l;
    int flag_n;
    int flag_f;
    int flag_h;
    int flag_s;
    char *pattern;
} air;


void s21_grep(int argc, char* argv[]);
air* s21_init_flags(air* f);
int s21_op(int argc, char* argv[], air* f);
void s21_output(int argc, char **argv, air *tab, int count_files);
int s21_file(air* f, int argc, char* argv[]);
regex_t s21_comp_pattern(regex_t regex, air* f);
void s21_print_file_name(int count_file, air* f, char* argv_now);
void s21_flag_n(int count_str, air* f);

#endif  // SRC_GREP_S21_GREP_H_
