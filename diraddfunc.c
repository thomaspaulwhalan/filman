#ifndef DIRADDFUNC_H
#include <diraddfunc.h>

int parentaddress(char *diradd)
{
	/* Empty address array */
	for (int addcount = MAXLENGTH; addcount >= 0; addcount--) {
		if (diradd[addcount] != '/') {
			diradd[addcount] = '\0';
		}
		else if (diradd[addcount] == '/') {
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
	FAIL_IF_R(testchange < 0, -1);

	return 0;
}

/* This function changes the universal
 * variable diraddress to the /home/user
 * address of the current user */
int homeaddress(char *diradd)
{
	struct passwd *pw = getpwuid(getuid());	
	char *gohome = pw->pw_dir;

	for (int addcount = 0; addcount < MAXLENGTH; addcount++) {
		diradd[addcount] = '\0';
	}
	strcpy(diradd, gohome);



	int testchange = chdir(diradd);
	FAIL_IF_R(testchange < 0, -1);

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
	diradd[0] = '/';

	int testchange = chdir(diradd);
	FAIL_IF_R(testchange < 0, -1);

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
	FAIL_IF_R(testchange < 0, -1);

	return 0;
}

#endif
