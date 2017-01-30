#pragma once

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#define PATH_STRLEN strlen
#define PATH_TOLOWER tolower
#define PATH_STRCMP strcmp
#define PATH_STRNCPY strncpy
#define PATH_STRCPY strcpy
//#define PATH_STRCAT strcat
#define PATH_STRNCAT strncat
#define PATH_SNPRINTF snprintf

FILE* fopen_unix(const char* fname, const char* mode);
