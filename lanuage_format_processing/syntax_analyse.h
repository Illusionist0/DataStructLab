#ifndef LANUAGE_FORMAT_PROCESSING_GRAMMAR_ANALYSE_H
#define LANUAGE_FORMAT_PROCESSING_GRAMMAR_ANALYSE_H

typedef struct ASTTree{
    /*�����ֵܱ�ʾ��*/
    struct ASTTree* l;
    struct ASTTree* r;
    int type;//�﷨����
    struct data{
        int type;//�ʷ�����
        char *data;
    }data;
}ASTTree;

//�洢�����������ڼ���������
typedef struct VariableDefNode{
    int size;//��¼��������
    //char *name;//���ڼ�¼�����������ⲿ����
    char *variable[20];//���ڼ�¼������
    struct VariableDefNode *next;
}VDN;

typedef enum data_kind{
    EXTDEFLIST=1,
    EXTVARDEF,
    EXTVARTYPE,
    EXTVARLIST,//�ⲿ���������н��
    EXTVAR,//�ⲿ���������
    FUNCDEF,//����������
    FUNCRETURNTYPE,//��������ֵ���ͽ��
    FUNCNAME,//���������
    FUNCFORMALPARALIST,//������ʽ�������н��
    FUNCFORMALPARADEF,//������ʽ�������10
    FUNCFORMALPARATYPE,//�����β����ͽ��
    FUNCFORMALPARA,//�����β������
    FUNCBODY,//��������
    LOCALVARDEFLIST,//�ֲ������������н��
    LOCALVARDEF,//�ֲ�����������15
    LOCALVARTYPE,//�ֲ��������ͽ��
    LOCALVARNAMELIST,//�ֲ�����������
    LOCALVARNAME,//�ֲ�������
    STATELIST,//������н��
    OPERAND,//���������20
    OPERATOR,//��������
    EXPRESSION,//���ʽ
    IFPART,//if��䲿��
    ELSEPART,//else����
    IFSTATEMENT,//if���25
    IFELSESTATEMENT,//if-else���
    WHILESTATEMENT,//while�����
    WHILEPART,//while���������
    WHILEBODY,//while�����
    FORSTATEMENT,//for�����30
    FORPART,//for�������
    FORPART1,//for�������һ
    FORPART2,//for���������
    FORPART3,//for���������
    FORBODY,//for�����35
    RETURNSTATEMENT,//return���
    BREAKSTATEMENT,//break���
    DOWHILESTATEMENT,//do-whileѭ�����
    DOWHILEBODY,//do-while�����
    DOWHILECONDITION,//do-while����40
    CONTINUESTATEMENT,//continue���
    FUNCCLAIM,//��������
    ARRAYDEF,//���鶨��
    ARRAYTYPE,//��������
    ARRAYNAME,//������45
    ARRAYSIZE,//�����С
}kind;

void syntax_analyse();//�����������
void freeTree(ASTTree* root);//�ͷ�rootΪ����ȫ�����
ASTTree* program();//<����>�ӳ���
ASTTree* ExtDefList();//<�ⲿ������������>�ӳ���
ASTTree* ExtDef();
ASTTree* ExtVarDef();
ASTTree* FuncDef();
ASTTree* FormParaList();//�β�����
ASTTree* FormParaDef();//�βζ���
ASTTree* CompState();//<�������>�ӳ���
ASTTree* LocalVarDefList();//�ֲ����������ӳ���
ASTTree* StateList();//<�������>�ӳ���
ASTTree* Statement();//<���>�ӳ���
ASTTree* Expression(int endsym);//<���ʽ>�ӳ���
char Precede(int c1,int c2);//�Ƚ����ȼ�����
void returntoken(int w,FILE* fp);//��������tokem���ص��ļ�����
void showType(int type);
void PreorderTranverse(ASTTree* root,int depth);
int addname(char *token_text);//����������ӵ������
int checkname_exist(char* token_text);
ASTTree* ArrayDef();

#endif //LANUAGE_FORMAT_PROCESSING_GRAMMAR_ANALYSE_H
