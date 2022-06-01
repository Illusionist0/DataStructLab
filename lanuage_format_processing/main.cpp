#include <stdio.h>
#include "gettoken.h"
#include <stdlib.h>
#include "word_analyse.h"
#include "syntax_analyse.h"
#include "format.h"

char *filename;
FILE* fp;

int main(int argc,char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s <filename>", argv[0]);
        exit(-1);
    }
    filename=argv[1];
    int op;
    while (1) {
        system("cls");
        printf(" ��������������������������������������������\n");
        printf("     ���������ѡ����    \n");
        printf("     1.�ʷ�����          \n");
        printf("     2.�﷨����          \n");
        printf("     3.��ʽ��            \n");
        printf("     0.�˳�              \n");
        printf(" ��������������������������������������������\n");
        scanf("%d",&op);
        switch (op){
            case 1:
                fp=fopen(filename,"r");
                word_analyse();
                printf("�����������\n");
                fclose(fp);
                getchar();
                getchar();
                break;
            case 2:
                fp=fopen(filename,"r");
                syntax_analyse();
                printf("�����������\n");
                fclose(fp);
                getchar();
                getchar();
                break;
            case 3:
                fp=fopen(filename,"r");
                format();
                printf("�����������\n");
                fclose(fp);
                getchar();
                getchar();
                break;
            case 0:
                system("clear");
                printf("��лʹ��\n");
                return 0;
            default:
                printf("������������������\n");
                printf("�����������\n");
                getchar();
                getchar();
                break;
        }
    }
}
