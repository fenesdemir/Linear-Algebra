#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <math.h>

struct nokta
{
    float x;
    float y;
    float z;
};
struct typeBir
{
    float x;
    float y;
    float z;
    int r;
    int g;
    int b;
};

struct typeIki
{
    float x;
    float y;
    float z;
};

struct dosyaIsim
{
    char isim[50];
    int hata;
};
int main()
{
    struct dosyaIsim isimList[99];
    int secim = 0;
    int sayac = 0;
    int secimBir = 0;

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir ("C:\\projeler\\DosyaOkumaProje\\")) != NULL) //belirtilen yerdeki dosyalarin isimlerini kaydet.
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if(strstr(ent->d_name,".nkt") != NULL && strstr(ent->d_name,"output") == NULL )
            {
                strcpy(isimList[sayac].isim, ent->d_name);
                sayac ++;
            }
        }
        closedir (dir);
    }
    else
    {
        perror ("");
        return EXIT_FAILURE;
    }

    FILE *fp;

    secimEtiketi:

    printf("Lutfen seciminizi giriniz.\n");
    scanf("%d", &secim);

    if(secim == 1){
        secimBir = 1;
        fp = fopen("output.nkt", "a");
        fprintf(fp, "Secim 1\n");
        fclose(fp);

        for(int i = 0; i< sayac; i++){
            fp = fopen(isimList[i].isim, "r"); //dosya acildi

        if (fp == NULL)
        {
            perror("Dosya acilamadi.\n");
            exit(EXIT_FAILURE);
        }

        char ch[100];
        char dataType[20];
        int noktalar = 0;
        int satir = 0;
        int type = 0;
        int vers = 0;
        int hataDurum = 0;

        while(fgets(ch,99,fp) != 0 && satir<5)
        {
            char * ptr = strstr(ch,"NOKTALAR");

            if( ptr != NULL)
            {
                const char *ayrac = " ";
                char *kelime = strtok(ch, ayrac);
                while (kelime != NULL)
                {
                    noktalar = atoi(kelime);
                    kelime = strtok(NULL, ayrac);
                }
            }
            else if(strcmp(ch,"ALANLAR x y z r g b\n") == 0)
            {
                type = 1;
            }
            else if(strcmp(ch,"ALANLAR x y z\n") == 0)
            {
                type = 2;
            }
            else if(strcmp(ch,"DATA ascii\n") == 0)
            {
                strcpy(dataType,ch);
            }
            else if(strcmp(ch,"DATA binary\n") == 0)
            {
                strcpy(dataType,ch);
            }else if(strcmp(ch,"VERSION 1\n") == 0){
                vers = 1;
            }
            satir++;
        }
        fclose(fp);

        //okuma bitir
        //sonuc yazdirma
        fp = fopen("output.nkt", "a");

        if(type != 1 && type != 2){
            fprintf(fp,"%s dosyasi hatali, tip bilgisi hatali.\n", isimList[i].isim);
            isimList[i].hata = 1;
            hataDurum = 1;
        }
        if((noktalar - (int) noktalar )!= 0){
            fprintf(fp,"%s dosyasi hatali, nokta bilgisi hatali.\n", isimList[i].isim);
            isimList[i].hata = 1;
            hataDurum = 1;
        }
        if( strcmp(dataType,"DATA ascii\n")!= 0 && strcmp(dataType,"DATA binary\n") != 0){
            fprintf(fp,"%s dosyasi hatali, data tipi hatali.\n", isimList[i].isim);
            isimList[i].hata = 1;
            hataDurum = 1;
        }
        if(vers != 1){
            fprintf(fp,"%s dosyasi hatali, versiyon bilgisi hatali\n", isimList[i].isim);
            isimList[i].hata = 1;
            hataDurum = 1;
        }

        if(hataDurum != 0){
            printf("Hatalardan dolayi %s dosyasiyla islem yapilamamaktadir.\n", isimList[i].isim);
            fprintf(fp,"Hatalardan dolayi %s dosyasiyla islem yapilamamaktadir.\n", isimList[i].isim);
            isimList[i].hata = 1;
        }
        fclose(fp);
        satir = 0;


        if(type == 1 && strcmp(dataType,"DATA ascii\n") == 0)
        {
            struct typeBir tipBir[noktalar];

            int noktaDurum = 0;
       	    int hataliNoktalar[99];
            int hataSayac =0;

            char bir[99];
            char isim[99];
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "r");

            if (fp == NULL)
            {
                perror("Dosya acilamadi.\n");
                exit(EXIT_FAILURE);
            }
            while(fgets(bir,99,fp) != 0)
            {
                if(satir >= 5)
                {
                    const char *ayrac = " ";
                    char *deger = strtok(bir, ayrac);
                    int yeniSayac = 0;
                    while (deger!= NULL)
                    {
                        if(yeniSayac == 0 )
                        {
                            tipBir[satir-5].x = atof(deger);
                        }
                        else if(yeniSayac == 1)
                        {
                            tipBir[satir-5].y = atof(deger);
                        }
                        else if(yeniSayac == 2)
                        {
                            tipBir[satir-5].z = atof(deger);
                        }
                        else if(yeniSayac == 3)
                        {
                            tipBir[satir-5].r = atoi(deger);
                        }
                        else if(yeniSayac == 4)
                        {
                            tipBir[satir-5].g = atoi(deger);
                        }
                        else if(yeniSayac == 5)
                        {
                            tipBir[satir-5].b = atoi(deger);
                        }
                        deger = strtok(NULL, ayrac);
                        yeniSayac ++;
                    }
                    if(yeniSayac != 6){
                        hataliNoktalar[hataSayac] = satir-5;   // while deger != null un altına
                        noktaDurum = 1;
                        isimList[i].hata = 1;
                        hataSayac ++;
                    }
                }
                satir++;
            }
            fclose(fp);

            fp = fopen("output.nkt","a");
            fprintf(fp, "\n%s\n", isim);
            fclose(fp);

                if(noktalar != satir-5){
                    fp = fopen("output.nkt","a");
                    fprintf(fp, "Nokta sayisi gecerli degildir.\n");
                    isimList[i].hata = 1;
                    fclose(fp);
                }else{
                    fp = fopen("output.nkt","a");
                    fprintf(fp, "Nokta sayisi dogrudur.\n");
                    fclose(fp);
                }
                if(noktaDurum != 0){
                        for(int i = 0; i<hataSayac; i++){
                            fp = fopen("output.nkt","a");
                            fprintf(fp,"%d numarali nokta bilgileri hatali girilmistir\n",hataliNoktalar[i]+1);  //case1 in içine
                            isimList[i].hata = 1;
                            fclose(fp);
                        }
                }else{
                    fp = fopen("output.nkt","a");
                    fprintf(fp,"Nokta bilgileri dogrudur.\n");
                    fclose(fp);
                }

                fprintf(fp,"\n\n");
        }
        else if(type == 2 && strcmp(dataType,"DATA ascii\n") == 0)
        {
            struct typeIki tipIki[noktalar];

            int noktaDurum = 0;
       	    int hataliNoktalar[99];
            int hataSayac =0;

            char iki[99];
            fp = fopen(isimList[i].isim, "r");
            char isim[99];
            strcpy(isim, isimList[i].isim);

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }
            while(fgets(iki,99,fp) != 0)
            {
                if(satir >= 5)
                {
                    const char *ayrac = " ";
                    char *deger = strtok(iki, ayrac);
                    int yeniSayac = 0;
                    while (deger!= NULL)
                    {
                        if(yeniSayac == 0 )
                        {
                            tipIki[satir-5].x = atof(deger);
                        }
                        else if(yeniSayac == 1)
                        {
                            tipIki[satir-5].y = atof(deger);
                        }
                        else if(yeniSayac == 2)
                        {
                            tipIki[satir-5].z = atof(deger);
                        }
                        deger = strtok(NULL, ayrac);
                        yeniSayac ++;
                    }
                     if(yeniSayac != 3){
                        hataliNoktalar[hataSayac] = satir-5;   // while deger != null un altına
                        noktaDurum = 1;
                        isimList[i].hata = 1;
                        hataSayac ++;
                    }
                }
                satir++;
            }
            fclose(fp);

            fp = fopen("output.nkt","a");
            fprintf(fp, "%s\n", isim);
            fclose(fp);

                if(noktalar != satir-5){
                    fp = fopen("output.nkt","a");
                    fprintf(fp, "Nokta sayisi gecerli degildir.\n");
                    isimList[i].hata = 1;
                    fclose(fp);
                }else{
                    fp = fopen("output.nkt","a");
                    fprintf(fp, "Nokta sayisi dogrudur.\n");
                    fclose(fp);
                    //goto etiket2;
                }
                if(noktaDurum != 0){
                        for(int i = 0; i<hataSayac; i++){
                            fp = fopen("output.nkt","a");
                            fprintf(fp,"%d numarali nokta bilgileri hatali girilmistir\n",hataliNoktalar[i]+1);
                            isimList[i].hata = 1;
                            fclose(fp);
                        }
                }else{
                    fp = fopen("output.nkt","a");
                    fprintf(fp,"Nokta bilgileri dogru.\n");
                    fclose(fp);
                }
                fprintf(fp,"\n\n");
        }
        else if(type == 1 && strcmp(dataType,"DATA binary\n") == 0)
        {
            struct typeBir tipBir[noktalar];
            char uc[99];
            char isim[99];
            int noktaSayiHata = 0;
            int noktaDegerHata = 0;
            int binarySatir = 0;
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "rb");

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }

            for(int i = 0; i < noktalar+5; i++)
            {
                if(i <5)
                {
                    fgets(uc,99,fp);
                }
                else if(i >= 5)
                {
                    fread(&tipBir[i-5], sizeof(struct typeBir), noktalar, fp);
                    binarySatir++;
                }
            }
            fclose(fp);
            fflush(stdin);

            fp = fopen("output.nkt","a");
            fprintf(fp, "\n%s\n", isim);
            fclose(fp);

                if(noktalar != binarySatir){
                    fp = fopen("output.nkt","a");
                    fprintf(fp, "Nokta sayisi gecerli degildir.\n");
                    isimList[i].hata = 1;
                    fclose(fp);
                    noktaSayiHata = 1;
                }else{
                    fp = fopen("output.nkt","a");
                    fprintf(fp, "Nokta sayisi dogru girilmistir.\n");
                    fclose(fp);
                }
                for(int i = 0; i<noktalar ; i++){
                        if(sizeof(tipBir[i]) != 24){
                            fp = fopen("output.nkt","a");
                            fprintf(fp,"%d numarali nokta bilgileri tam degildir.\n\n",i+1);
                            isimList[i].hata = 1;
                            fclose(fp);
                            noktaDegerHata = 1;
                        }
                }
                if(noktaDegerHata != 0 || noktaSayiHata != 0){
                    fp = fopen("output.nkt","a");
                    fprintf(fp,"Nokta hatalidir.\n\n");
                    isimList[i].hata = 1;
                    fclose(fp);
                }else{
                    fp = fopen("output.nkt","a");
                    fprintf(fp,"Nokta degerleri dogru girilmistir.\n\n");
                    fclose(fp);
                }
        }
        else if(type == 2 && strcmp(dataType,"DATA binary\n") == 0)
        {
            struct typeIki tipIki[noktalar];
            isimList[i].hata = 0;
            char dort[99];
            char isim[99];
            int noktaSayiHata = 0;
            int noktaDegerHata = 0;
            int binarySatir = 0;
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "rb");

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }
            for(int i = 0; i < noktalar+5; i++)
            {
                if(i <5)
                {
                    fgets(dort,99,fp);

                }
                else if(i >= 5)
                {
                    fread(&tipIki[i-5], sizeof(struct typeIki), noktalar, fp);
                    binarySatir++;
                }
            }
            fclose(fp);

            fp = fopen("output.nkt","a");
            fprintf(fp, "%s\n", isim);
            fclose(fp);

                if(noktalar != binarySatir){
                    fp = fopen("output.nkt","a");
                    fprintf(fp, "Nokta sayisi gecerli degildir.\n");
                    isimList[i].hata = 1;
                    fclose(fp);
                    noktaSayiHata = 1;
                }else{
                    fp = fopen("output.nkt","a");
                    fprintf(fp, "Nokta sayisi uyumludur.\n");
                    isimList[i].hata = 0;
                    fclose(fp);
                }

                for(int i = 0; i<noktalar ; i++){
                        if(sizeof(tipIki[i]) != 12){
                            fp = fopen("output.nkt","a");
                            fprintf(fp,"%d numarali nokta bilgileri tam degildir.\n",i+1);
                            fclose(fp);
                            isimList[i].hata = 1;
                            noktaDegerHata = 1;
                        }
                }

                if(noktaDegerHata != 0 || noktaSayiHata != 0){
                    fp = fopen("output.nkt","a");
                    fprintf(fp,"Dosya hatalidir.\n");
                    fclose(fp);
                }else{
                    fp = fopen("output.nkt","a");
                    fprintf(fp,"Nokta degerleri dogru girilmistir.\n");
                    isimList[i].hata = 0;
                    fclose(fp);
                }

                fprintf(fp,"\n\n");
          }
        }
        printf("Secilen islem tamamlandi.\n");
        goto secimEtiketi;
    }else if(secim == 2){

        if(secimBir != 1){
            printf("Hata. Programin duzgun calisabilmesi icin en az bir defa 1. secimin yapilmasi gerekir.\n");
            goto secimEtiketi;
        }

        fp = fopen("output.nkt", "a");
        fprintf(fp, "\nSecim 2\n\n");
        fclose(fp);

        for(int i = 0; i< sayac; i++){
            fp = fopen(isimList[i].isim, "r");
                if(isimList[i].hata != 0){
                    fclose(fp);
                    goto terminate2;
                }

        if (fp == NULL)
        {
            perror("Dosya acilamadi.\n");
            exit(EXIT_FAILURE);
        }

        char ch[100];
        char dataType[20];
        int noktalar = 0;
        int satir = 0;
        int type = 0;
        int vers = 0;

        while(fgets(ch,99,fp) != 0 && satir<5)
        {
            char * ptr = strstr(ch,"NOKTALAR");

            if( ptr != NULL)
            {
                const char *ayrac = " ";
                char *kelime = strtok(ch, ayrac);
                while (kelime != NULL)
                {
                    noktalar = atoi(kelime);
                    kelime = strtok(NULL, ayrac);
                }
            }
            else if(strcmp(ch,"ALANLAR x y z r g b\n") == 0)
            {
                type = 1;
            }
            else if(strcmp(ch,"ALANLAR x y z\n") == 0)
            {
                type = 2;
            }
            else if(strcmp(ch,"DATA ascii\n") == 0)
            {
                strcpy(dataType,ch);
            }
            else if(strcmp(ch,"DATA binary\n") == 0)
            {
                strcpy(dataType,ch);
            }else if(strcmp(ch,"VERSION 1\n") == 0){
                vers = 1;
            }
            satir++;
        }
        fclose(fp);

        satir = 0;

        if(type == 1 && strcmp(dataType,"DATA ascii\n") == 0){

            struct typeBir tipBir[noktalar];

            char bir[99];
            char isim[99];
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "r");

            if (fp == NULL)
            {
                perror("Dosya acilamadi.\n");
                exit(EXIT_FAILURE);
            }
            while(fgets(bir,99,fp) != 0)
            {
                if(satir >= 5)
                {
                    const char *ayrac = " ";
                    char *deger = strtok(bir, ayrac);
                    int yeniSayac = 0;
                    while (deger!= NULL)
                    {
                        if(yeniSayac == 0 )
                        {
                            tipBir[satir-5].x = atof(deger);
                        }
                        else if(yeniSayac == 1)
                        {
                            tipBir[satir-5].y = atof(deger);
                        }
                        else if(yeniSayac == 2)
                        {
                            tipBir[satir-5].z = atof(deger);
                        }
                        else if(yeniSayac == 3)
                        {
                            tipBir[satir-5].r = atoi(deger);
                        }
                        else if(yeniSayac == 4)
                        {
                            tipBir[satir-5].g = atoi(deger);
                        }
                        else if(yeniSayac == 5)
                        {
                            tipBir[satir-5].b = atoi(deger);
                        }
                        deger = strtok(NULL, ayrac);
                        yeniSayac ++;
                    }
                }
                satir++;
            }
            fclose(fp);
            float enYakin = sqrt(pow((tipBir[0].x - tipBir[1].x), 2) + pow((tipBir[0].y - tipBir[1].y), 2) + pow((tipBir[0].z - tipBir[1].z), 2));
            int yakin1, yakin2, uzak1, uzak2;
            float enUzak = sqrt(pow((tipBir[0].x - tipBir[1].x), 2) + pow((tipBir[0].y - tipBir[1].y), 2) + pow((tipBir[0].z - tipBir[1].z), 2));

            fp = fopen("output.nkt","a");
            fprintf(fp, "\n%s\n", isim);
            fclose(fp);
                //en yakin ve en uzak nokta
                for(int i = 0; i <noktalar; i++)
                {
                    for(int j = i+1; j<noktalar; j++)
                    {
                        if (sqrt(pow((tipBir[i].x - tipBir[j].x), 2) + pow((tipBir[i].y - tipBir[j].y), 2) + pow((tipBir[i].z - tipBir[j].z), 2)) <= enYakin)
                        {
                            enYakin = sqrt(pow((tipBir[i].x - tipBir[j].x), 2) + pow((tipBir[i].y - tipBir[j].y), 2) + pow((tipBir[i].z - tipBir[j].z), 2));
                            yakin1 = i;
                            yakin2 = j;
                        }
                    }
                }

                fp = fopen("output.nkt","a");
                fprintf(fp,"En yakin noktalar : %d numarali  %f, %f, %f, %d, %d, %d\n", yakin1+1, tipBir[yakin1].x, tipBir[yakin1].y, tipBir[yakin1].z, tipBir[yakin1].r, tipBir[yakin1].g, tipBir[yakin1].b);
                fprintf(fp,"En yakin noktalar : %d numarali  %f, %f, %f, %d, %d, %d\n", yakin2+1, tipBir[yakin2].x, tipBir[yakin2].y, tipBir[yakin2].z, tipBir[yakin2].r, tipBir[yakin2].g, tipBir[yakin2].b );

                //en uzak nokta

                for(int i = 0; i <noktalar; i++)
                {
                    for(int j = i+1; j<noktalar; j++)
                    {
                        if (sqrt(pow((tipBir[i].x - tipBir[j].x), 2) + pow((tipBir[i].y - tipBir[j].y), 2) + pow((tipBir[i].z - tipBir[j].z), 2)) >= enUzak)
                        {
                            enUzak = sqrt(pow((tipBir[i].x - tipBir[j].x), 2) + pow((tipBir[i].y - tipBir[j].y), 2) + pow((tipBir[i].z - tipBir[j].z), 2));
                            uzak1 = i;
                            uzak2 = j;
                        }
                    }
                }

                fprintf(fp,"En uzak noktalar : %d numarali  %f %f %f %d %d %d\n", uzak1+1, tipBir[uzak1].x, tipBir[uzak1].y, tipBir[uzak1].z, tipBir[uzak1].r, tipBir[uzak1].g, tipBir[uzak1].b);
                fprintf(fp,"En uzak noktalar : %d numarali  %f %f %f %d %d %d\n", uzak2+1, tipBir[uzak2].x, tipBir[uzak2].y, tipBir[uzak2].z, tipBir[uzak2].r, tipBir[uzak2].g, tipBir[uzak2].b);
                fprintf(fp, "\n");
                fclose(fp);
        }else if(type == 2 && strcmp(dataType,"DATA ascii\n") == 0){

            struct typeIki tipIki[noktalar];

            char iki[99];
            fp = fopen(isimList[i].isim, "r");
            char isim[99];
            strcpy(isim, isimList[i].isim);

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }
            while(fgets(iki,99,fp) != 0)
            {
                if(satir >= 5)
                {
                    const char *ayrac = " ";
                    char *deger = strtok(iki, ayrac);
                    int yeniSayac = 0;
                    while (deger!= NULL)
                    {
                        if(yeniSayac == 0 )
                        {
                            tipIki[satir-5].x = atof(deger);
                        }
                        else if(yeniSayac == 1)
                        {
                            tipIki[satir-5].y = atof(deger);
                        }
                        else if(yeniSayac == 2)
                        {
                            tipIki[satir-5].z = atof(deger);
                        }
                        deger = strtok(NULL, ayrac);
                        yeniSayac ++;
                    }

                }
                satir++;
            }
            fclose(fp);

            float enYakin = sqrt(pow((tipIki[0].x - tipIki[1].x), 2) + pow((tipIki[0].y - tipIki[1].y), 2) + pow((tipIki[0].z - tipIki[1].z), 2));
            int yakin1, yakin2, uzak1, uzak2;
            float enUzak = sqrt(pow((tipIki[0].x - tipIki[1].x), 2) + pow((tipIki[0].y - tipIki[1].y), 2) + pow((tipIki[0].z - tipIki[1].z), 2));

            fp = fopen("output.nkt","a");
            fprintf(fp, "%s\n", isim);
            fclose(fp);

            for(int i = 0; i <noktalar; i++)
                {
                    for(int j = i+1; j<noktalar; j++)
                    {
                        if (sqrt(pow((tipIki[i].x - tipIki[j].x), 2) + pow((tipIki[i].y - tipIki[j].y), 2) + pow((tipIki[i].z - tipIki[j].z), 2)) <= enYakin)
                        {
                            enYakin = sqrt(pow((tipIki[i].x - tipIki[j].x), 2) + pow((tipIki[i].y - tipIki[j].y), 2) + pow((tipIki[i].z - tipIki[j].z), 2));
                            yakin1 = i;
                            yakin2 = j;
                        }
                    }
                }
                fp = fopen("output.nkt","a");
                fprintf(fp, "En yakin noktalar : %d numarali  %f, %f, %f\n", yakin1+1, tipIki[yakin1].x, tipIki[yakin1].y, tipIki[yakin1].z);
                fprintf(fp, "En yakin noktalar : %d numarali  %f, %f, %f\n", yakin2+1, tipIki[yakin2].x, tipIki[yakin2].y, tipIki[yakin2].z);

                //en uzak nokta

                for(int i = 0; i <noktalar; i++)
                {
                    for(int j = i+1; j<noktalar; j++)
                    {
                        if (sqrt(pow((tipIki[i].x - tipIki[j].x), 2) + pow((tipIki[i].y - tipIki[j].y), 2) + pow((tipIki[i].z - tipIki[j].z), 2)) >= enUzak)
                        {
                            enUzak = sqrt(pow((tipIki[i].x - tipIki[j].x), 2) + pow((tipIki[i].y - tipIki[j].y), 2) + pow((tipIki[i].z - tipIki[j].z), 2));
                            uzak1 = i;
                            uzak2 = j;
                        }
                    }
                }

                fprintf(fp, "En uzak noktalar : %d numarali  %f, %f, %f\n", uzak1+1, tipIki[uzak1].x, tipIki[uzak1].y, tipIki[uzak1].z);
                fprintf(fp, "En uzak noktalar : %d numarali  %f, %f, %f\n", uzak2+1, tipIki[uzak2].x, tipIki[uzak2].y, tipIki[uzak2].z);
                fprintf(fp, "\n");
                fclose(fp);

        }else if(type == 1 && strcmp(dataType,"DATA binary\n") == 0){

            struct typeBir tipBir[noktalar];
            char uc[99];
            char isim[99];
            int binarySatir = 0;
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "rb");

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }

            for(int i = 0; i < noktalar+5; i++)
            {
                if(i <5)
                {
                    fgets(uc,99,fp);
                }
                else if(i >= 5)
                {
                    fread(&tipBir[i-5], sizeof(struct typeBir), noktalar, fp);
                    binarySatir++;
                }
            }
            fclose(fp);


            float enYakin = sqrt(pow((tipBir[0].x - tipBir[1].x), 2) + pow((tipBir[0].y - tipBir[1].y), 2) + pow((tipBir[0].z - tipBir[1].z), 2));
            int yakin1, yakin2, uzak1, uzak2;
            float enUzak = sqrt(pow((tipBir[0].x - tipBir[1].x), 2) + pow((tipBir[0].y - tipBir[1].y), 2) + pow((tipBir[0].z - tipBir[1].z), 2));

            fp = fopen("output.nkt","a");
            fprintf(fp, "\n%s\n", isim);
            fclose(fp);

            for(int i = 0; i <noktalar; i++)
                {
                    for(int j = i+1; j<noktalar; j++)
                    {
                        if (sqrt(pow((tipBir[i].x - tipBir[j].x), 2) + pow((tipBir[i].y - tipBir[j].y), 2) + pow((tipBir[i].z - tipBir[j].z), 2)) <= enYakin)
                        {
                            enYakin = sqrt(pow((tipBir[i].x - tipBir[j].x), 2) + pow((tipBir[i].y - tipBir[j].y), 2) + pow((tipBir[i].z - tipBir[j].z), 2));
                            yakin1 = i;
                            yakin2 = j;
                        }
                    }
                }
                fp = fopen("output.nkt","a");
                fprintf(fp, "En yakin noktalar : %d numarali  %f, %f, %f, %d, %d, %d\n", yakin1+1, tipBir[yakin1].x, tipBir[yakin1].y, tipBir[yakin1].z, tipBir[yakin1].r, tipBir[yakin1].g, tipBir[yakin1].b);
                fprintf(fp, "En yakin noktalar : %d numarali  %f, %f, %f, %d, %d, %d\n", yakin2+1, tipBir[yakin2].x, tipBir[yakin2].y, tipBir[yakin2].z, tipBir[yakin2].r, tipBir[yakin2].g, tipBir[yakin2].b);

                //en uzak nokta

                for(int i = 0; i <noktalar; i++)
                {
                    for(int j = i+1; j<noktalar; j++)
                    {
                        if (sqrt(pow((tipBir[i].x - tipBir[j].x), 2) + pow((tipBir[i].y - tipBir[j].y), 2) + pow((tipBir[i].z - tipBir[j].z), 2)) >= enUzak)
                        {
                            enUzak = sqrt(pow((tipBir[i].x - tipBir[j].x), 2) + pow((tipBir[i].y - tipBir[j].y), 2) + pow((tipBir[i].z - tipBir[j].z), 2));
                            uzak1 = i;
                            uzak2 = j;
                        }
                    }
                }

                fprintf(fp, "En uzak noktalar : %d numarali  %f, %f, %f, %d, %d, %d\n", uzak1+1, tipBir[uzak1].x, tipBir[uzak1].y, tipBir[uzak1].z, tipBir[uzak1].r, tipBir[uzak1].g, tipBir[uzak1].b);
                fprintf(fp, "En uzak noktalar : %d numarali  %f, %f, %f, %d, %d, %d\n", uzak2+1, tipBir[uzak2].x, tipBir[uzak2].y, tipBir[uzak2].z, tipBir[uzak2].r, tipBir[uzak2].g, tipBir[uzak2].b);
                fprintf(fp, "\n");
                fclose(fp);

        }else if(type == 2 && strcmp(dataType,"DATA binary\n")== 0){

                 struct typeIki tipIki[noktalar];
            char dort[99];
            char isim[99];
            int binarySatir = 0;
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "rb");

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }

            for(int i = 0; i < noktalar+5; i++)
            {
                if(i <5)
                {
                    fgets(dort,99,fp);

                }
                else if(i >= 5)
                {
                    fread(&tipIki[i-5], sizeof(struct typeIki), noktalar, fp);
                    binarySatir++;
                }
            }
            fclose(fp);

            float enYakin = sqrt(pow((tipIki[0].x - tipIki[1].x), 2) + pow((tipIki[0].y - tipIki[1].y), 2) + pow((tipIki[0].z - tipIki[1].z), 2));
            int yakin1, yakin2, uzak1, uzak2;
            float enUzak = sqrt(pow((tipIki[0].x - tipIki[1].x), 2) + pow((tipIki[0].y - tipIki[1].y), 2) + pow((tipIki[0].z - tipIki[1].z), 2));

            fp = fopen("output.nkt","a");
            fprintf(fp, "%s\n", isim);
            fclose(fp);

            for(int i = 0; i <noktalar; i++)
                {
                    for(int j = i+1; j<noktalar; j++)
                    {
                        if (sqrt(pow((tipIki[i].x - tipIki[j].x), 2) + pow((tipIki[i].y - tipIki[j].y), 2) + pow((tipIki[i].z - tipIki[j].z), 2)) <= enYakin)
                        {
                            enYakin = sqrt(pow((tipIki[i].x - tipIki[j].x), 2) + pow((tipIki[i].y - tipIki[j].y), 2) + pow((tipIki[i].z - tipIki[j].z), 2));
                            yakin1 = i;
                            yakin2 = j;
                        }
                    }
                }
                fp = fopen("output.nkt","a");
                fprintf(fp,"En yakin noktalar : %d numarali  %f, %f, %f\n", yakin1+1, tipIki[yakin1].x, tipIki[yakin1].y, tipIki[yakin1].z);
                fprintf(fp,"En yakin noktalar : %d numarali  %f, %f, %f\n", yakin2+1, tipIki[yakin2].x, tipIki[yakin2].y, tipIki[yakin2].z);

                //en uzak nokta

                for(int i = 0; i <noktalar; i++)
                {
                    for(int j = i+1; j<noktalar; j++)
                    {
                        if (sqrt(pow((tipIki[i].x - tipIki[j].x), 2) + pow((tipIki[i].y - tipIki[j].y), 2) + pow((tipIki[i].z - tipIki[j].z), 2)) >= enUzak)
                        {
                            enUzak = sqrt(pow((tipIki[i].x - tipIki[j].x), 2) + pow((tipIki[i].y - tipIki[j].y), 2) + pow((tipIki[i].z - tipIki[j].z), 2));
                            uzak1 = i;
                            uzak2 = j;
                        }
                    }
                }

                fprintf(fp,"En uzak noktalar : %d numarali  %f, %f, %f\n", uzak1+1, tipIki[uzak1].x, tipIki[uzak1].y, tipIki[uzak1].z);
                fprintf(fp,"En uzak noktalar : %d numarali  %f, %f, %f\n", uzak2+1, tipIki[uzak2].x, tipIki[uzak2].y, tipIki[uzak2].z);
                fprintf(fp, "\n");
                fclose(fp);
            }
            terminate2:
            printf(" ");
        }
        printf("Isleminiz tamamlandi.\n");
        goto secimEtiketi;
    }else if(secim == 3){

        if(secimBir != 1){
            printf("Hata. Programin duzgun calisabilmesi icin en az bir defa 1. secimin yapilmasi gerekir.\n");
            goto secimEtiketi;
        }

        fp = fopen("output.nkt", "a");
        fprintf(fp, "\nSecim 3\n\n");
        fclose(fp);

        for(int i = 0; i< sayac; i++){
            fp = fopen(isimList[i].isim, "r");
                if(isimList[i].hata != 0){
                    fclose(fp);
                    goto terminate3;
                }

        if (fp == NULL)
        {
            perror("Dosya acilamadi.\n");
            exit(EXIT_FAILURE);
        }

        char ch[100];
        char dataType[20];
        int noktalar = 0;
        int satir = 0;
        int type = 0;
        int vers = 0;

        while(fgets(ch,99,fp) != 0 && satir<5)
        {
            char * ptr = strstr(ch,"NOKTALAR");

            if( ptr != NULL)
            {
                const char *ayrac = " ";
                char *kelime = strtok(ch, ayrac);
                while (kelime != NULL)
                {
                    noktalar = atoi(kelime);
                    kelime = strtok(NULL, ayrac);
                }
            }
            else if(strcmp(ch,"ALANLAR x y z r g b\n") == 0)
            {
                type = 1;
            }
            else if(strcmp(ch,"ALANLAR x y z\n") == 0)
            {
                type = 2;
            }
            else if(strcmp(ch,"DATA ascii\n") == 0)
            {
                strcpy(dataType,ch);
            }
            else if(strcmp(ch,"DATA binary\n") == 0)
            {
                strcpy(dataType,ch);
            }else if(strcmp(ch,"VERSION 1\n") == 0){
                vers = 1;
            }
            satir++;
        }
        fclose(fp);

        satir = 0;

        if(type == 1 && strcmp(dataType,"DATA ascii\n") == 0){
            struct typeBir tipBir[noktalar];

            char bir[99];
            char isim[99];
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "r");

            if (fp == NULL)
            {
                perror("Dosya acilamadi.\n");
                exit(EXIT_FAILURE);
            }
            while(fgets(bir,99,fp) != 0)
            {
                if(satir >= 5)
                {
                    const char *ayrac = " ";
                    char *deger = strtok(bir, ayrac);
                    int yeniSayac = 0;
                    while (deger!= NULL)
                    {
                        if(yeniSayac == 0 )
                        {
                            tipBir[satir-5].x = atof(deger);
                        }
                        else if(yeniSayac == 1)
                        {
                            tipBir[satir-5].y = atof(deger);
                        }
                        else if(yeniSayac == 2)
                        {
                            tipBir[satir-5].z = atof(deger);
                        }
                        else if(yeniSayac == 3)
                        {
                            tipBir[satir-5].r = atoi(deger);
                        }
                        else if(yeniSayac == 4)
                        {
                            tipBir[satir-5].g = atoi(deger);
                        }
                        else if(yeniSayac == 5)
                        {
                            tipBir[satir-5].b = atoi(deger);
                        }
                        deger = strtok(NULL, ayrac);
                        yeniSayac ++;
                    }
                }
                satir++;
            }
            fclose(fp);

            float buyukX, kucukX, buyukY, kucukY, buyukZ, kucukZ;
            struct nokta kup[8];

            fp = fopen("output.nkt","a");
            fprintf(fp, "\n%s\n", isim);
            fclose(fp);
                //Kup

                buyukX = tipBir[0].x;
                kucukX = tipBir[0].x;

                buyukY = tipBir[0].y;
                kucukY = tipBir[0].y;

                buyukZ = tipBir[0].z;
                kucukZ = tipBir[0].z;

                for(int i =0; i<noktalar; i++)
                {
                    if(tipBir[i].x > buyukX)
                    {
                        buyukX = tipBir[i].x;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipBir[i].x < kucukX)
                    {
                        kucukX = tipBir[i].x;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipBir[i].y > buyukY)
                    {
                        buyukY = tipBir[i].y;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipBir[i].y < kucukY)
                    {
                        kucukY = tipBir[i].y;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipBir[i].z > buyukZ)
                    {
                        buyukZ = tipBir[i].z;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipBir[i].z < kucukZ)
                    {
                        kucukZ = tipBir[i].z;
                    }
                }

                float uzaklik = buyukX- kucukX;

                if((buyukY - kucukY)>uzaklik)
                {
                    uzaklik = buyukY - kucukY;
                }
                if((buyukZ - kucukZ)>uzaklik)
                {
                    uzaklik = buyukZ - kucukZ;
                }

                kup[0].x = buyukX;
                kup[0].y = buyukY;
                kup[0].z = buyukZ;

                kup[1].x = buyukX - uzaklik;
                kup[1].y = buyukY;
                kup[1].z = buyukZ;

                kup[2].x = buyukX;
                kup[2].y = buyukY - uzaklik;
                kup[2].z = buyukZ;

                kup[3].x = buyukX;
                kup[3].y = buyukY;
                kup[3].z = buyukZ - uzaklik;

                kup[4].x = buyukX - uzaklik;
                kup[4].y = buyukY - uzaklik;
                kup[4].z = buyukZ;

                kup[5].x = buyukX;
                kup[5].y = buyukY - uzaklik;
                kup[5].z = buyukZ - uzaklik;

                kup[6].x = buyukX - uzaklik;
                kup[6].y = buyukY;
                kup[6].z = buyukZ - uzaklik;

                kup[7].x = buyukX - uzaklik;
                kup[7].y = buyukY - uzaklik;
                kup[7].z = buyukZ - uzaklik;

                fp = fopen("output.nkt","a");

                for(int i = 0; i< 8 ; i++)
                {
                    fprintf(fp,"Kup %d numarali kose koordinatlari : %f %f %f 255 255 255\n", i+1, kup[i].x, kup[i].y, kup[i].z);
                }
                fprintf(fp, "\n");
                fclose(fp);

        }else if(type == 2 && strcmp(dataType,"DATA ascii\n") == 0){

            struct typeIki tipIki[noktalar];

            char iki[99];
            fp = fopen(isimList[i].isim, "r");
            char isim[99];
            strcpy(isim, isimList[i].isim);

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }
            while(fgets(iki,99,fp) != 0)
            {
                if(satir >= 5)
                {
                    const char *ayrac = " ";
                    char *deger = strtok(iki, ayrac);
                    int yeniSayac = 0;
                    while (deger!= NULL)
                    {
                        if(yeniSayac == 0 )
                        {
                            tipIki[satir-5].x = atof(deger);
                        }
                        else if(yeniSayac == 1)
                        {
                            tipIki[satir-5].y = atof(deger);
                        }
                        else if(yeniSayac == 2)
                        {
                            tipIki[satir-5].z = atof(deger);
                        }
                        deger = strtok(NULL, ayrac);
                        yeniSayac ++;
                    }
                }
                satir++;
            }
            fclose(fp);
            float buyukX, kucukX, buyukY, kucukY, buyukZ, kucukZ;
            struct nokta kup[8];

            fp = fopen("output.nkt","a");
            fprintf(fp, "\n%s\n", isim);
            fclose(fp);


                buyukX = tipIki[0].x;
                kucukX = tipIki[0].x;

                buyukY = tipIki[0].y;
                kucukY = tipIki[0].y;

                buyukZ = tipIki[0].z;
                kucukZ = tipIki[0].z;

                for(int i =0; i<noktalar; i++)
                {
                    if(tipIki[i].x > buyukX)
                    {
                        buyukX = tipIki[i].x;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipIki[i].x < kucukX)
                    {
                        kucukX = tipIki[i].x;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipIki[i].y > buyukY)
                    {
                        buyukY = tipIki[i].y;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipIki[i].y < kucukY)
                    {
                        kucukY = tipIki[i].y;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipIki[i].z > buyukZ)
                    {
                        buyukZ = tipIki[i].z;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipIki[i].z < kucukZ)
                    {
                        kucukZ = tipIki[i].z;
                    }
                }

                float uzaklik = buyukX- kucukX;

                if((buyukY - kucukY)>uzaklik)
                {
                    uzaklik = buyukY - kucukY;
                }
                if((buyukZ - kucukZ)>uzaklik)
                {
                    uzaklik = buyukZ - kucukZ;
                }

                kup[0].x = buyukX;
                kup[0].y = buyukY;
                kup[0].z = buyukZ;

                kup[1].x = buyukX - uzaklik;
                kup[1].y = buyukY;
                kup[1].z = buyukZ;

                kup[2].x = buyukX;
                kup[2].y = buyukY - uzaklik;
                kup[2].z = buyukZ;

                kup[3].x = buyukX;
                kup[3].y = buyukY;
                kup[3].z = buyukZ - uzaklik;

                kup[4].x = buyukX - uzaklik;
                kup[4].y = buyukY - uzaklik;
                kup[4].z = buyukZ;

                kup[5].x = buyukX;
                kup[5].y = buyukY - uzaklik;
                kup[5].z = buyukZ - uzaklik;

                kup[6].x = buyukX - uzaklik;
                kup[6].y = buyukY;
                kup[6].z = buyukZ - uzaklik;

                kup[7].x = buyukX - uzaklik;
                kup[7].y = buyukY - uzaklik;
                kup[7].z = buyukZ - uzaklik;

                fp = fopen("output.nkt","a");

                for(int i = 0; i< 8 ; i++)
                {
                    fprintf(fp,"Kup %d numarali kose koordinatlari : %f %f %f\n", i+1, kup[i].x, kup[i].y, kup[i].z);
                }
                fprintf(fp, "\n");
                fclose(fp);
        }else if(type == 1 && strcmp(dataType,"DATA binary\n") == 0){

            struct typeBir tipBir[noktalar];
            char uc[99];
            char isim[99];
            int binarySatir = 0;
            strcpy(isim, isimList[i].isim);


            fp = fopen(isimList[i].isim, "rb");
            fprintf(fp, "%s\n",isim);

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }

            for(int i = 0; i < noktalar+5; i++)
            {
                if(i <5)
                {
                    fgets(uc,99,fp);
                }
                else if(i >= 5)
                {
                    fread(&tipBir[i-5], sizeof(struct typeBir), noktalar, fp);
                    binarySatir++;
                }
            }
            fclose(fp);
            float buyukX, kucukX, buyukY, kucukY, buyukZ, kucukZ;
            struct nokta kup[8];

            fp = fopen("output.nkt","a");
            fprintf(fp, "\n%s\n", isim);
            fclose(fp);

            buyukX = tipBir[0].x;
                kucukX = tipBir[0].x;

                buyukY = tipBir[0].y;
                kucukY = tipBir[0].y;

                buyukZ = tipBir[0].z;
                kucukZ = tipBir[0].z;

                for(int i =0; i<noktalar; i++)
                {
                    if(tipBir[i].x > buyukX)
                    {
                        buyukX = tipBir[i].x;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipBir[i].x < kucukX)
                    {
                        kucukX = tipBir[i].x;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipBir[i].y > buyukY)
                    {
                        buyukY = tipBir[i].y;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipBir[i].y < kucukY)
                    {
                        kucukY = tipBir[i].y;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipBir[i].z > buyukZ)
                    {
                        buyukZ = tipBir[i].z;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipBir[i].z < kucukZ)
                    {
                        kucukZ = tipBir[i].z;
                    }
                }

                float uzaklik = buyukX- kucukX;

                if((buyukY - kucukY)>uzaklik)
                {
                    uzaklik = buyukY - kucukY;
                }
                if((buyukZ - kucukZ)>uzaklik)
                {
                    uzaklik = buyukZ - kucukZ;
                }

                kup[0].x = buyukX;
                kup[0].y = buyukY;
                kup[0].z = buyukZ;

                kup[1].x = buyukX - uzaklik;
                kup[1].y = buyukY;
                kup[1].z = buyukZ;

                kup[2].x = buyukX;
                kup[2].y = buyukY - uzaklik;
                kup[2].z = buyukZ;

                kup[3].x = buyukX;
                kup[3].y = buyukY;
                kup[3].z = buyukZ - uzaklik;

                kup[4].x = buyukX - uzaklik;
                kup[4].y = buyukY - uzaklik;
                kup[4].z = buyukZ;

                kup[5].x = buyukX;
                kup[5].y = buyukY - uzaklik;
                kup[5].z = buyukZ - uzaklik;

                kup[6].x = buyukX - uzaklik;
                kup[6].y = buyukY;
                kup[6].z = buyukZ - uzaklik;

                kup[7].x = buyukX - uzaklik;
                kup[7].y = buyukY - uzaklik;
                kup[7].z = buyukZ - uzaklik;

                fp = fopen("output.nkt","a");

                for(int i = 0; i< 8 ; i++)
                {
                    fprintf(fp,"Kup %d numarali kose koordinatlari : %f %f %f 255 255 255\n", i+1, kup[i].x, kup[i].y, kup[i].z);
                }
                fprintf(fp, "\n");
                fclose(fp);

        }else if(type == 2 && strcmp(dataType,"DATA binary\n") == 0){

            struct typeIki tipIki[noktalar];
            char dort[99];
            char isim[99];
            int binarySatir = 0;
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "rb");
            fprintf(fp, "%s\n",isim);

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }
            for(int i = 0; i < noktalar+5; i++)
            {
                if(i <5)
                {
                    fgets(dort,99,fp);
                }
                else if(i >= 5)
                {
                    fread(&tipIki[i-5], sizeof(struct typeIki), noktalar, fp);
                    binarySatir++;
                }
            }
            fclose(fp);
            float buyukX, kucukX, buyukY, kucukY, buyukZ, kucukZ;
            struct nokta kup[8];

            fp = fopen("output.nkt","a");
            fprintf(fp, "\n%s\n", isim);
            fclose(fp);

                buyukX = tipIki[0].x;
                kucukX = tipIki[0].x;

                buyukY = tipIki[0].y;
                kucukY = tipIki[0].y;

                buyukZ = tipIki[0].z;
                kucukZ = tipIki[0].z;

                for(int i =0; i<noktalar; i++)
                {
                    if(tipIki[i].x > buyukX)
                    {
                        buyukX = tipIki[i].x;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipIki[i].x < kucukX)
                    {
                        kucukX = tipIki[i].x;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipIki[i].y > buyukY)
                    {
                        buyukY = tipIki[i].y;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipIki[i].y < kucukY)
                    {
                        kucukY = tipIki[i].y;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipIki[i].z > buyukZ)
                    {
                        buyukZ = tipIki[i].z;
                    }
                }

                for(int i =0; i<noktalar; i++)
                {
                    if(tipIki[i].z < kucukZ)
                    {
                        kucukZ = tipIki[i].z;
                    }
                }

                float uzaklik = buyukX- kucukX;

                if((buyukY - kucukY)>uzaklik)
                {
                    uzaklik = buyukY - kucukY;
                }
                if((buyukZ - kucukZ)>uzaklik)
                {
                    uzaklik = buyukZ - kucukZ;
                }

                kup[0].x = buyukX;
                kup[0].y = buyukY;
                kup[0].z = buyukZ;

                kup[1].x = buyukX - uzaklik;
                kup[1].y = buyukY;
                kup[1].z = buyukZ;

                kup[2].x = buyukX;
                kup[2].y = buyukY - uzaklik;
                kup[2].z = buyukZ;

                kup[3].x = buyukX;
                kup[3].y = buyukY;
                kup[3].z = buyukZ - uzaklik;

                kup[4].x = buyukX - uzaklik;
                kup[4].y = buyukY - uzaklik;
                kup[4].z = buyukZ;

                kup[5].x = buyukX;
                kup[5].y = buyukY - uzaklik;
                kup[5].z = buyukZ - uzaklik;

                kup[6].x = buyukX - uzaklik;
                kup[6].y = buyukY;
                kup[6].z = buyukZ - uzaklik;

                kup[7].x = buyukX - uzaklik;
                kup[7].y = buyukY - uzaklik;
                kup[7].z = buyukZ - uzaklik;

                fp = fopen("output.nkt","a");

                for(int i = 0; i< 8 ; i++)
                {
                    fprintf(fp,"Kup %d numarali kose koordinatlari : %f %f %f\n", i+1, kup[i].x, kup[i].y, kup[i].z);
                }
                fprintf(fp, "\n");
                fclose(fp);
            }
            terminate3:
            printf(" ");
        }
        printf("Sectiginiz islem tamamlandi.\n");
        goto secimEtiketi;
    }else if(secim == 4){

        if(secimBir != 1){
            printf("Hata. Programin duzgun calisabilmesi icin en az bir defa 1. secimin yapilmasi gerekir.\n");
            goto secimEtiketi;
        }

        int r;
        struct nokta kure;
        fp = fopen("output.nkt", "a");
        fprintf(fp, "\nSecim 4\n\n");
        fclose(fp);

        printf("Kurenin x'i: \n");
                scanf("%f", &kure.x);
                printf("Kurenin y'i: \n");
                scanf("%f", &kure.y);
                printf("Kurenin z'i: \n");
                scanf("%f", &kure.z);
                printf("Kurenin yaricapi: \n");
                scanf("%d", &r);

        for(int i = 0; i< sayac; i++){
            fp = fopen(isimList[i].isim, "r");
                if(isimList[i].hata != 0){
                    fclose(fp);
                    goto terminate4;
                }

        if (fp == NULL)
        {
            perror("Dosya acilamadi.\n");
            exit(EXIT_FAILURE);
        }
        char ch[100];
        char dataType[20];
        int noktalar = 0;
        int satir = 0;
        int type = 0;
        int vers = 0;

        while(fgets(ch,99,fp) != 0 && satir<5)
        {
            char * ptr = strstr(ch,"NOKTALAR");

            if( ptr != NULL)
            {
                const char *ayrac = " ";
                char *kelime = strtok(ch, ayrac);
                while (kelime != NULL)
                {
                    noktalar = atoi(kelime);
                    kelime = strtok(NULL, ayrac);
                }
            }
            else if(strcmp(ch,"ALANLAR x y z r g b\n") == 0)
            {
                type = 1;
            }
            else if(strcmp(ch,"ALANLAR x y z\n") == 0)
            {
                type = 2;
            }
            else if(strcmp(ch,"DATA ascii\n") == 0)
            {
                strcpy(dataType,ch);
            }
            else if(strcmp(ch,"DATA binary\n") == 0)
            {
                strcpy(dataType,ch);
            }else if(strcmp(ch,"VERSION 1\n") == 0){
                vers = 1;
            }
            satir++;
        }
        fclose(fp);

        satir = 0;

        if(type == 1 && strcmp(dataType,"DATA ascii\n") == 0){

            struct typeBir tipBir[noktalar];

            char bir[99];
            char isim[99];
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "r");

            if (fp == NULL)
            {
                perror("Dosya acilamadi.\n");
                exit(EXIT_FAILURE);
            }
            while(fgets(bir,99,fp) != 0)
            {
                if(satir >= 5)
                {
                    const char *ayrac = " ";
                    char *deger = strtok(bir, ayrac);
                    int yeniSayac = 0;
                    while (deger!= NULL)
                    {
                        if(yeniSayac == 0 )
                        {
                            tipBir[satir-5].x = atof(deger);
                        }
                        else if(yeniSayac == 1)
                        {
                            tipBir[satir-5].y = atof(deger);
                        }
                        else if(yeniSayac == 2)
                        {
                            tipBir[satir-5].z = atof(deger);
                        }
                        else if(yeniSayac == 3)
                        {
                            tipBir[satir-5].r = atoi(deger);
                        }
                        else if(yeniSayac == 4)
                        {
                            tipBir[satir-5].g = atoi(deger);
                        }
                        else if(yeniSayac == 5)
                        {
                            tipBir[satir-5].b = atoi(deger);
                        }
                        deger = strtok(NULL, ayrac);
                        yeniSayac ++;
                    }
                }
                satir++;
            }
            fclose(fp);
            fp = fopen("output.nkt","a");
            fprintf(fp, "\n%s\n", isim);
            fclose(fp);

                fp = fopen("output.nkt","a");

                for(int i = 0; i< noktalar ; i++)
                {
                    if (sqrt(pow((tipBir[i].x - kure.x), 2) + pow((tipBir[i].y - kure.y), 2) + pow((tipBir[i].z - kure.z), 2)) <= r)
                    {
                        fprintf(fp,"%d numarali nokta kurenin icindedir.\n", i+1);
                    }
                }
                fprintf(fp, "\n");
                fclose(fp);

        }else if(type == 2 && strcmp(dataType,"DATA ascii\n") == 0){
            struct typeIki tipIki[noktalar];

            char iki[99];
            fp = fopen(isimList[i].isim, "r");
            char isim[99];
            strcpy(isim, isimList[i].isim);

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }
            while(fgets(iki,99,fp) != 0)
            {
                if(satir >= 5)
                {
                    const char *ayrac = " ";
                    char *deger = strtok(iki, ayrac);
                    int yeniSayac = 0;
                    while (deger!= NULL)
                    {
                        if(yeniSayac == 0 )
                        {
                            tipIki[satir-5].x = atof(deger);
                        }
                        else if(yeniSayac == 1)
                        {
                            tipIki[satir-5].y = atof(deger);
                        }
                        else if(yeniSayac == 2)
                        {
                            tipIki[satir-5].z = atof(deger);
                        }
                        deger = strtok(NULL, ayrac);
                        yeniSayac ++;
                    }
                }
                satir++;
            }
            fclose(fp);

            fp = fopen("output.nkt","a");
            fprintf(fp, "%s\n", isim);
            fclose(fp);
                fp = fopen("output.nkt","a");

                for(int i = 0; i< noktalar ; i++)
                {
                    if (sqrt(pow((tipIki[i].x - kure.x), 2) + pow((tipIki[i].y - kure.y), 2) + pow((tipIki[i].z - kure.z), 2)) <= r)
                    {
                        fprintf(fp,"%d numarali nokta kurenin icindedir.\n", i+1);
                    }
                }
                fprintf(fp, "\n");
                fclose(fp);
        }else if(type == 1 && strcmp(dataType,"DATA binary\n") == 0){

            struct typeBir tipBir[noktalar];
            char uc[99];
            char isim[99];
            int binarySatir = 0;
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "rb");

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }

            for(int i = 0; i < noktalar+5; i++)
            {
                if(i <5)
                {
                    fgets(uc,99,fp);
                }
                else if(i >= 5)
                {
                    fread(&tipBir[i-5], sizeof(struct typeBir), noktalar, fp);
                    binarySatir++;
                }
            }
            fclose(fp);
            fp = fopen("output.nkt","a");
            fprintf(fp, "\n%s\n", isim);
            fclose(fp);

                fp = fopen("output.nkt","a");

                for(int i = 0; i< noktalar ; i++)
                {
                    if (sqrt(pow((tipBir[i].x - kure.x), 2) + pow((tipBir[i].y - kure.y), 2) + pow((tipBir[i].z - kure.z), 2)) <= r)
                    {
                        fprintf(fp,"%d numarali nokta kurenin icindedir.\n", i+1);
                    }
                }
                fprintf(fp, "\n");
                fclose(fp);
        }else if(type == 2 && strcmp(dataType,"DATA binary\n") == 0){

            struct typeIki tipIki[noktalar];
            char dort[99];
            char isim[99];
            int binarySatir = 0;
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "rb");

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }

            for(int i = 0; i < noktalar+5; i++)
            {
                if(i <5)
                {
                    fgets(dort,99,fp);

                }
                else if(i >= 5)
                {
                    fread(&tipIki[i-5], sizeof(struct typeIki), noktalar, fp);
                    binarySatir++;
                }


            }
            fclose(fp);
            fp = fopen("output.nkt","a");
            fprintf(fp, "%s\n", isim);
            fclose(fp);
                fp = fopen("output.nkt","a");

                for(int i = 0; i< noktalar ; i++)
                {
                    if (sqrt(pow((tipIki[i].x - kure.x), 2) + pow((tipIki[i].y - kure.y), 2) + pow((tipIki[i].z - kure.z), 2)) <= r)
                    {
                        fprintf(fp,"%d numarali nokta kurenin icindedir.\n", i+1);
                    }
                }
                fprintf(fp, "\n");
                fclose(fp);
            }
            terminate4:
            printf(" ");
        }
        printf("Sectiginiz islem tamamlandi.\n");
        goto secimEtiketi;
    }else if(secim == 5){

        if(secimBir != 1){
            printf("Hata. Programin duzgun calisabilmesi icin en az bir defa 1. secimin yapilmasi gerekir.\n");
            goto secimEtiketi;
        }

        fp = fopen("output.nkt", "a");
        fprintf(fp, "\nSecim 5\n\n");
        fclose(fp);

        for(int i = 0; i< sayac; i++){
            fp = fopen(isimList[i].isim, "r");
                if(isimList[i].hata != 0){
                    fclose(fp);
                    goto terminate5;
                }

        if (fp == NULL)
        {
            perror("Dosya acilamadi.\n");
            exit(EXIT_FAILURE);
        }
        char ch[100];
        char dataType[20];
        int noktalar = 0;
        int satir = 0;
        int type = 0;
        int vers = 0;


        while(fgets(ch,99,fp) != 0 && satir<5)
        {
            char * ptr = strstr(ch,"NOKTALAR");

            if( ptr != NULL)
            {
                const char *ayrac = " ";
                char *kelime = strtok(ch, ayrac);
                while (kelime != NULL)
                {
                    noktalar = atoi(kelime);
                    kelime = strtok(NULL, ayrac);
                }
            }
            else if(strcmp(ch,"ALANLAR x y z r g b\n") == 0)
            {
                type = 1;
            }
            else if(strcmp(ch,"ALANLAR x y z\n") == 0)
            {
                type = 2;
            }
            else if(strcmp(ch,"DATA ascii\n") == 0)
            {
                strcpy(dataType,ch);
            }
            else if(strcmp(ch,"DATA binary\n") == 0)
            {
                strcpy(dataType,ch);
            }else if(strcmp(ch,"VERSION 1\n") == 0){
                vers = 1;
            }
            satir++;
        }
        fclose(fp);

        satir = 0;

        if(type == 1 && strcmp(dataType,"DATA ascii\n") == 0){
            struct typeBir tipBir[noktalar];

            char bir[99];
            char isim[99];
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "r");

            if (fp == NULL)
            {
                perror("Dosya acilamadi.\n");
                exit(EXIT_FAILURE);
            }
            while(fgets(bir,99,fp) != 0)
            {
                if(satir >= 5)
                {
                    const char *ayrac = " ";
                    char *deger = strtok(bir, ayrac);
                    int yeniSayac = 0;
                    while (deger!= NULL)
                    {
                        if(yeniSayac == 0 )
                        {
                            tipBir[satir-5].x = atof(deger);
                        }
                        else if(yeniSayac == 1)
                        {
                            tipBir[satir-5].y = atof(deger);
                        }
                        else if(yeniSayac == 2)
                        {
                            tipBir[satir-5].z = atof(deger);
                        }
                        else if(yeniSayac == 3)
                        {
                            tipBir[satir-5].r = atoi(deger);
                        }
                        else if(yeniSayac == 4)
                        {
                            tipBir[satir-5].g = atoi(deger);
                        }
                        else if(yeniSayac == 5)
                        {
                            tipBir[satir-5].b = atoi(deger);
                        }
                        deger = strtok(NULL, ayrac);
                        yeniSayac ++;
                    }
                }
                satir++;
            }
            fclose(fp);

            int sayac = 0;
            float ort;
            float toplam = 0;

            fp = fopen("output.nkt","a");
            fprintf(fp, "\n%s\n", isim);
            fclose(fp);

            for(int i = 0; i <noktalar; i++)
                {
                    for(int j = i+1; j<noktalar; j++)
                    {
                        toplam = toplam + sqrt(pow((tipBir[i].x - tipBir[j].x), 2) + pow((tipBir[i].y - tipBir[j].y), 2) + pow((tipBir[i].z - tipBir[j].z), 2));
                        sayac ++;
                    }
                }
                ort = toplam/sayac;
                fp = fopen("output.nkt","a");
                fprintf(fp,"Ortalama uzaklik: %f\n", ort);
                fprintf(fp, "\n\n");
                fclose(fp);

        }else if(type == 2 && strcmp(dataType,"DATA ascii\n") == 0){

            struct typeIki tipIki[noktalar];

            char iki[99];
            fp = fopen(isimList[i].isim, "r");
            char isim[99];
            strcpy(isim, isimList[i].isim);

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }
            while(fgets(iki,99,fp) != 0)
            {
                if(satir >= 5)
                {
                    const char *ayrac = " ";
                    char *deger = strtok(iki, ayrac);
                    int yeniSayac = 0;
                    while (deger!= NULL)
                    {
                        if(yeniSayac == 0 )
                        {
                            tipIki[satir-5].x = atof(deger);
                        }
                        else if(yeniSayac == 1)
                        {
                            tipIki[satir-5].y = atof(deger);
                        }
                        else if(yeniSayac == 2)
                        {
                            tipIki[satir-5].z = atof(deger);
                        }
                        deger = strtok(NULL, ayrac);
                        yeniSayac ++;
                    }
                }
                satir++;
            }
            fclose(fp);
            float ort;
            float toplam = 0;
            int sayac = 0;

            fp = fopen("output.nkt","a");
            fprintf(fp, "%s\n", isim);
            fclose(fp);

                for(int i = 0; i <noktalar; i++)
                {
                    for(int j = i+1; j<noktalar; j++)
                    {
                        toplam = toplam + sqrt(pow((tipIki[i].x - tipIki[j].x), 2) + pow((tipIki[i].y - tipIki[j].y), 2) + pow((tipIki[i].z - tipIki[j].z), 2));
                        sayac ++;
                    }
                }

                fp = fopen("output.nkt","a");
                ort = toplam/sayac;
                fprintf(fp, "Ortalama uzaklik: %f\n", ort);
                fprintf(fp,"\n\n");
                fclose(fp);

        }else if(type == 1 && strcmp(dataType,"DATA binary\n") == 0){

             struct typeBir tipBir[noktalar];
            char uc[99];
            char isim[99];
            int binarySatir = 0;
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "rb");

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }

            for(int i = 0; i < noktalar+5; i++)
            {
                if(i <5)
                {
                    fgets(uc,99,fp);
                }
                else if(i >= 5)
                {
                    fread(&tipBir[i-5], sizeof(struct typeBir), noktalar, fp);
                    binarySatir++;
                }
            }
            fclose(fp);
            float ort;
            float toplam = 0;
            int sayac = 0;

            fp = fopen("output.nkt","a");
            fprintf(fp, "\n%s\n", isim);
            fclose(fp);

                for(int i = 0; i <noktalar; i++)
                {
                    for(int j = i+1; j <noktalar; j++)
                    {
                        toplam = toplam + sqrt(pow((tipBir[i].x - tipBir[j].x), 2) + pow((tipBir[i].y - tipBir[j].y), 2) + pow((tipBir[i].z - tipBir[j].z), 2));
                        sayac ++;
                    }
                }
                fp = fopen("output.nkt","a");
                ort = toplam/sayac;
                fprintf(fp,"Ortalama uzaklik: %f\n", ort);
                fprintf(fp, "\n\n");
                fclose(fp);

        }else if(type == 2 && strcmp(dataType,"DATA binary\n") == 0){

            struct typeIki tipIki[noktalar];
            char dort[99];
            char isim[99];
            int binarySatir = 0;
            strcpy(isim, isimList[i].isim);

            fp = fopen(isimList[i].isim, "rb");

            if (fp == NULL)
            {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            }

            for(int i = 0; i < noktalar+5; i++)
            {
                if(i <5)
                {
                    fgets(dort,99,fp);

                }
                else if(i >= 5)
                {
                    fread(&tipIki[i-5], sizeof(struct typeIki), noktalar, fp);
                    binarySatir++;
                }


            }
            fclose(fp);

            float ort;
            float toplam = 0;
            int sayac = 0;

            fp = fopen("output.nkt","a");
            fprintf(fp, "%s\n", isim);
            fclose(fp);

                for(int i = 0; i <noktalar; i++)
                {
                    for(int j = i+1; j<noktalar; j++)
                    {
                        toplam = toplam + sqrt(pow((tipIki[i].x - tipIki[j].x), 2) + pow((tipIki[i].y - tipIki[j].y), 2) + pow((tipIki[i].z - tipIki[j].z), 2));
                        sayac ++;
                    }
                }
                fp = fopen("output.nkt","a");
                ort = toplam/sayac;
                fprintf(fp,"Ortalama uzaklik: %f\n", ort);
                fprintf(fp,"\n\n");
                fclose(fp);
            }
            terminate5:
            printf(" ");
        }
        printf("Sectiginiz islem tamamlanmistir.\n");
        goto secimEtiketi;
    }else{
        printf("Programdan cikiliyor.\n");
    }
    return 0;
}
