#include <iostream>
#include "A.h"
#include<string.h>
extern char valueRemain[12][15];
extern char otherValue[23][15];
int examindimension(treenode* proot);
void delete_table();
void compare_para(params * paramlist,treenode * proot,int line);
int getexpdi(treenode * proot);
int getdifromtable(char * name,char *type);
table_term* getaterm(char *name,char *type);
int find_return(treenode* proot,char * type);
int find_block_last(treenode* proot);
void write_table(treenode* proot);
bool  find_last_return(treenode * proot);

static table_term * terms[10000];
static int table_top =0;
static int level_list[1000];
static int level_top=0;

extern err_pair * err[1000];
extern int error_number;
char error_process[2][20]={"FUNCTION","VARIABLE"};
extern char valueRemain[12][15];
extern char otherValue[23][15];
extern char grammer_type[32][20];
extern char tritype[3][15];
extern char grammer_type[32][20];
extern char specialString[20];

bool search_redefine(char * name, char * type){
    int top = level_list[level_top];//第一个
    for(int m=table_top-1;m>=top;m--){
        if(strcmp(terms[m]->name,name)==0&& strcmp(terms[m]->type,type)==0){
            return true;
        }
    }
    return false;
}
bool search_define(char * name,char *type){
    for(int m=table_top-1;m>=0;m--){
        if(strcmp(terms[m]->name,name)==0&& strcmp(terms[m]->type,type)==0){
            return true;
        }
    }
    return false;
}

params * make_param_list(treenode * proot){

    proot = proot->lowernode;

    int di =examindimension(proot);
    params* head = new params(di);
    params* temppara = head;
    params* lpara = head;
    while(proot->rightnode!=NULL){
        proot = proot->rightnode;
        proot=proot->rightnode;
        di = examindimension(proot);
        temppara = new params(di);
        lpara->set_next(temppara);
        lpara=temppara;
    }
    return head;
}

int  examindimension(treenode * proot){
    proot= proot->lowernode;
    proot=proot->rightnode;
    wordpair * word = proot->word;
    int count = 2;
    while (proot->rightnode != NULL) {
        proot = proot->rightnode;
        count++;
    }
    int l;
    if (count == 2) {
        l=0;
    } else if (count <= 5) {
        l=1;
    } else l=2;
    if(search_redefine(word->value,error_process[1])){
        err[error_number++] = new err_pair('b',word->linenumber);
    }else {
        terms[table_top++]=new table_term(error_process[1],word->value,valueRemain[2],l,NULL);
    }
    return l;
}

void write_table(treenode * proot){
    bool success_decl= false;
    static int in_while=0;
    treenode * nextnode  ;
    nextnode = proot->lowernode;

    if(strcmp(proot->type,specialString)==0){
        in_while++;
    }
    else if(strcmp(proot->type,grammer_type[2])==0){
        if(strcmp(proot->word->word_type,valueRemain[3])==0||strcmp(proot->word->word_type,valueRemain[4])==0){
            if(in_while==0){
                err[error_number++] =new err_pair('m',proot->word->linenumber);
            }
        }
    }
        // const def
    else if(strcmp(proot->type,grammer_type[3])==0){ //constdef
        treenode* tempnode = proot->lowernode; //idnet
        wordpair* pair = tempnode->word;

        int dimension=0;
        while(tempnode->rightnode!=NULL){
            tempnode = tempnode->rightnode;
            if(tempnode->word!=NULL) {
                if (strcmp(tempnode->word->word_type, otherValue[19]) == 0) {
                    dimension++;
                }
            }
        }
        if(search_redefine(pair->value,error_process[1])){
            err[error_number]=new err_pair('b',pair->linenumber);
            error_number++;
        }else {
            terms[table_top] = new table_term
                    (error_process[1], pair->value, valueRemain[1], dimension, NULL);
            success_decl= true;
        }
    }

        //vardef
    else if(strcmp(proot->type,grammer_type[16])==0) { //vardef
        treenode* tempnode = proot->lowernode; //idnet
        wordpair* pair = tempnode->word;

        int dimension=0;
        while(tempnode->rightnode!=NULL){
            tempnode = tempnode->rightnode;
            if(tempnode->word!=NULL) {
                if (strcmp(tempnode->word->word_type, otherValue[19]) == 0) {
                    dimension++;
                }
            }
        }
        if(search_redefine(pair->value,error_process[1])){
            err[error_number]=new err_pair('b',pair->linenumber);
            error_number++;
        }else {
            terms[table_top] = new table_term
                    (error_process[1], pair->value, valueRemain[2], dimension, NULL);
            success_decl= true;
        }
    }

        // funcdef
    else if(strcmp(proot->type,grammer_type[18])==0|| strcmp(proot->type,grammer_type[19])==0){//funcdef
        treenode* tempnode  = proot->lowernode; // ftype
        wordpair * pair;
        if(strcmp(proot->type,grammer_type[19])==0){
            pair = tempnode->word;
        }else {
            pair = tempnode->lowernode->word;
        }
        char return_type [15];
        strcpy(return_type,pair->word_type); //ftype
        int last_line = find_block_last(tempnode);
        int l =find_return(tempnode,return_type);
        /* if(l==-1){
             if(strcmp(return_type,valueRemain[2])==0){

                 err[error_number++] =new err_pair('g',last_line);
             }
         }*/


        tempnode = tempnode->rightnode; // ident
        wordpair * pair2 = tempnode->word;

        if(search_redefine(pair2->value,error_process[0])) {
            err[error_number++]=new err_pair('b',pair2->linenumber);
        }
        tempnode = tempnode->rightnode; //(
        tempnode = tempnode->rightnode;//block, ), params
        int loc = table_top;
        table_top++;
        level_top++;
        level_list[level_top] = table_top;

        params * funcparams;

        if (strcmp(tempnode->type, grammer_type[21]) == 0) {
            funcparams = make_param_list(tempnode);
            tempnode = tempnode->rightnode;
        } else {
            funcparams = NULL;
        }
        if (strcmp(tempnode->type, grammer_type[2]) == 0) {
            tempnode = tempnode->rightnode;
        }
        nextnode = tempnode->lowernode;// block的 {
        if(!find_last_return(nextnode)){
            if(strcmp(return_type,valueRemain[2])==0){
                err[error_number++] =new err_pair('g',last_line);
            }
        }
        terms[loc] = new table_term(error_process[0], pair2->value,return_type,0,funcparams);
    }
    else if(strcmp(proot->type,grammer_type[23])==0){
        level_list[++level_top]=table_top;
    }
    else if(strcmp(proot->type,grammer_type[7])==0){ // unaryexp

        treenode* tempnode = proot->lowernode;
        if(strcmp(tempnode->type,grammer_type[2])==0){// final ident
            wordpair * word = tempnode->word;
            if(!search_define(word->value,error_process[0])){ //没有定义函数
                err[error_number++] = new err_pair('c',word->linenumber);
            }else{
                int m = table_top-1;
                for(;m>=0;m--){
                    if(strcmp(terms[m]->name,word->value)==0&& strcmp(terms[m]->type,error_process[0])==0){
                        break;
                    }
                }
                tempnode = tempnode->rightnode; // (
                int line = tempnode->word->linenumber;
                if(tempnode->rightnode!=NULL) {
                    tempnode = tempnode->rightnode;
                    compare_para(terms[m]->paramlist,tempnode,line);
                }else if(terms[m]->paramlist!=NULL){
                    err[error_number++] = new err_pair('d',tempnode->word->linenumber);
                }
            }
        }
    }else if(strcmp(proot->type,grammer_type[13])==0){//lval
        treenode* tempnode = proot->lowernode;
        if(!search_define(tempnode->word->value,error_process[1])){
            err[error_number++]=new err_pair('c',tempnode->word->linenumber);
        }else{
            table_term* terma=getaterm(tempnode->word->value,error_process[1]);
            if(strcmp(terma->value_type,valueRemain[1])==0){
                if(proot->rightnode!=NULL){
                    err[error_number++]=new err_pair('h',tempnode->word->linenumber);
                }
            }
        }
    }
    if(nextnode!=NULL) {
        write_table(nextnode);
    }
    if(strcmp(proot->type,grammer_type[18])==0|| strcmp(proot->type,grammer_type[19])==0||
       strcmp(proot->type,grammer_type[23])==0){
        delete_table() ;
        level_top--;
    }
    if(strcmp(proot->type,grammer_type[16])==0||strcmp(proot->type,grammer_type[3])==0){
        if(success_decl){
            table_top++;
            success_decl = false;
        }
    }
    if(strcmp(proot->type,specialString)==0){
        in_while--;
    }
    if(proot->rightnode!=NULL){
        write_table(proot->rightnode);
    }
}

void delete_table(){
    int m = level_list[level_top];
    for(;table_top>=m;table_top--){
        delete terms[table_top];
        terms[table_top] = NULL;
    }
    table_top++;
}

void compare_para(params * paramlist, treenode * proot,int line ){
    params * now_para = paramlist;
    treenode * tempnode ;
    if(strcmp(proot->type,grammer_type[9])!=0){
        if(paramlist==NULL) return ;
        else{
            err[error_number++] =new err_pair('d',line);
            return ;
        }
    }
    else{
        tempnode=proot->lowernode;
        while(true){
            if(now_para==NULL){
                err[error_number++]=new err_pair('d',line);
                return ;
            }
            int di = getexpdi(tempnode);
            if(di==-2) {
                return;
            }else if(di!=now_para->dimension){
                err[error_number++]= new err_pair('e',line);
                return ;
            }
            now_para=now_para->next_paramterm;
            if(tempnode->rightnode==NULL){
                break;
            }else{
                tempnode = tempnode->rightnode;
                tempnode = tempnode->rightnode;
            }
        }
        if(now_para!=NULL){
            err[error_number++]=new err_pair('d',line);
            return ;
        }
    }
}

int getexpdi(treenode * proot){
    treenode * tempnode = proot;
    while(strcmp(tempnode->type,grammer_type[7])!=0){//uneryexp
        tempnode = tempnode->lowernode;
    }
    tempnode = tempnode->lowernode; // op,pri,ident
    if (strcmp(tempnode->type, grammer_type[2]) == 0) {
        return getdifromtable(tempnode->word->value,error_process[0]);
    }else if(strcmp(tempnode->type,grammer_type[8])==0){
        return 0;
    }else {
        tempnode = tempnode->lowernode;
        if(strcmp(tempnode->type,grammer_type[2])==0){//final
            if(strcmp(tempnode->word->word_type,tritype[2])==0){//number
                return 0;
            }
            else return getexpdi(tempnode->rightnode); //
        }else if(strcmp(tempnode->type,grammer_type[12])==0){
            return 0;
        }else{
            tempnode = tempnode->lowernode; //ident
            wordpair * word = tempnode->word;
            int m =getdifromtable(word->value,error_process[1]);
            int count =1;
            while(tempnode->rightnode!=NULL){
                count ++;
                tempnode =tempnode->rightnode;
            }
            if(count ==1){
                return m;
            }else if(count <=4){
                return m-1;
            }else {
                return m-2;
            }
        }
    }
}

int getdifromtable(char * name,char *type){
    int m =table_top-1;
    for(;m>=0;m--){
        if(strcmp(terms[m]->name,name)==0&&strcmp(terms[m]->type,type)==0){
            break;
        }
    }
    if(m==-1) return -2;
    if(strcmp(terms[m]->value_type,valueRemain[11])==0){
        return -1;
    }else {
        return terms[m]->dimension;
    }
}

table_term* getaterm(char * name, char *type){
    int m = table_top-1;
    for(;m>=0;m--){
        if(strcmp(terms[m]->name,name)==0&&strcmp(terms[m]->type,type)==0){
            break;
        }
    }
    if(m==-1) return NULL;
    else return terms[m];
}

int find_return (treenode* proot,char * return_type){
    treenode* tempnode ;
    if(strcmp(proot->type,grammer_type[2])==0){
        if(strcmp(proot->word->word_type,valueRemain[10])==0){ //return
            if(proot->rightnode==NULL){ // return 没有返回值
                /*if(strcmp(return_type,valueRemain[11])!=0){
                    err[error_number++]=new err_pair('')
                }*/
                return 0;
            }else {
                tempnode  = proot->rightnode;
            }
            if(strcmp(tempnode->type,grammer_type[2])==0){//;
                return 0;
            }else {
                if(strcmp(return_type,valueRemain[11])==0){
                    err[error_number++] =new err_pair('f',proot->word->linenumber);
                }
                return 1; // 有返回值
            }
        }
    }
    if(proot->lowernode==NULL&& proot->rightnode==NULL){
        return -1;
    }else if(proot->lowernode==NULL){
        return find_return((proot->rightnode),return_type);
    }else{
        int low_find = find_return(proot->lowernode,return_type);
        if(low_find==-1){
            if(proot->rightnode==NULL) return -1;
            else return find_return(proot->rightnode,return_type);
        }else{
            return low_find;
        }
    }
}

int find_block_last(treenode* proot) {
    while(proot->rightnode!=NULL){
        proot=proot->rightnode;
    }
    proot=proot->lowernode;
    while(proot->rightnode!=NULL){
        proot=proot->rightnode;
    }
    return proot->word->linenumber;
}

bool find_last_return(treenode* proot){

    while(strcmp(proot->rightnode->type,grammer_type[24])==0){
        proot = proot->rightnode;
    }
    if(strcmp(proot->type,grammer_type[2])==0){
        return false;
    }
    treenode* tempnode= proot->lowernode->lowernode;
    if(strcmp(tempnode->type,grammer_type[2])!=0){
        return false;
    }
    else if(strcmp(tempnode->word->word_type,valueRemain[10])!=0){
        return false;
    }
    return true;
}
