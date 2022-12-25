#include "s21_grep.h"
static int err = 0;  // ERROR == -1

int main(int argc, char* argv[]) {
    s21_grep(argc, argv);
    return 0;
}

void s21_grep(int argc, char* argv[]) {
    air* f = calloc(1, (sizeof(air)));
    int count_file = 0;
    int count_flags = 0;
    if (f == NULL) {
        exit(1);
    } else {
        f = s21_init_flags(f);
        count_flags = s21_op(argc, argv, f);
        count_file = s21_file(f, argc, argv);
        if (argc == 1) {
            printf("[-flag] [-pattern]");
        } else {
            for (int i = 1; i < argc; i++) {
                if (argv[i][0] != '-' && count_flags == 0) {
                    f->pattern = realloc(f->pattern, strlen(argv[i]) + 10);
                    strcat(f->pattern, argv[i]);
                    strcat(f->pattern, "|");
                    i = argc;
                }
            }
            if (err == 0) {
                s21_output(argc, argv, f, count_file);
            }
        }
        free(f->pattern);
        free(f);
    }
}


int s21_op(int argc, char* argv[], air* f) {
    int count_flags = 0;
    for (int i = 1; i < argc; i++) {
        int check_e = 0;
        int check_f = 0;
        if (argv[i][0] == '-') {
            for (size_t j = 1; j < strlen(argv[i]); j++) {
                switch (argv[i][j]) {
                    case 'e':
                        f->flag_e = 1;
                        if (argv[i+1] != NULL && check_e != 1) {
                            check_e = 1;
                            f->pattern = realloc(f->pattern, strlen(argv[i]) + 1000);
                            strcat(f->pattern, argv[i + 1]);
                            strcat(f->pattern, "|");
                        }
                        count_flags++;
                        break;
                    case 'f':
                        f->flag_f = 1;
                        if (argv[i+1] != NULL && check_f != 1) {
                            check_f = 1;
                            FILE* file;
                            if ((file = fopen(argv[i + 1], "r")) != NULL) {
                                while (!feof(file)) {
                                    char* str = calloc(5000, sizeof(char) + 1);
                                    fgets(str, 5000, file);
                                    f->pattern = realloc(f->pattern, strlen(str) + 1000);
                                    if (feof(file) && strchr(str, '\n') == NULL) {
                                        strcat(f->pattern, str);
                                    } else {
                                        strncat(f->pattern, str, strlen(str) -1);
                                    }
                                    strcat(f->pattern, "|");
                                    free(str);
                                }
                                fclose(file);
                            } else {
                                if (f->flag_s != 1) {
                                    printf("No such file or directory");
                                }
                            }
                        }
                        count_flags++;
                        break;
                    case 'i':
                        f->flag_i = 1;
                        break;
                    case 'v':
                        f->flag_v = 1;
                        break;
                    case 'c':
                        f->flag_c = 1;
                        break;
                    case 'l':
                        f->flag_l = 1;
                        break;
                    case 'n':
                        f->flag_n = 1;
                        break;
                    case 'h':
                        f->flag_h = 1;
                        break;
                    case 's':
                        f->flag_s = 1;
                        break;
                    default:
                        printf("Find Error!");
                        err = -1;
                        break;
                }
                if (err == -1) {
                    break;
                }
            }
            if (check_e == 1 || check_f == 1) {
                i++;
            }
            if (err == -1) {
                    break;
            }
        }
    }
    return count_flags;
}

air* s21_init_flags(air* f) {
    f->flag_e = 0;
    f->flag_i = 0;
    f->flag_v = 0;
    f->flag_c = 0;
    f->flag_l = 0;
    f->flag_n = 0;
    f->flag_f = 0;
    f->flag_h = 0;
    f->flag_s = 0;
    f->pattern = calloc(1, 1);
    return f;
}

int s21_file(air* f, int argc, char* argv[]) {
        int res = 0;
    if (f->flag_f || f->flag_e) {
        for (int i = 1; i < argc; i++) {
            if (argv[i - 1][0] == '-'
            && (strstr(argv[i - 1], "e") != NULL || strstr(argv[i - 1], "f") != NULL))
                i++;
            if (i >= argc)
                break;
            if (argv[i][0] != '-') {
                res++;
            }
        }
    } else {
        int checker = 0;
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-') {
                checker++;
                if (checker > 1) {
                    res++;
                }
            }
        }
    }
    return res;
}


void s21_output(int argc, char **argv, air *f, int count_files) {
    FILE *fd;
    int zaglushka = 0;

    f->pattern[strlen(f->pattern) - 1] = '\0';
    if (f->flag_e || f->flag_f) {
        zaglushka++;
    }
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (((strchr(argv[i - 1], 'e')) != NULL || (strchr(argv[i - 1], 'f') != NULL))
                && argv[i - 1][0] == '-') {
                i++;
            }
        }
        zaglushka++;
        if (argv[i] != NULL && zaglushka > 1 && argv[i][0] != '-') {
            if ((fd = fopen(argv[i], "r")) != NULL) {
                int counter_str = 1;
                int l = 0;
                int count_c = 0;

                while (!feof(fd)) {
                    char *buf = NULL;
                    regex_t regex;
                    int sucsess = 0;

                    buf = (char*)calloc(5000, sizeof(char) + 2);
                    fgets(buf, 5000, fd);
                    regex = s21_comp_pattern(regex, f);
                    sucsess = regexec(&regex, buf, 0, NULL, 0);

                    // flag c
                    if (f->flag_c == 1) {
                        if (f->flag_v == 1) {
                            if (sucsess == 1 && !feof(fd)) {
                                count_c++;
                            }
                        } else if (sucsess == 0 && f->flag_v != 1 && !feof(fd)) {
                                count_c++;
                        }
                    }
                    // flag l
                    if (f->flag_l == 1) {
                        if (f->flag_v == 1) {
                            if (sucsess == 1) {
                                l = 1;
                            }
                        } else if (sucsess == 0 && f->flag_v != 1) {
                            l = 1;
                        }
                    }
                    // output string in stdout
                    if (f->flag_c == 0 && f->flag_l == 0) {
                        if (f->flag_v == 1) {
                            if (sucsess == 1) {
                                if (!feof(fd)) {
                                    s21_print_file_name(count_files, f, argv[i]);
                                    s21_flag_n(counter_str, f);
                                }
                                printf("%s", buf);
                            }
                        } else if (sucsess == 0 && f->flag_v != 1) {
                            if (!feof(fd)) {
                                s21_print_file_name(count_files, f, argv[i]);
                                s21_flag_n(counter_str, f);
                            }
                            printf("%s", buf);
                        }
                    }
                    counter_str++;
                    regfree(&regex);
                    free(buf);
                }  // EOF argv[i]

                // output flag c and flag l
                if (f->flag_c == 1 && f->flag_l == 0) {
                    s21_print_file_name(count_files, f, argv[i]);
                    printf("%d\n", count_c);
                } else if (f->flag_l == 1 && f->flag_c == 0 && l == 1) {
                    printf("%s\n", argv[i]);
                } else if (f->flag_c == 1 && f->flag_l == 1) {
                    count_c = l;
                    s21_print_file_name(count_files, f, argv[i]);
                    printf("%d\n", count_c);
                    if (l == 1) {
                        printf("%s\n", argv[i]);
                    }
                } else {
                    if (!feof(fd)) {
                        s21_print_file_name(count_files, f, argv[i]);
                    }
                }
                fclose(fd);
            }
        }
    }
}

regex_t s21_comp_pattern(regex_t regex, air* f) {
    if (f->flag_i == 1) {
        regcomp(&regex, f->pattern, REG_ICASE);
        if (&regcomp != 0) {
            exit(1);
        }
    } else {
        regcomp(&regex, f->pattern, REG_EXTENDED);
        if (&regcomp != 0) {
            exit(1);
        }
    }
    return regex;
}

void s21_flag_n(int count_str, air* f) {
    if (f->flag_n == 1) {
        printf("%d:", count_str);
    }
}

void s21_print_file_name(int count_file, air* f, char* argv_now) {
    if (count_file > 1 && f->flag_h == 0) {
        printf("%s:", argv_now);
    }
}
