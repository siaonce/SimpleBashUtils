#include "s21_cat.h"


int main(int argc, char* argv[]) {
    if (argc > 1) {
        s21_cat(argc, argv);
    }
    return 0;
}

void s21_cat(int argc, char* argv[]) {
    fg* q = calloc(1, sizeof(fg));
    if (q == NULL) {
        exit(1);
    } else {
        q = s21_init_flag(q);
        int i = 1;
        while (i < argc) {
            if (argv[i][0] == '-') {
                q = s21_op(argv, q, i);
            } else {
                break;
            }
            i++;
        }
        while (i < argc) {
            s21_cat_output(q, argv, i);
            i++;
        }
        free(q);
    }
}

// flags in terminal

fg* s21_op(char* argv[], fg* q, int i) {
    for (unsigned long j = 1; j < strlen(argv[i]); j++) {
        switch (argv[i][j]) {
            case 'b':
                q->flag_b = 1;
                break;
            case 'e':
                q->flag_e = 1;
                q->flag_v = 1;
                break;
            case 'n':
                q->flag_n = 1;
                break;
            case 's':
                q->flag_s = 1;
                break;
            case 't':
                q->flag_v = 1;
                q->flag_t = 1;
                break;
            case 'E':
                q->flag_e = 1;
                break;
            case 'v':
                q->flag_v = 1;
                break;
            case 'T':
                q->flag_t = 1;
                break;
            case '-':
                if (strcmp(argv[i], "--number") == 0) {
                    q->flag_n = 1;
                }
                if (strcmp(argv[i], "--number-nonblank") == 0) {
                    q->flag_b = 1;
                }
                if (strcmp(argv[i], " --squeeze-blank") == 0) {
                    q->flag_s = 1;
                }
                argv[i][j] = '\0';
                break;
            case '?':
                printf("Find Error!");
                break;
        }
    }
    return q;
}



fg* s21_init_flag(fg* q) {
    q->flag_b = 0;
    q->flag_e = 0;
    q->flag_s = 0;
    q->flag_n = 0;
    q->flag_t = 0;
    q->flag_v = 0;
    return q;
}


int s21_flag_s(char now_ch, char pr_pr_ch, char pr_ch) {
    int ok = 0;
    if ((pr_pr_ch == '\n') && (pr_ch == '\n') && (now_ch == '\n')) {
        ok = 1;
    }
    return ok;
}

int s21_flag_n(int counter) {
    printf("%6d\t", counter);
    counter++;
    return counter;
}

int s21_flag_b(int counter) {
    printf("%6d\t", counter);
    counter++;
    return counter;
}

void s21_flag_e() {
    printf("$");
}

int s21_flag_t(int counter) {
    counter = 73;
    printf("^");

    return counter;
}

char s21_flag_v(char now_ch) {
    if ((now_ch >= 0) && (now_ch <= 31) && (now_ch != 10) && (now_ch != 9)) {
        printf("^");
        now_ch += 64;
    }
    if (now_ch == 127) {
        now_ch -= 64;
        printf("^");
    }
    return now_ch;
}



void s21_cat_output(fg* q, char* argv[], int i) {
    FILE* cat = fopen(argv[i], "r");
    int counter = 1;
    char now_ch = 0;
    int pro = 0;
    char pr_pr_ch = 0;
    char pr_ch = '\n';
    if (cat == NULL) {
        printf("No such file or directory");
    }
    while ((now_ch = fgetc(cat)) != EOF) {
        if ((q->flag_s == 1) && (s21_flag_s(now_ch, pr_pr_ch, pr_ch) == 1)) {
            continue;
        }
        if ((q->flag_n == 1) && (q->flag_b == 0) && (pro == 0)) {
            counter = s21_flag_n(counter);
            pro++;
        }
        if ((q->flag_b == 1) && (pro == 0) && (now_ch != '\n')) {
                counter = s21_flag_b(counter);
                pro++;
            }
        if ((q->flag_e == 1) && (q->flag_v == 1) && (now_ch == '\n')) {
            s21_flag_e();
            now_ch = s21_flag_v(now_ch);
        }
        if (q->flag_v == 1) {
            now_ch = s21_flag_v(now_ch);
        }
        if ((q->flag_t == 1) && (q->flag_v == 1) && (now_ch == '\t')) {
            now_ch = s21_flag_v(now_ch);
            now_ch = s21_flag_t(counter);
        }
        if ((q->flag_t == 1) && (q->flag_v == 0) && (now_ch == '\t')) {
            now_ch = s21_flag_t(counter);
        }
        if ((q->flag_e == 1) && (q->flag_v == 0) && (now_ch == '\n')) {
            s21_flag_e();
        }
        if (now_ch == '\n') {
            pro = 0;
        }
        printf("%c", now_ch);
        pr_pr_ch = pr_ch;
        pr_ch = now_ch;
    }
    fclose(cat);
}
