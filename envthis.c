#include<stdlib.h>
#include<limits.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<pwd.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
  if (argv[1] == NULL ) {
    printf("Please enter the variable name as argument\nenvthis HOME\n");
    return(1);
  }
  char *p = argv[1];
  while (*p != '\0') {
    *p = toupper(*p);
    p++;
  }

  char path[PATH_MAX];
  getcwd(path, PATH_MAX);
  struct passwd *user = getpwuid(getuid());
  char* homedir = strcat(user->pw_dir, "/.bashrc");

  FILE *fp = fopen(homedir, "a");
  fprintf(fp, "%s", strcat(argv[1],"="));
  fprintf(fp, "%s", path);
  fprintf(fp, "%s", "\n");
  fclose(fp);
  return 0;
}
