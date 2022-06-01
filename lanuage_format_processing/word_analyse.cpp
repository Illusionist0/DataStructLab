#include <stdio.h>
#include <stdlib.h>
#include "word_analyse.h"
#include "gettoken.h"

extern char token_text[20];//��ŵ�������ֵ
extern int cnt_lines;
extern FILE* fp;

int word_analyse() {
    int kind;
    if(fp==NULL){
        printf("�ļ���ʧ��\n");
    } else{
        printf("�ļ��򿪳ɹ�\n");
    }
    printf("\n");
    printf("  �������");
    printf("\t����ֵ\n");
    do {
        kind=gettoken(fp);
        if(kind!=ERROR_TOKEN) {
            switch (kind) {
                case IDENT:
                    printf("   ��ʶ��");
                    break;
                case INT_CONST:
                    printf("  ���ͳ���");
                    break;
                case FLOAT_CONST:
                    printf("�����ͳ���");
                    break;
                case CHAR_CONST:
                    printf("  �ַ�����");
                    break;
                case STRING_CONST:
                    printf("�ַ�������");
                    break;
                case KEYWORD:
                    printf("   �ؼ���");
                    break;
                case INT:
                    printf("���͹ؼ���");
                    break;
                case DOUBLE:
                    printf("���͹ؼ���");
                    break;
                case FLOAT:
                    printf("���͹ؼ���");
                    break;
                case CHAR:
                    printf("���͹ؼ���");
                    break;
                case SHORT:
                    printf("���͹ؼ���");
                    break;
                case LONG:
                    printf("���͹ؼ���");
                    break;
                case IF:
                    printf("   �ؼ���");
                    break;
                case ELSE:
                    printf("   �ؼ���");
                    break;
                case DO:
                    printf("   �ؼ���");
                    break;
                case WHILE:
                    printf("   �ؼ���");
                    break;
                case FOR:
                    printf("   �ؼ���");
                    break;
                case STRUCT:
                    printf("   �ؼ���");
                    break;
                case BREAK:
                    printf("   �ؼ���");
                    break;
                case SWITCH:
                    printf("   �ؼ���");
                    break;
                case CASE:
                    printf("   �ؼ���");
                    break;
                case TYPEDEF:
                    printf("   �ؼ���");
                    break;
                case RETURN:
                    printf("   �ؼ���");
                    break;
                case CONTINUE:
                    printf("   �ؼ���");
                    break;
                case VOID:
                    printf("   �ؼ���");
                    break;
                case EQ:
                    printf("   ���ں�");
                    break;
                case NEQ:
                    printf("  �����ں�");
                    break;
                case ASSIGN:
                    printf("   ��ֵ��");
                    break;
                case LP:
                    printf("   ������");
                    break;
                case RP:
                    printf("   ������");
                    break;
                case LB:
                    printf("  �������");
                    break;
                case RB:
                    printf("  �Ҵ�����");
                    break;
                case LM:
                    printf("  ��������");
                    break;
                case RM:
                    printf("  ��������");
                    break;
                case SEMI:
                    printf("     �ֺ�");
                    break;
                case LOGIC:
                    printf("    �߼���");
                    break;
                case COMMA:
                    printf("     ����");
                    break;
                case TIMES:
                    printf("     �˺�");
                    break;
                case DIVIDE:
                    printf("     ����");
                    break;
                case ANNO:
                    printf("     ע��");
                    break;
                case PLUS:
                    printf("     �ӷ�");
                    break;
                case PLUSPLUS:
                    printf("  ��������");
                    break;
                case MINUS:
                    printf("     ����");
                    break;
                case MINUSMINUS:
                    printf("  �Լ�����");
                    break;
                case MORE:
                    printf("   ���ں�");
                    break;
                case MOREEQUAL:
                    printf("���ڵ��ں�");
                    break;
                case LESS:
                    printf("   С�ں�");
                    break;
                case LESSEQUAL:
                    printf("С�ڵ��ں�");
                    break;
                case INCLUDE:
                    printf("ͷ�ļ�����");
                    break;
                case MACRO:
                    printf("   �궨��");
                    break;
                case ARRAY:
                    printf("     ����");
                    break;
            }
            printf("\t%s\n", token_text);
        } else{
            printf("\t��%d�г��ִ���\n",cnt_lines);
            break;
        }
        if (feof(fp))   break;
    } while (kind!=-1);

    return 0;
}
