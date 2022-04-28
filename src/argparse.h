#ifndef _ARGPARSE_H
#define _ARGPARSE_H

#include <getopt.h>
#include <stdlib.h>
#include <iostream>

struct options_t {
    char *inputs;
    int num_cluster;
    int dims;
    int max_num_iter;
    int seed;
    double threshold;
    bool c;
    int implementation;
};

void get_opts(int argc, char **argv, struct options_t *opts);
#endif
