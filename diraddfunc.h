#ifndef DIRADDFUNC_H
#define DIRADDFUNC_H

#ifndef _stdio_h_
#include<stdio.h>
#endif
#ifndef _string_h_
#include<string.h>
#endif
#ifndef _pwd_h_
#include<pwd.h>
#endif
#ifndef _unistd_h_
#include<unistd.h>
#endif

#ifndef NERROR_H
#include<nerror.h>
#endif

/* Generally, the maximum character length
 * of any given directory address should
 * fir within the confines of a 256 byte
 * character array */
#ifndef MAXLENGTH
#define MAXLENGTH 256
#endif

/* Function prototypes.
 * These are defined below */
int parentaddress(char *diradd);
int homeaddress(char *diradd);
int rootaddress(char *diradd);
int newaddress(char *diradd, const char *newdir);

/* A universal variable to hold the
 * directory address. I have
 * pre-initialized it to be the root
 * directory */
static char diraddress[MAXLENGTH] = "/";

#endif //DIRADDFUNC_H
