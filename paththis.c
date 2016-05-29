#include<unistd.h>
#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pwd.h>
#include<string.h>

int main(void)
{
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	struct passwd *user = getpwuid(getuid());
	char *homedir = strcat(user->pw_dir,"/.bashrc");
	FILE *profile;
	profile=fopen(homedir, "a");	
	fprintf(profile,"%s", "PATH=");
	fprintf(profile,"%s", path);
	fprintf(profile,"%s", "/:$PATH\n");
	fclose(profile);
	return 0;
}
