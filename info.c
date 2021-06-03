#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <proc/sysinfo.h>

#include "info.h"

struct logo logos[] = {
    [ARCHLINUX] = {{"\033[1;34m      /\\\033[0m",
                    "\033[1;34m     /  \\\033[0m",
                    "\033[1;34m    /\\   \\\033[0m",
                    "   /      \\",
                    "  /   ,,   \\",
                    " /   |  |  -\\",
                    "/_-''    ''-_\\"}, 34},

    [FEDORA] = {{"\033[1;37m      _____",
                 "     /   __)\033[1;34m\\\033[0m",
                 "     |  /  \033[1;34m\\ \\\033[0m",
                 "  \033[1;34m__\033[1;37m_|  |_\033[1;34m_/ /\033[0m",
                 " \033[1;34m/ \033[1;37m(_    _)\033[1;34m_/\033[0m",
                 "\033[1;34m/ /\033[1;37m  |  |\033[0m",
                 "\033[1;34m\\ \\\033[1;37m__/  |\033[0m",
                 " \033[1;34m\\\033[1;37m(_____/\033[0m"}, 34},

    [MANJARO] = {{"\033[1;32m||||||||| ||||\033[0m",
                  "\033[1;32m||||||||| ||||\033[0m",
                  "\033[1;32m||||      ||||\033[0m",
                  "\033[1;32m|||| |||| ||||\033[0m",
                  "\033[1;32m|||| |||| ||||\033[0m",
                  "\033[1;32m|||| |||| ||||\033[0m",
                  "\033[1;32m|||| |||| ||||\033[0m"}, 32},

    [UBUNTU] = {{"\033[1;33m         _\033[0m",
                 "\033[1;33m     ---(_)\033[0m",
                 "\033[1;33m _/  ---  \\\033[0m",
                 "\033[1;33m(_) |   |   \033[0m",
                 "\033[1;33m  \\  --- _/\033[0m",
                 "\033[1;33m     ---(_)\033[0m"}, 33},

    [LINUXMINT] = {{"\033[1;32m _____________\033[0m",
                    "\033[1;32m|_            \\\033[0m",
                    "\033[1;32m  |  \033[0m| _____  \033[1;32m|\033[0m",
                    "\033[1;32m  |  \033[0m| | | |  \033[1;32m|\033[0m",
                    "\033[1;32m  |  \033[0m| | | |  \033[1;32m|\033[0m",
                    "\033[1;32m  |  \033[0m\\__\033[1;32m___/  \033[1;32m|\033[0m",
                    "\033[1;32m  \\___________/\033[0m"}, 32},

    [DEBIAN] = {{"\033[1;31m   ,---._\033[0m",
                 "\033[1;31m /`  __  \\\033[0m",
                 "\033[1;31m|   /    |\033[0m",
                 "\033[1;31m|   `.__.`\033[0m",
                 "\033[1;31m \\\033[0m",
                 "\033[1;31m  `-,_\033[0m"}, 31},

    [POPOS] = {{"\033[1;34m______\033[0m",
                "\033[1;34m\\   _ \\        __\033[0m",
                "\033[1;34m \\ \\ \\ \\      / /\033[0m",
                "\033[1;34m  \\ \\_\\ \\    / /\033[0m",
                "\033[1;34m   \\  ___\\  /_/\033[0m",
                "\033[1;34m    \\ \\    _\033[0m",
                "\033[1;34m   __\\_\\__(_)_\033[0m",
                "\033[1;34m  (___________)\033[0m"}, 34},

    [OPENSUSE] = {{"\033[1;32m    _______\033[0m",
                   "\033[1;32m-___|   __ \\\033[0m",
                   "\033[1;32m       / \033[0m.\033[1;32m\\ \\\033[0m",
                   "\033[1;32m       \\__/ \033[1;32m|\033[0m",
                   "\033[1;32m     _______|\033[0m",
                   "\033[1;32m     \\_______\033[0m",
                   "\033[1;32m--__________/\033[0m"}, 32},

    [UNKNOWN] = {{"    .--.",
                  "   |o_o |",
                  "   |:_/ |",
                  "  //   \\ \\",
                  " (|     | )",
                  "/'|_   _/'\\",
                  "\\___)=(___/"}, 33}
};

static char *pkgcmd[] = {
    [ARCHLINUX] = "pacman -Qq | wc -l",
    [FEDORA] = "rpm -qa | wc -l",
    [MANJARO] = "pacman -Qq | wc -l",
    [UBUNTU] = "dpkg-query -W | wc -l",
    [LINUXMINT] = "dpkg-query -W | wc -l",
    [DEBIAN] = "dpkg-query -W | wc -l",
    [POPOS] = "dpkg-query -W | wc -l",
    [OPENSUSE] = "rpm -qa | wc -l",
    [UNKNOWN] = "echo -1"
};

char *getosname() {
    FILE *file = fopen("/etc/os-release", "r");
    if(!file) {
        file = fopen("/lib/os-release", "r");
        if(!file)
            die("fopen");
    }

    if(fseek(file, 0, SEEK_END) == -1)
        die("fseek");

    long size = 0;
    if((size = ftell(file)) == -1)
        die("ftell");

    rewind(file);

    char *data = malloc(size + 1);
    if(!data)
        die("malloc");

    fread(data, size, 1, file);
    data[size] = 0;
    fclose(file);

    char *name = NULL;
    for(char *c = data; *c; c++) {
        if(!strncmp(c, "NAME=", 5)) {
            c += 5;
            char *start;
            long len = 0;
            if(*c == '\"' || *c == '\'')
                for(start = ++c; *c && (*c != '\"' && *c != '\''); c++, len++);
            else
                for(start = c; *c && *c != '\n' ; c++, len++);

            name = malloc(len + 1);
            if(!name)
                die("malloc");

            strncpy(name, start, len);
            name[len] = 0;
            break;
        }
        else
            for(; *c && *c != '\n'; c++);
    }

    free(data);
    return name;
}

int getdistro(char *os) {
    if(eq("Arch Linux"))
        return ARCHLINUX;
    else if(eq("Fedora") || eq("Fedora Linux"))
        return FEDORA;
    else if(eq("Manjaro Linux"))
        return MANJARO;
    else if(eq("Ubuntu"))
        return UBUNTU;
    else if(eq("Linux Mint"))
        return LINUXMINT;
    else if(eq("Debian GNU/Linux"))
        return DEBIAN;
    else if(eq("Pop!_OS"))
        return POPOS;
    else if(eq("openSUSE") || eq("openSUSE Tumbleweed") || eq("openSUSE Leap"))
        return OPENSUSE;
    else
        return UNKNOWN;
}

int getpkgcount(int distro) {
    FILE *file = popen(pkgcmd[distro], "r");
    if(!file)
        die("popen");
    int count = 0;
    fscanf(file, "%d", &count);
    pclose(file);
    return count;
}

char *getkernelver() {
    struct utsname name;
    if(uname(&name) == -1)
        die("uname");
    char *ver = malloc(strlen(name.release) + 1);
    if(!ver)
        die("malloc");
    strcpy(ver, name.release);
    return ver;
}

struct uptime getuptime() {
    struct sysinfo info;
    if(sysinfo(&info) == -1)
        die("sysinfo");
    long sec = info.uptime;
    return (struct uptime) {sec / 86400, sec / 3600 % 24, sec / 60 % 60};
}

struct mem getmeminfo() {
    meminfo();
    return (struct mem) {kb_main_used / 1024, kb_main_total / 1024};
}
