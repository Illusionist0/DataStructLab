#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stack>
#include "syntax_analyse.h"
#include "gettoken.h"

using namespace std;

extern char token_text[20];//���������ֵ
extern char string_num[20];
extern int cnt_lines;
int w,type;//ȫ�ֱ�������ŵ�ǰ����ĵ����������
int mistake=0;//ȫ�ֱ��������κ�һ��δ֪�����ᵼ����ֵ��Ϊ1
extern FILE* fp;
VDN* Vroot;//������������ڵ�
int isVoid,hasReturn,isInRecycle=0;


void syntax_analyse(){
    ASTTree* root=program();
    if(root==NULL||mistake==1){
        printf("�����﷨����\n");
        return;
    } else{
        printf("������ȷ\n\n");
        /*�����﷨��*/
        /*�ݹ��������*/
        PreorderTranverse(root,0);
    }
}

ASTTree* program(){
    w=gettoken(fp);
    while(w==ANNO||w==INCLUDE){
        w=gettoken(fp);
    }
    Vroot=(VDN*)malloc(sizeof(VDN));
    Vroot->size=0;
    Vroot->next=NULL;
    ASTTree *p=ExtDefList();
    if(p!=NULL){
        if(isVoid==0&&hasReturn==0){
            printf("���󣺺���ȱ�ٷ���ֵ\n");
            exit(0);
        }
        /*�����﷨��ȷ�������﷨�����ڵ�ָ�룬�ɱ�����ʾ*/
        ASTTree* root=p;//�ⲿ�������н��
        root->type =EXTDEFLIST;
        return root;
    } else{
        //���﷨����
        mistake=1;
        return NULL;
    }
}

/*�ͷ�rootΪ����ȫ�����*/
void freeTree(ASTTree* root){
    if(root){
        freeTree(root->l);
        freeTree(root->r);
        free(root);
    }
}

ASTTree* ExtDefList(){//�����ⲿ��������
    if (feof(fp))   return NULL;
    if(mistake==1){
        return NULL;
    }
    if(w==-1){
        return NULL;
    }
    ASTTree* root=(ASTTree*)malloc(sizeof(ASTTree));//�ⲿ�������н��
    root->data.data=NULL;
    root->type=EXTDEFLIST;
    root->l=ExtDef();//�ⲿ�����ʵ������
    w=gettoken(fp);
    while(w==ANNO||w==INCLUDE){
        w=gettoken(fp);
    }//ѭ��ֱ����ȡ�����е�ͷ�ļ���Ԥ����
    root->r=ExtDefList();//��ȡ�����е��ⲿ��������
    return root;
}

/*�﷨��λ<�ⲿ����>�ӳ���*/
//����ǰ�Ѿ�������һ������
ASTTree* ExtDef(){
    if (feof(fp))   return NULL;
    int a;
    if(mistake==1){
        return NULL;
    }
    if(w!=INT&&w!=DOUBLE&&w!=CHAR&&w!=LONG&&w!=SHORT&&w!=FLOAT&&w!=VOID){
        printf("��%d�г��ִ���\n",cnt_lines);//%
        printf("�����ⲿ������ִ���\n");
        exit(0);
    }
    type=w;//��������˵����
    w=gettoken(fp);
    while(w==ANNO||w==INCLUDE){
        w=gettoken(fp);
    }
    if(w!=IDENT&&w!=ARRAY){
        printf("��%d�г��ִ���\n",cnt_lines);
        printf("�����ⲿ������ִ���\n");
        exit(0);
    }
    a=w;
    char token_text0[20];
    strcpy(token_text0,token_text);//�����һ�����������ߺ�������token_text0
    ASTTree* p;
    w=gettoken(fp);
    while(w==ANNO||w==INCLUDE){
        w=gettoken(fp);
    }
    strcpy(token_text,token_text0);
    if(w==LP){
        p=FuncDef();
    }else if(a==ARRAY){//���鶨�壬������������������˷ֺ�
        p=ArrayDef();
    } else{
        p=ExtVarDef();
    }
    return p;
}

/*�������鶨����ӳ���*/
ASTTree* ArrayDef(){
    //��ʱtoken_text�а�����������������
    if(type==VOID){
        //�������Ͳ�����void
        printf("��%d�г��ִ���\n",cnt_lines);
        printf("�����������Ͳ�����void\n");
        exit(0);
    }
    ASTTree* root=(ASTTree*)malloc(sizeof(ASTTree));
    root->type=ARRAYDEF;
    root->l=NULL;
    root->r=NULL;
    root->data.data=NULL;
    ASTTree* p=(ASTTree*)malloc(sizeof(ASTTree));
    p->type=ARRAYTYPE;
    p->l=NULL;
    p->r=NULL;
    if(type==INT){
        p->data.data="int";
    }if(type==DOUBLE){
        p->data.data="double";
    }if(type==CHAR){
        p->data.data="char";
    }if(type==FLOAT){
        p->data.data="float";
    }if(type==LONG){
        p->data.data="long";
    }if(type==SHORT){
        p->data.data="short";
    }
    root->l=p;
    p=(ASTTree*)malloc(sizeof(ASTTree));
    p->type=ARRAYNAME;
    p->l=NULL;
    p->r=NULL;
    char* token_text3=(char*)malloc(20*sizeof(char));
    strcpy(token_text3,token_text);
    p->data.data=token_text3;
    root->r=p;
    ASTTree* q=(ASTTree*)malloc(sizeof(ASTTree));
    q->type=ARRAYSIZE;
    q->l=NULL;
    q->r=NULL;
    q->data.data=string_num;
    p->l=q;
    return root;
}//�˳�ʱ���ܶ������


/*�����ⲿ�����ӳ���*/
ASTTree* ExtVarDef(){
    if(mistake==1){
        return NULL;
    }
    int cnt;
    if(type==VOID){
        //�ⲿ�������Ͳ�����void
        printf("��%d�г��ִ���\n",cnt_lines);
        printf("�����ⲿ�������Ͳ�����void\n");
        exit(0);
    }

    int u;
    u=addname(token_text);
    if(u==1){
        //�������ظ�����
        mistake=1;
        return NULL;
    }

    ASTTree* root=(ASTTree*)malloc(sizeof(ASTTree));//�����ⲿ��������Ľ��
    root->l=NULL;
    root->r=NULL;
    root->data.data=NULL;
    root->type=EXTVARDEF;
    ASTTree* p=(ASTTree*)malloc(sizeof(ASTTree));//�����ⲿ�������ͽ��
    p->l=NULL;
    p->r=NULL;
    p->data.data=NULL;
    p->type=EXTVARTYPE;
    p->data.type=type;
    if(type==INT){
        p->data.data="int";
    }if(type==DOUBLE){
        p->data.data="double";
    }if(type==CHAR){
        p->data.data="char";
    }if(type==FLOAT){
        p->data.data="float";
    }if(type==LONG){
        p->data.data="long";
    }if(type==SHORT){
        p->data.data="short";
    }
    root->l=p;
    p=(ASTTree*)malloc(sizeof(ASTTree));//�����ⲿ���������н��
    p->l=NULL;
    p->r=NULL;
    p->data.data=NULL;
    p->type=EXTVARLIST;
    root->r=p;
    p->l=(ASTTree*)malloc(sizeof(ASTTree));//�����ⲿ���������
    p->l->l=NULL;
    p->l->r=NULL;
    p->data.data=NULL;
    p->l->type=EXTVAR;
    char *token_text1=(char*)malloc(25* sizeof(char));//@
    strcpy(token_text1,token_text);
    p->l->data.data=token_text1;
    while(1){
        if(w!=COMMA&&w!=SEMI){
            if(cnt_lines>cnt){
                cnt_lines--;
            }
            printf("��%d�г��ִ���\n",cnt_lines);//%
            printf("�����ⲿ�������崦���ִ���\n");
            exit(0);
        }
        if(w==SEMI){
            return root;
        }
        w=gettoken(fp);
        if(w!=IDENT){
            printf("��%d�г��ִ���\n",cnt_lines);//%
            printf("�����ⲿ�������崦���ִ���\n");
            exit(0);
        }

        int u;
        u=addname(token_text);
        if(u==1){
            //�������ظ�����
            mistake=1;
            return NULL;
        }

        ASTTree* q=(ASTTree*)malloc(sizeof(ASTTree));//�����ⲿ���������н��
        q->l=NULL;
        q->r=NULL;
        q->data.data=NULL;
        q->type=EXTVARLIST;
        p->r=q;
        p=q;
        p->l=(ASTTree*)malloc(sizeof(ASTTree));//�����ⲿ���������
        p->l->l=NULL;
        p->l->r=NULL;
        p->l->data.data=NULL;
        p->l->type=EXTVAR;
        char *token_text1=(char*)malloc(25* sizeof(char));
        strcpy(token_text1,token_text);
        p->l->data.data=token_text1;
        cnt=cnt_lines;
        w=gettoken(fp);
        while(w==ANNO||w==INCLUDE){
            w=gettoken(fp);
        }
    }
}//�˳�����ʱû�ж��ȡ����

/*�����ⲿ�����ӳ���*/
ASTTree* FuncDef(){
    if(mistake==1){
        return NULL;
    }
    ASTTree* root=(ASTTree*)malloc(sizeof(ASTTree));// ���ɺ���������
    root->data.data=NULL;
    root->type=FUNCDEF;
    ASTTree* p=(ASTTree*)malloc(sizeof(ASTTree));//���ɷ���ֵ���ͽ��
    p->data.data=NULL;
    p->type=FUNCRETURNTYPE;
    p->data.type=type;
    if(type==INT){
        p->data.data="int";
        isVoid=0;
    }if(type==DOUBLE){
        p->data.data="double";
        isVoid=0;
    }if(type==CHAR){
        p->data.data="char";
        isVoid=0;
    }if(type==FLOAT){
        p->data.data="float";
        isVoid=0;
    }if(type==LONG){
        p->data.data="long";
        isVoid=0;
    }if(type==SHORT){
        p->data.data="short";
        isVoid=0;
    }if(type==VOID){
        p->data.data="void";
        isVoid=1;
    }
    p->l=NULL;
    p->r=NULL;
    root->l=p;
    /*�������*/
    ASTTree* q=(ASTTree*)malloc(sizeof(ASTTree));
    q->data.data=NULL;
    q->type=FUNCNAME;
    char *token_text1=(char*)malloc(25* sizeof(char));
    strcpy(token_text1,token_text);
    q->data.data=token_text1;

    //�ڱ����������������µĽ��$
    VDN* last=Vroot;
    while (last->next!=NULL){
        last=last->next;
    }
    last->next=(VDN*)malloc(sizeof(VDN));
    last=last->next;
    last->next=NULL;
    last->size=0;

    root->r=q;
    q->l=FormParaList();
    /*�ж��Ǻ��ֺ�������*/
    w=gettoken(fp);
    while(w==ANNO||w==INCLUDE){
        w=gettoken(fp);
    }
    if(w==SEMI){
        /*����ԭ������*/
        root->r->r=NULL;//��������Ϊ��
        root->type=FUNCCLAIM;
    }
    else if(w==LB){
        //�����壨������䣩�ӳ���
        q->r=CompState();
        q->r->type=FUNCBODY;
    } else{
        printf("��%d�г��ִ���\n",cnt_lines);
        printf("���󣺺������崦����\n");
        exit(0);
    }
    return root;
}

/*������ʽ�����ӳ���*/
ASTTree* FormParaList(){
    if(mistake==1){
        return NULL;
    }
    //��һ�ν���˺���֮ǰ���Ѿ�ʶ����������
    w=gettoken(fp);
    while(w==ANNO||w==INCLUDE){
        w=gettoken(fp);
    }
    if(w==RP){
        return NULL;
    }
    if(w==COMMA){
        w=gettoken(fp);
        while(w==ANNO||w==INCLUDE){
            w=gettoken(fp);
        }
    }
    ASTTree* root=(ASTTree*)malloc(sizeof(ASTTree));//������ʽ�������н��
    root->data.data=NULL;
    root->type=FUNCFORMALPARALIST;
    root->l=FormParaDef();
    root->r=FormParaList();
    return root;
}

ASTTree* FormParaDef(){
    if(mistake==1){
        return NULL;
    }
    if(w!=INT&&w!=DOUBLE&&w!=CHAR&&w!=LONG&&w!=SHORT&&w!=FLOAT){
        printf("��%d�г��ִ���\n",cnt_lines);
        printf("�����βζ��崦���ִ���\n");
        exit(0);
    }
    type=w;//��������˵����
    w=gettoken(fp);
    while(w==ANNO||w==INCLUDE){
        w=gettoken(fp);
    }
    if(w!=IDENT){
        printf("��%d�г��ִ���\n",cnt_lines);
        printf("�����βζ��崦���ִ���\n");
        exit(0);
    }
    ASTTree* root=(ASTTree*)malloc(sizeof(ASTTree));//������ʽ��������Ľ��
    root->data.data=NULL;
    root->type=FUNCFORMALPARADEF;
    ASTTree* p=(ASTTree*)malloc(sizeof(ASTTree));//�����β����ͽ��
    p->data.data=NULL;
    p->type=FUNCFORMALPARATYPE;
    p->data.type=type;
    if(type==INT){
        p->data.data="int";
    }if(type==DOUBLE){
        p->data.data="double";
    }if(type==CHAR){
        p->data.data="char";
    }if(type==FLOAT){
        p->data.data="float";
    }if(type==LONG){
        p->data.data="long";
    }if(type==SHORT){
        p->data.data="short";
    }if(type==VOID){
        p->data.data="void";
    }
    p->l=NULL;
    p->r=NULL;
    root->l=p;
    p=(ASTTree*)malloc(sizeof(ASTTree));//�����β������
    p->data.data=NULL;
    p->type=FUNCFORMALPARA;

    int u;
    u=addname(token_text);
    if(u==1){
        //�������ظ�����
        mistake=1;
        return NULL;
    }

    char *token_text1=(char*)malloc(25* sizeof(char));
    strcpy(token_text1,token_text);
    p->data.data=token_text1;//��¼�β���
    p->l=NULL;
    p->r=NULL;
    root->r=p;
    return root;
}

/*��������ӳ���*/
ASTTree* CompState(){
    if(mistake==1){
        return NULL;
    }
    /*���ô��ӳ���ʱ���Ѿ������˵���{����������ʱ���������}�������������*/
    ASTTree* root=(ASTTree*)malloc(sizeof(ASTTree));//���ɸ��������
    root->data.data=NULL;
    root->l=NULL;
    root->r=NULL;
    w=gettoken(fp);
    while(w==ANNO||w==INCLUDE){
        w=gettoken(fp);
    }
    if(w==INT||w==DOUBLE||w==CHAR||w==LONG||w==SHORT||w==FLOAT){
        root->l=LocalVarDefList();
    } else{
        /*�޾ֲ���������*/
        root->l=NULL;
    }
    /*���ô�����������ӳ���*/
    root->r=StateList();
    if(w==RB){
        //�����Ҵ����Ž���
        return root;
    } else{
        printf("���󣺴�����������\n");
        exit(0);
        mistake=1;
        freeTree(root);
        return NULL;
    }

}

/*�ֲ����������ӳ���*/
ASTTree* LocalVarDefList(){
    if(mistake==1){
        return NULL;
    }
    /*����ȡһ�У����ֺŽ���*/
    ASTTree* root=(ASTTree*)malloc(sizeof(ASTTree));//�ֲ������������н��
    root->data.data=NULL;
    root->type=LOCALVARDEFLIST;
    root->l=NULL;
    root->r=NULL;
    ASTTree* p=(ASTTree*)malloc(sizeof(ASTTree));//�ֲ�����������
    p->data.data=NULL;
    p->type=LOCALVARDEF;
    p->l=NULL;
    p->r=NULL;
    root->l=p;
    p->l=(ASTTree*)malloc(sizeof(ASTTree));//�ֲ��������ͽ��
    p->l->data.data=NULL;
    p->l->type=LOCALVARTYPE;
    char *token_text1=(char*)malloc(25* sizeof(char));
    strcpy(token_text1,token_text);
    p->l->data.data=token_text1;
    p->l->l=NULL;
    p->l->r=NULL;
    w=gettoken(fp);
    while(w==ANNO||w==INCLUDE){
        w=gettoken(fp);
    }
    ASTTree* q=(ASTTree*)malloc(sizeof(ASTTree));//���������н��
    q->data.data=NULL;
    q->l=NULL;
    q->r=NULL;
    p->r=q;
    q->type=LOCALVARNAMELIST;
    q->l=(ASTTree*)malloc(sizeof(ASTTree));//�ֲ����������
    q->l->data.data=NULL;
    q->l->type=LOCALVARNAME;
    char *token_text2=(char*)malloc(25* sizeof(char));
    strcpy(token_text2,token_text);
    q->l->data.data=token_text2;
    q->l->l=NULL;
    q->l->r=NULL;

    int u;
    u=addname(token_text);
    if(u==1){
        //�������ظ�����
        mistake=1;
        return NULL;
    }

    while (1){
        w=gettoken(fp);
        while(w==ANNO||w==INCLUDE){
            w=gettoken(fp);
        }
        if(w==SEMI){
            /*�ֲ������������*/
            q->r=NULL;
            w=gettoken(fp);//���һ��
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            break;
        }
        else if(w==COMMA){
            w=gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            ASTTree* s=(ASTTree*)malloc(sizeof(ASTTree));//���������н��
            s->data.data=NULL;
            q->r=s;
            q=q->r;
            q->type=LOCALVARNAMELIST;
            q->l=(ASTTree*)malloc(sizeof(ASTTree));//�ֲ����������
            q->l->data.data=NULL;
            q->l->type=LOCALVARNAME;
            char *token_text1=(char*)malloc(25* sizeof(char));
            strcpy(token_text1,token_text);
            q->l->data.data=token_text1;

            int u;
            u=addname(token_text);
            if(u==1){
                //�������ظ�����
                freeTree(root);
                mistake=1;
                return NULL;
            }
        } else{
            printf("��%d�г��ִ���\n",cnt_lines);//%
            printf("���󣺾ֲ������������\n");
            exit(0);
        }
    }
    /*�ֲ�����������ܲ�ֻһ��*/
    if(w==INT||w==DOUBLE||w==CHAR||w==LONG||w==SHORT||w==FLOAT){
        root->r=LocalVarDefList();
    } else{
        root->r=NULL;
    }
    root->r=NULL;
    return root;
    //�˺�������ʱ�Ƕ����һ���ʵ�
}

/*<�������>�ӳ���*/
/*����ǰ�Ѿ�����������еĵ�һ������*/
ASTTree* StateList(){
    if(mistake==1){
        return NULL;
    }
    ASTTree* root=NULL;
    ASTTree* r1=Statement();
    if(r1==NULL){
        //��������ѽ������߳��ִ���
        return NULL;
    } else{
        root=(ASTTree*)malloc(sizeof(ASTTree));
        root->data.data=NULL;
        root->type=STATELIST;
        root->l=r1;
        root->r=NULL;
        w=gettoken(fp);
        while(w==ANNO||w==INCLUDE){
            w=gettoken(fp);
        }
        if(w!=RB){//����û����
            root->r=StateList();
            return root;
        } else{//���������
            return root;
        }

    }
    //�������ʱ�����ȡ��һ������
}

/*<����һ�����>�ӳ���*/
ASTTree* Statement(){
    if(mistake==1){
        return NULL;
    }
    //���ô˳���ʱ�����ĵ�һ�������Ѿ�����
    ASTTree* root=(ASTTree*)malloc(sizeof(ASTTree));
    root->l=NULL;
    root->r=NULL;
    root->data.data=NULL;
    switch (w){
        case IF: {//�����������
            w = gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if (w != LP) {
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����if������\n");
                exit(0);
            }
            //������ʽ���ӳ���
            w = gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            ASTTree *p1 = (ASTTree *) malloc(sizeof(ASTTree));
            p1->data.data=NULL;
            p1->type = IFPART;
            p1->l = Expression(RP);//����һ����ʽ
            if (p1->l == NULL) {
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����if����������ֳ���\n");
                exit(0);
            }
            w = gettoken(fp);//�����������
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if (w == LB) {
                /*if���ֵ�������д�������ס�ģ������ж�����䣬���Ҵ����Ž���*/
                w = gettoken(fp);
                while(w==ANNO||w==INCLUDE){
                    w=gettoken(fp);
                }
                p1->r = StateList();
            } else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == IDENT||w==KEYWORD||w==IF||w==WHILE||w==ELSE||w==FOR||w==DO) {
                /*û�д����ţ��˴�ֻ����һ�����*/
                p1->r = Statement();
                p1->r->r=NULL;
            } else {
                printf("����if��������\n");
                mistake=1;
                return NULL;
            }
            root->l = p1;//if���ִ������
            w = gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if (w == ELSE) {
                root->type = IFELSESTATEMENT;
                ASTTree *p2 = (ASTTree *) malloc(sizeof(ASTTree));
                p2->data.data=NULL;
                p2->type = ELSEPART;
                root->r = p2;
                w = gettoken(fp);
                while(w==ANNO||w==INCLUDE){
                    w=gettoken(fp);
                }
                if (w == LB) {
                    /*else���ֵ�������д�������ס�ģ������ж�����䣬���Ҵ����Ž���*/
                    w = gettoken(fp);
                    while(w==ANNO||w==INCLUDE){
                        w=gettoken(fp);
                    }
                    p2->r = StateList();
                } else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == IDENT||w==KEYWORD) {
                    /*û�д����ţ��˴�ֻ����һ�����*/
                    p2->r = Statement();
                    p2->r->r=NULL;
                } else if(w==IF) {
                    p2->l=Statement();
                }else{
                    printf("����else�Ӿ����\n");
                    mistake=1;
                    return NULL;
                }
            } else{
                root->type = IFSTATEMENT;
                returntoken(w,fp);

            }
            return root;
        }
        case WHILE:{
            isInRecycle=1;
            w = gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if (w != LP) {
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����while������\n");
                exit(0);
                mistake=1;
                return NULL;
            }
            //������ʽ���ӳ���
            w = gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            ASTTree *p1 = (ASTTree *) malloc(sizeof(ASTTree));
            p1->data.data=NULL;
            p1->type = WHILEPART;//while�������
            p1->l=NULL;
            p1->r=NULL;
            p1->l = Expression(RP);//����һ����ʽ while�������
            if (p1->l == NULL) {
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����while����������ֳ���\n");
                exit(0);
                mistake=1;
                return NULL;
            }//while�������ִ������
            ASTTree* p2=(ASTTree*)malloc(sizeof(ASTTree));
            p2->data.data=NULL;
            p2->type=WHILEBODY;
            p2->l=NULL;
            p2->r=NULL;
            w = gettoken(fp);//�����������
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if (w == LB) {
                /*while���ֵ�������д�������ס�ģ������ж�����䣬���Ҵ����Ž���*/
                w = gettoken(fp);
                while(w==ANNO||w==INCLUDE){
                    w=gettoken(fp);
                }
                p2->r = StateList();
            } else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == IDENT||w==KEYWORD) {
                /*û�д����ţ��˴�ֻ����һ�����*/
                p2->r = Statement();
                p2->r->r=NULL;
            } else {
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����while������\n");
                exit(0);
                mistake=1;
                return NULL;
            }
            root->type=WHILESTATEMENT;
            root->l=p1;
            root->r=p2;
            isInRecycle=0;
            return root;
        }
        case FOR:{
            isInRecycle=1;
            w = gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if (w != LP) {
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����for������\n");
                exit(0);
                mistake=1;
                return NULL;
            }
            w = gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            ASTTree *p1 = (ASTTree *) malloc(sizeof(ASTTree));
            p1->data.data=NULL;
            p1->type = FORPART;//for�������
            ASTTree* q=(ASTTree*)malloc(sizeof(ASTTree));//����һ
            p1->l=q;
            q->type=FORPART1;
            q->data.data=NULL;
            q->l=Expression(SEMI);
            if(q->l==NULL){
                q->data.data="��";
            }
            w=gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            q->r=(ASTTree*)malloc(sizeof(ASTTree));//for���������
            q=q->r;
            q->type=FORPART2;
            q->data.data=NULL;
            q->l=Expression(SEMI);
            if(q->l==NULL){
                q->data.data="��";
            }
            w=gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            q->r=(ASTTree*)malloc(sizeof(ASTTree));//for���������
            q=q->r;
            q->r=NULL;
            q->type=FORPART3;
            q->data.data=NULL;
            q->l=Expression(RP);
            if(q->l==NULL){
                q->data.data="��";
            }
            //for����������ִ������
            ASTTree* p2=(ASTTree*)malloc(sizeof(ASTTree));//for�������
            p2->l=NULL;
            p2->r=NULL;
            p2->type=FORBODY;
            p2->data.data=NULL;
            w = gettoken(fp);//�����������
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if (w == LB) {
                /*for��������д�������ס�ģ������ж�����䣬���Ҵ����Ž���*/
                w = gettoken(fp);
                while(w==ANNO||w==INCLUDE){
                    w=gettoken(fp);
                }
                p2->r = StateList();
            } else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST || w == IDENT||w==KEYWORD) {
                /*û�д����ţ��˴�ֻ����һ�����*/
                p2->r = Statement();
                p2->r->r=NULL;
            } else {
                printf("����for������\n");
                exit(0);
            }
            root->type=FORSTATEMENT;
            root->l=p1;
            root->r=p2;
            isInRecycle=0;
            return root;
        }
        case RETURN:{
            hasReturn=1;
            if(isVoid==1){
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("���󣺺�����Ӧ���з���ֵ\n");
                exit(0);
            }
            root->type=RETURNSTATEMENT;
            root->l=NULL;
            root->r=NULL;
            w=gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            root->r=Expression(SEMI);
            return root;
        }
        case DO:{
            isInRecycle=1;
            w=gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if(w!=LB){
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����do-while���ȱ�ٴ�����\n");
                exit(0);
            }
            ASTTree* p1=(ASTTree*)malloc(sizeof(ASTTree));
            p1->type=DOWHILEBODY;
            p1->l=NULL;
            p1->r=NULL;
            ASTTree* p2=(ASTTree*)malloc(sizeof(ASTTree));
            p2->type=DOWHILECONDITION;
            p2->l=NULL;
            p2->r=NULL;
            root->l=p1;
            root->r=p2;
            root->data.data=NULL;
            p1->data.data=NULL;
            p2->data.data=NULL;
            w = gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            p1->l=StateList();

            //do-while����崦�����
            w=gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if(w!=WHILE){
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����do-while���ȱ��while�ؼ���\n");
                freeTree(root);
                exit(0);
            }
            root->type=DOWHILESTATEMENT;
            w=gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if(w!=LP){
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����do-while���ȱ��while����\n");
                freeTree(root);
                exit(0);
            }
            w=gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            p2->l=Expression(RP);
            if(p2->l==NULL){
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����do-while���ȱ������\n");
                exit(0);
            }
            w=gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if(w!=SEMI){
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����do-while���ȱ�ٷֺ�\n");
                freeTree(root);
                exit(0);
            }
            isInRecycle=0;
            return root;
        }
        case BREAK:{
            w=gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if(w!=SEMI){
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����break���ȱ�ٷֺ�\n");
                exit(0);
            }
            if(isInRecycle==0){
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("���󣺷�ѭ������г�����break���\n");
                exit(0);
            }
            root->type=BREAKSTATEMENT;
            return root;
        }
        case CONTINUE:{
            w=gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
            if(w!=SEMI){
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("����continue���ȱ�ٷֺ�\n");
                exit(0);
            }
            if(isInRecycle==0){
                printf("��%d�г��ִ���\n",cnt_lines);
                printf("���󣺷�ѭ������г�����continue���\n");
                exit(0);
            }
            root->type=CONTINUESTATEMENT;
            return root;
        }
        case INT_CONST:
        case FLOAT_CONST:
        case CHAR_CONST:
        case IDENT:
        case ARRAY:
            return Expression(SEMI);

    }
    return root;
}//�������ʱû�ж������

/*<���ʽ>�ӳ���*/
//����ǰ�Ѿ�������һ������
ASTTree* Expression(int endsym){//endsym�Ǳ��ʽ�������ţ��ֺŻ�����С����
    if(mistake==1){
        return NULL;
    }
    if(w==endsym){//���forѭ�����ܻ�������Ϊ�յ����
        return NULL;
    }
    int error=0;
    stack<ASTTree*> op;//�����ջ
    ASTTree* p=(ASTTree*)malloc(sizeof(ASTTree));//������ֹ���Ž��
    p->data.data=NULL;
    p->type=OPERATOR;
    p->data.type=POUND;
    op.push(p);
    stack<ASTTree*> opn;//������ջ
    ASTTree *t,*t1,*t2,*root;
    while (((w!=endsym)||(op.top()->data.type!=POUND))&&!error){
        if(op.top()->data.type==RP){//ȥ����
            if(op.size()<3){
                error++;
                break;
            }
            op.pop();
            op.pop();
        }
        if(w==IDENT){
            if(checkname_exist(token_text)==0){
                mistake=1;
            }
        }
        if(w==IDENT||w==INT_CONST||w==FLOAT_CONST||w==CHAR_CONST||w==ARRAY||w==STRING_CONST){
            p=(ASTTree*)malloc(sizeof(ASTTree));
            p->data.data=NULL;
            p->type=OPERAND;
            char *token_text1=(char*)malloc(25* sizeof(char));
            strcpy(token_text1,token_text);
            p->data.data=token_text1;
            opn.push(p);
            w=gettoken(fp);
            while(w==ANNO||w==INCLUDE){
                w=gettoken(fp);
            }
        }
        else if(w==endsym){
            ASTTree* p=(ASTTree*)malloc(sizeof(ASTTree));//������ֹ���Ž��
            p->data.data=NULL;
            p->type=OPERATOR;
            p->data.type=POUND;
            while(op.top()->data.type!=POUND){
                t2 = opn.top();
                if (!t2&&(op.top()->data.type)!=PLUSPLUS&&(op.top()->data.type)!=MINUSMINUS) {
                    error++;
                    break;
                }
                if(t2!=NULL)
                    opn.pop();
                if (opn.size()==0) {
                    t1 = NULL;
                }else{
                    t1 = opn.top();
                }
                if (!t1&&(op.top()->data.type)!=PLUSPLUS&&(op.top()->data.type)!=MINUSMINUS) {
                    error++;
                    break;
                }
                if(t1!=NULL)
                    opn.pop();
                t = op.top();
                if (!t) {
                    error++;
                    break;
                }
                op.pop();
                t->l = t1;
                t->r = t2;
                opn.push(t);

            }
            if(opn.size()!=1){
                error++;
            }

        }
        else if(w>=EQ&&w<=MINUSMINUS) {
            char *token_text1=(char*)malloc(25* sizeof(char));//@
            switch (Precede(op.top()->data.type, w)) {
                case '<':
                    p = (ASTTree *) malloc(sizeof(ASTTree));
                    p->data.data=NULL;
                    p->type = OPERATOR;
                    p->data.type = w;
                    strcpy(token_text1,token_text);
                    p->data.data=token_text1;
                    op.push(p);
                    w = gettoken(fp);
                    while(w==ANNO||w==INCLUDE){
                        w=gettoken(fp);
                    }
                    break;
                case '='://ȥ����
                    t = op.top();
                    if (!t) {
                        error++;
                        op.pop();
                    }
                    w = gettoken(fp);
                    while(w==ANNO||w==INCLUDE){
                        w=gettoken(fp);
                    }
                    break;
                case '>':
                    t2 = opn.top();
                    if (!t2&&(op.top()->data.type)!=PLUSPLUS&&(op.top()->data.type)!=MINUSMINUS) {
                        error++;
                        break;
                    }
                    if(t2!=NULL)
                        opn.pop();
                    if (opn.size()==0) {
                        t1 = NULL;
                    }else{
                        t1 = opn.top();
                    }
                    if (!t1&&(op.top()->data.type)!=PLUSPLUS&&(op.top()->data.type)!=MINUSMINUS) {
                        error++;
                        break;
                    }
                    if(t1!=NULL)
                        opn.pop();
                    t = op.top();
                    if (!t) {
                        error++;
                        break;
                    }
                    op.pop();
                    t->l = t1;
                    t->r = t2;
                    opn.push(t);

                    p = (ASTTree *) malloc(sizeof(ASTTree));
                    p->data.data=NULL;
                    p->type = OPERATOR;
                    p->data.type = w;
                    strcpy(token_text1,token_text);
                    p->data.data=token_text1;
                    op.push(p);

                    w = gettoken(fp);
                    while(w==ANNO||w==INCLUDE){
                        w=gettoken(fp);
                    }
                    break;
                case '\0':
                    printf("��%d�г��־���\n",cnt_lines);
                    printf("���棺����δ֪�����\n");
                    exit(0);
            }
        }
            else{
            error=1;
        }
    };
    if((opn.size()==1)&&(op.top()->data.type==POUND)&&error==0){
        t=opn.top();
        opn.pop();
        root=(ASTTree*)malloc(sizeof(ASTTree));
        root->data.data=NULL;
        root->l=NULL;
        root->r=NULL;
        root->type=EXPRESSION;
        root->l=t;
        return root;
    } else{
        printf("��%d�г��ִ���\n",cnt_lines);
        printf("���󣺱��ʽ���ִ���\n");
        exit(0);
    }
    //��������ʱwΪendsym
}



/*�Ƚ����ȼ�����*/
char Precede(int c1,int c2){
    if(mistake==1){
        return NULL;
    }
    if(c1==PLUS||c1==MINUS){
        switch (c2){
            case PLUS:
            case MINUS:
            case RP:
            case POUND:
            case MORE:
            case LESS:
            case MOREEQUAL:
            case LESSEQUAL:
            case EQ:
            case NEQ:
            case ASSIGN:
                return '>';
            case TIMES:
            case DIVIDE:
            case LP:
            case PLUSPLUS:
            case MINUSMINUS:
                return '<';
            default:
                return '\0';
                break;

        }
    } else if(c1==TIMES||c1==DIVIDE){
        switch (c2){
            case PLUS:
            case MINUS:
            case RP:
            case POUND:
            case TIMES:
            case DIVIDE:
            case MORE:
            case LESS:
            case MOREEQUAL:
            case LESSEQUAL:
            case EQ:
            case NEQ:
                return '>';
            case LP:
            case PLUSPLUS:
            case MINUSMINUS:
                return '<';
            default:
                return '\0';
        }
    } else if (c1==LP){
        switch (c2){
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case PLUSPLUS:
            case MINUSMINUS:
                return '<';
            case RP:
                return '=';
            case MORE:
            case LESS:
            case MOREEQUAL:
            case LESSEQUAL:
            case EQ:
            case NEQ:
            case POUND:
                return '>';
            default:
                return '\0';
        }
    } else if(c1==RP){
        switch (c2){
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case MORE:
            case LESS:
            case MOREEQUAL:
            case LESSEQUAL:
            case EQ:
            case NEQ:
            case PLUSPLUS:
            case MINUSMINUS:
            case POUND:
                return '>';
            default:
                return '\0';
        }
    } else if(c1==ASSIGN){
        switch (c2){
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case MORE:
            case LESS:
            case MOREEQUAL:
            case LESSEQUAL:
            case EQ:
            case NEQ:
            case PLUSPLUS:
            case MINUSMINUS:
                return '<';
            case POUND:
                return '>';
            default:
                return '\0';
        }
    }else if(c1==MORE||c1==LESS||c1==MOREEQUAL||c1==LESSEQUAL){
        switch (c2){
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case PLUSPLUS:
            case MINUSMINUS:
                return '<';
            case RP:
            case MORE:
            case LESS:
            case MOREEQUAL:
            case LESSEQUAL:
            case EQ:
            case NEQ:
            case POUND:
                return '>';
            default:
                return '\0';
        }
    }else if(c1==EQ||c1==NEQ){
        switch (c2){
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case MORE:
            case LESS:
            case MOREEQUAL:
            case LESSEQUAL:
            case PLUSPLUS:
            case MINUSMINUS:
                return '<';
            case RP:
            case EQ:
            case NEQ:
            case POUND:
                return '>';
            default:
                return '\0';
        }
    }else if(c1==POUND){
        switch (c2){
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case MORE:
            case LESS:
            case MOREEQUAL:
            case LESSEQUAL:
            case RP:
            case EQ:
            case NEQ:
            case ASSIGN:
            case PLUSPLUS:
            case MINUSMINUS:
                return '<';
            case POUND:
                return '=';
            default:
                return '\0';
        }
    }else if(c1==PLUSPLUS||c1==MINUSMINUS){
        switch (c2){
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case MORE:
            case LESS:
            case MOREEQUAL:
            case LESSEQUAL:
            case EQ:
            case NEQ:
            case ASSIGN:
            case POUND:
                return '>';
            case RP:
                return '<';
            default:
                return '\0';
        }
    }
}

void returntoken(int w,FILE* fp){
    int digit=strlen(token_text);
    int i;
    for(i=0;i<digit;i++){
        ungetc(token_text[digit-1-i],fp);
    }
}

void showType(int type){
    switch (type){
        case 1:
            printf("�ⲿ��������\n");
            break;
        case 2:
            printf("�ⲿ��������\n");
            break;
        case 3:
            printf("�ⲿ��������\n");
            break;
        case 4:
            printf("�ⲿ����������\n");
            break;
        case 5:
            printf("�ⲿ������\n");
            break;
        case 6:
            printf("��������\n");
            break;
        case 7:
            printf("��������ֵ����\n");
            break;
        case 8:
            printf("������\n");
            break;
        case 9:
            printf("�����β�����\n");
            break;
        case 10:
            printf("�����βζ���\n");
            break;
        case 11:
            printf("�����β�����\n");
            break;
        case 12:
            printf("�����β���\n");
            break;
        case 13:
            printf("������\n");
            break;
        case 14:
            printf("�ֲ�������������\n");
            break;
        case 15:
            printf("�ֲ���������\n");
            break;
        case 16:
            printf("�ֲ���������\n");
            break;
        case 17:
            printf("�ֲ�����������\n");
            break;
        case 18:
            printf("�ֲ�������\n");
            break;
        case 19:
            printf("�������\n");
            break;
        case 20:
            printf("������\n");
            break;
        case 21:
            printf("�����\n");
            break;
        case 22:
            printf("���ʽ\n");
            break;
        case 23:
            printf("if�������\n");
            break;
        case 24:
            printf("else�����\n");
            break;
        case 25:
            printf("if���\n");
            break;
        case 26:
            printf("if-else���\n");
            break;
        case 27:
            printf("while���\n");
            break;
        case 28:
            printf("while�������\n");
            break;
        case 29:
            printf("while�����\n");
            break;
        case 30:
            printf("for���\n");
            break;
        case 31:
            printf("for�������\n");
            break;
        case 32:
            printf("for�������һ\n");
            break;
        case 33:
            printf("for���������\n");
            break;
        case 34:
            printf("for���������\n");
            break;
        case 35:
            printf("for�����\n");
            break;
        case 36:
            printf("return���\n");
            break;
        case 37:
            printf("break���\n");
            break;
        case 38:
            printf("do-while���\n");
            break;
        case 39:
            printf("do-while�����\n");
            break;
        case 40:
            printf("do-whileѭ������\n");
            break;
        case 41:
            printf("continue���\n");
            break;
        case 42:
            printf("��������\n");
            break;
        case 43:
            printf("���鶨��\n");
            break;
        case 44:
            printf("��������\n");
            break;
        case 45:
            printf("������\n");
            break;
        case 46:
            printf("�����С\n");
            break;
    }
}

void PreorderTranverse(ASTTree* root,int depth){
    if(root== NULL) printf("");
    else{
        int i;//��������
        for(i=0;i<depth;i++){
            printf("\t");
        }
        showType(root->type);
        if(root->data.data!=NULL){
            for(i=0;i<depth;i++){
                printf("\t");
            }
            printf("%s\n",root->data.data);
        }
        PreorderTranverse(root->l,depth+1);
        PreorderTranverse(root->r,depth+1);
    }
}

int addname(char *token_text){
    if(mistake==1){
        return NULL;
    }
    int i,flag=0;
    VDN *p=Vroot;
    while(p->next!=NULL){
        p=p->next;
    }//�µı�����һ���ᱻ��ӵ����һ�������
    //��ӱ���֮ǰ�ȼ������Ƿ��Ѿ��������
    for(i=0;i<(p->size);i++){
        if(strcmp(token_text,p->variable[i])==0){
            flag=1;
            break;
        }
    }
    if(flag==1){
        printf("��%d�г��ִ���\n",cnt_lines);
        printf("���󣺱����ظ�����\n");
        exit(0);
        mistake=1;
        return flag;
    }
    char *savename=(char*)malloc(25* sizeof(char));
    strcpy(savename,token_text);
    p->variable[p->size]=savename;
    p->size++;
    return flag;

}

int checkname_exist(char* token_text){
    if(mistake==1){
        return NULL;
    }
    //����ֵΪ1��������Ѿ����������Ϊ0�������û�б������
    //�������Ƿ���ڣ�ֻ��Ҫ���������Ƿ��ڵ�һ�����������һ�����
    int i;
    int flag=0;
    VDN* p=Vroot;
    while(p->next!=NULL){
        p=p->next;
    }
    for(i=0;i<(p->size);i++){
        if(strcmp(token_text,p->variable[i])==0){
            flag=1;
            break;
        }
    }
    for(i=0;i<(Vroot->size);i++){
        if(strcmp(token_text,Vroot->variable[i])==0){
            flag=1;
            break;
        }
    }
    if(flag==0){
        printf("��%d�г��ִ���\n",cnt_lines);
        printf("����ʹ����δ������ı���\n");
        exit(0);
        mistake=1;
    }
    return flag;
}

