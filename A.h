#include<string.h>
#include<string>
#include<stdlib.h>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS


struct wordpair{
    char word_type[15];
    char value[500];
    int linenumber;
};

class err_pair{
public :
    char type;
    int line ;
    char contain[500];
    err_pair(char  type,int line,char * contain= nullptr){
        this->type=type;
        this->line = line;
        if(contain!=nullptr)
        strcpy(this->contain,contain);
    };
    void print_err(){
        printf("%c %d\n",type,line);
    }
};
class treenode{
    public:
        char type[20];
        wordpair* word;
        treenode* rightnode;
        treenode* lowernode;
        treenode(){
          word=NULL;
          rightnode=NULL;
          lowernode=NULL;
        };
        void settype(char* type){
            strcpy(this->type,type);
        };
        void set_word_pair(wordpair* word){
            this->word = word;
        };
        void set_right_node(treenode* rightnode){
            this->rightnode=rightnode;
        };
        void set_lower_node(treenode* lowernode){
            this->lowernode=lowernode;
        };
};

class params{
public:
    int dimension;
    params* next_paramterm;
    params(int di,params *next=NULL ){
        dimension = di;
        next_paramterm = next;
    }
    void set_next(params* next){
        next_paramterm=next;
    }
};


class table_term{
public:
    char type[15];//function , variable
    char name[500]; //
    char value_type[15];//int, void, const
    int  dimension; //变量维数
    params * paramlist;
    table_term(char *type,char *name,char * value_type,
               int dimension,params* head){
        strcpy(this->type,type);
        strcpy(this->name,name);
        strcpy(this->value_type,value_type);
        this->dimension = dimension;
        this->paramlist = head;
    }
};

class sou_mid_table_term{
public :
    char name[500];
    char new_name[20];
    char value_type[15];//const var
    int dimen1;
    int dimen2;
    int tdimen;
    int *  value;

    sou_mid_table_term(const char *name ,const char *new_name,const char *value_type,int tdimen=0,int dimen2=1,int dimen1=1,int * value =NULL){
        strcpy(this->name,name);
        strcpy(this->new_name,new_name);
        strcpy(this->value_type,value_type);
        this->dimen1=dimen1;
        this->dimen2=dimen2;
        this->tdimen = tdimen;
        this->value=value;
    }
    void set_value(int d1,int d2,int value){
        int m = d1*dimen2+d2;
        *(this->value+m)=value;
    }

    int get_value(int di1,int di2) {
        int m = di1 * dimen2 + di2;
        return *(value + m);
    }

    void get_value_arr(int di1, int * value_des){
        if(di1!=-1){
            int m = di1*dimen2;
            int i=0;
            for(i=0;i<dimen2;i++){
                *(value_des+i) = *(value+m+i);
            }
        }else {
            int total = dimen1*dimen2;
            for(int i=0;i<total;i++){
                *(value_des+i)=*(value+i);
            }
        }
    }
};

class mid_mips_term{
public:
    char name[20];
    char type[20];
    char offset[20];
    bool is_pointer;

    mid_mips_term(const char * name,const char* type,int offset,bool is_pointer= false){
        strcpy(this->name,name);
        strcpy(this->type,type);
        char temp[20];
        //itoa(offset,temp,10);
        number_to_string1(offset,temp);
        strcpy(this->offset,temp);
        this->is_pointer=is_pointer;
    }
    void number_to_string1(int value,char * string){
        int length =0;
        int mod;
        char temp[20];
        while(value>=10){
            mod =  value%10;
            temp[length++] = mod+'0';
            value = value/10;
        }
        temp[length] = value+'0';
        for(int i=0;i<=length;i++){
            string[i]=temp[length-i];
        }
        string[length+1]='\0';
    }
};

class var_pair{
public:
    char  name[20];
    char subscript[20];
    bool have_down_sign;
    var_pair(char * name,char * offset,bool have_downsign= false){
        strcpy(this->name,name);
        strcpy(this->subscript, offset);
        have_down_sign= have_downsign;
    }

};

