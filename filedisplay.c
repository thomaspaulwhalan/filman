#ifndef FILEDISPLAY_H
#include<filedisplay.h>

int displaydir(WINDOW *win, int heightofwin, struct dirent **directory, int numoffiles, int counter)
{
	struct stat sbuf;
	int scrolltrack = counter;
	wmove(win, 0, 0);
	while (counter < numoffiles && counter < heightofwin + scrolltrack) {
		stat(directory[counter]->d_name, &sbuf);
		if (S_ISDIR(sbuf.st_mode)) {
			if (strcmp(".", directory[counter]->d_name) == 0) {
				wprintw(win, "--RETURN HOME\n");
			}
			else if (strcmp("..", directory[counter]->d_name) == 0) {
				wprintw(win, "--PARENT DIR\n");
			}
			/* Add '<' and '>' to directories */
			else {
				wprintw(win, "<%s>\n", directory[counter]->d_name);
			}
		}
		else {
			wprintw(win, "%s\n", directory[counter]->d_name);
		}
		
		counter++;
		wrefresh(win);
	}
	return 0;
}

/* This is the function that displays the contents of text files. */
int displaytext(WINDOW *win, int heightofwin, struct dirent **directory, int counter, int ystartpos)
{
	FILE *contentsfile;
	char ch;
	int ycounter = 0;
	contentsfile = fopen(directory[counter]->d_name, "r");
	wmove(win, 0, 0);
	
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
	
	waddch(win, '\n');
	
	fclose(contentsfile);
	
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

#endif // FILEDISPLAY_H
