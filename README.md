# Sistem Operasi D-06 (2021)
Praktikum Sistem Operasi Modul 3 - Thread dan IPC
Kelompok : 
1. Ramadhan Arif - 05111940000162
2. Zulfayanti Sofia Solichin - 05111940000189
3. Rayhan Daffa Alhafish - 05111940000227

## Penjelasan dan Penyelesaian soal no.1
- **Penjelasan dan Penyelesaian Soal 1a**<br>
- **Penjelasan dan Penyelesaian Soal 1b**<br>
- **Penjelasan dan Penyelesaian Soal 1c**<br>
- **Penjelasan dan Penyelesaian Soal 1d**<br>
- **Penjelasan dan Penyelesaian Soal 1e**<br>
## Kendala yang dialami
  1. xx
  2. xx
  3. xx
## Output

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
Pada soal 3e, setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.<br>
Sebagai contoh, pada bagian soal 3a dan soal 3c, dilakukan pemanggilan fungsi `categorizeFolder` secara rekursif yang memanfaatkan thread.<br>
Pada soal 3b,
    ```
    categorizeFolder(argv[2], threadSize); // recursive
    ```
    Pada soal 3c,
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
  1. Kendala yang dialami dalam mengerjakan soal nomor 3 ini adalah pada saat mencoba soal 3a untuk mengkategorikan beberapa file, terdapat suatu error pada salah satu folder, sedangkan folder lainnya tidak. Folder yang bermasalah biasanya memiliki nama ekstensi yang sesuai tetapi disertai dengan simbol atau karakter tertentu setelah nama ekstensi.
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
