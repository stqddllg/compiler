#include<stdio.h>
#include<string.h>
#include "A.h"

extern struct wordpair word_pairs[10000];
extern err_pair * err[1000];
extern int error_number;

/*const char remainStrings[12][15]= {"main","const","int","break","continue",
                                   "if","else","while","getint","printf","return","void"
                                  };
/*const char valueRemain[12][15]= {"MAINTK","CONSTTK","INTTK","BREAKTK","CONTINUETK",
                                 "IFTK","ELSETK","WHILETK","GETINTTK","PRINTFTK","RETURNTK","VOIDTK"
                                };
const char otherString[23][5]= {"!","&&","||","+","-","*","/","%","<","<=",
                                ">",">=","==","!=","=",";",",","(",")","[","]","{","}"
                               };
/*const char otherValue[23][15]= {"NOT","AND","OR","PLUS","MINU","MULT","DIV",
                                "MOD","LSS","LEQ","GRE","GEQ","EQL","NEQ","ASSIGN","SEMICN","COMMA","LPARENT",
                                "RPARENT","LBRACK","RBRACK","LBRACE","RBRACE"
                               };*/


char grammer_type[32][20]={"<CompUnit>","<ConstDecl>","<Final>","<ConstDef>","<ConstExp>","<AddExp>","<MulExp>","<UnaryExp>",
                            "<UnaryOp>","<FuncRParams>","<Exp>","<PrimaryExp>","<Number>","<LVal>","<ConstInitVal>","<VarDecl>",
                            "<VarDef>","<InitVal>","<FuncDef>","<MainFuncDef>","<FuncType>","<FuncFParams>","<FuncFParam>","<Block>",
                            "<BlockItem>","<Cond>","<Stmt>","<FormatString>","<LOrExp>","<LAndExp>","<EqExp>","<RelExp>"};//13

char specialString[20]="whstatment";

char tritype[3][15]={"IDENFR","STRCON","INTCON"};
extern char valueRemain[12][15];
extern char otherValue[23][15];

treenode * root;

int now_pointer=0;

char valuetemp[500];
char typetemp[15];

void getwhilestatment(treenode * proot);
void getdecl(treenode* proot);
void getconstdef(treenode* proot);
void getconstexp(treenode * proot);
void getaddexp(treenode* proot);
void getmulexp(treenode* proot);
void getunaryexp(treenode* proot);
void getunaryop(treenode* proot);
void getfuncdef(treenode* proot);
void getmainfuncdef(treenode* proot);
void getblock(treenode* proot);
void getblockitem(treenode* proot);
void getcond(treenode* proot);
void getconinitval(treenode* proot);
void getexp(treenode* proot);
void getfunctype(treenode* proot);
void getfuncfffparaaaaa(treenode* proot);//��������
void getfuncfffparams(treenode* proot);//�����������
void getfuncparams(treenode* proot);//��������
void getstatment(treenode* proot,bool special2);
int getformatstring(treenode* proot);
void getvardef(treenode* proot);
void getinitval(treenode* proot);
void getprimaryexp(treenode* proot);
void getnumber(treenode* proot);
void getlval(treenode* proot);
void getlandexp(treenode* proot);
void getlorexp(treenode* proot);
void getrelexp(treenode* proot);
void geteqexp(treenode* proot);
bool is_exp_start(char * inp);
void clean_treenode(treenode* proot);

void getwordvalue(char *tt,int m=0){
    strcpy(tt,word_pairs[m+now_pointer].value);
}
void getwordtype(char *tt,int m=0){
    strcpy(tt,word_pairs[m+now_pointer].word_type);
}

treenode* pronode(int wnumber,int typenumber){
    treenode* ppnode = new treenode;
    ppnode->settype(grammer_type[typenumber]);
    ppnode->set_word_pair(&word_pairs[wnumber]);
    return ppnode;
}

treenode* get_semic(){//15 ; 18 ) 20 ] othervalue
    char stringtype[15];
    getwordtype(stringtype);
    if(strcmp(otherValue[15],stringtype)!=0){
        throw "semic lose";
    }
    else return  pronode(now_pointer++,2);
}

treenode* get_rparent(){
    char stringtype[15];
    getwordtype(stringtype);
    if(strcmp(otherValue[18],stringtype)!=0){
        throw "rparent lose";
    }
    else return  pronode(now_pointer++,2);
}

treenode * get_rbrack(){
    char stringtype[15];
    getwordtype(stringtype);
    if(strcmp(otherValue[20],stringtype)!=0){
        throw "rbrack lose";
    }
    else return  pronode(now_pointer++,2);
}

void analysis_all_grammer(){

    root=new treenode;
    root->settype(grammer_type[0]);

    treenode * nownode=root;
    treenode * tempnode = new treenode;

    nownode->set_lower_node(tempnode);
    nownode = tempnode;
    char stringtype[15];
    getwordtype(stringtype,2);
    while(strcmp(stringtype,otherValue[17])!=0){
        getdecl(tempnode);
        tempnode = new treenode;
        nownode->set_right_node(tempnode);
        nownode= tempnode;
        getwordtype(stringtype,2);
    }
    getwordtype(stringtype,1);
    while(strcmp(stringtype,valueRemain[0])!=0){//main
        getfuncdef(tempnode);
        tempnode=new treenode;
        nownode->set_right_node(tempnode);
        nownode =tempnode;
        getwordtype(stringtype,1);
    }
    getmainfuncdef(tempnode);
}

int search_last_line(treenode* proot){
    while(proot->word==NULL){
        if(proot->rightnode!=NULL){
            proot = proot->rightnode;
        }else if(proot->lowernode!=NULL) proot = proot->lowernode;
        else return -1;
    }
    return proot->word->linenumber;
}

void getfuncdef(treenode * proot){
    treenode * tempnode = new treenode;
    getfunctype(tempnode);
    proot->settype(grammer_type[18]); //
    proot->set_lower_node(tempnode);
    proot = tempnode;


        tempnode = pronode(now_pointer++,2); //ident
        proot->set_right_node(tempnode);
        proot=tempnode;

        tempnode = pronode(now_pointer++,2); // (
        proot->set_right_node(tempnode);
        proot=tempnode;

    char stringtype[15];
    getwordtype(stringtype);
    if(strcmp(stringtype,otherValue[18])!=0){ // )
        if(strcmp(stringtype,otherValue[21])!=0) { //{ 读到了声明参数
            tempnode = new treenode;
            getfuncfffparams(tempnode);
            proot->set_right_node(tempnode);
            proot = tempnode;
        }
    }
    getwordtype(stringtype);
    if(strcmp(stringtype,otherValue[21])==0) { // 没读到右括号，读到了左大括号
        err[error_number] = new err_pair('j', search_last_line(proot));
        error_number++;
    }else {
        tempnode = pronode(now_pointer++, 2);
        proot->set_right_node(tempnode);// )
        proot = tempnode;
    }

    tempnode = new treenode;
    getblock(tempnode);
    proot->set_right_node(tempnode); //block
    return ;

}

void getfuncfffparams(treenode * proot){
    proot->settype(grammer_type[21]);
    treenode* tempnode = new treenode;
    proot->set_lower_node(tempnode);
    proot= tempnode;
    getfuncfffparaaaaa(tempnode);
    char stringtype[15];
    getwordtype(stringtype);
    while(strcmp(stringtype,otherValue[16])==0){
        tempnode = pronode(now_pointer++,2);
        proot->set_right_node(tempnode);
        proot = tempnode;
        tempnode =new treenode;
        getfuncfffparaaaaa(tempnode);
        proot->set_right_node(tempnode);
        proot = tempnode;
        getwordtype(stringtype);
    }
    return ;
}

void getfuncfffparaaaaa(treenode *proot){
    proot->settype(grammer_type[22]);
    treenode* tempnode = pronode(now_pointer++,2); //int
    proot->set_lower_node(tempnode);
    proot = tempnode;

    tempnode = pronode(now_pointer++,2); //indent
    proot->set_right_node(tempnode);
    proot=tempnode;

    char stringtype[15];
    getwordtype(stringtype);
    if(strcmp(otherValue[19],stringtype)==0){
        tempnode = pronode(now_pointer++,2);
        proot->set_right_node(tempnode);
        proot = tempnode;

        try{
            tempnode = get_rbrack();
            proot->set_right_node(tempnode);
            proot = tempnode;
        }catch (const char *msg){
            err[error_number] = new err_pair('k', search_last_line(proot));
            error_number++;
        }

        getwordtype(stringtype);
    }else {
        return ;
    }
    if(strcmp(otherValue[19],stringtype)==0){
        tempnode = pronode(now_pointer++,2);
        proot->set_right_node(tempnode); //[
        proot = tempnode;

        tempnode = new treenode;
        getconstexp(tempnode);
        proot->set_right_node(tempnode);
        proot = tempnode;

        try{
            tempnode = get_rbrack();
            proot->set_right_node(tempnode);
        }catch (const char *msg){
            err[error_number] = new err_pair('k', search_last_line(proot));
            error_number++;
        }
    }
    return ;
}

void getblock(treenode * proot){
    proot->settype(grammer_type[23]);
    treenode * tempnode = pronode(now_pointer++,2);
    proot->set_lower_node(tempnode);
    proot = tempnode;
    char stringtype[15];
    getwordtype(stringtype);
    while(strcmp(stringtype,otherValue[22])!=0){
        tempnode = new treenode;
        getblockitem(tempnode);
        proot->set_right_node(tempnode);
        proot = tempnode;
        getwordtype(stringtype);
    }
    tempnode = pronode(now_pointer++,2);
    proot->set_right_node(tempnode);
    return ;
}

void getblockitem(treenode* proot){
    proot->settype(grammer_type[24]);
    char stringtype[15];
    getwordtype(stringtype);
    if(strcmp(stringtype,valueRemain[2])==0||strcmp(stringtype,valueRemain[1])==0){
        treenode* tempnode = new treenode;
        proot->set_lower_node(tempnode);
        getdecl(tempnode);
    }else{
        treenode* tempnode = new treenode;
        proot->set_lower_node(tempnode);
        getstatment(tempnode,false);
    }
}
void getstatment(treenode* proot,bool special=false){
    if(special){
        proot->settype(specialString);
    }else {
        proot->settype(grammer_type[26]);
    }
    char stringtype[15];
    getwordtype(stringtype);

    if(strcmp(stringtype,otherValue[21])==0){// getblock
        treenode* tempnode= new treenode;
        proot->set_lower_node(tempnode);
        getblock(tempnode);
    }else if(strcmp(stringtype,valueRemain[5])==0){ // if

        treenode* tempnode = pronode(now_pointer++,2);//if
        proot->set_lower_node(tempnode);
        proot=tempnode;

        tempnode =pronode(now_pointer++,2);//(
        proot->set_right_node(tempnode);
        proot = tempnode;

        tempnode = new treenode;
        getcond(tempnode); //cond
        proot->set_right_node(tempnode);
        proot = tempnode;

        try {
            tempnode = get_rparent();//)
            proot->set_right_node(tempnode);
            proot = tempnode;
        }catch (const char * msg){
            err[error_number] = new err_pair('j', search_last_line(proot));
            error_number++;
        }

        tempnode = new treenode;
        getstatment(tempnode);
        proot->set_right_node(tempnode); //stat
        proot = tempnode;

        getwordtype(stringtype);
        if(strcmp(stringtype,valueRemain[6])==0){  //����else
            tempnode = pronode(now_pointer++,2);
            proot->set_right_node(tempnode);
            proot = tempnode;
            tempnode = new treenode;
            getstatment(tempnode);
            proot->set_right_node(tempnode);
        }
    } else if(strcmp(stringtype,valueRemain[7])==0){//while
        treenode * tempnode = pronode(now_pointer++,2); //while
        proot->set_lower_node(tempnode);
        proot = tempnode;

        tempnode =pronode(now_pointer++,2);//(
        proot->set_right_node(tempnode);
        proot = tempnode;

        tempnode = new treenode;
        getcond(tempnode); //cond
        proot->set_right_node(tempnode);
        proot = tempnode;

        try {
            tempnode = get_rparent();//)
            proot->set_right_node(tempnode);
            proot = tempnode;
        }catch (const char * msg){
            err[error_number] = new err_pair('j', search_last_line(proot));
            error_number++;
        }


        tempnode = new treenode;
        getstatment(tempnode,true);
        proot->set_right_node(tempnode); //stat
        proot = tempnode;
    } else if (strcmp(stringtype,valueRemain[4])==0||strcmp(stringtype,valueRemain[3])==0){ //break continue
        treenode* tempnode =pronode(now_pointer++,2);//break
        proot->set_lower_node(tempnode);
        proot = tempnode;

        try {
            tempnode = get_semic();//)
            proot->set_right_node(tempnode);
            proot = tempnode;
        }catch (const char * msg){
            err[error_number] = new err_pair('i', search_last_line(proot));
            error_number++;
        }

    } else if(strcmp(stringtype,valueRemain[10])==0){ //return
        treenode * tempnode = pronode(now_pointer++,2);
        proot->set_lower_node(tempnode);
        proot = tempnode;


        getwordtype(stringtype);
        if(is_exp_start(stringtype)){
            tempnode = new treenode;
            proot->set_right_node(tempnode);
            getexp(tempnode);
            proot=tempnode;
        }

        try {
            tempnode = get_semic();//)
            proot->set_right_node(tempnode);
            proot = tempnode;
        }catch (const char * msg){
            err[error_number] = new err_pair('i', search_last_line(proot));
            error_number++;
        }


    } else if (strcmp(stringtype,valueRemain[9])==0){ //printf
        int print_line = word_pairs[now_pointer].linenumber;
        treenode * tempnode = pronode(now_pointer++,2); //printf
        proot->set_lower_node(tempnode);
        proot = tempnode;

        tempnode =pronode(now_pointer++,2);//(
        proot->set_right_node(tempnode);
        proot = tempnode;

        tempnode = new treenode;
        int res_number = getformatstring(tempnode);
        treenode * tempnode22 = tempnode;
        if (res_number ==-1){
            wordpair * temp_pair = tempnode->lowernode->word;
            err[error_number] =new err_pair('a',temp_pair->linenumber,temp_pair->value);
            error_number++;
        }
        proot->set_right_node(tempnode);
        proot= tempnode;

        getwordtype(stringtype);
        int exp_count=0;
        while(strcmp(stringtype,otherValue[16])==0){
            tempnode =pronode(now_pointer++,2);//,
            proot->set_right_node(tempnode);
            proot = tempnode;

            tempnode = new treenode;
            getexp(tempnode);//
            proot->set_right_node(tempnode);
            proot=tempnode;
            getwordtype(stringtype);
            exp_count++;
        }
        if(res_number!=-1){
            if(exp_count!=res_number){
                wordpair * temp_pair = tempnode22->lowernode->word;
                err[error_number] = new err_pair('l',print_line,temp_pair->value);
                error_number++;
            }
        }

        try {
            tempnode = get_rparent();//)
            proot->set_right_node(tempnode);
            proot = tempnode;
        }catch (const char * msg){
            err[error_number] = new err_pair('j', search_last_line(proot));
            error_number++;
        }

        try {
            tempnode = get_semic();//;
            proot->set_right_node(tempnode);
            proot = tempnode;
        }catch (const char * msg){
            err[error_number] = new err_pair('i', search_last_line(proot));
            error_number++;
        }


    } else if(strcmp(stringtype,otherValue[21])==0){ //block
        treenode* tempnode = new treenode;
        getblock(tempnode);
        proot->set_lower_node(tempnode);
        proot = tempnode;
    } else{
        bool have_back = false;
        char stringtype[15];

        getwordtype(stringtype);
        treenode * prootdoc= proot;
        int now_pointdoc = now_pointer;
        int error_numberdoc = error_number; //回溯准备

        treenode * tempnode;
        if(strcmp(stringtype,tritype[0])==0){ // ident
            tempnode = new treenode;
            getlval(tempnode);
            getwordtype(stringtype);
            if(strcmp(stringtype,otherValue[14])==0){ // =
                proot->set_lower_node(tempnode);
                proot = tempnode;
                tempnode= pronode(now_pointer++,2);
                proot->set_right_node(tempnode);
                proot=tempnode;
            }else{
                have_back = true;
                clean_treenode(tempnode);
                now_pointer = now_pointdoc;
                for(int i=error_numberdoc;i<error_number;i++){
                    delete err[i];
                    err[i]= nullptr;
                }
                error_number = error_numberdoc;
            }
            getwordtype(stringtype);
            if(strcmp(stringtype,valueRemain[8])==0){
                for(int j=0;j<2;j++){
                    tempnode = pronode(now_pointer++,2);
                    proot->set_right_node(tempnode);
                    proot = tempnode;
                }
                try {
                    tempnode = get_rparent();//;
                    proot->set_right_node(tempnode);
                    proot = tempnode;
                }catch (const char * msg){
                    err[error_number] = new err_pair('j', search_last_line(proot));
                    error_number++;
                }
                try {
                    tempnode = get_semic();//;
                    proot->set_right_node(tempnode);
                    proot = tempnode;
                }catch (const char * msg){
                    err[error_number] = new err_pair('i', search_last_line(proot));
                    error_number++;
                }
            }else { // exp ;
                tempnode = new treenode;
                getexp(tempnode);
                if(have_back){
                    proot->set_lower_node(tempnode);
                }else{
                    proot->set_right_node(tempnode);
                }
                proot = tempnode;
                try {
                    tempnode = get_semic();//;
                    proot->set_right_node(tempnode);
                    proot = tempnode;
                }catch (const char * msg){
                    err[error_number] = new err_pair('i', search_last_line(proot));
                    error_number++;
                }
            }
        }else{
            getwordtype(stringtype);
            if(strcmp(stringtype,otherValue[15])==0){ // ;
                tempnode = pronode(now_pointer++,2);
                proot->set_lower_node(tempnode);
                return;
            }else {
                tempnode = new treenode;
                getexp(tempnode);
                proot->set_lower_node(tempnode);
                proot = tempnode;
                try {
                    tempnode = get_semic();//;
                    proot->set_right_node(tempnode);
                    proot = tempnode;
                } catch (const char *msg) {
                    err[error_number] = new err_pair('i', search_last_line(proot));
                    error_number++;
                }
            }
        }
    }/*
        bool getequal = false;
        int i=0;
        while(strcmp(stringtype,otherValue[15])!=0){//;
            if(strcmp(stringtype,otherValue[14])==0){//=
                getequal = true;
                break;
            }
            i++;
            getwordtype(stringtype,i);
        }
        if(getequal){ // lval
            treenode *tempnode = new treenode;
            proot->set_lower_node(tempnode);
            getlval(tempnode);
            proot = tempnode;

            tempnode = pronode(now_pointer++,2);// =
            proot->set_right_node(tempnode);
            proot=tempnode;

            getwordtype(stringtype);
            if(strcmp(stringtype,valueRemain[8])==0){
                for(int j=0;j<4;j++){
                    tempnode = pronode(now_pointer++,2);
                    proot->set_right_node(tempnode);
                    proot = tempnode;
                }
            }else{
                tempnode = new treenode;
                getexp(tempnode);
                proot->set_right_node(tempnode);
                proot=tempnode;

                tempnode = pronode(now_pointer++,2);
                proot->set_right_node(tempnode);
                proot = tempnode;
            }
        }else{
            getwordtype(stringtype);
            if(strcmp(stringtype,otherValue[15])!=0){//;
                treenode * tempnode = new treenode;
                getexp(tempnode);
                proot->set_lower_node(tempnode);
                proot=tempnode;
                tempnode = pronode(now_pointer++,2);
                proot->set_right_node(tempnode);
                proot=tempnode;
            }else{
                treenode* tempnode = pronode(now_pointer++,2);
                proot->set_lower_node(tempnode);
                proot=tempnode;
            }
        }
    }*/
}

void clean_treenode(treenode* proot){
    treenode * lower = proot->lowernode;
    treenode * right = proot->rightnode;
    delete proot;
    if(lower!=NULL){
        clean_treenode(lower);
    }
    if(right!=NULL){
        clean_treenode(right);
    }
}

int getformatstring(treenode* proot){
    proot->settype(grammer_type[27]);
    treenode * tempnode = pronode(now_pointer++,2);
    proot->set_lower_node(tempnode);
    int m = 1 ;// 0
    int count=0;
    char * temp = tempnode->word->value;
    while(temp[m]!='"'){
        if(temp[m]=='\\'){
            if(temp[m+1]!='n'){
                return -1;
            }
        }else if (temp[m]=='%') {
            if (temp[m + 1] == 'd') {
                count++;
            } else return -1;
        }
        else if(!(temp[m]<=33&&temp[m]>=32||temp[m]<=126&&temp[m]>=40)){
            return -1;
        }
        m++;
    }
    return count;
}

void getcond(treenode* proot){
    proot->settype(grammer_type[25]);
    treenode* tempnode = new treenode;
    proot->set_lower_node(tempnode);
    getlorexp(tempnode);
    return ;
}

void getlorexp(treenode* proot){
    proot->settype(grammer_type[28]);

    treenode * tempnode  = new treenode;
    treenode * pproot = tempnode;
    treenode * tempnode2 = tempnode;

    getlandexp(tempnode);

    char stringtype[15];
    getwordtype(stringtype);

    while(strcmp(stringtype,otherValue[2])==0){
        tempnode = new treenode;
        tempnode->settype(grammer_type[28]);
        tempnode->set_lower_node(pproot);
        pproot = tempnode;

        tempnode2 = pronode(now_pointer++,2); // ||
        pproot->set_right_node(tempnode2);

        tempnode = new treenode;
        getlandexp(tempnode);
        tempnode2->set_right_node(tempnode);
        getwordtype(stringtype);
    }

    proot->set_lower_node(pproot);
    return ;
}

void getlandexp(treenode* proot){
    proot->settype(grammer_type[29]);

    treenode * tempnode  = new treenode;
    treenode * pproot = tempnode;
    treenode * tempnode2 = tempnode;

    geteqexp(tempnode);

    char stringtype[15];
    getwordtype(stringtype);

    while(strcmp(stringtype,otherValue[1])==0){
        tempnode = new treenode;
        tempnode->settype(grammer_type[29]);
        tempnode->set_lower_node(pproot);
        pproot = tempnode;

        tempnode2 = pronode(now_pointer++,2); // ||
        pproot->set_right_node(tempnode2);

        tempnode = new treenode;
        geteqexp(tempnode);
        tempnode2->set_right_node(tempnode);
        getwordtype(stringtype);
    }

    proot->set_lower_node(pproot);
    return ;
}

void geteqexp(treenode* proot){
    proot->settype(grammer_type[30]);

    treenode * tempnode  = new treenode;
    treenode * pproot = tempnode;
    treenode * tempnode2 = tempnode;

    getrelexp(tempnode);

    char stringtype[15];
    getwordtype(stringtype);

    while(strcmp(stringtype,otherValue[12])==0||strcmp(stringtype,otherValue[13])==0){
        tempnode = new treenode;
        tempnode->settype(grammer_type[30]);
        tempnode->set_lower_node(pproot);
        pproot = tempnode;

        tempnode2 = pronode(now_pointer++,2); // ||
        pproot->set_right_node(tempnode2);

        tempnode = new treenode;
        getrelexp(tempnode);
        tempnode2->set_right_node(tempnode);
        getwordtype(stringtype);
    }

    proot->set_lower_node(pproot);
    return ;
}

void getrelexp(treenode* proot){
    proot->settype(grammer_type[31]);

    treenode * tempnode  = new treenode;
    treenode * pproot = tempnode;
    treenode * tempnode2 = tempnode;

    getaddexp(tempnode);

    char stringtype[15];
    getwordtype(stringtype);

    while(strcmp(stringtype,otherValue[8])==0||strcmp(stringtype,otherValue[9])==0||strcmp(stringtype,otherValue[10])==0||strcmp(stringtype,otherValue[11])==0){
        tempnode = new treenode;
        tempnode->settype(grammer_type[31]);
        tempnode->set_lower_node(pproot);
        pproot = tempnode;

        tempnode2 = pronode(now_pointer++,2); // ||
        pproot->set_right_node(tempnode2);

        tempnode = new treenode;
        getaddexp(tempnode);
        tempnode2->set_right_node(tempnode);
        getwordtype(stringtype);
    }

    proot->set_lower_node(pproot);
    return ;
}

void getfunctype(treenode * proot){
    proot->settype(grammer_type[20]);
    treenode * tempnode  = pronode(now_pointer++,2);
    proot->set_lower_node(tempnode);
    return;
}
void getmainfuncdef(treenode* proot){
    proot->settype(grammer_type[19]);

    treenode *tempnode = pronode(now_pointer++,2);
    proot->set_lower_node(tempnode);
    proot = tempnode;

    tempnode = pronode(now_pointer++,2);
    proot->set_right_node(tempnode);
    proot=tempnode;

    tempnode = pronode(now_pointer++,2);
    proot->set_right_node(tempnode);
    proot=tempnode;

    try{
        tempnode = get_rparent();
        proot->set_right_node(tempnode);
        proot=tempnode;
    }catch (const char *msg){
        err[error_number] = new err_pair('j', search_last_line(proot));
        error_number++;
    }

    tempnode =new treenode;
    getblock(tempnode);
    proot->set_right_node(tempnode);
    return ;
}

void getdecl(treenode* proot){
    char valuetemp[15];
    getwordvalue(valuetemp);
    if(valuetemp[0]=='c'){ // const int
        proot->settype(grammer_type[1]);
        treenode* tempnode = pronode(now_pointer++,2);//const
        proot->set_lower_node(tempnode);
        proot= tempnode;
        tempnode = pronode(now_pointer++,2);//int
        proot->set_right_node(tempnode);
        proot=tempnode;
        tempnode = new treenode;
        getconstdef(tempnode);//constdef
        proot->set_right_node(tempnode);
        proot = tempnode;
        char stringtype[15];
        getwordtype(stringtype);
        while(strcmp(stringtype,otherValue[16])==0){
            tempnode = pronode(now_pointer++,2);
            proot->set_right_node(tempnode);
            proot = tempnode;
            tempnode = new treenode;
            getconstdef(tempnode);
            proot->set_right_node(tempnode);
            proot = tempnode;
            getwordtype(stringtype);
        }
        try {
            tempnode = get_semic();
            proot->set_right_node(tempnode);
        }
        catch (const char * msg){
            err[error_number] = new err_pair('i', search_last_line(proot));
            error_number++;
        }
        //tempnode = pronode(now_pointer++,2);
        return ;
    }
    else { // int
        proot->settype(grammer_type[15]);
        treenode* tempnode=pronode(now_pointer++,2);
        proot->set_lower_node(tempnode);
        proot= tempnode;

        tempnode = new treenode;
        proot->set_right_node(tempnode);
        getvardef(tempnode);//valdef
        proot = tempnode;

        char stringtype[15];
        getwordtype(stringtype);
        while(strcmp(stringtype,otherValue[16])==0){
            tempnode = pronode(now_pointer++,2);
            proot->set_right_node(tempnode);
            proot = tempnode;

            tempnode = new treenode;
            proot->set_right_node(tempnode);
            getvardef(tempnode);
            proot = tempnode;
            getwordtype(stringtype);
        }
        try {
            tempnode = get_semic();
            proot->set_right_node(tempnode);
        }
        catch (const char * msg){
            err[error_number] = new err_pair('i', search_last_line(proot));
            error_number++;
        }
        return ;
    }
}

void getvardef(treenode* proot){
    proot->settype(grammer_type[16]);
    treenode * tempnode = pronode(now_pointer++,2);
    proot->set_lower_node(tempnode);
    proot = tempnode;
    char stringtype[15];
    getwordtype(stringtype);
    while(strcmp(stringtype,otherValue[19])==0){
        tempnode = pronode(now_pointer++,2);//[
        proot->set_right_node(tempnode);
        proot= tempnode;//[

        tempnode = new treenode;
        proot->set_right_node(tempnode);
        getconstexp(tempnode);
        proot = tempnode; // constexp
        try {
            tempnode = get_rbrack();
            proot->set_right_node(tempnode);
            proot = tempnode;
        }catch (const char * msg){
            err[error_number] = new err_pair('k', search_last_line(proot));
            error_number++;
        }
        getwordtype(stringtype);
    }
    if(strcmp(stringtype,otherValue[14])==0){
        tempnode = pronode(now_pointer++,2);// =
        proot->set_right_node(tempnode);
        proot=tempnode;
        tempnode = new treenode;
        getinitval(tempnode);
        proot->set_right_node(tempnode);
    }
    return ;
}

void getinitval(treenode* proot){
    proot->settype(grammer_type[17]);
    char stringtype[15];
    getwordtype(stringtype);
    if(strcmp(stringtype,otherValue[21])==0){
        treenode* tempnode = pronode(now_pointer++,2);//{
        proot->set_lower_node(tempnode);
        proot = tempnode;//{

        getwordtype(stringtype);
        if(strcmp(stringtype,otherValue[22])==0){
            tempnode=pronode(now_pointer++,2);
            proot->set_right_node(tempnode);
            return ;
        }

        tempnode = new treenode;
        proot->set_right_node(tempnode);
        getinitval(tempnode);
        proot = tempnode;// initival

        getwordtype(stringtype);
        while(strcmp(stringtype,otherValue[16])==0){ //,
            tempnode = pronode(now_pointer++,2);//,
            proot->set_right_node(tempnode);
            proot = tempnode;

            tempnode = new treenode;
            proot->set_right_node(tempnode);
            getinitval(tempnode);
            proot = tempnode;//initval

            getwordtype(stringtype);
        }
        tempnode = pronode(now_pointer++,2);
        proot->set_right_node(tempnode);
        return ;
    }
    else{
        treenode * tempnode = new treenode;
        proot->set_lower_node(tempnode);
        getexp(tempnode);
        return ;
    }
}

void getconstdef(treenode* proot){
    proot->settype(grammer_type[3]);
    char valuetemp[15];
    treenode* tempnode = pronode(now_pointer++,2);//ident

    proot->set_lower_node(tempnode);

    proot = tempnode;//ident
    getwordtype(valuetemp);

    while(strcmp(valuetemp,otherValue[19])==0){
        tempnode = pronode(now_pointer++,2);//[
        proot->set_right_node(tempnode);
        proot= tempnode;//[

        tempnode = new treenode;
        proot->set_right_node(tempnode);
        getconstexp(tempnode);
        proot = tempnode; // constexp

        try{
            tempnode = get_rbrack();
            proot->set_right_node(tempnode);
            proot = tempnode;
        }catch (const char * msg){
            err[error_number] = new err_pair('k', search_last_line(proot));
            error_number++;
        }
        getwordtype(valuetemp);
    }

    // =
    tempnode = pronode(now_pointer++,2);// =
    proot->set_right_node(tempnode);
    proot=tempnode;

    tempnode = new treenode;
    getconinitval(tempnode);
    proot->set_right_node(tempnode);
    return ;
}

void getconinitval(treenode* proot){
    proot->settype(grammer_type[14]);
    char stringtype[15];
    getwordtype(stringtype);
    if(strcmp(stringtype,otherValue[21])==0){//{
        treenode * tempnode = pronode(now_pointer++,2);
        proot->set_lower_node(tempnode);
        proot = tempnode;//{

        getwordtype(stringtype);
        if(strcmp(stringtype,otherValue[22])==0){//}
            tempnode=pronode(now_pointer++,2);
            proot->set_right_node(tempnode);
            return ;
        }
        tempnode = new treenode;
        proot->set_right_node(tempnode);
        getconinitval(tempnode);
        proot = tempnode;// initival

        getwordtype(stringtype);

        while(strcmp(stringtype,otherValue[16])==0){ //,
            tempnode = pronode(now_pointer++,2);//,
            proot->set_right_node(tempnode);
            proot = tempnode;
            tempnode = new treenode;
            proot->set_right_node(tempnode);
            getconinitval(tempnode);
            proot = tempnode;//coninitval
            getwordtype(stringtype);
        }
        tempnode = pronode(now_pointer++,2);
        proot->set_right_node(tempnode);
        return ;
    }
    else {
        treenode* tempnode=new treenode;
        getconstexp(tempnode);
        proot->set_lower_node(tempnode);
        return ;
    }
}

void getconstexp(treenode * proot){
    proot->settype(grammer_type[4]);
    treenode * tempnode = new treenode;
    proot->set_lower_node(tempnode);
    getaddexp(tempnode);
    return ;
}

void getaddexp(treenode* proot){
    proot->settype(grammer_type[5]);
    treenode* pproot = new treenode;
    getmulexp(pproot);
    char typestring[15];
    getwordtype(typestring);
    while(strcmp(typestring,otherValue[4])==0||strcmp(typestring,otherValue[3])==0){
        treenode* tempnode = new treenode;
        tempnode->set_lower_node(pproot);
        tempnode->settype(grammer_type[5]);
        pproot = tempnode;
        tempnode = pronode(now_pointer++,2);// + / -
        pproot->set_right_node(tempnode);
        treenode * tempnode2 = tempnode;
        tempnode = new treenode;
        getmulexp(tempnode);
        tempnode2->set_right_node(tempnode);
        getwordtype(typestring);
    }
    proot->set_lower_node(pproot);
}

// finish
void getmulexp(treenode* proot){
    proot->settype(grammer_type[6]);
    treenode* pproot = new treenode;
    getunaryexp(pproot);
    char typestring[15];
    getwordtype(typestring);
    while(strcmp(typestring,otherValue[5])==0||strcmp(typestring,otherValue[6])==0||strcmp(typestring,otherValue[7])==0){
        treenode* tempnode = new treenode;
        tempnode->set_lower_node(pproot);
        tempnode->settype(grammer_type[6]);
        pproot = tempnode;
        tempnode = pronode(now_pointer++,2);// * / %
        pproot->set_right_node(tempnode);
        treenode * tempnode2 = tempnode;
        tempnode = new treenode;
        getunaryexp(tempnode);
        tempnode2->set_right_node(tempnode);
        getwordtype(typestring);
    }
    proot->set_lower_node(pproot);
    return ;
}

bool is_exp_start(char* inp){
    int a1 = strcmp(inp,otherValue[3]);
    int a2 = strcmp(inp,otherValue[4]);
    int a3 = strcmp(inp,otherValue[17]);
    int a4 = strcmp(inp,tritype[0]);
    int a5 = strcmp(inp,tritype[2]);
    int a6 = strcmp(inp,otherValue[0]);
    if((a1*a2*a3*a4*a5*a6)==0){
        return true;
    }
    else return false;
}

void getunaryexp(treenode* proot){
    proot->settype(grammer_type[7]);
    char typetemp[15];
    getwordtype(typetemp);
    int a1 = strcmp(typetemp,otherValue[0]);
    int a2 = strcmp(typetemp,otherValue[3]);
    int a3 = strcmp(typetemp,otherValue[4]);
    if(a1==0||a2==0||a3==0){
        treenode * tempnode = new treenode;
        proot->set_lower_node(tempnode);
        getunaryop(tempnode);
        proot = tempnode;
        tempnode= new treenode;
        proot->set_right_node(tempnode);
        getunaryexp(tempnode);
        return ;
    }
    else {
        char temptype[15];
        if(strcmp(typetemp,tritype[0])==0){ //ident
            getwordtype(temptype,1);
            if(strcmp(temptype,otherValue[17])==0){ //(
                treenode *tempnode = pronode(now_pointer++,2);//ident
                proot->set_lower_node(tempnode);
                proot = tempnode;

                tempnode = pronode(now_pointer++,2);
                proot->set_right_node(tempnode);//(
                proot = tempnode;

                getwordtype(temptype);

                if(strcmp(temptype,otherValue[18])!=0){// )
                    if(is_exp_start(temptype)) {
                        tempnode = new treenode;
                        proot->set_right_node(tempnode);
                        getfuncparams(tempnode);
                        proot = tempnode;
                    }
                }
                try {
                    tempnode = get_rparent();
                    proot->set_right_node(tempnode);
                }catch(const char *msg){
                    err[error_number] = new err_pair('j', search_last_line(proot));
                    error_number++;
                }
                return ;
            }
        }
        treenode * tempnode = new treenode;
        proot->set_lower_node(tempnode);
        getprimaryexp(tempnode);
        return;
    }
}

void getprimaryexp(treenode* proot){
    proot->settype(grammer_type[11]);
    char tempstring[15];
    getwordtype(tempstring);
    if(strcmp(tempstring,otherValue[17])==0){
        treenode* tempnode = pronode(now_pointer++,2);
        proot->set_lower_node(tempnode);
        proot=tempnode; //
        tempnode = new treenode;
        getexp(tempnode);
        proot->set_right_node(tempnode);
        proot = tempnode; //
        try {
            tempnode = pronode(now_pointer++, 2);
            proot->set_right_node(tempnode);
        }catch (const char * msg){
            err[error_number] = new err_pair('j', search_last_line(proot));
            error_number++;
        }
        return ;
    }else if(strcmp(tempstring,tritype[2])==0){
        treenode* tempnode = new treenode;
        proot->set_lower_node(tempnode);
        getnumber(tempnode);
        return ;
    }else{
        treenode* tempnode = new treenode;
        proot->set_lower_node(tempnode);
        getlval(tempnode);
        return ;
    }
}

void getlval(treenode* proot){
    proot->settype(grammer_type[13]);
    treenode * tempnode = pronode(now_pointer++,2);
    proot->set_lower_node(tempnode);
    proot=tempnode;
    char stringtype[15];
    getwordtype(stringtype);
    while(strcmp(stringtype,otherValue[19])==0){

        tempnode = pronode(now_pointer++,2);//[
        proot->set_right_node(tempnode);
        proot = tempnode;//[
        tempnode = new treenode;
        proot->set_right_node(tempnode);
        getexp(tempnode);
        proot = tempnode;//exp
        try {
            tempnode = get_rbrack(); //]
            proot->set_right_node(tempnode);//]
            proot = tempnode;
        }catch (const char * msg){
            err[error_number] = new err_pair('k', search_last_line(proot));
            error_number++;
        }
        getwordtype(stringtype);
    }
    return ;
}

void getnumber(treenode* proot){
    proot->settype(grammer_type[12]);
    treenode *tempnode = pronode(now_pointer++,2);
    proot->set_lower_node(tempnode);
    return;
}

void getfuncparams(treenode * proot){
    proot->settype(grammer_type[9]);
    treenode *tempnode = new treenode;
    proot->set_lower_node(tempnode);
    getexp(tempnode);
    char temptype[15];
    getwordtype(temptype);
    while(strcmp(temptype,otherValue[16])==0){//�Ƿ���ڣ�
        proot = tempnode;
        tempnode = pronode(now_pointer++,2);
        proot->set_right_node(tempnode);
        proot= tempnode;
        tempnode = new treenode;
        proot->set_right_node(tempnode);
        getexp(tempnode);
        getwordtype(temptype);
    }
    return ;
}

void getexp(treenode * proot){
    proot->settype(grammer_type[10]);
    treenode * tempnode = new treenode;
    proot->set_lower_node(tempnode);
    getaddexp(tempnode);
    return ;
}

void getunaryop(treenode* proot){
    proot->settype(grammer_type[8]);
    treenode* tempnode = pronode(now_pointer++,2);
    proot->set_lower_node(tempnode);
    return ;
}


