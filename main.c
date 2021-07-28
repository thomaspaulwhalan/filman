#ifndef NCURSES_H
#include<ncurses.h>
#endif
#ifndef DIRENT_H
#include<dirent.h>
#endif
#ifndef STAT_H
#include<sys/stat.h>
#endif
#ifndef string_H
#include<string.h>
#endif
#ifndef PWD_H
#include<pwd.h>
#endif
#ifndef UNISTD_H
#include<unistd.h>
#endif
#ifndef STDLIB_H
#include<stdlib.h>
#endif
#ifndef ERRNO_H
#include<errno.h>
#endif

#ifndef DIRADDFUNC_H
#include<diraddfunc.h>
#endif
#ifndef FILEDISPLAY_H
#include<filedisplay.h>
#endif
#ifndef NERROR_H
#include<nerror.h>
#endif

int main(void)
{
	initscr();
	int testhad;
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);

	WINDOW *boxwin;
	WINDOW *scrollwin;
	WINDOW *dialogwin;

	boxwin = newwin(SCROLLHEIGHT + 2, COLS, 0, 0);
	scrollwin = newwin(SCROLLHEIGHT, COLS - 2, 1, 1);
	dialogwin = newwin(4, 30, SCROLLHEIGHT - 3, 1); 
	
	wbkgd(boxwin, COLOR_PAIR(2));
	wbkgd(scrollwin, COLOR_PAIR(2));
	box(boxwin, 0, 0);
	wbkgd(dialogwin, COLOR_PAIR(1));

	mvwprintw(boxwin, 0, 1, "FILE VIEWER");
	mvwprintw(boxwin, SCROLLHEIGHT+1, 1, "Press 'q' to exit");

	wrefresh(boxwin);
	wrefresh(scrollwin);
	
	curs_set(0);
	noecho();
	keypad(scrollwin, TRUE);

	struct dirent **dirr;
	int numberoffiles;
	rootaddress(diraddress);
	numberoffiles = scandir(diraddress, &dirr, NULL, alphasort);
	struct stat testsbuf;

	int filecounter = 0;
	int highlighter = 0;

	int textystartpos;

	displaydir(scrollwin, SCROLLHEIGHT, dirr, numberoffiles, filecounter);
	mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);

	int ch;
	while (true) {
		ch = wgetch(scrollwin);
		switch (ch) {
			case KEY_DOWN:
				/* If the number of files is contained within the screen size*/
				if (numberoffiles < (SCROLLHEIGHT + 1)) {
					/* If the highlighted file is not the last in the list,
					 * Highlight the next item in the list */
					if (highlighter < numberoffiles - 1) {
						mvwchgat(scrollwin, highlighter, 0, 20, A_NORMAL, 0, NULL);
						highlighter++;
						mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
						wrefresh(scrollwin);
					}
					/* If the highlighted file IS the last in the list,
					 * go back to the top */
					else if (highlighter == numberoffiles - 1) {
						mvwchgat(scrollwin, highlighter, 0, 20, A_NORMAL, 0, NULL);
						highlighter = 0;
						mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
						wrefresh(scrollwin);
					}
				}
				/* If the number of files goes beyond the screen,
				 * they are dealt with in the following */
				/* If highlighter is less than the number of files shown
				 * on the screen up to the second to last item,
				 * Highlight the next item in the list */
				else if (highlighter < SCROLLHEIGHT - 1) { /* height of window - 1 */
					mvwchgat(scrollwin, highlighter, 0, 20, A_NORMAL, 0, NULL);
					highlighter++;
					mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
					wrefresh(scrollwin);
				}
				/* If the filecounter is less than the number of files - screen size,
				 * keep the highligh at the same position, but scroll the screen down,
				 * thus highlighting the next item */
				else if (filecounter < (numberoffiles - SCROLLHEIGHT)) {
					filecounter++;
					displaydir(scrollwin, SCROLLHEIGHT, dirr, numberoffiles, filecounter);
					mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
				}
				/* If the very last item in list is highlighted,
				 * Go all the way back to the top and highlight
				 * the first item */
				else if (filecounter == (numberoffiles - SCROLLHEIGHT)) {
					highlighter = 0;
					filecounter = 0;
					displaydir(scrollwin, SCROLLHEIGHT, dirr, numberoffiles, filecounter);
					mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
					wrefresh(scrollwin);
				}
				break;
			case KEY_UP:
				/* If there are not enough files to fill the screen */
				if (numberoffiles < (SCROLLHEIGHT + 1)) {
					/* If the first item is highlighted,
					 * highlight the last item in the list */
					if (highlighter == 0) {
						mvwchgat(scrollwin, highlighter, 0, 20, A_NORMAL, 0, NULL);
						highlighter = numberoffiles - 1;
						mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
						wrefresh(scrollwin);
					}
					/* Otherwise, highlight the previous item in the list */
					else if (highlighter > 0) {
						mvwchgat(scrollwin, highlighter, 0, 20, A_NORMAL, 0, NULL);
						highlighter--;
						mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
						wrefresh(scrollwin);
					}
				}
				/* If the number of files goes beyond the screen,
				 * they are dealt with in the following */
				/* if the first item in the list is highlighted..., */
				else if (highlighter == 0 ) {
					/* If this is the very first item in the list,
					 * highlight the very last item in the list */
					if (filecounter == 0) {
						filecounter = numberoffiles - (SCROLLHEIGHT - 1);
						mvwchgat(scrollwin, highlighter, 0, 20, A_NORMAL, 0, NULL);
						displaydir(scrollwin, SCROLLHEIGHT, dirr, numberoffiles, filecounter);
						highlighter = SCROLLHEIGHT - 1;
						mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
						wrefresh(scrollwin);
					}
					/* If this is the first item on the screen,
					 * but more exist before it, scroll up one*/
					if (filecounter > 0)
						filecounter--;
						mvwchgat(scrollwin, highlighter, 0, 20, A_NORMAL, 0, NULL);
						displaydir(scrollwin, SCROLLHEIGHT, dirr, numberoffiles, filecounter);
						mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
						wrefresh(scrollwin);
				}
				/* If the top item is not highlighted,
				 * highlight the previous item */
				else if (highlighter > 0) {
					mvwchgat(scrollwin, highlighter, 0, 20, A_NORMAL, 0, NULL);
					highlighter--;
					mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
					wrefresh(scrollwin);
				}
				break;
				/* Press 'q' to exit */
			case 'q':
				endwin();
				return 0;
				break;
			case '\n':
				/* First we need to test whether the file
				 * is a directory or not */
				stat(dirr[highlighter+filecounter]->d_name, &testsbuf);
				if (S_ISDIR(testsbuf.st_mode)) {
					if (strcmp(dirr[highlighter+filecounter]->d_name, ".") == 0) {
						testhad = homeaddress(diraddress);
						if (testhad < 0) {
							endwin();
							puts("Oops! Something went werong!");
							return 0;
						}
					}
					else if (strcmp(dirr[highlighter+filecounter]->d_name, "..") == 0) {
						parentaddress(diraddress);
					}
					else {
						/* Use newaddress "dirparse.h" to update the
						 * working directory */
						newaddress(diraddress, dirr[highlighter+filecounter]->d_name);
					}
					/* Get file list */
					numberoffiles = scandir(diraddress, &dirr, NULL, alphasort);
					wclear(scrollwin);
					
					highlighter = 0;
					filecounter = 0;
					
					displaydir(scrollwin, SCROLLHEIGHT, dirr, numberoffiles, filecounter);
					
					mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
					wrefresh(scrollwin);	
				}
				/* If the file is a regular file */
				if (S_ISREG(testsbuf.st_mode)) {
					/* textystartpos is the first line in the file */	
					textystartpos = 0;
					/* Clear the screen then display the contents of the file */
					wclear(scrollwin);
					displaytext(scrollwin, SCROLLHEIGHT, dirr, highlighter+filecounter, textystartpos);
					while ((ch = wgetch(scrollwin)) != 'q') {
						switch (ch) {
							case KEY_DOWN:
								/* There is currently nothing to stop
								 * us from scrolling to infinity. I'll
								 * deal with it later */
								textystartpos++;
								break;
							case KEY_UP:
								/* This just stops us from scrolling past
								 * the start of the file */
								if (textystartpos != 0) {
									textystartpos--;
								}
								break;
							default:
								break;
						}
						displaytext(scrollwin, SCROLLHEIGHT, dirr, highlighter+filecounter, textystartpos);
					}
					/* Return to main screen */
					wclear(scrollwin);
					displaydir(scrollwin, SCROLLHEIGHT, dirr, numberoffiles, filecounter);
					mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
					wrefresh(scrollwin);	
				}
				break;
			case 'd':
				stat(dirr[highlighter+filecounter]->d_name, &testsbuf);
				if (highlighter + filecounter == 0) {
					continue;
				}
				else if (highlighter + filecounter == 1) {
					continue;
				}
				else if (S_ISREG(testsbuf.st_mode)) {
					wclear(dialogwin);
					wprintw(dialogwin, "Are you sure that you want to delete the file: '%s'?\n\n", dirr[highlighter+filecounter]->d_name);
					wprintw(dialogwin, "y/n");
					wrefresh(dialogwin);
					while ((ch = wgetch(dialogwin))) {
						if (ch == 'n') {
							break;
						}
						else if (ch == 'y') {
							remove(dirr[highlighter+filecounter]->d_name);
							numberoffiles = scandir(diraddress, &dirr, NULL, alphasort);
							highlighter = 0;
							filecounter = 0;
							break;
						}
					}
					wclear(scrollwin);
					displaydir(scrollwin, SCROLLHEIGHT, dirr, numberoffiles, filecounter);
					mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
					wrefresh(scrollwin);
				}
				break;
			/* To rename a file press 'r' - NOT IMPLEMENTED */
			case 'r':
				break;
			/* To edit a file press 'e' - NOT IMPLEMENTED */
			case 'e':
				break;
			default:
				break;
		}
	}

	endwin();
	
	return 0;
}
