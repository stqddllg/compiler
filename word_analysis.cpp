#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"A.h"

using namespace std;
extern err_pair * err[1000];
extern int error_number;
const char remainStrings[12][15]= {"main","const","int","break","continue",
                                   "if","else","while","getint","printf","return","void"
                                  };
char valueRemain[12][15]= {"MAINTK","CONSTTK","INTTK","BREAKTK","CONTINUETK",
                                 "IFTK","ELSETK","WHILETK","GETINTTK","PRINTFTK","RETURNTK","VOIDTK"
                                };
 char otherString[23][5]= {"!","&&","||","+","-","*","/","%","<","<=",
                                ">",">=","==","!=","=",";",",","(",")","[","]","{","}"
                               };
char otherValue[23][15]= {"NOT","AND","OR","PLUS","MINU","MULT","DIV","MOD",
                          "LSS","LEQ","GRE","GEQ","EQL","NEQ","ASSIGN","SEMICN","COMMA","LPARENT",
                                "RPARENT","LBRACK","RBRACK","LBRACE","RBRACE"
                               };
int reserve ;
int wordNumber ;
int line=1;


extern struct wordpair word_pairs[10000];

bool isEmpty(char x)
{
    if(x==' '||x=='\n')
    {
        return true ;
    }
    else return false;
}
bool isCharacter(char x)
{
    if((65<=x&&x<=90)||(97<=x&&x<=122)||(x==95)) return true;
    else return false;
}
bool isCharacterOrDigit(char x)
{
    return isCharacter(x) || (x<='9'&&x>='0');
}
int isRemainString(char* a)
{
    for (int i=0; i<12; i++)
    {
        if(strcmp(a,remainStrings[i])==0)
        {
            return i;
        }
    }
    return -1;
}
int isOtherCharacter(char *a)
{
    for(int i=0; i<23; i++)
    {
        if(strcmp(a,otherString[i])==0)
        {
            return i;
        }
    }
    return -1;

}
bool isDigit(char x)
{
    return (x<='9'&&x>='0');
}
void getword(char *pos)
{
    static FILE *inputfile = fopen("testfile.txt","r+");
    int input;
    //char temp[10000] = "";
    int temp_length = 0;
    bool isnumber= false;
    bool isCstring = false;
    int countd=0;
    /*
    if(reserve==-1||reserve==EOF){
    	pos[0]='\0';
    	return ;
    }
    */
    if(reserve==0)
    {
        input = fgetc(inputfile);
    }
    else
    {
        input=reserve;
        reserve=0;
    }
    while (input!=EOF)
    {
        if(temp_length!=0)
        {
            if(isnumber)//正在读一个数字
            {
                if(isDigit(input))
                {
                    pos[temp_length]=input;
                    temp_length++;
                    input = fgetc(inputfile);
                    continue;
                }
                else
                {
                    reserve = input;
                    pos[temp_length]='\0';
                    return;
                }
            }
            else if(isCstring) //变量名
            {
                if(isCharacterOrDigit(input))
                {
                    pos[temp_length] = input;
                    temp_length++;
                    input = fgetc(inputfile);
                    continue;
                }
                else
                {
                    reserve = input;
                    pos[temp_length]='\0';
                    return;
                }
            }
            else // 格式字符串“”
            {
                pos[temp_length]=input;
                temp_length++;
                if(input=='"')
                {
                    pos[temp_length]='\0';
                    return ;
                }
                input = getc(inputfile);
                /*
                if(input<=33&&input>=32||input<=126&&input>=40||input =='%') {
                    if(input =='\\'){
                        input = fgetc(inputfile);
                        if(input =='n'){
                            continue;
                        }
                        else{
                            pos[temp_length]=input;
                            temp_length++;
                        }
                    }else if(input =='%'){
                        input = fgetc(inputfile);
                        if(input =='d'){
                            countd++;//
                            continue;
                        }
                        else{
                            pos[temp_length]=input;
                            temp_length++;
                        }
                    }
                    else {
                        input = fgetc(inputfile);
                        continue;
                    }
                }
                while(input!='"') {
                    input = fgetc(inputfile);
                    pos[temp_length]=input;
                    temp_length++;
                }
                pos[temp_length]='\0';
                err[error_number] = new err_pair('a',line,pos);
                error_number++;
                 */
            }
        }
        else
        {
            if(input ==' '||input == '\t'||input =='\r')
            {
                input = fgetc(inputfile);
                continue;
            }
            else if(input == '\n')
            {
                input = fgetc(inputfile);
                line++;
                continue;
            }
            else if(isCharacter(input))
            {
                isCstring = true;
                pos[temp_length]=input;
                temp_length++;
                input = fgetc(inputfile);
            }
            else if(isDigit(input))
            {
                isnumber =true;
                pos[temp_length]=input;
                temp_length++;
                input = fgetc(inputfile);
            }
            else if (input == '<'||input=='>'||input =='!'||input =='=')
            {
                pos[0]=input;
                input = fgetc(inputfile);
                if(input=='=')
                {
                    pos[1]=input;
                    pos[2]='\0';
                    return ;
                }
                else
                {
                    pos[1] = '\0';
                    reserve = input;
                    return ;
                }
            }
            else if(input =='%'||input =='+'||input == '-'||input =='*'||input ==';'||
                    input ==','||input == '('||input == ')'||input=='['||input==']'||input=='{'||input=='}')
            {
                pos[0]= input;
                pos[1]='\0';
                return ;
            }
            else if(input == '/')
            {
                input=fgetc(inputfile);
                if (input=='/')
                {
                    while((input=fgetc(inputfile))!='\n')
                    {
                        if(input == EOF)
                        {
                            pos[0]='\0';
                            return ;
                        }
                    }
                    line ++;
                    input = fgetc(inputfile);
                }
                else if(input == '*')
                {
                    bool needget = true;
                    while(true)
                    {
                        if(needget) input = fgetc(inputfile);
                        if(input =='\n') line ++;
                        if(input == EOF)
                        {
                            pos[0]='\0';
                            return ;
                        }
                        if(input == '*')
                        {
                            if((input = fgetc(inputfile))=='/')
                            {
                                break;
                            }
                            if(input == '*')
                            {
                                needget =false;
                            }
                            else if(input == EOF)
                            {
                                pos[0]='\0';
                                return ;
                            }
                            else
                            {
                                needget = true;
                            }
                        }
                    }
                    input = fgetc(inputfile);
                    continue;
                }
                else
                {
                    pos[0]= '/';
                    pos[1]='\0';
                    reserve = input;
                    return ;
                }
            }
            else if (input=='&'||input=='|')
            {
                pos[0]= input;
                input = fgetc(inputfile);
                pos[1]=input;
                pos[2]='\0';
                return ;
            }
            else if (input =='"')
            {
                pos[temp_length]= input;
                temp_length++;
                input = fgetc(inputfile);
                continue;
            }
            else
            {
                //printf("%c",input);
                break;
            }
        }
    }
    pos[temp_length]='\0';
    return ;
}
void clear(char *a)
{
    for(int i=0; *(a+i)!='\0'; i++)
    {
        *(a+i)='\0';
    }
}
void analysis_all_word()
{

    char  word[500] ;
    word[0] = '\0';
    clear(word);
    getword(word);
    printf("%s", word);
    int word_sign ;
    char word_temp_mod[15]="";
    bool isright = true;
    while(*word!='\0')
    {
        isright = true;
        word_sign = isRemainString(word);
        if(word_sign != -1)
        {
            strcpy(word_temp_mod,valueRemain[word_sign]);
        }
        else if((word_sign=isOtherCharacter(word))!=-1)
        {
            strcpy(word_temp_mod,otherValue[word_sign]);
        }
        else if(word[0]=='"')
        {
            strcpy(word_temp_mod,"STRCON");
        }
        else if(isDigit(word[0]))
        {
            strcpy(word_temp_mod, "INTCON");
        }
        else if(isCharacter(word[0]))
        {
            strcpy(word_temp_mod, "IDENFR");
        }
        else
        {
            //strcpy(word_temp_mod , "WRONG");
            isright = false;
        }
        if(isright)
        {
            word_pairs[wordNumber].linenumber=line;
            strcpy(word_pairs[wordNumber].value, word);
            strcpy(word_pairs[wordNumber].word_type, word_temp_mod);
            wordNumber++;
        }
        clear(word);
        getword(word) ;
        printf("%s", word);
    }
}
