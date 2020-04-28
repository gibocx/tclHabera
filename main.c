#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int replaceInFile(FILE *fhSrc, FILE *fhTrg, char text_to_replace[])
{
    char buffer[256];
    char *pos;

    while (fgets(buffer, sizeof(buffer), fhSrc) != NULL)
    {
        pos = strstr(buffer, "DE0_CV_$name");
        if (pos != NULL)
        {
            char * temp = calloc(strlen(buffer) - strlen("DE0_CV_$name") + strlen(text_to_replace) + 1, 1);
            memcpy(temp, buffer, pos - buffer);
            memcpy(temp + (pos - buffer), text_to_replace, strlen(text_to_replace));
            memcpy(temp + (pos - buffer) + strlen(text_to_replace),
                        pos + strlen("DE0_CV_$name"),
                        1 + strlen(buffer) - ((pos - buffer) + strlen("DE0_CV_$name")));
            fputs(temp, fhTrg);
            free(temp);
        }
        else
        {
            fputs(buffer, fhTrg);
        }
    }
    return 1;
}


int main()
{
    FILE *fhSrc;
    FILE *fhTrg;
    FILE *fhTemp;
    FILE *fhNew;

    char Ordner[64];
    char FILENAME[64];

    printf("TCL Habera by gibocx 2020 V0.0\n");

        system("dir /b /ad >> temp");
        fhTemp = fopen("temp","r");
        if(fhTemp == NULL)
        {
            printf("Temp-Datei konnte nicht geöffnet werden!\nEnter-Taste druecken zum Beenden!");
            getchar();
        }
        else
        {
            while(fscanf(fhTemp, "\n%[^\n]", Ordner) && !feof(fhTemp))
            {
                printf("\n%16s:",Ordner);

                strcpy(FILENAME,"./");
                strcat(FILENAME,Ordner);
                strcat(FILENAME,"/");
                strcat(FILENAME,Ordner);
                strcat(FILENAME,".tcl");
                fhNew = fopen(FILENAME,"r");

                if(fhNew == NULL)
                {
                    fhTrg = fopen(FILENAME,"w+");
                    if(fhTrg == NULL)
                    {
                        printf("\033[0;31m\nKonnte %16s.tcl nicht erstellen!\033[0m",FILENAME);break;
                    }

                    fhSrc = fopen("DE0_CV_$name.tcl", "r");
                    if (fhSrc == NULL)
                    {
                        printf("DE0_CV_$name.tcl wurde nicht gefunden!\nDie Datei muss im gleichem Ordner wie das Programm liegen!\n\nEnter-Taste druecken zum beenden!");
                        getchar();
                        return -1;
                    }
                    replaceInFile(fhSrc, fhTrg, Ordner);
                    fclose(fhTrg);
                    fclose(fhSrc);
                    printf("\t \033[1;32m%16s.tcl erstellt\033[0m",Ordner);
                }
                else
                {
                    printf("\t \033[0;32m%16s.tcl vorhanden\033[0m", Ordner);
                }
                fclose(fhNew);

            }
        }
        fclose(fhTemp);
        remove("temp");

    printf("\n\nEnter zum beenden");
    getchar();
    return 0;
}
