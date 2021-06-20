#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info.h"

int length(char *);

int main() {
    char *os = getosname();
    if(!os) {
        os = malloc(8);
        if(!os)
            die("malloc");
        strcpy(os, "unknown");
    }

    int distro = getdistro(os);
    int pkgs = getpkgcount(distro);
    char *ver = getkernelver();

    struct uptime up = getuptime();
    struct memory mem = getmeminfo();

    struct logo logo = logos[distro];
    int color = logo.color;

    int max = 0;
    for(int i = 0; i != 5; i++) {
        char *line = logo.lines[i];
        int temp = length(line);
        if(temp > max)
            max = temp;
    }

    for(int row = 0; logo.lines[row]; row++) {
        char *line = logo.lines[row];
        printf("%s", line);

        int len = length(line);
        for(int n = 4 + max - len; n > 0; n--)
            putchar(' ');

        switch(row) {
            case 0:
                printf("\033[1;%dmos\033[0m      %s", color, os);
                free(os);
                break;
            case 1:
                printf("\033[1;%dmkernel\033[0m  %s", color, ver);
                free(ver);
                break;
            case 2:
                printf("\033[1;%dmup\033[0m      ", color);
                if(up.d)
                    printf("%ldd ", up.d);
                if(up.h)
                    printf("%ldh ", up.h);
                if(up.m)
                    printf("%ldm", up.m);
                break;
            case 3:
                if(pkgs != -1)
                    printf("\033[1;%dmpkgs\033[0m    %d", color, pkgs);
                else
                    printf("\033[1;%dmpkgs\033[0m    unknown", color);
                break;
            case 4:
                printf("\033[1;%dmmemory\033[0m  %luM / %luM", color, mem.used, mem.total);
                break;
        }
        putchar('\n');
    }
    putchar('\n');
    return 0;
}

int length(char *s) {
    int len = 0;
    for(char *c = s; *c; c++) {
        if(*c == 033) {
            for(c++; *c && *c != 'm'; c++);
            continue;
        }
        len++;
    }
    return len;
}
