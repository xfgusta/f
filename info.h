#ifndef INFO_H
#define INFO_H

#define eq(s) (!strcmp(os, s))
#define die(s) {perror(s); exit(1);}

enum {
    ARCHLINUX,
    FEDORA,
    MANJARO,
    UBUNTU,
    LINUXMINT,
    DEBIAN,
    POPOS,
    OPENSUSE,
    UNKNOWN
};

struct logo {
    char *lines[20];
    int color;
};

extern struct logo logos[];

struct uptime {
    long d, h, m;
};

struct memory {
    unsigned long used, total; 
};

char *getosname();
int getdistro(char *);
int getpkgcount(int);
char *getkernelver();
struct uptime getuptime();
struct memory getmeminfo();

#endif
