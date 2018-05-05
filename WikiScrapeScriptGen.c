#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
  int opt;
  char* path;
  if(argc > 2){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  while((opt = getopt(argc, argv, "")) != -1){
    switch(opt){
      case '?':
        printf("ERROR: Invalid argument\n");
        break;
    }
  }
  path = argv[optind];
  FILE* fp;
  if(path == NULL){
    printf("ERROR: No file argument indicated\n");
    return 1;
  }
  else{
    fp = fopen(path, "w");
    if(fp == NULL){
      printf("ERROR: Failed to open or create file\n");
      return 1;
    }
  }
  int i, j, txtCount = 0, grepCmdPos = 0;
  char grepCmd[11][BUFSIZ];
  for(i = 0; i < 11; i++) strcpy(grepCmd[i], "grep -oh [a-zA-Z]* ");
  for(i = 65; i < 91; i++){ //65 equivalent to 'A'; 90 equivalent to 'Z'
    for(j = 65; j < 91; j++){
      char wgetCmd[BUFSIZ];
      sprintf(wgetCmd, "wget https://en.wikipedia.org/wiki/%c%c -O %c%c.html&\n",
        (char) i, (char) j, (char) i, (char) j);
      fputs(wgetCmd, fp);
      if(txtCount++ == 66){
        txtCount = 0;
        grepCmdPos++;
      }
      char temp[BUFSIZ];
      sprintf(temp, "%c%c.txt ", (char) i, (char) j);
      strcat(grepCmd[grepCmdPos], temp);
    }
  }
  fputs("barrier\n", fp);
  for(i = 65; i < 91; i++){
    for(j = 65; j < 91; j++){
      char lynxCmd[BUFSIZ];
      sprintf(lynxCmd, "lynx -dump -nolist %c%c.html > %c%c.txt&\n",
        (char) i, (char) j, (char) i, (char) j);
      fputs(lynxCmd, fp);
    }
  }
  fputs("barrier\n", fp);
  for(i = 0; i < 11; i++){
    char temp[BUFSIZ];
    sprintf(temp, "> allwords%d.txt\n", i);
    strcat(grepCmd[i], temp);
    fputs(grepCmd[i], fp);
  }
  fputs("cat ", fp);
  for(i = 0; i < 11; i++){
    char temp[BUFSIZ];
    sprintf(temp, "allwords%d.txt ", i);
    fputs(temp, fp);
  }
  fputs("> allwords.txt\n", fp);
  fputs("sort -o allwords_sorted.txt allwords.txt\n", fp);
  fputs("uniq -ic allwords_sorted.txt count_uniqwords.txt\n", fp);
  fputs("sort -k 1,1n count_uniqwords.txt\n", fp);
  fputs("quit\n", fp);
  return 0;
}
