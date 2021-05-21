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
- **Penjelasan dan Penyelesaian Soal 3b**<br>
- **Penjelasan dan Penyelesaian Soal 3c**<br>
- **Penjelasan dan Penyelesaian Soal 3d**<br>
- **Penjelasan dan Penyelesaian Soal 3e**<br>
## Kendala yang dialami
  1. xx
  2. xx
  3. xx
## Output
