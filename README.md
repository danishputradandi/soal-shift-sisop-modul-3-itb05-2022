# soal-shift-sisop-modul-3-ITB05-2022

Pengerjaan Soal Shift Modul 3 oleh ITB05

# Anggota

| Nama                           | NRP          | 
| -------------------------------| -------------| 
| Nadine Haninta                 | `5027201014` | 
| Danish Putra Dandi             | `5027201048` | 
| Mutiara Nuraisyah Dinda R.     | `5027201054` |

# Konten
* [Sumber Soal Praktikum Sistem Operasi Modul 3 Tahun 2022](#sumber-soal-praktikum-sistem-operasi-modul-3-tahun-2022)
* [Soal 1](#soal-1)
    * [Soal 1.a](#soal-1a)
    * [Soal 1.b](#soal-1b)
    * [Soal 1.c](#soal-1c)
    * [Soal 1.d](#soal-1d)
    * [Soal 1.e](#soal-1e)
	* [Kendala](#kendala)
    * [Dokumentasi](#dokumentasi)
* [Soal 2](#soal-2)
    * [Soal 2.a](#soal-2a)
    * [Soal 2.b](#soal-2b)
    * [Soal 2.c](#soal-2c)
    * [Soal 2.d](#soal-2d)
    * [Soal 2.e](#soal-2e)
    * [Soal 2.f](#soal-2f)
    * [Soal 2.g](#soal-2g)
    * [Kendala](#kendala)
    * [Dokumentasi](#dokumentasi)
* [Soal 3](#soal-3)
    * [Soal 3.a](#soal-3a)
    * [Soal 3.b](#soal-3b)
    * [Soal 3.c](#soal-3c)
    * [Soal 3.d](#soal-3d)
    * [Soal 3.e](#soal-3e)
    * [Kendala](#kendala)
    * [Dokumentasi](#dokumentasi)


## Sumber Soal Praktikum Sistem Operasi Modul 3 Tahun 2022

[Sumber Soal](https://docs.google.com/document/d/1w3tYGgqIkHRFoqKJ9nKzwGLCbt30HM7S/edit)

## Soal 1
Novak adalah seorang mahasiswa biasa yang terlalu sering berselancar di internet. Pada suatu saat, Ia menemukan sebuah informasi bahwa ada suatu situs yang tidak memiliki pengguna. Ia mendownload berbagai informasi yang ada dari situs tersebut dan menemukan sebuah file dengan tulisan yang tidak jelas. Setelah diperhatikan lagi, kode tersebut berformat base64. Ia lalu meminta kepada anda untuk membuat program untuk memecahkan kode-kode di dalam file yang Ia simpan di drive dengan cara decoding dengan base 64. Agar lebih cepat, Ia sarankan untuk menggunakan thread.

### Soal 1a
Download dua file zip dan unzip file zip tersebut di dua folder yang berbeda dengan nama quote untuk file zip quote.zip dan music untuk file zip music.zip. Unzip ini dilakukan dengan bersamaan menggunakan thread.

### Penyelesaian
Menggunakan fungsi `unzipFiles` yang akan melakukan proses dan melibatkan thread. Hal ini disyaratkan bahwa thread sudah dideklarasikan di awal dan folder `music` dan `quote` (di fungsi `makeDir()`).
```c
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
```
yang mana fungsi ini dipanggil dalam fungsi `int main()` menggunakan format multithread sebagai berikut,
```c
int main() {
  ...
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
  ...
}
```
### Soal 1b
Decode semua file .txt yang ada dengan base 64 dan masukkan hasilnya dalam satu file .txt yang baru untuk masing-masing folder (Hasilnya nanti ada dua file .txt) pada saat yang sama dengan menggunakan thread dan dengan nama quote.txt dan music.txt. Masing-masing kalimat dipisahkan dengan newline/enter.

### Penyelesaian
Menggunakan template decode dengan basis 64 dari [Sumber Decode B64](https://fm4dd.com/programming/base64/base64_stringencode_c.shtm) dengan penambahan fungsi untuk mendecode dokumen satu-persatu menggunakan loop yang berasal dari dokumen hasil unzip di folder `music` dan `quote`.

```c
void* decodeFile(void *arg){
  FILE *ptr, *musicDec, *quoteDec;
  char str[100];
  char temp[100];

  pthread_t id=pthread_self();
	if(pthread_equal(id,thread_1[0])){
    //Melakukan loop
    for(int i=1;i<10;i++){
      //Membuka dokumen di folder music
      sprintf(temp,"music/m%d.txt", i);
      ptr = fopen(temp, "r");
      if (ptr == NULL){
        printf("Error opening file %s!",temp);            
        exit(0);
        }
          
      while (fgets(str, 100, ptr) != NULL) {
        char mydst[1024] = "";
        //b64 adalah template yang berasal dari sumber
        b64_decode(str, mydst);
        //Membuka (jika ada)atau membuat (jika belum ada) dokumen music.txt
        musicDec = fopen("music/music.txt", "a");
        //Mencetak hasil decode dengan newline
        fprintf(musicDec,"%s\n", mydst);
        }
        fclose(ptr);
        fclose(musicDec);
    }
  }
	else if(pthread_equal(id,thread_1[1])){
    //Melakukan loop
    for(int i=1;i<10;i++){
      //Membuka dokumen di folder quote
      sprintf(temp,"quote/q%d.txt", i);
      ptr = fopen(temp, "r");
      if (ptr == NULL){
        printf("Error opening file %s!",temp);     
        exit(0);
        }
      while (fgets(str, 100, ptr) != NULL) {
        char mydst[1024] = "";
        //b64 adalah template yang berasal dari sumber
        b64_decode(str, mydst);
        //Membuka (jika ada)atau membuat (jika belum ada) dokumen quote.text
        quoteDec = fopen("quote/quote.txt", "a");
        //Mencetak hasil decode dengan newline
        fprintf(quoteDec,"%s\n", mydst);
        }
        fclose(ptr);
        fclose(quoteDec);
    }
  }
	return NULL;
}
```
yang mana fungsi ini dipanggil dalam fungsi `int main()` menggunakan format multithread sebagai berikut,
```c
int main(){
  ...
  //Inisialisasi decoding
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
  ...
}
```
### Soal 1c
Pindahkan kedua file .txt yang berisi hasil decoding ke folder yang baru bernama hasil

### Penyelesaian
Menggunakan fungsi `moveDecodedFile` yang berisi program sebagai berikut dengan syarat bahwa folder `hasil` telah dibuat (di sini telah dibuat di fungsi `makeDir`),
```c
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
```
yang mana fungsi ini dipanggil dalam fungsi utama dengan,
```c
int main(){
  ...
  //Memindah File Hasil Decode
  moveDecodedFile("music");
  moveDecodedFile("quote");
  ...
}
```
### Soal 1d
Folder hasil di-zip menjadi file hasil.zip dengan password 'mihinomenest[Nama user]'. (contoh password : mihinomenestnovak)

### Penyelesaian
Menggunakan fungsi `zipFile` sebagai berikut,
```c
void *zipFile(char *folder){
  int status;
  char temp1[30], temp2[30];
  //Menyetel password -> mihinomenestnadine, karena menggunakan nama "nadine" dalam variabel temp1
  sprintf(temp1, "%s", "mihinomenestnadine");
  //Menyimpan nama file hasil zip di dalam variabel temp2
  sprintf(temp2, "%s.zip", folder);
  //Ganti directory ke hasil
  chdir("/home/nadnadine/Documents/soal-shift-sisop-modul-3-itb05-2022/soal1/hasil");
  //Menyimpan perintah yang akan digunakan untuk meng-zip file
  char *argv1[] = {"zip", "--password", temp1, temp2, "music.txt", "quote.txt", NULL};
  child1 = fork();
		if (child1==0) {
		    execv("/usr/bin/zip", argv1);
	    }
    else {
      while ((wait(&status)) > 0);
    }
  //Melakukan pemindahan file hasil zip ke dari folder "hasil" ke folder "soal1"
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
```
yang mana dalam fungsi utama dipanggil dengan, 
```c
int main(){
  ...
  zipFile("hasil");
  ...
}
```
### Soal 1e
Karena ada yang kurang, kalian diminta untuk unzip file hasil.zip dan buat file no.txt dengan tulisan 'No' pada saat yang bersamaan, lalu zip kedua file hasil dan file no.txt menjadi hasil.zip, dengan password yang sama seperti sebelumnya.

### Penyelesaian
Menggunakan fungsi `unzipFileHasil` untuk melakukan proses _unzip_ dokumen "hasil.zip" dan `updateNoAndZip` untuk menambahkan dokumen "no.txt" dan melakukan proses _zip_ kembali.<br>
Adapun untuk fungsi `unzipFileHasil`,
```c
void *unzipFileHasil(char *zip){
  int status;
  char temp1[30],temp2[30];
  sprintf(temp1, "%s", "mihinomenestnadine");
  sprintf(temp2, "%s", zip);
  //Menyimpan perintah untuk melakukan unzip
  char *argv1[] = {"unzip", "-P", temp1, temp2, NULL};
  //Menyimpan perintah untuk menghapus dokumen "hasil.zip" yang lama
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
```
dan fungsi `updateNoAndZip` (termasuk fungsi `writeNo`),
```c
void *updateNoAndZip(char *nama){
  int status;
  char temp1[30], temp2[30];
  //Update No.txt
  writeNo();
  ///Zip All
  //Menyetel password -> mihinomenestnadine, karena menggunakan nama "nadine" dalam variabel temp1
  sprintf(temp1, "%s", "mihinomenestnadine");
  //Menyimpan nama file hasil zip di dalam variabel temp2
  sprintf(temp2, "%s.zip", nama);
  //Menyimpan perintah untuk melakukan zip
  char *argv3[] = {"zip", "--password", temp1, temp2, "no.txt", "music.txt", "quote.txt", NULL};
  child1 = fork();
  if (child1==0) {
		    execv("/usr/bin/zip", argv3);
	    }
    else {
      while ((wait(&status)) > 0);
    }
}

void *writeNo(){
  FILE *no;
  //Membuat dokumen baru bernama "no.txt", karena sebelumnya tidak ada
  no = fopen("no.txt", "a");
  //Menulis "No" di dalam "no.txt"
  fprintf (no,"%s","No");
  fclose(no);
}
```
Adapun pemanggilan dalam fungsi utama,
```c
int main(){
  ...
  //Mengunzip Decoded, karena lupa..
  unzipFileHasil("hasil.zip");

  //Menambahkan Tulisan "No" dan Menzip kembali
  updateNoAndZip("hasil");
  ...
}
```

### Soal 1(Tambahan, tidak ada di soal)
Adapun untuk penataan dokumen yang lebih rapi, digunakan fungsi `deleteResidue` untuk menghapus dokumen `music.txt`, `quote.txt`, dan `no.txt` yang ada di folder `soal1` setelah di-compile dalam program sebagai berikut,
```c
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
```
Adapun pemanggilan dalam fungsi utama,
```c
int main(){
  ...
  //Menghapus file residu
  deleteResidue();
  ...
}
```
### Kendala
Tidak ada

### Dokumentasi
- Hasil susunan folder soal1<br>
![Folder Soal1](/Screenshots/soal1-folder.jpg)

- Hasil decode music.txt<br>
![music.txt](/Screenshots/soal1-music.jpg)

- Hasil decode quote.txt<br>
![quote.txt](/Screenshots/soal1-quote.jpg)

- Isi dari dokumen Hasil.zip<br>
![Hasil.zip](/Screenshots/soal1-hasil-zip.jpg)


## Soal 2
Bluemary adalah seorang Top Global 1 di salah satu platform online judge. Suatu hari Ia ingin membuat online judge nya sendiri, namun dikarenakan Ia sibuk untuk mempertahankan top global nya, maka Ia meminta kamu untuk membantunya dalam membuat online judge sederhana. Online judge sederhana akan dibuat dengan sistem client-server dengan beberapa kriteria sebagai berikut:

### Soal 2a
Pada saat client terhubung ke server, terdapat dua pilihan pertama yaitu register dan login. Jika memilih register, client akan diminta input id dan passwordnya untuk dikirimkan ke server. Data input akan disimpan ke file users.txt dengan format username:password. 

Jika client memilih login, server juga akan meminta client untuk input id dan passwordnya lalu server akan mencari data di users.txt yang sesuai dengan input client. Jika data yang sesuai ditemukan, maka client dapat login dan dapat menggunakan command-command yang ada pada sistem. Jika tidak maka server akan menolak login client. 

Username dan password memiliki kriteria sebagai berikut:
* Username unique (tidak boleh ada user yang memiliki username yang sama)
* Password minimal terdiri dari 6 huruf, terdapat angka, terdapat huruf besar dan kecil.

Format users.txt:

```
username:password
username2:password2
```

### Penyelesaian
* Server
```c
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <math.h>
#define PORT 8080
#define SO_REUSEPORT 15

int n;

struct user {
    char username[50];
    char password[50];
};

void regis(char *username, char *password) {
    FILE *fptr;
    fptr = fopen("users.txt", "a");
    fprintf(fptr, "%s:%s\n", username, password);
    fclose(fptr);
}

int search_user(char *username, char *password) {
    int n = 0, mode = 0;
    struct user users[50];
    int i = 0;
    do {
        memset(users[i].username, 0, sizeof(users[i].username));
        memset(users[i].password, 0, sizeof(users[i].password));
        i++;
    } while (i < 50);
    //Buka file dan baca file users.txt
    FILE *fptr;
    fptr = fopen("users.txt", "r");

    fseek(fptr, 0, SEEK_END);
    long fsize = ftell(fptr);
    rewind(fptr);

    char *str = (char *)malloc(sizeof(char) * (fsize + 1));
    fread(str, 1, fsize, fptr);

    for (int i = 0; i <= fsize; ++i) {
        if (str[i] == ':') {
            mode ^= 1;
            continue;
        }
        if (str[i] == '\n' || str[i] == ' ') {
            mode ^= 1;
            ++n;
            continue;
        }
        if (mode == 0) {
            sprintf(users[n].username, "%s%c", users[n].username, str[i]);
        } else {
            sprintf(users[n].password, "%s%c", users[n].password, str[i]);
        }
    }
    fclose(fptr);

    for (int i = 0; i < n; ++i) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    // Message untuk client
    char *login_success = "Login Success";
    char *login_failed = "Login Failed";
    char *register_success = "Register success";
    char *register_failed = "Sorry register failed, please try again";
    char *file_pattern_not_found = "File pattern doesn't match";
    char *less_than_8_char = "Password less than 8 character, register failed, please try again";
    char *login_or_register = "Selected: ";
    char *username = "username: ";
    char *password = "password: ";
    // Message dari client
    char buff[1024] = {0};
    char user_name[1024];
    char user_password[1024];
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("Accept");
            exit(EXIT_FAILURE);
        }
        while (1) {
            send(new_socket, login_or_register, strlen(login_or_register), 0);
            read(new_socket, buff, 1024);
            printf("user %s\n", buff);

            memset(user_name, 0, sizeof(user_name));
            memset(user_password, 0, sizeof(user_password));

            if (strcmp(buff, "register") == 0) {
                // username
                send(new_socket, username, strlen(username), 0);
                read(new_socket, user_name, 1024);
                printf("username: %s\n", user_name);
                
                // password
                send(new_socket, password, strlen(password), 0);
                read(new_socket, user_password, 1024);

                if(strlen(user_password) < 8){
                    send(new_socket, less_than_8_char, strlen(less_than_8_char), 0);
                }else{
                printf("password: %s\n", user_password);
                regis(user_name, user_password);
                send(new_socket, register_success, strlen(register_success), 0);
                }
            } else if (strcmp(buff, "login") == 0) {
                // username
                send(new_socket, username, strlen(username), 0);
                read(new_socket, user_name, 1024);
                printf("username: %s\n", user_name);
                
                // password
                send(new_socket, password, strlen(password), 0);
                read(new_socket, user_password, 1024);
                printf("password: %s\n", user_password);
                
                if (search_user(user_name, user_password)) {
                    send(new_socket, login_success, strlen(login_success), 0);
                    printf("%s\n", login_success);
                    break;
                } else {
                    send(new_socket, login_failed, strlen(login_failed), 0);
                    printf("%s\n", login_failed);
                }
            }
            memset(buff, 0, sizeof(buff));
            sleep(2);
        }
        printf("\n");
        close(new_socket);
    }
    return 0;
}
```
### Penjelasan
Pada client server perlu dilakukan penyambungan port dan juga server. Lalu dibuat beberapa function yakin `regis` yang digunakan untuk meregistrasi data user, diantaranya username dan password yang akan dicatat pada `users.txt` lalu berikutnya yakni ada ada function `search_user` yang digunakan untuk login, yang berfungsi untuk mencari username yang ada pada file users.txt.

* Client
```c
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>
#define PORT 8080

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0;
    struct sockaddr_in serv_addr;
    // Message for Server
    char answer[1024];
    // Message from Server
    char buff[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  // Saat gagal menyambung ke Server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    // Login dan Register
    do {
        memset(buff, 0, sizeof(buff));
        read(sock, buff, sizeof(buff));
        printf("Please select register or login: ");
        scanf("%s", answer);
        send(sock, answer, strlen(answer), 0);
        memset(answer, 0, sizeof(answer));

        // Username
        memset(buff, 0, sizeof(buff));
        read(sock, buff, sizeof(buff));
        printf("username: ");
        scanf("%s", answer);
        send(sock, answer, strlen(answer), 0);
        memset(answer, 0, sizeof(answer));
        // Password
        memset(buff, 0, sizeof(buff));
        read(sock, buff, sizeof(buff));
        printf("password: ");
        scanf("%s", answer);
        send(sock, answer, strlen(answer), 0);
        memset(answer, 0, sizeof(answer));

        // Gagal atau tidak
        read(sock, buff, 1024);
        printf("%s\n", buff);
    } while (strcmp(buff, "login success") != 0);

    printf("\n");
    return 0;
}
```
### Penjelasan
Sama halnya pada server, pada client server yang dilakukan terlebih dahulu adalah menyambungkan port dan server, jika sudah maka nanti server akan mengirim message. Message yang dikirim adalah berupa pilihan user dan feedbacknya. Nah, pada client server ini akan mengambil jawaban dari user dan menyimpannya pada variabel `answer` dan akan mengirimkan nya ke server dengan bantuan fungsi `memset`.

### Kendala
Kami mengalami kendala untuk menyelesaikan soal no 2 dan hanya bisa menyelesaikan satu poin yakni 2a. 

### Dokumentasi
- Menyambungkan server dan client, compile client terlebih dahulu baru client
![Connect Client n Server](/Screenshots/connect_client_server.png)
- Jika memilih register lalu memasukkan password kurang dari 6 karakter
![Password Kurang dari 6 Karakter](/Screenshots/password_less_than_6.png)
- Register sukses jika memenuhi syarat password
![Register Sukses](/Screenshots/regis_success.png)
- Login sukses jika memasukkan username dan password yang telah diregistrasi
![Login Sukses](/Screenshots/login_success.png)
- Isi file users.txt dengan catatan username dan password user yang berhasil registrasi
![File users.txt](/Screenshots/userstxt.png)





# Soal 3

Nami adalah seorang pengoleksi harta karun handal. Karena Nami memiliki waktu luang, Nami pun mencoba merapikan harta karun yang dimilikinya berdasarkan jenis/tipe/kategori/ekstensi harta karunnya. Setelah harta karunnya berhasil dikategorikan, Nami pun mengirimkan harta karun tersebut ke kampung halamannya.
Contoh jika program pengkategorian dijalankan

``` Program soal3 terletak di /home/[user]/shift3/hartakarun```

```
$ ./soal3 
# Hasilnya adalah sebagai berikut
/home/[user]/shift3/hartakarun
|-jpg
|--file1.jpg
|--file2.jpg
|--file3.jpg
|-c
|--file1.c
|-tar.gz
|--file1.tar.gz
```



a. Hal pertama yang perlu dilakukan oleh Nami adalah mengextract zip yang diberikan ke dalam folder ```  /home/[user]/shift3/ ```. Kemudian working directory program akan berada pada folder “/home/[user]/shift3/hartakarun/”. Karena Nami tidak ingin ada file yang tertinggal, program harus mengkategorikan seluruh file pada working directory secara rekursif

b. Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”.

c. Agar proses kategori bisa berjalan lebih cepat, setiap 1 file yang dikategorikan dioperasikan oleh 1 thread.

d. Untuk mengirimkan file ke Cocoyasi Village, nami menggunakan program client-server. Saat program client dijalankan, maka folder /home/[user]/shift3/hartakarun/” akan di-zip terlebih dahulu dengan nama “hartakarun.zip” ke working directory dari program client.

e. Client dapat mengirimkan file “hartakarun.zip” ke server dengan mengirimkan command berikut ke server
send hartakarun.zip


Karena Nami tidak bisa membuat programnya, maka Nami meminta bantuanmu untuk membuat programnya. Bantulah Nami agar programnya dapat berjalan!

## Catatan
Catatan:
- Kategori folder tidak dibuat secara manual, harus melalui program C

- Program ini tidak case sensitive. Contoh: JPG dan jpg adalah sama

- Jika ekstensi lebih dari satu (contoh “.tar.gz”) maka akan masuk ke folder dengan titik terdepan (contoh “tar.gz”)

- Dilarang juga menggunakan fork, exec dan system(), kecuali untuk bagian zip pada soal d

# Program no 3
```c
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

```

# Program client
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT    5500
#define IP      "127.0.0.1"
#define MAXBUF  1024

int main() {
    struct sockaddr_in serv_addr;
    int     s;
    int         sourse_fd;
    char        buf[MAXBUF];
    int         file_name_len, read_len;
    /* socket() */
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s == -1) {
        return 1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_port = htons(PORT);

    if(connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connect : ");
        printf("fail to connect.\n");
        close(s);
        return 1;
    }

    memset(buf, 0x00, MAXBUF);
    printf("write file name to send to the server:  ");
    scanf("%s", buf);

    printf(" > %s\n", buf);
    file_name_len = strlen(buf);

    send(s, buf, file_name_len, 0);
    sourse_fd = open(buf, O_RDONLY);
    if(!sourse_fd) {
        perror("Error : ");
        return 1;
    }

    while(1) {
        memset(buf, 0x00, MAXBUF);
        read_len = read(sourse_fd, buf, MAXBUF);
        send(s, buf, read_len, 0);
        if(read_len == 0) {
            break;
        }

    }

    return 0;
}
```

# Program Server
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // read, write
#include <arpa/inet.h>
#include <sys/types.h>  // socket, bind, accept, open
#include <sys/socket.h> // socket, bind, listen, accept
#include <sys/stat.h>   // open
#include <fcntl.h>      // open
#include <errno.h>

#define PORT    5500
#define MAXBUF  1024

int main() {
    int server_sockfd;
    int client_sockfd;
    int des_fd; // file num
    struct sockaddr_in serveraddr, clientaddr;
    int client_len, read_len, file_read_len;    // length
    char buf[MAXBUF];

    int check_bind;
    client_len = sizeof(clientaddr);

    /* socket() */
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sockfd == -1) {
        perror("socket error : ");
        exit(0);
    }

    /* bind() */
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family       = AF_INET;
    serveraddr.sin_addr.s_addr  = htonl(INADDR_ANY);
    serveraddr.sin_port         = htons(PORT);

    if(bind(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) > 0) {
        perror("bind error : ");
        exit(0);
    }

    /* listen */
    if(listen(server_sockfd, 5) != 0) {
        perror("listen error : ");
    }

    while(1) {
        char file_name[MAXBUF]; // local val
        memset(buf, 0x00, MAXBUF);

        /* accept() */
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, &client_len);
        printf("New Client Connect : %s\n", inet_ntoa(clientaddr.sin_addr));

        /* file name */
        read_len = read(client_sockfd, buf, MAXBUF);
        if(read_len > 0) {
            strcpy(file_name, buf);
            printf("%s > %s\n", inet_ntoa(clientaddr.sin_addr), file_name);
        } else {
            close(client_sockfd);
            break;
        }

        /* create file */

        des_fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL, 0700);
        if(!des_fd) {
            perror("file open error : ");
            break;
        }   
        /* file save */
        while(1) {
            memset(buf, 0x00, MAXBUF);
            file_read_len = read(client_sockfd, buf, MAXBUF);
            write(des_fd, buf, file_read_len);
            if(file_read_len == EOF | file_read_len == 0) {
                printf("finish file\n");
                break;
            }


        }


        close(client_sockfd);
        close(des_fd);
    }
    close(server_sockfd);
    return 0;
}
```

# Penyelesaian
### Gunakan Library berikut 

```c
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
```

## Mendeklarasikan folder yang diperlukan
```c
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
```
- Pada bagian ini, kita diminta untuk membuat sebuah folder yang sesuai dengan file zip pada `hartakarun.zip`, di dalam folder tersebut akan terdapat banyak file dan kita harus memasukkannya kedalam folder sesuai dengan nama file. Pada bagian fungsi `formatfile(void)`, akan dilakukan iterasi pembuatan directory baru menggunakan array dengan looping `do while` sampai lebih kecil dari 14

- perintah `S_IRWXU` digunakan untuk membaca, menulis, dan mengeksekusi izin yang ada pada looping tersebut

## Soal 3a, 3b Gunakan 2 fungsi utama untuk mengecek tipe file / directory serta berguna untuk mengkategorikan file

- Gunakan fungsi `is regular` untuk mengecek apakah tipe file tersebut reguler atau tidak
```c
int isRegular(const char *path){
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}

```
- Untuk fungsi ini menggunakan struct stat yang berisi beberapa elemen. Untuk mengecek apakah file merupakan tipe reguler (bukan merupakan folder), menggunakan salah satu elemen struct yaitu st_mode.

## Gunakanlah fungsi File Rekursif 
- Fungsi ini digunakan pada soal nomor 3a untuk mengkategorikan seluruh file pada directory secara rekursif
```c
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
```
-  Ada beberapa modifikasi yaitu untuk memasukkan setiap path ke dalam thread masing-masing, maka setiap path file akan disimpan ke dalam suatu array untuk digunakan lagi dalam fungsi `pindah file`

```c
if(isRegular(path)){
          strcpy(gandafile[*iter], path);
          *iter = *iter + 1;
        }
```
- Syntax ini berguna Apabila path/direktori masih berupa folder, akan terus dilakukan rekursif hingga menemukan list path yang berupa file reguler.

## Gunakan string strchr
- untuk mendapatkan ekstensi yang akan digunakan sebagai nama folder, gunakanlah fungsi `strchr` yang mana fungsi ini akan mengembalikan nilai  `.` pertama yang ditemukan akan langsung ditiadakan.

- String `strchr()` digunakan untuk mencari kemunculan pertama dari sebuah string di dalam string lain. Fungsi ini adalah alias dari fungsi strstr()
```c
strcpy(temporary4, path);
  
  char *fileExt3, dot1 = '.';

  fileExt3 = strchr(temporary4, dot1);

  ```

  ## Gunakan string strtok
  - Yang ketiga yaitu mendapatkan nama-nama file dengan melakukan delimiter "/". Untuk melakukan ini menggunakan strtok, dan looping untuk memotong string dan menghilangkan delimiter.

  - String `strtok`\adalah salah satu perintah dalam library string.h yang berfungsi untuk memisahkan string dari karakter (huruf) tertentu.
  ```c
  strcpy(temporary, path);

  char *token=strtok(temporary, "/");
  do{
       sprintf(namafile, "%s", token);
      token = strtok(NULL, "/");
  }
  ```

  ## Hitung berapa kali kemunculan .

  - Hitung berapa banyak `string` yang telah displit & menghitung berapa banyak kemunculan `.` pada nama file , dan untuk mencari nama file sendiri tanpa adanya ekstensi. Untuk melakukan ini menggunakan strtok, dan looping untuk memotong string dan menghilangkan delimiter. Adapun menggunakan `count` untuk menghitung kemunculan `.` pada path/string.

  ```c
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
    ```

## Cek pengkategorian file

```c
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
      ```

- if(dot == first)  untuk mengkategorikan file-file hidden agar bisa masuk ke folder `hidden`. Apabila terdapat `.` pada index pertama (yang mengindikasi file-, fileExt (yang akan menyimpan nama folder) akan menyimpan `hidden` sehingga akan dibuat folder `hidden` untuk file-file berjenis hidden.

-  else if(total >= 3) apabila kemunculan `.` lebih dari 1 `(total lebih dari sama dengan 3 dikarenakan yang dihitung banyak format bukan `.` ` . Apabila ekstensi lebih dari 1, akan masuk ke folder dengan titik terdepan. 

- else if (total <=1 ) apabila tidak ada ekstensi (count kurang dari 1), sehingga fileExt akan menyimpan "unknown" sehingga akan dibuat folder "unknown" untuk file-file tanpa ekstensi.

## Mengubah Ekstensi menjadi case yang tidak sensitif

```c
int i = 0;
  do
  {
    fileExt[i] = tolower(fileExt[i]);
    i++;
  }
  while(i < sizeof(fileExt));
  ```

  ## Membuat folder dengan syntax mkdir
  - Pada bagian ini berguna untuk melakukan pemindahan file dari `path` lama ke `path` yang baru.
  ```c
    strcpy(temporary, (char*) arg);
  mkdir(fileExt, 0777);
  strcat(cwd, "/"), strcat(cwd,fileExt),  strcat(cwd, "/"), strcat(cwd, namafile), strcpy(tempDest, cwd), rename(temporary, tempDest);
  return (void *) 1;
}
```

# Kendala
- Terdapat beberapa file yang tidak masuk sesuai dengan format folder

## Dokumentasi
- Memindahkan file ke folder secara rekursif 
![Rekursif File ](/Screenshots/filekefolder.PNG)

- Mengirim zip dari client
![Kirim zip dari client ](/Screenshots/kirimdariclient.PNG)

- Mengirim zip ke server
![Kirim ke server ](/Screenshots/kirimkeserver.PNG)

