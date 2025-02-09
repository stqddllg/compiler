#include <iostream>
#include "A.h"
#include<string.h>
using namespace std;
extern char valueRemain[12][15];
extern char otherValue[23][15];
extern void analysis_all_word();
extern void analysis_all_grammer();
extern int wordNumber;
       struct wordpair word_pairs[10000];
extern treenode *root;
extern char grammer_type[32][20];

       err_pair * err[1000];
       int error_number=0;

extern char tritype[3][15];
   void printtreenode(treenode* proot);
   void sort_errors();
   void print_all_error();


extern void write_table(treenode* proot);
extern void mid2mips();


extern char grammer_type[32][20];
extern char specialString[20];

extern string mid_codes[20000];
extern int mid_nums;

extern string formate_strings[10000];
extern int formate_top;

extern void gen_mid_code(treenode* proot);

void print_mid_code();
void print_formate_string();
void generate_mips();

extern string data_words[20000]; // data段内容
extern int data_number ;


extern string texts[20000]; //代码段内容
extern int text_number;

int main()
{
    static FILE* outputfile =fopen("output.txt","w+");
    analysis_all_word();

    analysis_all_grammer();
    //write_table(root);
    //sort_errors();
    //print_all_error();

    //printtreenode(root);
    gen_mid_code(root);
    print_formate_string();
    print_mid_code();
    mid2mips();
    generate_mips();
    /*for(int i=0;i<wordNumber;i++){
        fprintf(outputfile,"%s %s %d\n", word_pairs[i].word_type , word_pairs[i].value, word_pairs[i].linenumber);
    }*/
    //fprintf(outputfile,"%s\n",root->type);
    return 0;
}

void generate_mips(){
    FILE * mips_output_file = fopen("mips.txt","w+");
    fprintf(mips_output_file,".data\n");
    for(int i=0;i<data_number;i++){
        fprintf(mips_output_file,"%s\n",data_words[i].c_str());
    }
    fprintf(mips_output_file,".text\n");
    for(int i=0;i<text_number;i++){
        fprintf(mips_output_file,"%s\n",texts[i].c_str());
    }
    fprintf(mips_output_file,"end_zpqm:\n");
}

void print_mid_code(){
    for (int i=0;i<mid_nums;i++){
        cout<<mid_codes[i]<<endl;
    }
}

void print_formate_string(){
    for(int i=0;i<formate_top;i++){
        cout<<formate_strings[i]<<endl;
    }
}


void print_all_error(){
    static FILE* err_file = fopen("error.txt","w+");
    for(int i=0;i<error_number;i++)
    fprintf(err_file,"%d %c\n",err[i]->line,err[i]->type);
}

void sort_errors(){
    err_pair * temp_err;
    for (int i=0;i<error_number;i++){
        for(int j=1;j<error_number-i;j++){
            if((err[j]->line)<(err[j-1]->line)){
                temp_err=err[j];
                err[j]=err[j-1];
                err[j-1]=temp_err;
            }
        }
    }
    return ;
}

void printtreenode(treenode* proot){
    static FILE* outputfile =fopen("output.txt","w+");

    if(strcmp(proot->type,grammer_type[2])==0){
        wordpair *tempword_pair=proot->word;
        fprintf(outputfile,"%s %s\n",tempword_pair->word_type, tempword_pair->value); //

    }else{
        if(proot->lowernode != NULL){
            printtreenode(proot->lowernode);
        }
        if(strcmp(proot->type,grammer_type[24])!=0 && strcmp(proot->type,grammer_type[27])!=0&&strcmp(proot->type,specialString)!=0)
        fprintf(outputfile,"%s\n",proot->type);
        else if(strcmp(proot->type,specialString)==0){
            fprintf(outputfile,"%s\n",grammer_type[26]);
        }
    }
    //treenode* btempnode = proot->rightnode;
    if(proot->rightnode!=NULL){
        printtreenode(proot->rightnode);
    }
}

