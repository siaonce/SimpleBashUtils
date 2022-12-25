#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags {
    int flag_b;
    int flag_e;
    int flag_s;
    int flag_n;
    int flag_t;
    int flag_v;
} fg;


void s21_cat(int argc, char* argv[]);
fg* s21_op(char* argv[], fg* q, int i);
fg* s21_init_flag(fg* q);
int s21_flag_n(int counter);
int s21_flag_b(int counter);
void s21_flag_e();
int s21_flag_t(int counter);
char s21_flag_v(char now_ch);
int s21_flag_s(char now_ch, char pr_pr_ch, char pr_ch);
void s21_cat_output(fg* q, char* argv[], int i);

#endif  // SRC_CAT_S21_CAT_H_
