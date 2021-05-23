# Sistem Operasi D-06 (2021)
Praktikum Sistem Operasi Modul 3 - Thread dan IPC
Kelompok : 
1. Ramadhan Arif - 05111940000162
2. Zulfayanti Sofia Solichin - 05111940000189
3. Rayhan Daffa Alhafish - 05111940000227

## Penjelasan dan Penyelesaian soal no.1
Pada soal 1 ini kita diminta untuk membuat program client dan server untuk membuat database. Dimana pada halaman menu utama database terdapat menu `register` dan `login`, dan jika pengguna sudah melakuakan login terdapat menu `add` untuk menambah database, `download` untuk mendownload file yang terdapat pada database, `delete` untuk menghapus file yang terdapat pada database, `see` untuk melihat isi database, dan `find` untuk mencari file yang terdapa pada database.
- **Penjelasan dan Penyelesaian Soal 1a**<br>
  Pada soal 1a ini kita diminta untuk membuat opsi login dan register yang berisi inputan id dan password. Data id dan password ini disimpan pada file `akun.txt` dengan format `id:password`. Pertama dibuat fungsi untuk register pada aplikasi server.
  ```
    void daftar(char str[])
  {
      printf("DAFTAR USER\n");
      char idpwd [100];
      strcpy(idpwd,str);
      printf("%s\n",idpwd);
      FILE* file = fopen("akun.txt", "a") ;
      fputs(idpwd,file);
      fclose(file);
  }
  ```
  Fungsi ini dipanggil oleh fungsi pada aplikasi client sebagai berikut
  ```
    void Reg() {
      char id[50];
      char pwd[50];
      resR();
      scanf("%s",id);
      scanf("%s", pwd);

      sprintf(sent,"%s:%s\n",id,pwd);
      sends(sent);
      resR();
    }
  ```
  Pada fungsi di atas, program dimulai dengan menyimpan id dan password pengguna pada aplikasi client, karena untuk disimpan ke dalam file harus dalam bentuk `id:password` maka data disimpan dengan bentuk yang diubah untuk disesuaikan. Lalu data yang sudah di buat dikirim ke aplikasi server untuk nantinya di simpan pada file `akun.txt`.
  Pada fungsi `Reg()` terdapat fungsi `sends()` dan `resR()` untuk mempercepat saat pembuatan fungsi untuk mengirim dan menerima command yang akan digunakan.
  Lalu dibuat fungsi login pada aplikasi register
  ```
    bool LogUser(char str[]) {
      printf("User Login\n");
      char idpwd [100];
      strcpy(idpwd,str);
      printf("%s\n",idpwd);
      char *id;
      char tok[2]=":";
      char find[100];
      FILE* file = fopen("akun.txt","r");
      while (fgets(find,100,file)) {
          // printf("%s%s\n",find,idpwd);
          if (strcmp(find,idpwd)==0) {
              strcpy(upass,idpwd);
              fclose(file);
              id = strtok(idpwd,tok);
              strcpy(user,id);
              loggedIn = true;
              return true;
          }
      }
      return false;
    }
    ```
  Fungsi ini dipanggil oleh fungsi pada aplikasi client sebagai berikut
  ```
  void Log() {
      resR();
      char id[50];
      char pwd[50];
      scanf("%s",id);
      scanf("%s", pwd);
      sprintf(sent,"%s:%s\n",id,pwd);
      sends(sent);
      read(sock,recieve,1024);
      printf("%s\n",recieve);
      if(recieve[0]=='L'){
          loggedIn=true;
      }
      memset(recieve,0,sizeof(recieve));
  }
  ```
  Pada fungsi `Log()` di aplikasi client ini, pengguna diminta memasukkan id dan password yang sudah didaftarkan, lalu data yang dimasukkan dibuat menjadi seperti data di file `akun.txt` setelah diubah bentuknya string ini di kirim dengan menggunakan `sends()` ke aplikasi server untuk dibandingkan dengan data string akun yang sudah ada, jika terdapat string yang sama dimana artinya id dan password yang dimasukkan sudah benarvmaka pengguna akan masuk ke dalam server.<br>
  Selain itu soal ini juga meminta untuk membuat sistem yang dapat menerima multi-connections. Koneksi terhitung ketika aplikasi client tersambung dengan server. Jika terdapat 2 koneksi atau lebih maka harus menunggu sampai client pertama keluar untuk bisa melakukan login dan mengakses aplikasinya. Oleh karena itu dibuat fungsi main pada aplikasi server sebagai berikut.
  ```
    int main(int argc, char const *argv[]) {  
      struct sockaddr_in address;
      int opt = 1;
      int addrlen = sizeof(address);

      DIR* dir = opendir("FILES");
      if (dir) {
          closedir(dir);
      } else if (ENOENT == errno) {
          mkdir("FILES", 0777);
      }

      if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
          perror("socket failed");
          exit(EXIT_FAILURE);
      }

      if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt)) <0 ) {
          perror("setsockopt");
          exit(EXIT_FAILURE);
      }

      address.sin_family = AF_INET;
      address.sin_addr.s_addr = INADDR_ANY;
      address.sin_port = htons( PORT );

      if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
          perror("bind failed");
          exit(EXIT_FAILURE);
      }

      if (listen(server_fd, 3) < 0) {
          perror("listen");
          exit(EXIT_FAILURE);
      }
      while(1) {
          //clear the socket set 
          FD_ZERO(&readfds);  

          //add master socket to set 
          FD_SET(server_fd, &readfds);  
          max_sd = server_fd;  

          //add child sockets to set 
          for ( i = 0 ; i < max_clients ; i++)  
          {  
              //socket descriptor 
              sd = client_socket[i];  

              //if valid socket descriptor then add to read list 
              if(sd > 0)  
                  FD_SET( sd , &readfds);  

              //highest file descriptor number, need it for the select function 
              if(sd > max_sd)  
                  max_sd = sd;  
          } 
           //wait for an activity on one of the sockets , timeout is NULL , 
          //so wait indefinitely 
          activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  

          if ((activity < 0) && (errno!=EINTR))  
          {  
              printf("select error");  
          }  

          //If something happened on the master socket , 
          //then its an incoming connection 
          if (FD_ISSET(server_fd, &readfds))  
          {  
              if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
              {  
                  perror("accept");  
                  exit(EXIT_FAILURE);  
              }
               for (i = 0; i < max_clients; i++)  
              {  
                  //if position is empty 
                  if( client_socket[i] == 0 )  
                  {  
                      client_socket[i] = new_socket;  
                      printf("Adding to list of sockets as %d\n" , i);  

                      break;  
                  }  
              }  
          }
          sd = client_socket[0];
          connected = true;
          printf("CONNECTED\n");
          ...
      return 0;
  }
   ```
  
  **Output**
   
- **Penjelasan dan Penyelesaian Soal 1b**<br>
  Pada soal 1b ini meminta kita untuk membuat folder `FILES` secara otomatis jika server mulai dijalankan.
  ```
    DIR* dir = opendir("FILES");
      if (dir) {
          closedir(dir);
      } else if (ENOENT == errno) {
          mkdir("FILES", 0777);
      }
   ```
   Selain itu, diminta juga untuk membuat file `files.tsv` yang berisi path file saat berada di server, publisher, dan tahun publikasi. Setiap penambahan dan penghapusan file pada folder file yang bernama  `FILES` pada server akan memengaruhi isi dari file ini. Oleh karena itu file ini akan selalu dipanggil pada saat menjalankan fungsi menambahkan dan mengurangi file.
- **Penjelasan dan Penyelesaian Soal 1c**<br>
  Pada soal 1c kita diminta untuk membuat fitur untuk penambahan file pada server dengan memberikan command `add`.<br>
  Fungsi yang digunakan pada aplikasi server
  ```
      void writefile(char dir[]) {
      FILE *file = fopen(dir,"w");
      char buffer[1024]={0};

      memset(buffer,0,sizeof(buffer));
      int len = read(sd,buffer,1024);
      fprintf(file,"%s",buffer);

      printf("break\n");
      fclose(file);
  }

    void Add() { 
      char publisher[1024] = {0};
      char tahun[1024] = {0};
      char path[1024] = {0};
      sends("Publisher:");
      bRead();
      strcpy(publisher,recieve);
      sends("Tahun Publish:");
      bRead();
      strcpy(tahun,recieve);
      sends("Filepath:");
      bRead();
      strcpy(path,recieve);

      char *ptr1;
      char slash ='/';
      ptr1 = strrchr( path, slash );
      ptr1++;
      char fname[100];
      strcpy(fname,ptr1);
      char ext[10];
      char *ptr2;
      char dot ='.';
      ptr2 = strrchr( path, dot );
      ptr2++;
      strcpy(ext,ptr2);
      char dir[300] = "/home/zulfa/Documents/modul3/shift3/FILES/";
      strcat(dir,fname);
      FILE* tsv = fopen("files.tsv","a");
      char info[5000];
      sprintf(info,"%s\t%s\t%s\t%s\t%s\n",fname,publisher,tahun,ext,path);
      fputs(info,tsv);
      fclose(tsv);
      writefile(dir); 
      sends("File berhasil ditambahkan\n");
      FILE* log = fopen("running.log","a");
      fprintf(log,"Tambah: %s %s",fname,upass);
      fclose(log);
   }
  ```
  Fungsi ini dipanggil dengan fungsi pada aplikasi client
  ```
    void Add()
    {
        char temp[1024];
        for (int i=0;i<3;i++) {
            resR();
            scanf("%s",temp);
            temp[strcspn(temp,"\n")] =0;
            sends(temp);
        }
        FILE *sfd = fopen(temp,"rb");  
        char data[1024] = {0};

        memset(data,0,1024);
        size_t size = fread(data,sizeof(char),1024,sfd);
        send(sock,data,1024,0);

        fclose(sfd);
        resR();
    }
  ```
  Pada tahap ini, aplikasi client memanggil beberapa string dari fungsi add pada aplikasi server, karena diminta untuk menginput sebanyak 3 kali maka fungsi pemanggilan string dan pengiriman string dilakukan looping sebanyak 3 kali. Karena dibutuhkan juga data nama file dan jenis ekstensi dari file yang ditambahakan untuk disimpan, maka string path yang telah di input dipotong dengan menggunakan `ptr1 = strrchr( path, slash )` dan untuk mengambil data nama file. Selain itu digunakan juga fungsi `ptr2 = strrchr( path, dot)` untuk mengambil data nama ekstensi file. Lalu dipanggil juga fungsi `void writefile()` untuk memasukkan data yang diinginkan ke dalam folder `FILES`.<br>

  **Output**
  
- **Penjelasan dan Penyelesaian Soal 1d**<br>
  Pada soal ini kita diminta untuk membuat fitur dimana client dapat mendownload file yang telah ada dalam folder `FILES` di server, sehingga sistem harus dapat mengirim file ke client dengan memberikan command `download`.<br>
  Fungsi yang digunakan pada aplikasi server
  ```
    void download () {
      sends("namafile.extension\n");
      bRead();
      bool flag;
      char find[200];
      recieve[strcspn(recieve,"\n")] =0;
      strcpy(find,recieve);
      FILE* file = fopen("files.tsv", "r");
      char lines[1024];
      char publisher[200], tahun[200],filepath[200],ext[20],filename[200];
      while (fgets(lines,1024,file)) {
          sscanf(lines,"%[^\t]\t%s\t%s\t%s\t%s",filename,publisher,tahun,ext,filepath);
          if (strcmp(find,filename)==0){
              flag = true;
              break;
          }
      }
      if (flag==true) {
          sends("File ditemukan\n");
          char temp[104]="/home/zulfa/Documents/modul3/shift3/FILES/";
          strcat(temp,find);
          FILE *sfd = fopen(temp,"rb");  
          char data[4096] = {0};

          memset(data,0,4096);
          size_t size = fread(data,sizeof(char),4096,sfd);
          send(sd,data,strlen(data),0);

          fclose(sfd);
      }
      else {
          sends("File tidak ditemukan\n");
      }
  ```
  Fungsi ini dipanggil dengan fungsi pada aplikasi client
  ```
    void download() {
      resR();
      char temp[1024];
      scanf("%s",temp);
      temp[strcspn(temp,"\n")] =0;
      sends(temp);
      read(sock,recieve,1024);
      printf("%s\n",recieve);
      if (recieve[5]=='d') {
          char dir[300] = "/home/zulfa/Documents/modul3/shift3/";
          strcat(dir,temp);
          FILE *file = fopen(dir,"w");
          char buffer[4096]={0};

              memset(buffer,0,sizeof(buffer));
              int len = read(sock,buffer,4096); 
              fprintf(file,"%s",buffer);
          fclose(file);
      }
  }
  ```
  Dimana pada client, pengguna diminta untuk menginput dengan format `namafile.extension` lalu dilakukan pencarian pada server dengan `strcmp(find,filename)` untuk membandingakn string yang dicari dengan string yang sudah ada pada database. Jika ditemukan yang sama, maka file akan di dikirim ke client.<br>
  
  **Output**
  
- **Penjelasan dan Penyelesaian Soal 1e**<br>
  Pada soal 1e kita diminta menambahkan fitur agar client juga dapat menghapus file yang tersimpan di server. Namun pada folder `FILE` file yang dihapus hanya akan diganti namanya menjadi `old-NamaFile.ekstensi`. Ketika file telah diubah namanya, maka row dari file tersebut di file.tsv akan terhapus. Fitur ini dipanggil dengan menggunakan command `delete` pada client. Fungsi yang digunakan oleh server.
  ```
    void delete() {
      // sends("namafile.extension\n");
      bRead();
      bool flag = false;
      char finds[1024]={0};
      strcpy(finds,recieve);
      printf("%s\n",finds);
      FILE* fileR = fopen("files.tsv","r");
      FILE* fileW = fopen("temp.tsv","w");
      char data[1024] = {0};
      char publisher[200], tahun[200],filepath[200],ext[20],filename[200];
      while(fgets(data,1024,fileR)!=NULL){
          sscanf(data,"%[^\t]\t%s\t%s\t%s\t%s",filename,publisher,tahun,ext,filepath);
          if (strcmp(filename,finds)!=0) {
          fprintf(fileW,"%s",data);
          } 
          if (strcmp(filename,finds)==0){
              flag = true;
          }
          bzero(data,1024);
      }
      fclose(fileW);
      fclose(fileR);
      if (flag == true) {
      remove("files.tsv");
      rename("temp.tsv","files.tsv");
      FILE* log = fopen("running.log","a");
      fprintf(log,"Hapus: %s %s",finds,upass);
      fclose(log);
      char oldFile[200]= {0};
      char renamed[200]={0};
      char temp[104]="/home/zulfa/Documents/modul3/shift3/FILES/";
      strcat(oldFile,temp);
      strcat(renamed,temp);
      strcat(oldFile,finds);
      strcat(renamed,"old-");
      strcat(renamed,finds);
  //    printf("d");
      rename(oldFile,renamed);
      sends("Delete sukses\n");
      }
      else {
          sends("File tidak ditemukan\n");
      }
  }

  ```
  Fungsi ini dipanggil dengan fungsi pada aplikasi client
  ```
    void delete() {
      // resR();
      char temp[1024];
      scanf("%s",temp);
      temp[strcspn(temp,"\n")]=0;
      sends(temp);
      resR();
  }
  ```
  Pada client diminta untuk menginput nama file yang ingin dihapus dengan format `namafile.ekstensi`. Lalu, setelah itu string dikirim ke server untuk nantinya dilakukan fungsi pencarian, pada pencarian string tersebut akan dibandingakan dengan data string yang terdapat pada file `files.tsv`. Pada proses ini dilakukan looping sebanyak data yang terdapat pada database. Jika string yang dibandingkan berbeda, maka data tersebut akan dipindahkan fe file baru yang diberi nama `temp.tsv`, jika string yang dibandingkan sama, maka data tidak akan di-copy ke file `temp.tsv`. Setelah proses looping selesai, server akan menghapus file `files.tsv` lalu file `temp.tsv` akan diubah namanya menjadi `files.tsv`.<br>
  Selain itu dilakukan juga perbandingan string dengan files yang terdapat pada folder`FILES`. Jika ditemukan file dengan nama string yang sama, maka file tersebut akan direname menjadi `old-NamaFile.ekstensi`.<br>
  
  **Output**
  
- **Penjelasan dan Penyelesaian Soal 1f**<br>
  Pada soal 1f kita diminta untuk membuat fitur see dimana client dapat melihat semua isi `files.tsv`. Fungsi ini dipanggil dengan memberikan command `see` pada client. Fungsi yang digunakan oleh server.
  ```
    void see() {
      FILE* fileR = fopen("files.tsv","r");
      char data[1024] = {0};
      char tosend[10000]={0};
      char publisher[200], tahun[200],filepath[200],ext[20],filename[200];
      while(fgets(data,1024,fileR)!=NULL){
          sscanf(data,"%[^\t]\t%s\t%s\t%s\t%s",filename,publisher,tahun,ext,filepath);
          char tosendbuff [1024];
          sprintf(tosendbuff,"Nama: %s\nPublisher: %s\nTahun Publish: %s\nExtensi File: %s\nFilepath: %s\n\n",filename,publisher,tahun,ext,filepath);
          strcat(tosend,tosendbuff);
          bzero(data,1024);
      }
      fclose(fileR);
      sends(tosend);
  }
  ```
  Fungsi ini dipanggil dengan fungsi pada aplikasi client.
  ```
    void see(){
      char bigbuff[10000];
      read (sock,bigbuff,10000);
      printf("%s\n",bigbuff);
      memset(bigbuff,0,sizeof(bigbuff));  
  }
  ```
  Setelah memasukkan command `see` server akan membuka file `files.tsv` lalu mengcopy semua string pada file tersebut dengan cara looping sebanyak banyaknya data dengan format.
  ```
    Nama:
    Publisher:
    Tahun publishing:
    Ekstensi File : 
    Filepath : 
  ```
  Lalu string ini dikirim satu persatu ke client untuk di print.<br>
  
  **Output**
  
- **Penjelasan dan Penyelesaian Soal 1g**<br>
Pada soal 1g kita diminta untuk membuat fitur pencarian dengan memberikan suatu string. Hasilnya adalah semua nama file yang mengandung string tersebut. Fungsi yang digunakan oleh server.
  ```
    void find()
    {
        bRead();
        FILE* fileR = fopen("files.tsv","r");
        char find[200];
        bool flag = false;
        char data[1024] = {0};
        char tosend[10000]={0};
        char publisher[200], tahun[200],filepath[200],ext[20],filename[200];
        strcpy(find,recieve);
        while(fgets(data,1024,fileR)!=NULL){
               sscanf(data,"%[^\t]\t%s\t%s\t%s\t%s",filename,publisher,tahun,ext,filepath);
               char tosendbuff [1024];
               sprintf(tosendbuff,"Nama: %s\nPublisher: %s\nTahun Publish: %s\nExtensi File: %s\nFilepath: %s\n\n",filename,publisher,tahun,ext,filepath);
               char * beep;
               beep = strstr(filename,find);
               if(beep!=NULL){
                   flag=true;
                strcat(tosend,tosendbuff);   
               }
               bzero(data,1024);
           }
           fclose(fileR);
           if (!flag){
               sends("File tidak ditemukan\n");
           }
           else {
               sends(tosend);
           }
    }
  ```
  Fungsi ini dipanggil dengan fungsi pada aplikasi client.
    ```
      void find(){
          printf("Masukkan nama file\n");
          char find[200] = {0};
          scanf("%s",find);
          find[strcspn(find,"\n")]=0;
          sends(find);
          resR();
      }
    ```
    Setelah client memberikan string untuk mencari file, lalu dilakukan pencarian pada server dengan untuk membandingakn string yang dicari dengan string yang sudah ada pada database. Pada pembandingan string ini digunakan `strstr(filename,find)` agar nantinya string pada server akan dianggap benar jika memiliki string yang dicari (tidak harus sama persis dengan string yang dicari). Jika ditemukan yang sama, maka file akan ditampilkan dengan format
  ```
    Nama:
    Publisher:
    Tahun publishing:
    Ekstensi File : 
    Filepath : 
  ```
  
  **Output**
  
- **Penjelasan dan Penyelesaian Soal 1h**<br>
Pada soal 1h kita diminta untuk mencatat penambahan dan penghapusan file pada server di `running.log`. Oleh karena itu pada saat memanggil fingsi menambahakan dan menghapus file pada server ditambahkan fungsi untuk mencatat pada `running.log`.
  ```
    void Add() { 
        ...
        FILE* log = fopen("running.log","a");
        fprintf(log,"Tambah: %s %s",fname,upass);
        fclose(log);
    }
  ```
  ```
    void delete() {
        ...
        FILE* log = fopen("running.log","a");
        fprintf(log,"Hapus: %s %s",finds,upass);
        fclose(log);
        ...
    }
  ```
  
  **Output**
  
## Kendala yang dialami
  1. Pada 1d output yang keluar pada terminal client masih bermasalah jika dilanjutkan dengan input baru.
  2. xx
  3. xx

## Penjelasan dan Penyelesaian soal no.2
- **Penjelasan dan Penyelesaian Soal 2a**<br>
Pada soal 2a ini meminta kita untuk membuat sebuah program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Dengan ketentuan matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka). Hal yang pertama dilakukan adalah mendefinisikan matriksnya.
  ```
  //matriks asal
    int matriks1[4][3] =
      {
        4,1,4,
        2,1,3,
        4,2,2,
        1,1,4
      };

    int matriks2[3][6] =
    {
      2,1,3,2,0,3,
      1,4,4,0,0,2,
      1,1,0,1,2,1
    };

    //ukuran matriks
    int mat1I = 4, 
        mat1J = 3;
    int mat2I = 3,
        mat2J = 6;
    
    //nilai yang di return inisialisasi awal
    int returnNilai[4][6];

    //semaphore buat return matriks nya
    int returnMatriks = 0;

   ```
   Kemudian, kami membuat sebuah fungsi untuk menghitung matriks dan dapat sebuah fungsi print hasilnya.
   ```
   void *ngitungMatriks(void *simp)
   {
      int *pesan;
      pesan = (int *)simp;

      int iMatriks1 = pesan[0];
      int jMatriks2 = pesan[1];
      free(pesan);

      int jumlah = 0;
      int i;

      for(i = 0; i < mat1J; i++)
       {
         jumlah += matriks1[iMatriks1][i]*matriks2[i][jMatriks2];
       }

      returnNilai[iMatriks1][jMatriks2] = jumlah;
    }

    void *printHasil(void *pointer)
    {
       char *pesan; //nyimpen perintah
       pesan = (char *)pointer;

       printf("%s \n", pesan);
       for(int i = 0; i <= 8; i++)
        {
          printf("%s %d \n", pesan, i);
        }
    }
   ```
   Sebuah matriks terdapat sebuah value x dan y atau i dan j sama seperti artinya `iMatriks1 = pesan[0]` yang berarti value x atau kolom yang terdapat pada matriks 1 akan di simpan pada variabel pesan sama seperti baris seperti itu juga. 0 dan 1 itu menggambarkan ada dimana pesan itu disimpan 0, ada di kanan 1 di kiri. Selanjutnya, terdapat sebuah perulangan sampai `mat1J`dan di hitung jumlahnya dengan mangalikan matriks1 dan matriks2. Terdapat sebuah fungsi bernama `printHasil` yang digunakan untuk mengeprin hasil yang akan dihasilkan nantinya. Selanjutnya terdapat sebuah fungsi `main()` 
   ```
   int main()
    {
      pthread_t thread[mat1I][mat2J]; //inisialisasi awal
      int matriks[mat1J][mat2J];

      //mengirimkan single thread untuk setiap elemen dalam matriks hasil
      for(int i = 0; i < mat1I; i++)
      {
        for(int j = 0 ; j < mat2J; j++)
        {
            int *pesan;
            pesan = malloc(sizeof(int) * 2);
            pesan[0] = i;
            pesan[1] = j;
            void *pesanPtr = (void *)pesan;
            matriks[i][j] = pthread_create(&thread[i][j], NULL, ngitungMatriks, pesanPtr); //buat thread pertama
            //kalo error
            if(matriks[i][j])
            {
                fprintf(stderr, "Error - pthread_create() return code: %d\n", matriks[i][j]);
                exit(EXIT_FAILURE);
            }
        }
      }

      //join mat1I dan mat2J
      for(int i = 0; i < mat1I; i++)
      {
        for (int j = 0; j < mat2J; j++)
        {
           pthread_join( thread[i][j], NULL );
        //    printf("thread joined\n");
        }
      }

      //returnnilai
      for(int i = 0; i < mat1I; i++)
      {
        printf("%d ", returnNilai[i][0]);
        for(int j = 1; j < mat2J; j++)
        {
            printf("%d ", returnNilai[i][j]);
        }
        printf("\n");
      }
     exit(EXIT_SUCCESS);
    }
   ```
   
- **Penjelasan dan Penyelesaian Soal 2b**<br>
Pada soal 2b ini kita diminta untuk membuat program dengan menggunakan matriks output dari program sebelumnya (program `soal2a.c`). Untuk mengerjakan soal ini kami juga diminta untuk mengerjakan dengan menggunakan shared memeory yang kemudian matriks tersebut akan dilakukan perhitungan matriks baru (input user). Hal yang pertama dapat dilakukan adalah membuat input user yang akan dipasangkan deengan variable yang menampung matriks hasil (variable value) dengan cara : 
  ```
    printf("Masukkan 24 Input :\n");
      for(iter=0;iter<24;iter++){
        scanf("%d",&inputs[iter]);
    }
  ```
  Setelah membuat inputan user saatnya kami membuat sebuah struct untuk memasangkan tiap-tiap value dan input user kemudia di assign tiap array ke dalam stuct berdasarkan index serta membuat thread agar proses dapat bersambung
  ```
  struct pair
  {
    int angka;
    int batas;
  };
  
  for(iter=0;iter<24;iter++){
            xresult[iter] = value[iter];
            struct pair pair;
            pair.angka = xresult[iter];
            pair.batas = inputs[iter];

            int error = pthread_create(&tid[iter],NULL,faktorial,(void*)&pair);

            if(error != 0){
                printf("\nCan't create thread : [%s]",strerror(error));
            }   

            pthread_join(tid[i],NULL);
    }
  ```
  Langkah terakhir adalah memunculkan faktorial sesuai dengan ketentuan yang berada pada soal dimana ketika input lebih besar dibanding hasil perkalian matriks pada array value maka akan di outputkan seluruh faktorialnya. Jika ditemykan bahwa input lebih kecil daripada hasil perkalian matriks pada array value maka akan dioutputkan faktorialnya hingga batas inputnya. Serta apabila ditemukan inpurnya 0 maka akan dioutputkan nol pula. 
  ```
  void* faktorial(void *pairs){

    struct pair *pair = (struct pair *)pairs;
    int angka = pair->angka;
    int batas = pair->batas;
    if(angka == 0 || batas == 0){
        printf("0\n");
    }
    else{
        if(batas > angka){
            while(angka>0){
                if(angka == 1){
                    printf("%d",angka);
                }else{
                    printf("%d*",angka);
                }
                angka--;
            }
            printf("\n");
        }else if(angka >= batas){
            while(batas>0){
                if(batas == 1){
                    printf("%d",angka);
                }else{
                    printf("%d*",angka);
                }
                angka--;
                batas--;
            }
            printf("\n");
        }
    }
  }
  ```
  
- **Penjelasan dan Penyelesaian Soal 2c**<br>
Pada `soal2c` ini kita diminta untuk mengeck 5 proses teratas apa saja yang memakan resource komputer Loba dengan command "ps aux | sort -nrk 3,3 | head -5". Pada `soal2c` ini harus menggunakan IPC Pipes dan dilarang menggunakan system(). Hal pertama yang dapat digunakan adalah membuat`pipe0` yang digunakan standard input kedalam `ls` sebuah fungsi `int main(int argc, char *argv)`
  ```
  // fork (ps aux)
  //kalo misalkan error
    if ((id = fork()) == -1) 
    {
       printf("HARUSNYA SI GABAKAL TERJADI\n");
        perror("Fork Creation is Failde:(\n");
        exit(EXIT_FAILURE);
    } 
    
    else if (id == 0) 
    {
        // output to pipe1
        dup2(pipes1[1], 1);

        // close fds
        close(pipes1[0]);
        close(pipes1[1]);

        // exec
        char *jalaninPS[] = {"ps", "aux", NULL};
        execv("/bin/ps", jalaninPS);

        // kalo gajalan
        perror("bad exec ps:(");
        _exit(1);
    }
  ```
  Selanjutnya, ketika pembuatan sebuah `fork` untuk command `ps aux` dinyatakan berhasil, maka `pipe0` akan di close dan outputnya akan di `grep`. Sebelum ke langkah selanjutnya, kita akan membuat`pipe0` jika dikatakan berhasil maka`pipe1` dinyatakan sebagai parent process untuk mengambil input dari hasil output dari `pipe0`dan akan dilanjutkan ke tail dan ke standard ouput. Dan ketika di parent akan di ututkan dan di buat command `-nrk 3,3` dan argumen untuk mengecek resource 5 proses teratas.
  ```
  // parent

    // create pipe2
    if (pipe(pipes2) == -1) 
    {
       printf("HARUSNYA SI GABAKAL TERJADI\n");
        perror("Pipe2 is Failde:(\n");
        exit(EXIT_FAILURE);
    }

    // fork (grep root)
    if ((id = fork()) == -1) 
    {
        printf("HARUSNYA SI GABAKAL TERJADI\n");
        perror("Fork2 is Failde:(\n");
        exit(EXIT_FAILURE);
    } 

    else if (id == 0) 
    {
        dup2(pipes1[0], 0); // input pipes1
        dup2(pipes2[1], 1);// output pipes2

        // tutup pipesnya (fds)
        close(pipes1[0]);
        close(pipes1[1]);
        close(pipes2[0]);
        close(pipes2[1]);

        // exec sort
        char *sort[] = {"sort", "-nrk", "3.3", NULL};
        execv("/bin/sort", sort) ;

        // kalo gajalan
        perror("bad exec grep root");
        _exit(1) ;
    }
  //parentnya  
  // tutup yang ga kepake
    close(pipes1[0]);
    close(pipes1[1]);

    // fork (grep sbin)
    if ((id = fork()) == -1) 
    {
        printf("HARUSNYA SI GABAKAL TERJADI\n");
        perror("Grep SBIN is Failed:(\n");
        exit(EXIT_FAILURE);
    } 
    
    else if (id == 0) 
    {
        dup2(pipes2[0], 0); // input from pipe2

        // tutup kalo misalkan output to stdoutnya udah 
        close(pipes2[0]);
        close(pipes2[1]);

        // exec
        char *argv[] = {"head", "-5", NULL};
        execv("/bin/head", argv);

        // exec gajalan
        perror("bad exec grep sbin");
        _exit(1);
    }
  ```
  
## Kendala yang dialami
  1. Ketika meng-run hasil 2a jika diganti inputannya maka akan berbeda hasilnya, Saya mengatasinya dengan cara memperbesar batasan ketika meng-print hasil.
  2. `Segmentation Fault` ketika mengerjakan soal 2b.
## Output 
  1. 2a <br>
  Ouput hasil no 2a dapat dilihat di bawah ini:<br>
   ![soal2](https://github.com/rayhandaffa/soal-shift-sisop-modul-3-D06-2021/blob/main/soal2/output2a.jpg)<br> 
  2. 2b<br>
  Ouput hasil no 2b dapat dilihat di bawah ini: <br> 
  ![soal2](https://github.com/rayhandaffa/soal-shift-sisop-modul-3-D06-2021/blob/main/soal2/output2b.jpg)<br> 
  3. 2c<br> 
  Ouput hasil no 2c dapat dilihat di bawah ini: <br> 
  ![soal2](https://github.com/rayhandaffa/soal-shift-sisop-modul-3-D06-2021/blob/main/soal2/ouput2c.jpg)<br> 

## Penjelasan dan Penyelesaian soal no.3
- **Penjelasan dan Penyelesaian Soal 3a**<br>
Diminta untuk membuat program C untuk mengkategorikan file. Program C ini akan memindahkan file sesuai ekstensinya ke dalam folder. <br>
Pada soal 3a, program diminta menerima opsi -f, sehingga pengguna dapat menambahkan argumen file yang dapat dikategorikan sebanyak yang diinginkan pengguna. <br>

    ```
    // soal3a
    if (argc > 2 && strcmp(argv[1], "-f") == 0)
    {
        pthread_t tid[argc - 2]; // initialize thread
        int count = 0;

        for (int i = 2; i < argc; i++)
        {
            if (access(argv[i], F_OK) == 0)
            {
                pthread_create(&tid[count], NULL, categorize, (void *)argv[i]); // categorize file

                printf("File %d : Berhasil Dikategorikan\n", i - 1);

                count++;
            }
            else
                printf("File %d : Sad, gagal :(\n", i - 1);
        }

        // as wait, join thread to wait for terminated
        for (int i = 0; i < count; i++)
            pthread_join(tid[i], NULL); // join thread

        return 0;
    }
    ```
    Pada program diatas, thread diinisialisasi dengan `pthread_t` dalam bentuk array `tid[argc - 2]`. Jika file dapat diakses, selanjutnya dapat dilakukan pengkategorian file dengan fungsi `pthread_create` dengan parameter `tid[count]`, `NULL`, `categorize`, dan `argv[i])`. Fungsi `categorize` adalah fungsi yang digunakan untuk mengkategorisasikan file. 
    ```
    pthread_create(&tid[count], NULL, categorize, (void *)argv[i]);
    ```
    Apabila file-file yang diinginkan telah dikategorikan, maka keluarkan output "File (i - 1) : Berhasil Dikategorikan". Dan ditambahkan join thread untuk menunggu terminasi, yang berfungsi sebagai wait antar thread.

- **Penjelasan dan Penyelesaian Soal 3b**<br>
Pada soal 3b, program diminta menerima opsi -d, sehingga pengguna dapat melakukan pengkategorian pada suatu directory. Namun pada opsi -d ini, user hanya bisa memasukkan input 1 directory saja.
    ```
    // soal3b
        else if (argc == 3 && strcmp(argv[1], "-d") == 0)
        {
            DIR *directoryFile = opendir(argv[2]);

            if (directoryFile)
            {
                struct dirent *dirPath;

                int threadSize = 0;
                while ((dirPath = readdir(directoryFile)) != NULL)
                    if (dirPath->d_type == DT_REG) // check if it's regular file
                        threadSize++;              // adjust thread size

                categorizeFolder(argv[2], threadSize); // recursive

                closedir(directoryFile);
                printf("Direktori sukses disimpan!\n");
            }
            else if (ENOENT == errno)
                printf("Yah, gagal disimpan :(\n"); // directory does not exist
        }
    ```
    Pada program diatas, dilakukan pengecekan apakah `argv[1]` sama dengan "-d". Kemudian direktori dibuka, apabila direktori tersebut tidak ada maka cetak "Yah, gagal disimpan :(". Apabila direktori tersebut ada maka berlanjut ke proses selanjutnya.
    
    `threadSize` diinisialisi dengan nilai `0`. Dilakukan pengecekan, apabila `dirPath->d_type` merupakan file reguler, maka naikkan nilai `threadSize`. `threadSize` ini digunakan untuk menyimpan jumlah thread yang dibutuhkan. Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.

    Kemudian, dilakukan pengkategorian folder dengan memanggil fungsi `categorizeFolder` yang dilakukan secara rekursif. Setelah selesai, tutup direktori dan cetak "Direktori sukses disimpan!".

- **Penjelasan dan Penyelesaian Soal 3c**<br>
Pada soal 3c, program diminta menerima opsi *, sehingga pengguna dapat mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut.
    ```
    // soal3c
        else if (argc == 2 && strcmp(argv[1], "*") == 0)
        {
            char *current = getenv("PWD"); // get the path where the program runs
            DIR *dir = opendir(current);   // open the current directory

            struct dirent *dirPath;
            int threadSize = 0;

            while ((dirPath = readdir(dir)) != NULL)
                if (dirPath->d_type == DT_REG) // check if it's regular file
                    threadSize++;              // adjust thread size

            categorizeFolder(current, threadSize); // call the categorizeFolder function
            closedir(dir);
        }
    ```
    Pertama, dilakukan pengecekan apakah `argv[1]` sama dengan "*". Kemudian dipanggil fungsi `getenv("PWD")` untuk mendapatkan path dimana program berjalan dan path directory saat ini, `current`, dibuka.

    `threadSize` diinisialisi dengan nilai `0`. Dilakukan pengecekan, apabila `dirPath->d_type` merupakan file reguler, maka naikkan nilai `threadSize`. `threadSize` ini digunakan untuk menyimpan jumlah thread yang dibutuhkan. Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.

    Kemudian, dilakukan pengkategorian folder dengan memanggil fungsi `categorizeFolder` untuk path direktori saat ini, `current`, yang dilakukan secara rekursif. Setelah selesai, tutup direktori.
- **Penjelasan dan Penyelesaian Soal 3d**<br>
Pada soal 3d, diharuskan semua file ada di dalam folder, jika ada file yang tidak berekstensi maka file tersebut disimpan di folder "Unknown". Jika file tersebut hidden, masukkan ke folder "Hidden". Langkah ini diimplementasikan dengan fungsi `checkExtension`. 
    ```
    // Function to get file extension
    char *checkExtension(char *dir)
    {
        char *unknown = {"Unknown"};
        char *hidden = {"Hidden"};
        char *temp = strrchr(dir, '/');
        char extension[400];

        if (temp[1] == '.')
            return hidden; // hidden file

        int i = 0;
        while (i < strlen(temp) && temp[i] != '.')
            i++; // checks for a 'dot' in the file name

        if (i == strlen(temp))
            return unknown; // unknown file

        for (int j = i; i < strlen(temp); i++)
            extension[i - j] = temp[i]; // get the file extension

        return lowercase(extension + 1); // change file extension to lowercase
    }
    ```
    Fungsi `checkExtension` ini digunakan untuk mengecek dan mendapatkan ekstensi dari file pada `source` yang sebelumnya di-passing oleh fungsi `categorize`.
    
    Pada fungsi `checkExtension` digunakan `strrchr(dir, '/')` untuk menemukan kemunculan terakhir karakter "/" dalam string dan nama file ditaruh dalam `temp`.

    Jika `temp[1]` sama dengan '.' (titik), maka file tersebut adalah file hidden, return `"Hidden"`.

    Lakukan perulangan untuk pengecekan '.' (titik) sampai akhir dari nama file. Lakukan increment pada variabel `i`, selama karakter yang diiterasi bukan '.' (titik).

    Jika `i` sama dengan panjang nama file, maka tidak ada '.' (titik) di nama file, dengan kata lain, file tersebut tidak memiliki ekstensi, return `"Unknown"`. Jika tidak, lanjutkan ke proses selanjutnya.

    Dilakukan iterasi untuk mendapatkan ekstensi dari nama file yang disimpan dalam array `extension`. Setelah itu, ubah ekstensi file menjadi huruf kecil dan return ekstensi tersebut.

    Kemudian, dilakukan pengkategorian file dengan memanggil fungsi `categorize` untuk direktori saat ini, `current`. Jika direktori tidak ada, buat direktori baru dengan nama ekstensi. Jika file tersebut adalah file hidden, maka buat direktori bernama `"Hidden"`. Jika file tersebut adalah file yang tidak memiliki ekstensi, maka buat direktori bernama `"Unknown"`. String yang menyimpan ekstensi ditunjuk oleh pointer `*ptrExtension`.
    ```
    mkdir(ptrExtension, 0755); // if directory does not exist, create a new directory with an extension name
    ```
    Berikut ini adalah fungsi `categorize`.
    ```
    // Function for categorizing files
    void *categorize(void *arg)
    {
        char *source = (char *)arg;

        char sourcePath[150];
        memcpy(sourcePath, (char *)arg, 400); // take the file path

        char *ptrExtension = checkExtension(source); // pointer to the file extension

        char extension[400];
        strcpy(extension, ptrExtension); // copy from pointer to extension

        DIR *dir = opendir(ptrExtension); // try to open the directory with the extension name
        if (dir)
            closedir(dir);
        else if (ENOENT == errno)
            mkdir(ptrExtension, 0755); // if directory does not exist, create a new directory with an extension name

        char *ptrFileName = checkName(sourcePath); // get the file name
        char *current = getenv("PWD");             // get the path where the program is run

        char destPath[402];
        sprintf(destPath, "%s/%s/%s", current, extension, ptrFileName); // concatenate the new path of the file

        rename(sourcePath, destPath); // rename old path to new path
    }
    ```

- **Penjelasan dan Penyelesaian Soal 3e**<br>
Pada soal 3e, setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat. Sebagai contoh, pada bagian soal 3a dan soal 3c, dilakukan pemanggilan fungsi `categorizeFolder` secara rekursif yang memanfaatkan thread.<br>
Berikut ini adalah pemanggilan fungsi `categorizeFolder` pada soal 3b.
    ```
    categorizeFolder(argv[2], threadSize); // recursive
    ```
    Berikut ini adalah pemanggilan fungsi `categorizeFolder` pada soal 3c.
    ```
    categorizeFolder(current, threadSize); // call the categorizeFolder function
    ```
    Ketika memanggil fungsi `categorizeFolder`, dilakukan passing parameter `threadSize` yang merupakan banyak thread yang dibutuhkan yang telah dihitung sebelumnya untuk setiap file reguler dengan melakukan pengecekan iterasi sebagai berikut.
    ```
    int threadSize = 0;
            while ((dirPath = readdir(directoryFile)) != NULL)
                if (dirPath->d_type == DT_REG) // check if it's regular file
                    threadSize++;              // adjust thread size
    ```
    Setelah masuk ke fungsi `categorizeFolder`, direktori dibuka dan dilakukan inisialisasi thread sebagai berikut.
    ```
    pthread_t tid[threadSize]; // initialize thread
    ```
    Setelah itu, dilakukan perulangan yang mana jika direktori path merupakan file biasa, maka dilakukan kombinasi path folder dan path file, lalu buat thread untuk mengkategorikan file tersebut. Jika file telah berhasil dikategorikan, tingkatkan nilai count untuk memproses file selanjutnya.
    ```
    if (dirPath->d_type == DT_REG)
        // if it's a regular file
        {
            sprintf(fileName[count], "%s/%s", folderPath, dirPath->d_name); // file path in the folder

            pthread_create(&tid[count], NULL, categorize, (void *)fileName[count]); // create thread to categorize files

            count++;
        }
    ```

## Kendala yang dialami
  1. Kendala yang dialami dalam mengerjakan soal nomor 3 ini adalah pada saat mencoba soal 3a untuk mengkategorikan beberapa file, terdapat suatu error pada salah satu folder, sedangkan folder lainnya tidak. Folder yang bermasalah biasanya memiliki nama ekstensi yang sesuai tetapi disertai dengan simbol atau karakter tertentu setelah nama ekstensi. Error output dapat dilihat pada screenshot output hasil no 3a.
## Output 
  1. 3a <br>
  Ouput hasil no 3a dapat dilihat di bawah ini:<br>
   ![soal3](https://github.com/rayhandaffa/soal-shift-sisop-modul-3-D06-2021/blob/main/soal3/output3a.png)<br> 
  2. 3b<br>
  Ouput hasil no 3b dapat dilihat di bawah ini: <br> 
  ![soal3](https://github.com/rayhandaffa/soal-shift-sisop-modul-3-D06-2021/blob/main/soal3/output3b.png)<br> 
  3. 3c<br> 
  Ouput hasil no 3c dapat dilihat di bawah ini: <br> 
  ![soal3](https://github.com/rayhandaffa/soal-shift-sisop-modul-3-D06-2021/blob/main/soal3/output3c.png)<br>
