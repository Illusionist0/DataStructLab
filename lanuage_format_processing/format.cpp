#include "format.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gettoken.h"

extern FILE* fp;
extern char token_text[20];//���������ֵ

void format(){
    int tabs=0;//��������
    int i;
    int flag=0;//���ڱ���޴����ŵ���Ҫ���������
    token* root,*tail,*p;
    while (1) {
        root=readline();
        if(root==NULL){
            break;
        }
        tail=root;
        while (tail->next!=NULL){
            tail=tail->next;
        }
        if((root==tail)&&(strcmp(tail->str,"}")==0)){
            tabs--;
        }
        if((root!=tail)&&(strcmp(root->str,"}")==0)){
            tabs--;
        }
        for(i=0;i<tabs;i++){//�������
            printf("\t");
        }
        if (flag==1){
            tabs--;
            flag=0;
        }
        p=root;
        while (p!=NULL){
            printf("%s ",p->str);
            p=p->next;
        }
        printf("\n");
        if(strcmp(tail->str,"{")==0){
            tabs++;
        }
        p=root;
        while (p!=NULL){
            if((strcmp(p->str,"if")==0)||(strcmp(p->str,"for")==0)||(strcmp(p->str,"while")==0)){
                if(strcmp(tail->str,"{")!=0&&strcmp(tail->str,";")!=0){
                    tabs++;
                    flag=1;
                }
            }
            p=p->next;
        }
    }
}

token* readline(){
    if (feof(fp))   return NULL;//�ļ���ȡβ
    char c;
    int w;
    token* root=NULL;
    token *tail=root;//��ɳ�ʼ��
    w=gettoken(fp);
    if(w==-1){
        return NULL;
    }
    root=(token*)malloc(sizeof(token));
    char *token_text1=(char*)malloc(25* sizeof(char));
    strcpy(token_text1,token_text);
    root->str=token_text1;
    root->next=NULL;
    tail=root;
    /*ÿʶ����һ�����ʾ��жϵ��ʺ����ǲ��ǻ��з�*/
    while ((c=fgetc(fp))!='\n'){
        ungetc(c,fp);
        gettoken(fp);
        tail->next=(token*)malloc(sizeof(token));
        tail=tail->next;
        token_text1=(char*)malloc(25* sizeof(char));
        strcpy(token_text1,token_text);
        tail->str=token_text1;
        tail->next=NULL;
    }
    return root;
}

