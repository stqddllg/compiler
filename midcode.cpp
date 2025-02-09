#include <iostream>
#include "A.h"
#include <stdlib.h>
#include<string.h>
extern char valueRemain[12][15];
extern char otherValue[23][15];
extern char otherString[23][5];
extern treenode *root;
extern char grammer_type[32][20];
extern char tritype[3][15];
extern char grammer_type[32][20];
extern char specialString[20];

static int blockno=0;

string mid_codes[20000];
int mid_nums=0;


sou_mid_table_term * smterms[20000];
static int table_top=0;
static int block_list[500];
static int now_block=0;
string formate_strings[10000];
int formate_top=0;

string while_begin_labels[1000];
string while_end_labels[1000];
static int while_layer=-1;


void  gen_temp_var(char *des);
void  gen_new_var(char * ori_name, char * new_name);
int search_table(char *find_name);
int cacu_add_exp(treenode * proot);
int  cacu_const_exp(treenode* proot);
int cacu_mul_exp(treenode* proot);
int cacu_unary_exp(treenode* proot);
int cacu_primary_exp(treenode* proot);
int cacu_number(treenode* proot);
int cacu_lval(treenode* proot);
void get_coninvial(treenode* proot,int* value,int tdi,int di1,int di2);
void get_const_array(treenode* proot,int * value,int tdi,int di1,int di2);
void pro_con_def(treenode* proot);
void pro_var_def(treenode* proot);
void pro_exp(treenode* proot,char * name_res);
void pro_add_exp(treenode* proot,char * name);
void pro_mul_exp(treenode* proot,char * name);
void pro_unary_exp(treenode* proot,char * name);
void pro_primary_exp(treenode* proot,char * name);
void pro_funcRParams(treenode * proot);
void pro_lval(treenode * proot,char *name);
void pro_fundef(treenode * proot);
void pro_funcFParams(treenode* proot);
void pro_funcFParam(treenode* proot);
void pro_block(treenode* proot);
void destory_table();
void pro_blockitem(treenode* proot);
void pro_statment(treenode* proot);
void pro_formate_string(treenode* proot);
void pro_const_decl(treenode * proot);
void pro_var_decl(treenode * proot);
bool contains_array(char * name);
extern void number_to_string(int value,char * string);
void pro_cond(treenode * proot,char * label_cond_false,char * label_cond_true);
void gen_new_label(char * des,int type);
void pro_lor_exp(treenode* proot,char * label_cond_true);
void pro_land_exp(treenode* proot, char * label_land_false);
void pro_eq_exp(treenode* proot,char * result);
void pro_real_exp(treenode* proot,char * result);

void gen_new_var(char * ori_name, char * new_name){
    static int var_counter=0;
    char temp[20];
    temp[0]='v';
    temp[1]='\0';
    char temp1[20];
    //itoa(var_counter++,temp1,10);
    number_to_string(var_counter++,temp1);
    strcat(temp,temp1);
    temp1[0]='b';
    temp1[1]='\0';
    strcat(temp,temp1);
    /*
    itoa(blockno,temp1,10);
    strcat(temp,temp1);
     */
    strcpy(new_name,temp);
    return ;
}

void  gen_temp_var(char * des){
    static int counter=0;
    char temp[20];
    des[0]='t';
    des[1]='\0';
    //itoa(counter++,temp,10);
    number_to_string(counter++,temp);
    strcat(des,temp);
    return;
}

void gen_formate_sting_number(char *des){
    static int counter;
    char temp[20];
    char pro[]="formate";
    strcpy(des,pro);
    //itoa(counter++,temp,10);
    number_to_string(counter++,temp);
    strcat(des,temp);
    return;
}


void gen_mid_code(treenode * proot){
    treenode * nextnode = proot->lowernode;
    if(strcmp(proot->type,grammer_type[1])==0){
        pro_const_decl(proot);
        nextnode = NULL;
    }else if (strcmp(proot->type,grammer_type[15])==0){
        pro_var_decl(proot);
        nextnode = NULL;
    }else if (strcmp(proot->type,grammer_type[18])==0){
        pro_fundef(proot);
        nextnode=NULL;
    }else if (strcmp(proot->type,grammer_type[19])==0){
        pro_fundef(proot);
        nextnode=NULL;
    }

    if(nextnode!=NULL){
        gen_mid_code(nextnode);
    }
    if(proot->rightnode!=NULL){
        gen_mid_code(proot->rightnode);
    }
}

void pro_const_decl(treenode * proot){
    treenode * tempnode = proot->lowernode->rightnode->rightnode;
    pro_con_def(tempnode);
    tempnode = tempnode->rightnode;
    while(strcmp(tempnode->word->word_type,otherValue[16])==0){
        tempnode = tempnode->rightnode;
        pro_con_def(tempnode);
        tempnode=tempnode->rightnode;
    }
}

void pro_var_decl(treenode* proot){
    treenode * tempnode = proot->lowernode->rightnode;
    pro_var_def(tempnode);
    tempnode=tempnode->rightnode;
    while(strcmp(tempnode->word->word_type,otherValue[16])==0){
        tempnode = tempnode->rightnode;
        pro_var_def(tempnode);
        tempnode=tempnode->rightnode;
    }
}

void pro_fundef(treenode * proot){
    treenode *tempnode = proot->lowernode;
    string  str1;
    if(strcmp(tempnode->type,grammer_type[20])==0) {//functype
        str1 = tempnode->lowernode->word->value;
    }else{
        str1 = tempnode->word->value;
    }
    tempnode=tempnode->rightnode;//funcname
    str1 = str1+" "+tempnode->word->value+" ("+")";
    mid_codes[mid_nums++]=str1;
    tempnode=tempnode->rightnode->rightnode;
    now_block++;
    block_list[now_block]=table_top;
    if(strcmp(tempnode->type,grammer_type[21])==0){ //
        pro_funcFParams(tempnode);
        tempnode=tempnode->rightnode;
    }
    tempnode=tempnode->rightnode;
    pro_block(tempnode);
    string str12 = "FunFin";
    mid_codes[mid_nums++] = str12;
    destory_table();
    now_block--;
}
void destory_table(){
    int i;
    for(i=table_top-1;i>=block_list[now_block];i--){
        delete smterms[i];
        smterms[i]=NULL;
    }
    block_list[now_block]=0;
    table_top=i+1;
}

void pro_block(treenode* proot){
    proot=proot->lowernode->rightnode;//"{"
    if(proot->rightnode==NULL){
        return ;
    }else{
        pro_blockitem(proot);
    }
    while(proot->rightnode!=NULL){
        proot=proot->rightnode;
        if(strcmp(proot->type,grammer_type[24])==0) {
            pro_blockitem(proot);
        }
    }
}

void pro_blockitem(treenode* proot){
    proot=proot->lowernode;
    if(strcmp(proot->type,grammer_type[1])==0){ // constdecl
        pro_const_decl(proot);
    }else if (strcmp(proot->type,grammer_type[15])==0){ // vardecl
        pro_var_decl(proot);
    }else if (strcmp(proot->type,grammer_type[26])==0){
        pro_statment(proot);
    } //
}

void pro_statment(treenode* proot){
    proot=proot->lowernode;
    if(strcmp(proot->type,grammer_type[13])==0){ //lval
        char temp[20];
        pro_lval(proot,temp);
        proot=proot->rightnode->rightnode;
        char temp2[20];
        if(strcmp(proot->type,grammer_type[10])==0){ //lval =exp
            pro_exp(proot,temp2);
            string str1 = temp;
            str1=str1+" = "+temp2;
            mid_codes[mid_nums++]=str1;
        } else if(strcmp(proot->type,grammer_type[2])==0){ //
            string str1 = temp;
            str1 =str1+" = getint()";
            mid_codes[mid_nums++]=str1;
        }
    }else if(strcmp(proot->type,grammer_type[10])==0){ //exp
        char temp[20];
        pro_exp(proot,temp);
    }else if (strcmp(proot->type,grammer_type[23])==0){ //block
        now_block++;
        block_list[now_block]=table_top;
        pro_block(proot);
        destory_table();
        now_block--;
    }else if(strcmp(proot->type,grammer_type[2])==0){
        //
        char wtype[15];
        strcpy(wtype,proot->word->word_type);
        if(strcmp(wtype,otherValue[16])==0){
            return ;
        }else if(strcmp(wtype,valueRemain[10])==0){ // return
            proot =proot->rightnode;
            if(strcmp(proot->type,grammer_type[10])==0){
                char temp[20];
                pro_exp(proot,temp);
                string str1 = "RET ";
                str1=str1+temp;
                mid_codes[mid_nums++] = str1;
            }else{
                string str1 = "RET 0";
                mid_codes[mid_nums++]=str1;
            }
        }else if (strcmp(wtype,valueRemain[9])==0){ //printf
            proot=proot->rightnode->rightnode;
            pro_formate_string(proot);
        }else if (strcmp(wtype,valueRemain[5])==0){ //if
            proot=proot->rightnode->rightnode;

            char label[20];
            gen_new_label(label,1); // label of cond == false

            char label1[20];
            gen_new_label(label1,0);// label of cond==true;

            pro_cond(proot,label,label1);

            string str1;
            str1 = label1;
            str1 = str1+":";
            mid_codes[mid_nums++] = str1;

            proot = proot->rightnode->rightnode;
            pro_statment(proot);

            if(proot->rightnode!=NULL){
                char label2[20];
                gen_new_label(label2,2); //

                str1 = "jump ";
                str1 =str1 + label2;
                mid_codes[mid_nums++] = str1;

                str1 = label;
                str1 = str1+":";
                mid_codes[mid_nums++]=str1;

                proot =proot->rightnode->rightnode;
                pro_statment(proot);
                str1 = label2;
                str1 = str1+":";
                mid_codes[mid_nums++]=str1;
            }else{
                str1 = label;
                str1 = str1+":";
                mid_codes[mid_nums++]=str1;
            }
        }else if(strcmp(wtype,valueRemain[7])==0){//while
            proot=proot->rightnode->rightnode;

            char label[20]; // while end_label
            gen_new_label(label,2);

            char label2[20]; // while begin
            gen_new_label(label2,3);

            char label3[20];
            gen_new_label(label3,3);

            pro_cond(proot,label,label2);

            string str1;

            str1 = label2;
            str1 = str1+":";
            mid_codes[mid_nums++]=str1; // label_begin_loop:

            treenode * tempnode;
            tempnode = proot->rightnode->rightnode;
            while_begin_labels[++while_layer]=label3; //begin
            while_end_labels[while_layer]=label; //end
            pro_statment(tempnode);
            while_layer--;

            str1 = label3;
            str1=str1+":";
            mid_codes[mid_nums++]=str1;

            pro_cond(proot,label,label2);

            str1 = label;
            str1=str1+":";
            mid_codes[mid_nums++]=str1;

        }else if(strcmp(wtype,valueRemain[3])==0){//break;
            string str1 = "jump ";
            str1 = str1+while_end_labels[while_layer];
            mid_codes[mid_nums++] = str1;
        }else if(strcmp(wtype,valueRemain[4])==0){//continue
            string str1 = "jump ";
            str1 = str1 + while_begin_labels[while_layer];
            mid_codes[mid_nums++]=str1;
        }
    }
}

void gen_new_label(char * des,int type){
    static int counter =0;
    if(type==0){ // true label
        string str1 = "lab_tru_";
        char temp[20] ;
        number_to_string(counter++,temp);
        str1 = str1 + temp;
        strcpy(des,str1.c_str());
    }else if (type==1){// false label
        string str1  ="lab_fal_";
        char temp[20];
        number_to_string(counter++,temp);
        str1 = str1 + temp;
        strcpy(des,str1.c_str());
    }else if(type==2){
        string str1 = "lab_end_";
        char temp[20];
        number_to_string(counter++,temp);
        str1 = str1 + temp;
        strcpy(des,str1.c_str());
    }else if (type==3){
        string str1 = "lab_lpbe_";
        char temp[20];
        number_to_string(counter++,temp);
        str1 = str1 + temp;
        strcpy(des,str1.c_str());
    }else if(type ==4){
        string str1 = "lab_aend_";
        char temp[20];
        number_to_string(counter++,temp);
        str1 = str1 + temp;
        strcpy(des,str1.c_str());
    }else if(type==5){
        string str1="lab_afal_";
        char temp[20];
        number_to_string(counter++,temp);
        str1 = str1 + temp;
        strcpy(des,str1.c_str());
    }
}

void pro_cond(treenode * proot,char * label_cond_false,char * label_cond_true){
    proot= proot->lowernode;
    char temp[20];
    pro_lor_exp(proot,label_cond_true);

    string str1 = "jump ";
    str1 = str1+label_cond_false;
    mid_codes[mid_nums++]=str1;



}

void pro_lor_exp(treenode* proot,char * label_cond_true){
    proot=proot->lowernode;
    if(strcmp(proot->type,grammer_type[29])==0){
        char label[20];//and_false_label
        gen_new_label(label,5);
        pro_land_exp(proot,label);
        string str1 = "jump ";
        str1 = str1+label_cond_true;
        mid_codes[mid_nums++]=str1;
        str1 = label;
        str1 = str1+":";
        mid_codes[mid_nums++]=str1;
    }else{
        pro_lor_exp(proot,label_cond_true);
        proot = proot->rightnode->rightnode;
        char label[20];//and_false_label
        gen_new_label(label,5);
        pro_land_exp(proot,label);
        string str1 = "jump ";
        str1 = str1+label_cond_true;
        mid_codes[mid_nums++]=str1;
        str1 = label;
        str1 = str1+":";
        mid_codes[mid_nums++]=str1;
    }
}

void pro_land_exp(treenode* proot, char * label_land_false){
    proot=proot->lowernode;
    if(strcmp(proot->type,grammer_type[29])==0){
        pro_land_exp(proot,label_land_false);
        proot=proot->rightnode->rightnode;
    }
    char result[20];
    pro_eq_exp(proot,result);
    string  str1 = "bez ";
    str1 = str1+result+" "+label_land_false;
    mid_codes[mid_nums++]=str1;

}

void pro_eq_exp(treenode* proot,char * result){
    proot=proot->lowernode;
    if(strcmp(proot->type,grammer_type[31])==0){
        pro_real_exp(proot,result);
    }else{
        string str1;
        char tempfin[20];
        gen_temp_var(tempfin);
        str1 = tempfin;
        str1 = str1+" = ";

        char temp[20];
        pro_eq_exp(proot,temp);
        str1 = str1+temp;

        proot=proot->rightnode;
        str1 = str1+" "+proot->word->value+" ";

        proot=proot->rightnode;
        pro_real_exp(proot,temp);
        str1=str1+temp;
        mid_codes[mid_nums++]=str1;

        strcpy(result,tempfin);
    }
}

void pro_real_exp(treenode* proot,char * result){
    proot=proot->lowernode;
    if(strcmp(proot->type,grammer_type[5])==0){
        pro_add_exp(proot,result);
    }else{
        string str1;
        char tempfin[20];
        gen_temp_var(tempfin);
        str1 = tempfin;
        str1 = str1+" = ";

        char temp[20];
        pro_real_exp(proot,temp);
        str1 = str1+temp;

        proot=proot->rightnode;
        str1 = str1+" "+proot->word->value+" ";

        proot=proot->rightnode;
        pro_add_exp(proot,temp);
        str1=str1+temp;
        mid_codes[mid_nums++]=str1;

        strcpy(result,tempfin);
    }
}


void pro_formate_string(treenode* proot){
    treenode* tempnode = proot;
    proot=proot->lowernode;
    char tempstr[500];
    strcpy(tempstr,proot->word->value);
    int len = strlen(tempstr);
    int i;
    char temp[500];
    int lentemp=0;
    for(i=1;i<len;i++){
        if(tempstr[i]=='%'){
            temp[lentemp]='\0';
            i++;
            string str1 = temp;
            char temp2[20];
            if(lentemp!=0) {
                formate_strings[formate_top++] = str1;

                gen_formate_sting_number(temp2);
                str1 = "print ";
                str1 = str1 + temp2;
                mid_codes[mid_nums++] = str1;
            }
            lentemp = 0;
            tempnode=tempnode->rightnode->rightnode;
            pro_exp(tempnode,temp2);
            str1 ="print ";
            str1=str1+temp2;
            mid_codes[mid_nums++]=str1;

        } else if(tempstr[i]=='\"'){
            if(lentemp!=0) {
                temp[lentemp] = '\0';
                string str1 = temp;
                formate_strings[formate_top++] = str1;
                char temp2[20];
                gen_formate_sting_number(temp2);
                str1 = "print ";
                str1 = str1 + temp2;
                mid_codes[mid_nums++] = str1;
            }
            return ;
        }
        else{
            temp[lentemp++]=tempstr[i];
        }
    }
}


void pro_funcFParams(treenode* proot){
    proot=proot->lowernode;
    pro_funcFParam(proot);
    while(proot->rightnode!=NULL){
        proot=proot->rightnode->rightnode;
        pro_funcFParam(proot);
    }
}

void pro_funcFParam(treenode* proot){
    string str1 ="para ";
    str1 =str1+"int ";
    proot=proot->lowernode;
    proot=proot->rightnode;
    int di=0;
    int di2=1;
    char ori_name[500];
    char new_name[20];
    strcpy(ori_name,proot->word->value);
    gen_new_var(ori_name,new_name);
    if(proot->rightnode!=NULL){
        proot=proot->rightnode->rightnode;
        di++;
    }if(proot->rightnode!=NULL){
        proot=proot->rightnode->rightnode;
        di2 =cacu_const_exp(proot);
        di++;
    }
    if(di==0){
        str1 =str1+new_name;
        mid_codes[mid_nums++]=str1;
    }else if(di==1){
         str1 =str1+new_name+"[]";
         //str1 = str1 + new_name;
        mid_codes[mid_nums++]=str1;
    }else if (di==2){
        char temp3[20];
        //itoa(di2,temp3,10);
        number_to_string(di2,temp3);
        str1 =str1+new_name+"[]";
        //str1 =str1+new_name+"[]["+temp3+"]";
        mid_codes[mid_nums++]=str1;
    }
    smterms[table_top++] = new sou_mid_table_term(ori_name,new_name,"para",di,di2);
}

void pro_var_def(treenode* proot){
    string str1 = "int ";
    treenode* tempnode = proot->lowernode; //ident
    char ori_name[500];
    char new_name[20];
    int tdi =0,di1=1,di2=1;
    int dires[3];
    strcpy(ori_name , tempnode->word->value);
    gen_new_var(ori_name,new_name);
    bool flag_invial= false;
    while(tempnode->rightnode!=NULL){
        tempnode= tempnode->rightnode;
        if(strcmp(tempnode->word->word_type,otherValue[19])==0){
            tempnode=tempnode->rightnode;
            dires[tdi++]=cacu_const_exp(tempnode);
            tempnode = tempnode->rightnode;
            continue;
        }
        else if(strcmp(tempnode->word->word_type,otherValue[14])==0){
            flag_invial= true;
            break;
        }
    }
    if(tdi==1){
        di2=dires[0];
    }else if(tdi==2){
        di2 = dires[1];
        di1= dires[0];
    }
    int di_size = di2*di1;
    str1 = str1+new_name;
    if(tdi==1){
        char temp[20];
        //itoa(di2,temp,10);
        number_to_string(di2,temp);
        str1 = str1+"["+temp+"]";
        mid_codes[mid_nums++]=str1;
    }else if(tdi==2){
        char temp[20];
        //itoa(di_size,temp,10);
        number_to_string(di_size,temp);
        str1 = str1+"["+temp+"]";
        mid_codes[mid_nums++]=str1;
    }else{
        mid_codes[mid_nums++]=str1;
    }

    if(flag_invial){
        tempnode = tempnode->rightnode; // invial
        if(tdi==0) {
            char temp_var_name[20];
            pro_exp(tempnode->lowernode, temp_var_name);
            str1 = new_name;
            str1 = str1+" = "+temp_var_name;
            mid_codes[mid_nums++] =str1;
        }else if (tdi==1){
            char temp[20];
            tempnode=tempnode->lowernode->rightnode;
            for(int i=0;i<di2;i++){
                str1 = new_name;
                //itoa(i,temp,10);
                number_to_string(i,temp);
                str1=str1+"["+temp+"] = ";
                pro_exp(tempnode->lowernode,temp);
                str1=str1+temp;
                mid_codes[mid_nums++]=str1;
                if(tempnode->rightnode->rightnode!=NULL){
                    tempnode=tempnode->rightnode->rightnode;
                }
            }
        }else if (tdi==2){
            tempnode =tempnode->lowernode->rightnode;
            treenode * tempnode1 ;
            char temp[20];
            for (int i=0;i<di1;i++){
                tempnode1 = tempnode->lowernode->rightnode;
                for(int j=0;j<di2;j++){
                    str1 = new_name;
                    //itoa(i*di2+j,temp,10);
                    number_to_string(i*di2+j,temp);
                    str1 = str1+"["+temp+"] = ";
                    pro_exp(tempnode1->lowernode,temp);
                    str1=str1+temp;
                    mid_codes[mid_nums++] = str1;
                    if(tempnode1->rightnode->rightnode!=NULL){
                        tempnode1=tempnode1->rightnode->rightnode;
                    }
                }
                if(tempnode->rightnode->rightnode!=NULL){
                    tempnode=tempnode->rightnode->rightnode;
                }
            }
        }
    }
    smterms[table_top++] =new sou_mid_table_term(ori_name,new_name,"int",tdi,di2,di1,NULL);
}

void pro_exp(treenode* proot,char * name_res){
    pro_add_exp(proot->lowernode,name_res);

}

bool contains_array(char * name){
    for(int i=0;i< strlen(name);i++){
        if(name[i]=='['){
            return true;
        }
    }
    return false;
}

void pro_add_exp(treenode* proot,char * name){
    proot=proot->lowernode;
    if(proot->rightnode!=NULL){
        char temp1[20];
        char temp2[20];
        char temp3[20];
        gen_temp_var(temp1);
        string str1 = temp1;
        str1 = str1+" = ";
        pro_add_exp(proot,temp2);
        str1 = str1+temp2;
        proot=proot->rightnode;
        str1 = str1+" "+proot->word->value+" ";
        pro_mul_exp(proot->rightnode,temp3);
        str1 = str1+temp3;
        mid_codes[mid_nums++]=str1;
        strcpy(name,temp1);
    }else{
        pro_mul_exp(proot,name);
    }
}

void pro_mul_exp(treenode* proot,char * name){
    proot=proot->lowernode;
    if(proot->rightnode!=NULL){
        char temp1[20];
        char temp2[20];
        char temp3[20];
        gen_temp_var(temp1);
        string str1 = temp1;
        str1 = str1+" = ";
        pro_mul_exp(proot,temp2);
        str1 = str1+temp2;
        proot=proot->rightnode;
        str1 = str1+" "+proot->word->value+" ";
        pro_unary_exp(proot->rightnode,temp3);
        str1 = str1+temp3;
        mid_codes[mid_nums++]=str1;
        strcpy(name,temp1);
    }else{
        pro_unary_exp(proot,name);
    }
}

void pro_unary_exp(treenode* proot,char * name){
    proot=proot->lowernode;
    if(strcmp(proot->type,grammer_type[11])==0){
        pro_primary_exp(proot,name);
    }else if (strcmp(proot->type,grammer_type[2])==0){
        char funcname[500];
        strcpy(funcname,proot->word->value);
        proot=proot->rightnode->rightnode;
        if(strcmp(proot->type,grammer_type[9])==0){
            pro_funcRParams(proot);
        }
        string str1 = "call ";
        str1 =str1+funcname;
        mid_codes[mid_nums++]=str1;
        char temp[20];
        gen_temp_var(temp);
        str1=temp;
        str1=str1+" = RET";
        mid_codes[mid_nums++]=str1;
        strcpy(name,temp);
    }else if (strcmp(proot->type,grammer_type[8])==0){
        char temp[20];
        gen_temp_var(temp);
        strcpy(name,temp);
        string str1 = temp;
        if(strcmp(proot->lowernode->word->value,otherString[0])==0){
            str1 = str1 +" = ! ";
            pro_unary_exp(proot->rightnode,temp);
            str1 = str1+temp;
            mid_codes[mid_nums++]=str1;
        }else {
            str1 = str1 + " = 0 ";
            pro_unary_exp(proot->rightnode, temp);
            str1 = str1 + proot->lowernode->word->value;
            str1 = str1 + " " + temp;
            mid_codes[mid_nums++] = str1;
        }
    }
}

void pro_primary_exp(treenode* proot,char * name){
    proot=proot->lowernode;
    if(strcmp(proot->type,grammer_type[2])==0){
        pro_exp(proot->rightnode,name);
    }else if(strcmp(proot->type,grammer_type[13])==0){
        pro_lval(proot,name);
    }else if(strcmp(proot->type,grammer_type[12])==0){
        strcpy(name,proot->lowernode->word->value);
    }
}


void pro_lval(treenode * proot,char *name){
    proot=proot->lowernode;
    int k=search_table(proot->word->value);
    char temp[20];
    strcpy(temp,smterms[k]->new_name);
    int di=0;
    char temp1[20];
    char temp2[20];
    if(proot->rightnode!=NULL){
        proot=proot->rightnode->rightnode;
        di++;
        pro_exp(proot,temp1);
        proot=proot->rightnode;
    }if(proot->rightnode!=NULL){
        proot=proot->rightnode->rightnode;
        di++;
        pro_exp(proot,temp2);
        proot=proot->rightnode;
    }
    if(di==0){
        strcpy(name,temp);
    }else if(di==1){
        string str1 = temp;
        if(contains_array(temp1)) {
            char temp[20];
            gen_temp_var(temp);
            string str1 = temp;
            str1 = str1 + " = " + temp1;
            mid_codes[mid_nums++] = str1;
            strcpy(temp1, temp);
        }
        if(smterms[k]->tdimen==2){
            char temp2[20];
            char temp5[20];
            gen_temp_var(temp2);
            string str2 = temp2;
            str2 =str2+" = "+temp1+" * ";
            //itoa(smterms[k]->dimen2*4,temp5,10);
            number_to_string(smterms[k]->dimen2*4,temp5);
            str2 = str2+temp5;
            mid_codes[mid_nums++] =str2;
            str1 = "( ";
            str1 = str1+temp+" + "+temp2+" )";
            strcpy(name,str1.c_str());
        }else {
            str1 = str1 + "[" + temp1 + "]";
            strcpy(name, str1.c_str());
        }
    }else if(di==2){
        char temp3[20];
        char temp4[20];

        gen_temp_var(temp3);
        string str1 = temp3;
        //itoa(smterms[k]->dimen2,temp4,10);
        number_to_string(smterms[k]->dimen2,temp4);
        str1=str1+" = "+temp1 +" * "+temp4; //

        mid_codes[mid_nums++]=str1;

        gen_temp_var(temp4);
        str1 = temp4; // subscript
        str1 = str1+" = "+temp3+" + "+temp2;

        mid_codes[mid_nums++] =str1;

        str1 = temp;
        str1 = str1+"["+temp4+"]";

        strcpy(name,str1.c_str());
    }
}

void pro_funcRParams(treenode * proot){
    string pushes[50];
    int rparacounter=0;
    proot=proot->lowernode;
    char temp[20];
    pro_exp(proot,temp);
    string str1 ="push ";
    str1=str1+temp;
    pushes[rparacounter++]=str1;
    while(proot->rightnode!=NULL){
        proot =proot->rightnode->rightnode;
        pro_exp(proot,temp);
        string str1 ="push ";
        str1=str1+temp;
        pushes[rparacounter++]=str1;
    }
    for(int i=0;i<rparacounter;i++){
        mid_codes[mid_nums++] = pushes[i];
    }
}

void pro_con_def(treenode* proot){
    string str1 = "const ";
    treenode* tempnode = proot->lowernode; //ident
    char ori_name[500];
    char new_name[20];
    int tdi =0,di1=1,di2=1;
    int dires[3];
    strcpy(ori_name , tempnode->word->value);
    gen_new_var(ori_name,new_name);
    tempnode = tempnode->rightnode; // [
    while(strcmp(tempnode->word->word_type,otherValue[19])==0){
        tempnode  = tempnode->rightnode;
        dires[tdi++]=cacu_const_exp(tempnode);
        tempnode = tempnode->rightnode->rightnode;
    }
    if(tdi==1){
        di2=dires[0];
    }else if(tdi==2){
        di2 = dires[1];
        di1= dires[0];
    }
    int di_size = di2*di1;
    int * values = (int*) malloc(4*di_size);
    tempnode=tempnode->rightnode;
    get_coninvial(tempnode,values,tdi,di1,di2);
    smterms[table_top++]=new sou_mid_table_term(ori_name,new_name,"const",tdi,di2,di1,values);
    str1 = str1+new_name;
    if(tdi==0){
        char temp[20];
        //itoa(*values,temp,10);
        number_to_string(*values,temp);
        str1 = str1 ;
        mid_codes[mid_nums++]=str1;
        str1 = new_name;
        str1 =str1+" = "+temp;
        mid_codes[mid_nums++]=str1;
    }else if (tdi ==1){
        char temp[20];
        //itoa(di2,temp,10);
        number_to_string(di2,temp);
        str1 = str1+"["+temp+"]";
        mid_codes[mid_nums++]=str1;
        int i = 0;
        for (;i<di2;i++){
            //itoa(i,temp,10);
            number_to_string(i,temp);
            str1 = new_name;
            str1 = str1+"["+temp+"] = ";
            //itoa(*(values+i),temp,10);
            number_to_string(*(values+i),temp);
            str1=str1+temp;
            mid_codes[mid_nums++]=str1;
        }
    }else if (tdi==2){
        char temp[20];
        //itoa(di1*di2,temp,10);
        number_to_string(di1*di2,temp);
        str1 = str1+"["+temp+"]";
        mid_codes[mid_nums++]=str1;
        int i=0;
        for (;i<di1*di2;i++){
            //itoa(i,temp,10);
            number_to_string(i,temp);
            str1 =  new_name;
            str1 = str1+"["+temp+"] = ";
            //itoa(*(values+i),temp,10);
            number_to_string(*(values+i),temp);
            str1 = str1+temp;
            mid_codes[mid_nums++]=str1;
        }
    }
}

void get_coninvial(treenode* proot,int* value,int tdi,int di1,int di2){
    if(tdi==0){
        *value = cacu_const_exp(proot->lowernode);
    }else if (tdi ==1){
        proot=proot->lowernode;
        if(strcmp(proot->type,grammer_type[4])==0){ // constexp
            get_const_array(proot,value,tdi,di1,di2);
        }else {
            int i ;
            for (i=0;i<di2;i++){
                proot = proot->rightnode;
                get_coninvial(proot,value+i,0,0,0);
                proot = proot->rightnode;
            }
        }
    }else {
        proot=proot->lowernode;
        if(strcmp(proot->type,grammer_type[4])==0){ // constexp
            get_const_array(proot,value,tdi,di1,di2);
        }else{
            int i;
            for(i=0;i<di1;i++){
                proot =proot->rightnode;
                get_coninvial(proot,value+i*di2,1,0,di2);
                proot=proot->rightnode;
            }
        }
    }
}

void get_const_array(treenode* proot,int * value,int tdi,int di1,int di2){
    while(strcmp(proot->type,grammer_type[2])!=0){
        proot=proot->lowernode;
    }
    int k =search_table(proot->word->value);
    treenode * tempnode = proot;
    int l =-1;
    if(proot->rightnode!=NULL){
        tempnode = tempnode->rightnode->rightnode;
         l = cacu_const_exp(tempnode);
    }
    smterms[k]->get_value_arr(l,value);
}


int  cacu_const_exp(treenode* proot){
    proot=proot->lowernode;
    return cacu_add_exp(proot);
}
int cacu_add_exp(treenode * proot){
    proot=proot->lowernode;
    if(proot->rightnode!=NULL){
        if(strcmp(proot->rightnode->word->word_type,otherValue[3])==0){
            return cacu_add_exp(proot)+ cacu_mul_exp(proot->rightnode->rightnode) ;
        }else{
            return cacu_add_exp(proot)- cacu_mul_exp(proot->rightnode->rightnode);
        }
    }else {
        return cacu_mul_exp(proot);
    }
}

int cacu_mul_exp(treenode* proot){
    proot=proot->lowernode;
    if(proot->rightnode!=NULL){
        if(strcmp(proot->rightnode->word->word_type,otherValue[5])==0){
            return cacu_mul_exp(proot)* cacu_unary_exp(proot->rightnode->rightnode) ;
        }else if(strcmp(proot->rightnode->word->word_type,otherValue[6])==0){
            return cacu_mul_exp(proot)/ cacu_unary_exp(proot->rightnode->rightnode);
        }else{
            return cacu_mul_exp(proot) % cacu_unary_exp(proot->rightnode->rightnode);
        }
    }else {
        return cacu_unary_exp(proot);
    }
}

int cacu_unary_exp(treenode* proot){
    proot=proot->lowernode;
    if(strcmp(proot->type,grammer_type[8])==0){//unaryop
        if(strcmp(proot->lowernode->word->word_type,otherValue[4])==0) {
            return -cacu_unary_exp(proot->rightnode);
        }else{
            return cacu_unary_exp(proot->rightnode);
        }
    }else{
        return cacu_primary_exp(proot);
    }
}

int cacu_primary_exp(treenode* proot){
    proot = proot->lowernode;
    if(strcmp(proot->type,grammer_type[2])==0){
        return cacu_const_exp(proot->rightnode);
    }else if(strcmp(proot->type,grammer_type[12])==0){//Number
        return cacu_number(proot);
    }else{
        return cacu_lval(proot);
    }
}

int cacu_number(treenode* proot){
    proot=proot->lowernode;
    int result = atoi(proot->word->value);
    return result;
}

int cacu_lval(treenode* proot){
    proot =proot->lowernode;
    int m = search_table(proot->word->value);
    int count =0;
    int res[3];
    while(proot->rightnode!=NULL){
        proot=proot->rightnode->rightnode;
        res[count++] = cacu_const_exp(proot);
        proot=proot->rightnode;
    }
    int dim1=0,dim2=0;
    if(count==1){
        dim2 = res[0];
    }else if(count==2){
        dim1=res[0];
        dim2=res[1];
    }
    return smterms[m]->get_value(dim1,dim2);
}

int search_table(char *find_name){
    int i ;
    for (i = table_top-1;i>=0;i--){
        if (strcmp(smterms[i]->name,find_name)==0){
            return i;
        }
    }return -1;
}





