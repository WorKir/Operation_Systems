#include <stdio.h>

#include <sys/stat.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <sys/types.h>

int Opened;
FILE * ex, * f1, * f2;

struct stat buff, buff1, buff2;

void State() {
  printf("\n__________________________________________________________________________\n | ID of device | Type and mode |User ID | Total size | \n_________________________________________________________________________\n");

  for (int i = 1; i < Opened; ++i) {

    char integer_string[32];
    char s[20] = "";

    sprintf(integer_string, "%d", i);
    strcat(s, integer_string);
    strcat(s, ".txt");

    stat(s, & buff);

    stat(s, & buff);
    printf("%s\n", "");
    printf("%s", s);
    printf("%s", "	| ");
    printf("%lld", buff.st_dev);
    printf("%s", "		|	");
    printf("%d", buff.st_mode);
    printf("%s", "		|	");
    printf("%d", buff.st_uid);
    printf("%s", "		|	");
    printf("%ld", buff.st_size);
    printf("%s\n", "	|");
  }
}

int main()

{
  Opened = 1;

  char s[] = "hi hello bonjour guten tag";
  char buff[20];

  char buff1[15];

  char buff2[45];

  printf("Open file.txt and read/write:\n");
  ex = fopen("file.txt", "w+");
  Opened++;
  State();

  fwrite(s, strlen(s) + 1, 1, ex);
  fseek(ex, 0, SEEK_SET);
  fread(buff, strlen(s) + 1, 1, ex);
  printf("%s\n", buff);
  fclose(ex);

  printf("Open 1.txt:\n");

  f1 = fopen("1.txt", "r");
  Opened++;

  State();
  fclose(f1);

  printf("\nOpen 2.txt:\n");

  f2 = fopen("2.txt", "r");
  Opened++;

  State();
  fclose(f2);

  f1 = fopen("1.txt", "wb");
  fwrite(buff, sizeof(buff), 1, f1);
  printf("\nSuccessfully written into 1.txt\n");
  fclose(f1);

  f2 = fopen("2.txt", "r");
  fread(buff1, sizeof(buff1), 1, f2);
  printf("Read 2.txt - SUCCESS\n");
  fclose(f2);

  f1 = fopen("1.txt", "wb");
  fwrite(buff, sizeof(buff2), 1, f1);
  printf("\nSuccessfully written into 1.txt\n");
  fclose(f1);

  return 0;
}