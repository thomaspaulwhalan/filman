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

/* This is the function that displays the directory files. */
int displaydir(WINDOW *win, int heightofwin, struct dirent **directory, int numoffiles, int counter) {
	/* struct stat sbuf helps us to get information
	 * regarding a file such as whether or not it
	 * is a directory */
	struct stat sbuf;
	/* scrolltrack is used in this way:
	 * It is set equal to counter (which in
	 * this program gets its value from
	 * the filecounter variable (above)).
	 * Continued explanation at the while loop */
	int scrolltrack = counter;
	/* Every time we use the displaydir function,
	 * we want the cursor moved back to the same
	 * starting position. It is the able to erase
	 * the old contents as it writes the new
	 * contents to screen. */
	wmove(win, 0, 0);
	/* So long as counter is less than the
	 * number of files, and counter is less than
	 * the height of the window + scrolltracker,
	 * print the items to the screen */
	/* Essentially, the first value of counter is
	 * used as the starting point. Items in directory
	 * before that point are no included and it only
	 * includes enough  items to fill the screen */
	/* Without scrolltrack being added to heightofwin,
	 * no new items will be appended */
	while (counter < numoffiles && counter < heightofwin + scrolltrack) {
		/* Fill out the sbuf stat structure */

		stat(directory[counter]->d_name, &sbuf);
		if (S_ISDIR(sbuf.st_mode)) {
			/* If the current directory is chosen, it
			 * will simply take you back to the directory
			 * that you were in when you first ran the
			 * program. For that reason, I have labelled
			 * it home.
			 * I will later change the Current Working
			 * Directory to be the HOME folder */
			if (strcmp(".", directory[counter]->d_name) == 0) {
				wprintw(win, "--RETURN HOME\n");
			}
			/* If the parent directory is chosen, it will
			 * take you to the parent directory */
			else if (strcmp("..", directory[counter]->d_name) == 0) {
				wprintw(win, "--PARENT DIR\n");
			}
		/* If an item is a directory, '<' and '>'
		 * are added one on either side of the
		 * directory name */
			else {
				wprintw(win, "<%s>\n", directory[counter]->d_name);
			}
		}
		/* I will add other markings as well.
		 * I would like a mark to show executable
		 * files, read and write status etc */
		/* If there is nothing else to add,
		 * Just print the plain old boring
		 * filename */
		else {
			wprintw(win, "%s\n", directory[counter]->d_name);
		}
		/* Increment counter or we'll never progress */
		counter++;
		wrefresh(win);
	}
	return 0;
}

/* This is the function that displays the contents of text files. */
int displaytext(WINDOW *win, int heightofwin, struct dirent **directory, int counter, int ystartpos) {
	/* Create a new FILE struct variable */
	FILE *contentsfile;
	/* The ch character variable is used to iterate throughthe text file */
	char ch;
	/* ycounter increments with each '\n' character it encounters
	 * until it reaches ystartpos. The contents of our file
	 * are displayed from that point onward */
	int ycounter = 0;
	/* Fill out the FILE struct variable */
	contentsfile = fopen(directory[counter]->d_name, "r");
	/* Before we display anything, we must change the cursor
	 * position to 0, 0 */
	wmove(win, 0, 0);
	/* While not the End of File, iterate through each character
	 * in the file and add it to the screen */
	while ((ch = fgetc(contentsfile)) != EOF) {
		if  (ycounter < ystartpos) {
			if (ch == '\n') {
				ycounter++;
			}
		}
		else if (ycounter != SCROLLHEIGHT + ystartpos) {
			if (ch == '\n') {
				ycounter++;
			}
			waddch(win, ch);
		}
	}
	/* If a newline is not the last character, whatever the
	 * last line is will fill the screen when the EOF is reached */
	waddch(win, '\n');
	/* Close the file to wrap things up */
	fclose(contentsfile);
	/* Refresh the window so that it all appears on the screen */
	wrefresh(win);

	return 0;
}

int copyfile(struct dirent **directory, int counter)
{

	return 0;
}

int pastefile(void)
{
	return 0;
}
