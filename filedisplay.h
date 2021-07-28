#ifndef FILEDISPLAY_H
#define FILEDISPLAY_H

#ifndef _ncurses_h_
#include<ncurses.h>
#endif
#ifndef _dirent_h_
#include<dirent.h>
#endif
#ifndef _stat_h_
#include<sys/stat.h>
#endif
#ifndef _string_h_
#include<string.h>
#endif
#ifndef NCURSES_H
#include<ncurses.h>
#endif
#ifndef DIRADDFUNC
#include<diraddfunc.h>
#endif

/* Defines */
/* SCROLLHEIGHT is the height of the scroll window */
#define SCROLLHEIGHT 20

#ifndef MAXLENGTH
#define MAXLENGTH 256
#endif

/* Function declarations */
/* displaydir() displays the contents of a directory. Currently set to the current directory */
int displaydir(WINDOW *win, int heightofwin, struct dirent **directory, int numoffiles, int counter);
/* displaytext() displays the contents of a file in text format */
int displaytext(WINDOW *win, int heightofwin, struct dirent **directory, int counter, int ystartpos);
/* copyfile() will copy the directory address of a file */
int copyfile(struct dirent **directory, int counter);
/* pastefile will take the address from copyfile() and
 * create a copy of it in the desired directory. If it
 * is pasted to the same directory, it will add
 * "copyof" to the start of the file name */
int pastefile(void);
char copypaste[MAXLENGTH];

#endif // FILEDISPLAY_H
