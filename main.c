#include<ncurses.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<pwd.h>
#include<unistd.h>
#include<stdlib.h>

#ifndef _diraddfunc_h_
#include"diraddfunc.h"
#endif
#ifndef _filedisplay_h_
#include"filedisplay.h"
#endif

#include<errno.h>

int main(void)
{
	initscr();
	int testhad;
	/* I probably won't keep too many color capabilities
	 * as I want the UI to be fairly simple,
	 * but we shall see */
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);

	/* Here I am declaring some two window variables.
	 * The first simply houses a box frame.
	 * The second sits inside the box window
	 * and is where most of the action happens */
	WINDOW *boxwin;
	WINDOW *scrollwin;
	WINDOW *dialogwin;

	boxwin = newwin(SCROLLHEIGHT + 2, COLS, 0, 0);
	scrollwin = newwin(SCROLLHEIGHT, COLS - 2, 1, 1);
	dialogwin = newwin(4, 30, SCROLLHEIGHT - 3, 1); 
	
	/* Set the colors for the windows */
	wbkgd(boxwin, COLOR_PAIR(2));
	wbkgd(scrollwin, COLOR_PAIR(2));
	box(boxwin, 0, 0);
	wbkgd(dialogwin, COLOR_PAIR(1));

	/* Headings for each of the windows
	 * and a quit message at the bottom
	 * of the boxwin screen */
	mvwprintw(boxwin, 0, 1, "FILE VIEWER");
	mvwprintw(boxwin, SCROLLHEIGHT+1, 1, "Press 'q' to exit");

	/* testwin is where my error messages show when I need it*/
	//WINDOW *testwin;
	//testwin = newwin(5, 0, 23, 0);
	//wbkgd(testwin, COLOR_PAIR(1));

	/* boxwin is refreshed first then scrollwin
	 * so that scrollwin is on top */
	wrefresh(boxwin);
	wrefresh(scrollwin);
	//wrefresh(testwin);
	
	/* Let's get rid of that ugly cursor and turn off echo */
	curs_set(0);
	noecho();
	/* And turn on keyboard capabilities for scrollwin */
	keypad(scrollwin, TRUE);

	/* dirr holds the names of the files in a directory */
	struct dirent **dirr;
	/* numberoffiles has a double use.
	 * I use it here to test whether or not scandir
	 * returns successful, but also when we get to
	 * testing where we are upto in THE SCROLL */
	int numberoffiles;
	/* Defined in dirparse.h this homeaddress
	 * changes the current working directory
	 * to the /home/user directory */
	rootaddress(diraddress);
	numberoffiles = scandir(diraddress, &dirr, NULL, alphasort);
	/* testsbuf will help us to determine whether or not
	 * a file is a directory and can be set as the
	 * new current directory */
	struct stat testsbuf;

	/* filecounter and highlighter are used to track
	 * where we are up to in THE SCROLL */
	int filecounter = 0;
	int highlighter = 0;

	/* textystartpos helps us when we are scrolling
	 * through the contents of a file */
	int textystartpos;

	/* This is our first use of displaydir.
	 * This gives us our initial instance
	 * of the sccrolling menu */
	displaydir(scrollwin, SCROLLHEIGHT, dirr, numberoffiles, filecounter);
	mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);

	/* good ol' ch variable. Whilst it holds characters,
	 * the KEY_UP, KEY_DOWN and others have a higher
	 * number than char variables can hold */
	int ch;
	while (TRUE) { // We don't want this nugget to end unless WE want it to
		/* We are about to start the switch, but first
		 * we need to get a value for ch */
		ch = wgetch(scrollwin);
		switch (ch) {
			case KEY_DOWN: // If the down key is pressed...,
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
					/* Get the sorted list of files and stor in dirr
					 * Also store the number of files in numberoffiles */
					numberoffiles = scandir(diraddress, &dirr, NULL, alphasort);
					wclear(scrollwin);
					/* Rest the highlighter and filecounter variables to 0
					 * That way when displaydir is used, we can start
					 * back in the proper position */
					highlighter = 0;
					filecounter = 0;
					/* Update the screen with the new working directory */
					displaydir(scrollwin, SCROLLHEIGHT, dirr, numberoffiles, filecounter);
					/* Highlight the first item in the list */
					mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
					wrefresh(scrollwin);	
				}
				/* If the file is a regular file */
				if (S_ISREG(testsbuf.st_mode)) {
					/* textystartpos is the first line in the file */	
					textystartpos = 0;
					/* Clear the scrreen then display the contents of the file */
					wclear(scrollwin);
					displaytext(scrollwin, SCROLLHEIGHT, dirr, highlighter+filecounter, textystartpos);
					/* Press q to exit */
					while ((ch = wgetch(scrollwin)) != 'q') {
						switch (ch) {
							/* If the down key is pressed, textystartpos is increased.
							 * When the window is refreshed, it starts from the next line.
							 * Thus it gives the appearance of scrolling */
							case KEY_DOWN:
								/* There is currently nothing to stop
								 * us from scrolling to infinity. I'll
								 * deal with it later */
								textystartpos++;
								break;
							/* If the up key is pressed, textystartpos is decreased.
							 * When the window is refreshed, it starts from the preceding line. */
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
						/* Update the screen to refelct any changes made */
						displaytext(scrollwin, SCROLLHEIGHT, dirr, highlighter+filecounter, textystartpos);
					}
					/* When we are finished viewing the contents of the file,
					 * We must fill the screen again with our directory list
					 * and highlight the first item in the list */
					wclear(scrollwin);
					displaydir(scrollwin, SCROLLHEIGHT, dirr, numberoffiles, filecounter);
					mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
					wrefresh(scrollwin);	
				}
				break;
			/* To delete a file press 'd' */
			case 'd':
				/* stat is used tp test file state */
				stat(dirr[highlighter+filecounter]->d_name, &testsbuf);
				/* If the first or second item is highlighted,
				 * the computer will continue */
				if (highlighter + filecounter == 0) {
					continue;
				}
				else if (highlighter + filecounter == 1) {
					continue;
				}
				/* If the file is a regular file,
				 * a confirmation dialog box will
				 * be displayed */
				else if (S_ISREG(testsbuf.st_mode)) {
					wclear(dialogwin);
					wprintw(dialogwin, "Are you sure that you want to delete the file: '%s'?\n\n", dirr[highlighter+filecounter]->d_name);
					wprintw(dialogwin, "y/n");
					wrefresh(dialogwin);
					while ((ch = wgetch(dialogwin))) {
						/* If 'n' is pressed, the programe goes back
						 * to the directory listing */
						if (ch == 'n') {
							break;
						}
						/* If 'y' is pressed... */
						else if (ch == 'y') {
							/* Delete the file */
							remove(dirr[highlighter+filecounter]->d_name);
							/* Update the directory listing */
							numberoffiles = scandir(diraddress, &dirr, NULL, alphasort);
							/* Reset the highlighter and filecounter position */
							highlighter = 0;
							filecounter = 0;
							break;
						}
					}
					/* Clear scrollwin, display new directory contents, highlight
					 * the first item, and refresh */
					wclear(scrollwin);
					displaydir(scrollwin, SCROLLHEIGHT, dirr, numberoffiles, filecounter);
					mvwchgat(scrollwin, highlighter, 0, 20, A_STANDOUT, 0, NULL);
					wrefresh(scrollwin);
				}
				break;
			/* To rename a file press 'r' */
			case 'r':
				break;
			/* To edit a file press 'e' */
			case 'e':
				break;
			default:
				break;
		}
	}

	endwin();
	
	return 0;
}
