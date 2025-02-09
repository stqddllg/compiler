#include <iostream>
#include "A.h"
#include <vector>
#include<stdlib.h>
using namespace std;

extern string mid_codes[20000];
extern int mid_nums;
static int code_pointer=0;

extern string formate_strings[10000];
extern int formate_top;

void pro_formate_strings();
void pro_global_var();
void pro_code_string(string str,vector<string>* des);
void pro_split_strings_glo(vector<string> des);
bool is_func_def(vector<string>  des);
bool is_part_var_def(vector<string> des);
bool is_op_equation(vector<string> des);
bool is_number(string name);
bool is_single_value(var_pair * varPair);

var_pair* pro_var_name(string name);
bool is_var_or_number(string name);
void register_op_offset(char * register_name,char *op ,int value);
void register_to_memory(const char * register_name,string var_name,const char* temp_register1, const char* temp_register2);
void single_memory_to_register( char *register_name, char* single_name);
void memory_to_register(const char * register_name,string  var_name,const char * temp_register_name);
//void memory_to_register(char* register_name, string  var_name,  char* temp_register_name);
void string_to_register(char * register_name,string des);
void pro_func_defs();
void pro_func_fpara();
void pro_func_content(string funcname);
void pro_part_var_def(vector<string> des);
void pro_op_equation(vector<string> des);
int search_table(string name);
void pro_push(vector<string> des);
void get_stack_top_to_register(const char * register_name);
void get_address_to_register(const char * register_name, string name );
void pro_call(vector<string> des);
void pro_ret(vector<string> des,string func_name);
void pro_print(vector<string> des);
bool is_var(string name);
void pro_assign(vector<string> des);
bool is_label(string code);

void number_to_string(int value,char * string){
    int length =0;
    int mod;
    char temp[20];
    bool flag= false;
    int start=0;
    if(value<0){
        value=-value;
        flag = true;
    }
    while(value>=10){
        mod =  value%10;
        temp[length++] = mod+'0';
        value = value/10;
    }
    temp[length] = value+'0';
    if(flag){
        string[0]='-';
        start=1;
    }
    for(int i=0;i<=length;i++){
        string[start+i]=temp[length-i];
    }
    string[start+length+1]='\0';
}



string data_words[20000]; // data段内容
int data_number = 0;


string texts[20000]; //代码段内容
int text_number=0;

mid_mips_term * mmterms[20000];
static int table_top=0;

static int offsets =0; // 运行栈的偏移量

static int push_offset =4;



void mid2mips(){
    pro_formate_strings();
    pro_global_var();
    pro_func_defs();
}

void pro_formate_strings(){ // fi
    char temp[20];
    for(;data_number<formate_top;data_number++){
        //itoa(data_number,temp,10);
        number_to_string(data_number,temp);
        string str1 ="formate";
        str1 = str1+temp+": .asciiz \"";
        str1 = str1 +formate_strings[data_number]+"\"";
        data_words[data_number]=str1;
    }
    string str1 = ".align 2";
    data_words[data_number++]=str1;
}

void pro_func_defs(){ //fi
    offsets = 0;
    while(true) {
        if(code_pointer>=mid_nums){
            return;
        }
        string str2 = mid_codes[code_pointer++];
        vector<string> split_strings;
        pro_code_string(str2, &split_strings);
        str2 = "func_";
        str2 = str2+split_strings[1];
        str2 = str2 + ":";
        texts[text_number++] = str2;

        pro_func_fpara();
        pro_func_content(split_strings[1]);
    }
}

void pro_func_content(string func_name){ //fi
    vector<string> split_string;
    string str2;
    while(true) {
        if(code_pointer>=mid_nums){
            return ;
        }
        str2 = mid_codes[code_pointer];
        pro_code_string(str2, &split_string);
        if(is_func_def(split_string)){
            return ;
        }else if(is_part_var_def(split_string)){
            pro_part_var_def(split_string);
            code_pointer++;
        }else if(is_op_equation(split_string)){
            pro_op_equation(split_string);
            code_pointer++;
        } else if(split_string[0]=="push"){
            pro_push(split_string);
            code_pointer++;
        } else if(split_string[0]=="call"){
            pro_call(split_string);
            code_pointer++;
        } else if(split_string[0] =="RET"){
            pro_ret(split_string,func_name);
            code_pointer++;
        } else if(split_string[0]=="print"){
            pro_print(split_string);
            code_pointer++;
        } else if(is_var(split_string[0])){
            pro_assign(split_string);
            code_pointer++;
        } else if(split_string[0]=="FunFin"){
            if(func_name=="main") {
                string str1;
                str1 = "j end_zpqm";
                texts[text_number++] = str1;
                code_pointer++;
            }else {
                str2 = "jr $31";
                texts[text_number++] = str2;
                code_pointer++;
            }
        }else if(split_string[0]=="jump"){
            str2 ="j ";
            str2 = str2+split_string[1];
            texts[text_number++]=str2;
            code_pointer++;
        }else if(is_label(split_string[0])){
            texts[text_number++]=str2;
            code_pointer++;
        }else if(split_string[0]=="bez"){
            str2 = "beq $0 $t1 ";
            memory_to_register("$t1",split_string[1],"$t2");
            str2 = str2+split_string[2];
            texts[text_number++]=str2;
            code_pointer++;
        }
        split_string.clear();
    }
}
bool is_label(string code){
    if(code[0]=='l'&&code[1]=='a'&&code[2]=='b'){
        return true;
    }else{
        return false;
    }
}

void pro_assign(vector<string> des){
    string str1 ;
    if(des[2]=="getint()"){
        str1 = "li $v0 5";
        texts[text_number++] = str1;
        str1 = "syscall";
        texts[text_number++]= str1;
        register_to_memory("$v0",des[0],"$t0","$t1");
    }else if(des[2]=="RET"){
        register_to_memory("$v0",des[0],"$t0","$t1");
    }
}

bool is_var(string name){
    if(name[0]=='t'||name[0]=='v'){
        return true;
    }else{
        return false;
    }
}

void pro_print(vector<string> des){
    string str1;
    if(des[1][0]=='f'){
        str1 = "li $v0 4";
        texts[text_number++]=str1;
        str1 = "la $a0 ";
        str1 = str1+des[1];
        texts[text_number++]=str1;
        str1 = "syscall";
        texts[text_number++]=str1;
    }else{
        memory_to_register("$a0",des[1],"$t1");
        str1 = "li $v0 1";
        texts[text_number++] = str1;
        str1 = "syscall";
        texts[text_number++] = str1;
    }
}

void pro_ret(vector<string> des,string func_name){
    string str1;
    if(func_name=="main"){
        str1 = "j end_zpqm";
        texts[text_number++]=str1;
    }else{
        memory_to_register("$t1",des[1],"$t2");
        str1 = "move $v0 $t1 ";
        texts[text_number++]=str1;
        str1 = "jr $31 ";
        texts[text_number++]=str1;
    }
}

void gen_new_name(char * name){
    static int counter=0;
    name[0]='r';
    name[1]='a';
    name[2]='\0';
    char temp[20];
    number_to_string(counter++,temp);
    strcat(name,temp);
    return;
}

void pro_call(vector<string> des){
    char temp1[20];
    gen_new_name(temp1);
    register_to_memory("$ra",temp1,"$t0","$t1");;


    push_offset=4;
    string str1 = "sub $sp $sp ";

    char temp[20];
    //itoa(offsets,temp,10);
    number_to_string(offsets,temp);
    str1 = str1+temp;
    texts[text_number++]= str1;


    str1 = "jal func_";
    str1 = str1+des[1];
    texts[text_number++] = str1;

    str1 = "add $sp $sp ";
    str1 = str1+temp;
    texts[text_number++]=str1;

    memory_to_register("$ra",temp1,"$t0");
    return ;

}

void pro_push(vector<string> des){
    string str1;
    if (des.size() == 2) {
        var_pair *varPair = pro_var_name(des[1]);
        if (is_single_value(varPair)) { // 数值压值
            memory_to_register("$t1", des[1], "$t2");
            get_stack_top_to_register("$t0");
            str1 = "sw $t1 0($t0)";
            texts[text_number++] = str1;

        }else {
            get_address_to_register("$t1",des[1]);
            get_stack_top_to_register("$t0");
            str1 = "sw $t1 0($t0)";
            texts[text_number++] =str1;
        }
    }else{
        //des[2]+des[4] -> stack_top;
        get_address_to_register("$t1",des[2]);
        memory_to_register("$t0",des[4],"$t2");
        str1 = "add $t1 $t1 $t0";
        texts[text_number++]=str1;
        get_stack_top_to_register("$t0");
        str1 = "sw $t1 0($t0)";
        texts[text_number++] = str1;
    }
    push_offset=push_offset+4;
}


void get_address_to_register(const char * register_name, string name ){
    int vof = search_table(name);
    string str1 ;
    if(strcmp(mmterms[vof]->type,"global")==0){
        str1 = "la ";
        str1 = str1+ register_name+" "+name;
        texts[text_number++] =str1;
    }else if(strcmp(mmterms[vof]->type,"part")==0) {
        str1 = "sub ";
        str1 = str1+register_name+ " $sp ";
        str1 = str1 + mmterms[vof]->offset;
        texts[text_number++] = str1;
    }else if(strcmp(mmterms[vof]->type,"para")==0) {
        str1 = "sub ";
        str1 = str1+register_name+" $sp ";
        str1 = str1 + mmterms[vof]->offset;
        texts[text_number++] = str1;

        str1 = "lw ";
        str1 = str1+register_name+" 0("+register_name+")";
        texts[text_number++]=str1;
    }
}

void get_stack_top_to_register(const char * register_name){
    string str1;
    char temp[20];
    int new_offset = offsets + push_offset;
    //itoa(new_offset, temp, 10);
    number_to_string(new_offset,temp);
    str1 = "sub ";
    str1 = str1 +register_name+" $sp "+ temp;
    texts[text_number++] = str1;
}

bool is_single_value(var_pair * varPair){
    if(varPair->have_down_sign){
        return true;
    }else if(is_number(varPair->name)){
        return true;
    }else{
        int vof = search_table(varPair->name);
        if(mmterms[vof]->is_pointer){
            return false;
        }else{
            return true;
        }
    }
}


void pro_op_equation(vector<string> des){

    if(des[2] == "!"){
        memory_to_register("$t1",des[3],"$t3");
        string str2 = "sltiu $t0 $t1 1";
        texts[text_number++]=str2;
        register_to_memory("$t0", des[0], "$t1", "$t2");
    }else {
        memory_to_register("$t1", des[2], "$t3");
        string str2;
        if (des.size() > 3) {
            memory_to_register("$t2", des[4], "$t3");
            if (des[3] == "+") {
                str2 = "add $t0 $t1 $t2";
                texts[text_number++] = str2;
            } else if (des[3] == "-") {
                str2 = "sub $t0 $t1 $t2";
                texts[text_number++] = str2;
            } else if (des[3] == "*") {
                str2 = "mul $t0 $t1 $t2";
                texts[text_number++] = str2;
            } else if (des[3] == "/") {
                str2 = "div $t0 $t1 $t2";
                texts[text_number++] = str2;
            } else if (des[3] == "%") {
                str2 = "div $t1 $t2";
                texts[text_number++] = str2;
                str2 = "mfhi $t0";
                texts[text_number++] = str2;
            }else if(des[3]=="<"){
                str2 = "slt $t0 $t1 $t2";
                texts[text_number++]=str2;
            }else if(des[3]==">"){
                str2 = "slt $t0 $t2 $t1";
                texts[text_number++]=str2;
            }else if(des[3]==">="){
                str2 = "sub $t0 $t2 $t1";
                texts[text_number++]=str2;
                str2 = "slti $t0 $t0 1";
                texts[text_number++]= str2;
            }else if(des[3]=="<="){
                str2 = "sub $t0 $t1 $t2";
                texts[text_number++]=str2;
                str2 = "slti $t0 $t0 1";
                texts[text_number++]= str2;
            }else if(des[3]=="=="){
                str2 = "sub $t0 $t1 $t2";
                texts[text_number++] = str2;
                str2 = "sltiu $t0 $t0 1";
                texts[text_number++] = str2;
            }else if(des[3]=="!="){
                str2 = "xor $t0 $t1 $t2";
                texts[text_number++]=str2;
            }
            register_to_memory("$t0", des[0], "$t1", "$t2");
        } else {
            register_to_memory("$t1", des[0], "$t0", "$t2");
        }
    }
}



bool is_op_equation(vector<string> des){
    if(des.size()<3){
        return false;
    }
    if(is_var_or_number(des[0]) && des[1] == "=" && (is_var_or_number(des[2])||des[2]=="!")){
        return true;
    }else{
        return false;
    }
}

void pro_part_var_def(vector<string> des){ // fini
    var_pair* varPair = pro_var_name(des[1]);
    int size = atoi(varPair->subscript)*4;
    offsets=offsets+size-4;
    mmterms[table_top++]= new mid_mips_term(varPair->name,(char*)"part",offsets,varPair->have_down_sign);
    offsets =offsets+4;
    return ;
}

bool is_part_var_def(vector<string> des){ // fini
    if(des[0]=="int"||des[0]=="const"){
        return true;
    }else {
        return false;
    }
}


void pro_func_fpara(){ //fini
    offsets = 0 ;
    vector<string> split_string ;
    while(true){
        if(code_pointer>=mid_nums){
            return ;
        }
        string str2 = mid_codes[code_pointer];
        pro_code_string(str2 , &split_string);
        if(!(split_string[0]=="para")){
            return ;
        }else{
            var_pair* varPair = pro_var_name(split_string[2]);
            mmterms[table_top++] = new mid_mips_term(varPair->name,"para",offsets,varPair->have_down_sign);
            offsets = offsets+4;
            code_pointer++;
        }
        split_string.clear();
    }
}



void pro_global_var(){
    vector<string> split_strings;
    for(; code_pointer < mid_nums; code_pointer++){
        string  str1 = mid_codes[code_pointer];
        pro_code_string(str1,&split_strings);

        if(is_func_def(split_strings)){
            /*string str2 = "sub";
            char temp[20];
            itoa(offsets,temp,10);
            str2 = str2+" $sp $sp "+temp;
            texts[text_number++] = str2;*/
            offsets=0;
            texts[text_number++]= "j func_main";
            return ;
        }

        pro_split_strings_glo(split_strings);
        split_strings.clear();
    }
}

bool is_func_def(vector<string>  des){// 该代码是否是函数定义代码
    if(des.size()<3){
        return false;
    }
    if((des[0]=="void"||des[0]=="int")&&(des[2]=="()")){
        return true;
    }else{
        return false;
    }
}

void pro_split_strings_glo(vector<string> des){
    if(des[0]=="const"|| des[0]=="int"){
        var_pair* vpair = pro_var_name(des[1]);
        int array_size = atoi(vpair->subscript) * 4;
        char temp[20];
        //itoa(array_size,temp,10);
        number_to_string(array_size,temp);
        string str1 = vpair->name;
        str1 = str1 + ": .space "+temp;
        data_words[data_number++]=str1;
        if(vpair->have_down_sign) {
            mmterms[table_top++] = new mid_mips_term(vpair->name, "global", 0, true);
        }
        else{
            mmterms[table_top++] = new mid_mips_term(vpair->name,"global",0,false);
        }
        delete vpair;
    }else {
        pro_op_equation(des);
    }
}

var_pair* pro_var_name(string name){
    int length = name.size();
    string single_name;
    string offset = "1";
    for(int i=0;i<length;i++){
        if(name[i]=='['){
            single_name = name.substr(0,i);
            offset = name.substr(i+1,length-i-2);
            return new var_pair((char*)single_name.c_str(),(char *)offset.c_str(),true);
        }
    }
    return new var_pair((char *)name.c_str(),(char *)offset.c_str(),false);
}

int search_table(string name){
    for(int i=table_top-1;i>=0;i--){
        if(strcmp(mmterms[i]->name,name.c_str())==0){
            return i;
        }
    }
    return -1;
}

//void memory_to_register(const char* register_name, string  var_name, char* temp_register_name) {
//    memory_to_register(const_cast<char*>(register_name), var_name,  temp_register_name);
//}

//void memory_to_register(const char* register_name, string  var_name, const char* temp_register_name) {
//    memory_to_register(const_cast<char*>(register_name), var_name, const_cast<char*>(temp_register_name));
//}



void memory_to_register(const char * register_name,string  var_name,const char * temp_register_name){
    string str1 ;
    var_pair* varPair = pro_var_name(var_name);
    if(is_number(varPair->name)){
        str1 = "li ";
        str1 = str1 + register_name +" "+var_name;
        texts[text_number++] = str1;
        return ;
    }
    if(varPair->have_down_sign){
        memory_to_register(register_name,varPair->subscript,temp_register_name);
        str1 = "sll ";
        str1 = str1+temp_register_name+" "+register_name+" 2";
        texts[text_number++] = str1;

    }
    int vof = search_table(varPair->name);
    if(strcmp(mmterms[vof]->type,"global")==0){
        str1 = "la ";
        str1 =str1 + register_name+" "+varPair->name;
        texts[text_number++] =str1 ;
        if(varPair->have_down_sign){
            str1 = "add ";
            str1=str1+register_name+" "+register_name+" "+temp_register_name;
            texts[text_number++] =str1;
        }
        str1 = "lw ";
        str1 = str1+register_name+" "+"0("+register_name+")";
        texts[text_number++] = str1;
    }else if(strcmp(mmterms[vof]->type,"part")==0){
        str1 = "sub ";
        str1 = str1 +register_name+" $sp "+mmterms[vof]->offset;
        texts[text_number++] = str1 ;
        if(varPair->have_down_sign){
            str1 = "add ";
            str1 = str1 + register_name+" "+register_name+" "+temp_register_name;
            texts[text_number++] =str1;
        }
        str1 = "lw ";
        str1 = str1+register_name+" 0("+register_name+")";
        texts[text_number++] =str1;
    }else if(strcmp(mmterms[vof]->type,"para")==0){
        str1 = "sub ";
        str1 = str1 +register_name+" $sp "+mmterms[vof]->offset;
        texts[text_number++] = str1 ;
        str1 = "lw ";
        str1 = str1+register_name+" 0("+register_name+")";
        texts[text_number++] = str1 ;
        if(varPair->have_down_sign){
            str1 = "add ";
            str1 =str1+register_name+" "+register_name+" "+temp_register_name;
            texts[text_number++] = str1;
            str1 = "lw ";
            str1 = str1+register_name+" 0("+register_name+")";
            texts[text_number++] = str1 ;
        }
    }
}


void register_to_memory(const char * register_name,string var_name,const char* temp_register1,const char* temp_register2){// 把寄存器值存入内存
    var_pair* varPair = pro_var_name(var_name); // 要存的变量的名字
    string str1;
    if(varPair->have_down_sign){
        memory_to_register(temp_register1,varPair->subscript,temp_register2);
        str1 = "sll ";
        str1 = str1+ temp_register1+" "+temp_register1+" 2";
        texts[text_number++] = str1 ;
    }
    int vof =search_table(varPair->name);
    if(vof == -1) {
        vof =table_top;
        mmterms[table_top++] = new mid_mips_term(varPair->name,"part",offsets,false);
        offsets = offsets+4;
    }


    if(strcmp(mmterms[vof]->type,"global")==0){
        str1 = "la ";
        str1 = str1+temp_register2+" "+varPair->name;
        texts[text_number++] =str1;
        if(varPair->have_down_sign){
            str1 = "add ";
            str1 = str1+temp_register2+" "+temp_register2+" "+temp_register1;
            texts[text_number++] =str1;
        }
        str1 = "sw ";
        str1 = str1+register_name+" 0("+temp_register2+")";
        texts[text_number++]=str1;
    }else if(strcmp(mmterms[vof]->type,"part")==0){
        str1 = "sub ";
        str1 = str1 + temp_register2+" $sp "+mmterms[vof]->offset;
        texts[text_number++] = str1;
        if(varPair->have_down_sign){
            str1 = "add ";
            str1 = str1+temp_register2+" "+temp_register2+" "+temp_register1;
            texts[text_number++]=str1;
        }
        str1 = "sw ";
        str1 = str1+register_name+" 0("+temp_register2+")";
        texts[text_number++]=str1;
    }else if(strcmp(mmterms[vof]->type,"para")==0){
        str1 = "sub ";
        str1 = str1+temp_register2+" $sp "+mmterms[vof]->offset;
        texts[text_number++]=str1;
        if(varPair->have_down_sign){
            str1 = "lw ";
            str1 = str1 + temp_register2+ " 0("+temp_register2+")";
            texts[text_number++] =str1;
            str1  ="add ";
            str1 = str1+temp_register2+" "+temp_register2+" "+temp_register1;
            texts[text_number++] = str1;
            str1="";
            str1 = "sw ";
            str1 = str1+ register_name+" 0("+temp_register2+")";
            texts[text_number++]=str1;
        }else{
            str1 = "sw ";
            str1 = str1+register_name+ " 0("+temp_register2+")";
            texts[text_number++]=str1;
        }
    }

}



bool is_var_or_number(string name){
    if(name[0]=='t'||(name[0]=='v'&&name[1]<='9'&&name[1]>='0'||(name[0]<='9')&&(name[0]>='0'))){
        return true;
    }else if(name[1]<='9'&&name[1]>='0'&&name[0]=='-'){
        return true;
    }
    else{
        return false;
    }
}

bool is_number(string name){
    if(name[0]<='9'&&name[0]>='0'||name[0]=='-'&&name[1]<='9'&&name[1]>='0'){
        return true;
    }else{
        return false;
    }
}


void pro_code_string(string str,vector<string>* des){// 把中间代码的一条string分段
    vector<string> split_strings;
    int length =0;
    int now_pos =0;
    string temp_string;
    for(int i=0;i<str.length();i++){
        if(str[i]==' '){
            temp_string = str.substr(now_pos,length);

            des->push_back(temp_string);
            now_pos = now_pos+length+1;
            length=0;
        }else {
            length++;
        }
    }
    if(length!=0){
        temp_string = str.substr(now_pos,length);
        des->push_back(temp_string);
    }
}

