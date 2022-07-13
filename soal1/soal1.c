#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

//Deklarasi Thread, child, dan lokasi
pthread_t thread_1[2], thread_2[2];
pid_t child1, child2, child3;

/*
Template Decode Base64
Source (Credit): https://fm4dd.com/programming/base64/base64_stringencode_c.shtm
Meliputi char b64, void decodeblock, dan void b64_decode
*/
char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void decodeblock(unsigned char in[], char *clrstr) {
  unsigned char out[4];
  out[0] = in[0] << 2 | in[1] >> 4;
  out[1] = in[1] << 4 | in[2] >> 2;
  out[2] = in[2] << 6 | in[3] >> 0;
  out[3] = '\0';
  strncat(clrstr, out, sizeof(out));
}

void b64_decode(char *b64src, char *clrdst) {
  int c, phase, i;
  unsigned char in[4];
  char *p;

  clrdst[0] = '\0';
  phase = 0; i=0;
  while(b64src[i]) {
    c = (int) b64src[i];
    if(c == '=') {
      decodeblock(in, clrdst); 
      break;
    }
    p = strchr(b64, c);
    if(p) {
      in[phase] = p - b64;
      phase = (phase + 1) % 4;
      if(phase == 0) {
        decodeblock(in, clrdst);
        in[0]=in[1]=in[2]=in[3]=0;
      }
    }
    i++;
  }
}
/*
Akhir dari Template Decode Base64
*/

void makeDir(){
  char *argv1[] = {"mkdir", "music", "-p", NULL};
  child1 = fork();
  if (child1==0) {
		execv("/usr/bin/mkdir", argv1);
	}

  char *argv2[] = {"mkdir", "quote", "-p", NULL};
  child2 = fork();
  if (child2==0) {
		execv("/usr/bin/mkdir", argv2);
	}

  char *argv3[] = {"mkdir", "hasil", "-p", NULL};
  child3 = fork();
  if (child3==0) {
		execv("/usr/bin/mkdir", argv3);
	}
}

void* unzipFiles(void *arg){
	char *argv1[] = {"unzip", "music.zip", "-d", "music", NULL};
	char *argv2[] = {"unzip", "quote.zip", "-d", "quote",NULL};
	unsigned long i=0;
  int status;
	pthread_t id=pthread_self();
	if(pthread_equal(id,thread_1[0])){
		child1 = fork();
		if (child1==0) {
		    execv("/usr/bin/unzip", argv1);
	    }
    else {
      while ((wait(&status)) > 0);
    }
	}
	else if(pthread_equal(id,thread_1[1])){
    child2 = fork();
		if (child2==0) {
		    execv("/usr/bin/unzip", argv2);
	    }
    else {
      while ((wait(&status)) > 0);
    }
	}
	return NULL;
}

void* decodeFile(void *arg){
  FILE *ptr, *musicDec, *quoteDec;
  char str[100];
  char temp[100];

  pthread_t id=pthread_self();
	if(pthread_equal(id,thread_1[0])){
    for(int i=1;i<10;i++){
      sprintf(temp,"music/m%d.txt", i);
      ptr = fopen(temp, "r");
      if (ptr == NULL){
        printf("Error opening file %s!",temp);            
        exit(0);
        }
          
      while (fgets(str, 100, ptr) != NULL) {
        char mydst[1024] = "";
        b64_decode(str, mydst);
        musicDec = fopen("music/music.txt", "a");
        fprintf(musicDec,"%s\n", mydst);
        }
        fclose(ptr);
        fclose(musicDec);
    }
  }
	else if(pthread_equal(id,thread_1[1])){
    for(int i=1;i<10;i++){
      sprintf(temp,"quote/q%d.txt", i);
      ptr = fopen(temp, "r");
      if (ptr == NULL){
        printf("Error opening file %s!",temp);     
        exit(0);
        }
          
      while (fgets(str, 100, ptr) != NULL) {
        char mydst[1024] = "";
        b64_decode(str, mydst);
        quoteDec = fopen("quote/quote.txt", "a");
        fprintf(quoteDec,"%s\n", mydst);
        }
        fclose(ptr);
        fclose(quoteDec);
    }
  }
	return NULL;
}

void *moveDecodedFile(char *argument){
  int status;
  char temp[30];
  sprintf(temp, "%s/%s.txt", argument, argument);
  char *argv[] = {"mv", temp, "hasil", NULL};
  child1 = fork();
		if (child1==0) {
		    execv("/usr/bin/mv", argv);
	    }
    else {
      while ((wait(&status)) > 0);
    }
}

void *zipFile(char *folder){
  int status;
  char temp1[30], temp2[30];
  sprintf(temp1, "%s", "mihinomenestnadine");
  sprintf(temp2, "%s.zip", folder);
  //Ganti directory ke hasil
  chdir("/home/nadnadine/Documents/soal-shift-sisop-modul-3-itb05-2022/soal1/hasil");
  char *argv1[] = {"zip", "--password", temp1, temp2, "music.txt", "quote.txt", NULL};
  child1 = fork();
		if (child1==0) {
		    execv("/usr/bin/zip", argv1);
	    }
    else {
      while ((wait(&status)) > 0);
    }
  char *argv2[] = {"mv", temp2, "../hasil.zip", NULL};
  child2 = fork();
		if (child2==0) {
        
		    execv("/usr/bin/mv", argv2);
	    }
    else {
      while ((wait(&status)) > 0);
    }
  //Ganti directory ke soal
  chdir("/home/nadnadine/Documents/soal-shift-sisop-modul-3-itb05-2022/soal1");
}

void *unzipFileHasil(char *zip){
  int status;
  char temp1[30],temp2[30];
  sprintf(temp1, "%s", "mihinomenestnadine");
  sprintf(temp2, "%s", zip);
  char *argv1[] = {"unzip", "-P", temp1, temp2, NULL};
  char *argv2[] = {"rm", temp2, NULL};
  child1 = fork();
		if (child1==0) {
		    execv("/usr/bin/unzip", argv1);
	    }
    else {
      while ((wait(&status)) > 0);
    }
  child2 = fork();
    if (child2==0) {
		    execv("/usr/bin/rm", argv2);
	    }
    else {
      while ((wait(&status)) > 0);
    }
}
void *writeNo(){
  FILE *no;
  no = fopen("no.txt", "a");
  fprintf (no,"%s","No");
  fclose(no);
}

void *updateNoAndZip(char *nama){
  int status;
  char temp1[30], temp2[30];
  //Update No.txt
  writeNo();
  ///Zip All
  sprintf(temp1, "%s", "mihinomenestnadine");
  sprintf(temp2, "%s.zip", nama);
  char *argv3[] = {"zip", "--password", temp1, temp2, "no.txt", "music.txt", "quote.txt", NULL};
  child1 = fork();
  if (child1==0) {
		    execv("/usr/bin/zip", argv3);
	    }
    else {
      while ((wait(&status)) > 0);
    }
}

void *deleteResidue(){
  int status;
  char *delete[] = {"rm", "music.txt", "quote.txt", "no.txt", NULL};
  child1 = fork();
  if (child1==0) {
		    execv("/usr/bin/rm", delete);
	    }
    else {
      while ((wait(&status)) > 0);
    }
}

int main() {
  //Membuat directory starter
  makeDir();

  //Inisialisasi Unzip Files
  int unzipping=0;
	int err;
  while (unzipping < 2){
    err=pthread_create(&(thread_1[unzipping]),NULL,&unzipFiles,NULL);
    if(err!=0) {
			printf("\n can't create thread : [%s]",strerror(err));
		}
		else {
			printf("\n create thread success\n");
		}
		unzipping++;
  }
	pthread_join(thread_1[0],NULL);
	pthread_join(thread_1[1],NULL);

  //Membuka dan Mengolah File
  int decoding=0;
  while (decoding < 2){
    err=pthread_create(&(thread_2[decoding]),NULL,&decodeFile,NULL);
    if(err!=0) {
			printf("\n can't create thread decode : [%s]",strerror(err));
		}
		else {
			printf("\n create thread decoding success\n");
		}
		decoding++;
  }
	pthread_join(thread_2[0],NULL);
	pthread_join(thread_2[1],NULL);

  //Memindah File Hasil Decode
  moveDecodedFile("music");
  moveDecodedFile("quote");

  //Membuat Zip dari Folder Decoded
  zipFile("hasil");

  //Mengunzip Decoded, karena lupa..
  //Makanya dicheck dulu :))
  unzipFileHasil("hasil.zip");

  //Menambahkan Tulisan "No" dan Menzip kembali
  updateNoAndZip("hasil");

  //Menghapus file residu
  deleteResidue();

  exit(0);
  return 0;
}
