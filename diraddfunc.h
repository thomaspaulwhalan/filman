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
char diraddress[MAXLENGTH] = "/";

/* This function takes the universal variable
 * diraddress and changes the directory path
 * that it represents to the parent directory
 * (unless already at root) */
int parentaddress(char *diradd)
{
	/* This for loop works backwards through the
	 * diradd array and zeros out upto and including
	 * the first '/' that it comes across */
	for (int addcount = MAXLENGTH; addcount >= 0; addcount--) {
		if (diradd[addcount] != '/') {
			diradd[addcount] = '\0';
		}
		else if (diradd[addcount] == '/') {
			/* This if statement ensures that if
			 * someone tries to go to the
			 * no-existent parent directory
			 * of root, they are unable to do
			 * so. The for loop simply breaks */
			if (addcount == 0) {
				break;
			}
			else {
				diradd[addcount] = '\0';
				break;
			}
		}
	}
	int testchange = chdir(diradd);
	
	if (testchange < 0) {
		return -1;
	}

	return 0;
}

/* This function changes the universal
 * variable diraddress to the /home/user
 * address of the current user */
int homeaddress(char *diradd)
{
	/* The following two lines gets the user id
	 * information from the system and stores
	 * the current user's /home/user to their
	 * own path */
	struct passwd *pw = getpwuid(getuid());	
	char *gohome = pw->pw_dir;

	/* This for loop copies the user id info
	 * into the diradd array */
	for (int addcount = 0; addcount < MAXLENGTH; addcount++) {
		diradd[addcount] = '\0';
	}
	strcpy(diradd, gohome);



	int testchange = chdir(diradd);
	if (testchange < 0) {
		return -1;
	}

	return 0;
}

/* This function changes the universal
 * variable diraddress back to the root
 * address */
int rootaddress(char *diradd)
{
	/* This for loop zeros out the diradd */
	for (int addcount = 0; addcount < MAXLENGTH; addcount++) {
		diradd[addcount] = '\0';
	}
	/* Then a '/' is added to the
	 * first position in the array */
	diradd[0] = '/';

	int testchange = chdir(diradd);
	if (testchange < 0) {
		return -1;
	}

	return 0;
}

/* This function takes the universal variable
 * diraddress and appends a new directory to
 * the end */
int newaddress(char *diradd, const char *newdir)
{
	char diraddress[MAXLENGTH] = "/";

	/* If diraddress is not already at root,
	 * add a '/' */
	if (strcmp(diradd, diraddress) != 0) {
		strcat(diradd, "/");
	}
	/* Otherwise, we are already at root and
	 * just the directory gets appended */
	strcat(diradd, newdir);

	int testchange = chdir(diradd);
	if (testchange < 0) {
		return -1;
	}

	return 0;
}
