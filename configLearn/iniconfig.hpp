#ifndef __RECONFIG_HPP__
#define __RECONFIG_HPP__

#include <iostream>
#include <vector>
#include <cstdio>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

using namespace std;

#define PRINT_ERRMSG(STR) fprintf(stderr, "line:[%d], msg:[%s], eMsg:[%s]\n", __LINE__, STR, strerror(errno))

#define MAX_VALUE       1024

#define TRIM_LEFT       1 
#define TRIM_RIHGT      2
#define TRIM_BETWEEN    3

#define ARRAY_NUM       6

typedef struct _option
{
    char * key;
    char * value;
    struct _option *next;
}Option;

typedef struct _data
{
    char * section;
    Option * option;
    struct _data * next;
}Data;

typedef struct _config
{
    unsigned char    comment;
    unsigned char    separator;
    char *  re_string;
    char    re_int;
    bool    re_bool;
    double  re_double;
    std::vector<float>   re_array;
    Data *  data;
}Config;

typedef struct __ARRAY{
    float array[ARRAY_NUM];
}Array;

#endif