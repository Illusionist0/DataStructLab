#ifndef LANGUAGE_FORMAT_PROCESSING_TOOL_GETTOKEN_H
#define LANGUAGE_FORMAT_PROCESSING_TOOL_GETTOKEN_H

typedef enum token_kind{
    ERROR_TOKEN= 1,
    IDENT,//��ʶ��
    INT_CONST,//���γ���
    FLOAT_CONST,
    CHAR_CONST,
    STRING_CONST,
    KEYWORD,
    INT,
    FLOAT,
    CHAR,
    LONG,
    SHORT,
    DOUBLE,
    IF,
    ELSE,
    DO,
    WHILE,
    FOR,
    STRUCT,
    BREAK,
    SWITCH,
    CASE,
    TYPEDEF,
    RETURN,
    CONTINUE,
    VOID,//26
    LB,//�������
    RB,//�Ҵ�����
    LM,//��������
    RM,//��������
    SEMI,//�ֺ�31
    COMMA,//����
    /*EQ��MINUSMINUSΪ���������������һ��*/
    EQ,//'=='
    NEQ,//��!=��
    ASSIGN,//'='35
    LP,//������
    RP,//������
    TIMES,//�˷�
    DIVIDE,//����
    PLUS,//�ӷ�40
    PLUSPLUS,//��������
    POUND,//����42
    MORE,//���ں�
    LESS,//С�ں�
    MOREEQUAL,//���ڵ���
    LESSEQUAL,//С�ڵ���
    MINUS,//����
    LOGIC,//&&
    MINUSMINUS,//�Լ�����
    ANNO,//ע��
    INCLUDE,//ͷ�ļ�����
    MACRO,//�궨��
    ARRAY,//����
}token_kind;

//�ؼ��ֲ��ұ�
static char* KeyWords[]={
        (char*)"struct",(char*)"break",(char*)"else",
        (char*)"switch",(char*)"case",(char*)"typedef",
        (char*)"return",(char*)"continue",
        (char*)"for",(char*)"void",(char*)"do",(char*)"if",(char*)"while"
};

int isNum(char c);
int isLetter(char c);
int isLorN(char c);
int gettoken(FILE*);

#endif //LANGUAGE_FORMAT_PROCESSING_TOOL_GETTOKEN_H
