#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <pthread.h>
#include <limits.h>
#include <ctype.h>






pthread_t tid[500];
char gandafile[2000][2000];
const char *name[] = {"jpg","jpeg", "png", "tar.gz", "c", "js", "zip", "gns3project","bin", "hex", "txt", "fbx", "gif" };

int  formatfile(void){
int formatNama = 0;
do
{
      mkdir(name[formatNama], S_IRWXU);
      ++formatNama;
}

while (formatNama < 14);
}
int isRegular(const char *path){
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}

void FileRekursive(char *basePath, int *iter)
{
    // int iter = 0;
  char path[1000];
  struct dirent *display;
  DIR *dir = opendir(basePath);



  while ((display = readdir(dir)) != NULL)
  {
    if (strcmp(display->d_name, ".") != 0 && strcmp(display->d_name, "..") != 0)
      {
        strcpy(path, basePath), strcat(path, "/"),strcat(path, display->d_name) ;

        if(isRegular(path)){
          strcpy(gandafile[*iter], path);
          *iter = *iter + 1;
        }
        else{
          FileRekursive(path, iter);
        }

      }
  }
    closedir(dir);
}



void *pindahFile(void *arg){
  char str[999], temporary[999], temporary2[999], temporary3[999], temporary4[999], path[1000], tempDest[1000], cwd[1000], namafile[1000], fileExt[1000], fileExt2[1000];

  int isFile = isRegular(path);
  getcwd(cwd, sizeof(cwd)), strcpy(path, (char*) arg), strcpy(temporary4, path);
  
  char *fileExt3, dot1 = '.';

  fileExt3 = strchr(temporary4, dot1);
  strcpy(temporary, path);

  char *token=strtok(temporary, "/");
  do{
       sprintf(namafile, "%s", token);
      token = strtok(NULL, "/");
  }
  while(token != NULL);

  strcpy(temporary, path), strcpy(temporary2, namafile), strcpy(temporary3, namafile);

  int total = 0;
  char *token2=strtok(temporary2, ".");

  sprintf(fileExt2, "%s", token2);
    do
    {
        total++;

      sprintf(fileExt, "%s", token2);

      token2=strtok(NULL, ".");
    }

    while(token2 != NULL);


  char dot = '.', first = temporary3[0];



  if(dot == first) 
    {
     strcpy(fileExt, "Hidden");
    }

  else if(total >= 3)
      {
        strcpy(fileExt, fileExt3+1);
      }

  else if (total <=1 )
      {
        strcpy(fileExt, "Unknown");
      }

  int i = 0;
  do
  {
    fileExt[i] = tolower(fileExt[i]);
    i++;
  }
  while(i < sizeof(fileExt));

  strcpy(temporary, (char*) arg);
  mkdir(fileExt, 0777);
  strcat(cwd, "/"), strcat(cwd,fileExt),  strcat(cwd, "/"), strcat(cwd, namafile), strcpy(tempDest, cwd), rename(temporary, tempDest);
  return (void *) 1;
}

int main(int argc, char *argv[]) {
  void *status;
  char basePath[1000], cwd[1000];
  int iter = 0;


    getcwd(cwd, sizeof(cwd)),  strcpy(basePath, cwd), FileRekursive(basePath, &iter);
    pthread_t tid[iter];
      int i = 0;
      do
      {
          pthread_create(&tid[i], NULL, pindahFile, (void*)gandafile[i]);
          pthread_join(tid[i], &status);
          i++;
      }
      while (i<iter);
      exit(EXIT_FAILURE);

}
