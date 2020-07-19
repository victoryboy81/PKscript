#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  PRESTR "repair_"

//***************************************************************************//
//global variable
char Tmpbuffer[300];
char buf2[300]={0};
char vector[300]={0};
int  scriptkeyposarr[20];
//***************************************************************************//

//***************************************************************************//
const int FUNCOUNT=5;
const int MAXChARCOUNT=30;
char funnametable[][30]={"DELETEPIN"                  ,
                         "INSERTPIN"                  ,
                         "COPYPASTEPIN"               ,
                         "REPLAYSYMBOLPIN"            ,
                         "TRIGSYMBOLZPIN"             };
//***************************************************************************//

//***************************************************************************//
//script mode SELECT table
//㏑砏玥:functionname_updatesymbol_rulekeywordstring_rulekeysymbol
enum scriptmode{
     DELETEPIN =1     ,
     INSERTPIN        ,
     COPYPASTEPIN     ,
     REPLAYSYMBOLPIN  ,
     TRIGSYMBOLZPIN
} SELMODE;

//***************************************************************************//

//***************************************************************************//
//ъscript  data尿癸莱﹚簍衡猭矪瞶
struct repairpinparam{
    int funparm1;
    int funparm2;
    int mode;
};
typedef struct repairpinparam repairpinparam;

struct repairpinparamarr{
    int funparm1[3];//や穿κ计
    int funparm2[3];//や穿κ计
    char mode[30];  //や穿30じ
    int funparm1len;//计じ计秖
    int funparm2len;//计じ计秖
    int modelen;    //じ计秖
};
typedef struct repairpinparamarr repairpinparamarr;

struct copysymbol{
    int pin[3];
    int repeat[3];
    int pinlen;
    int repeatlen;
};
typedef struct copysymbol copysymbol;
struct param{
    int pin;
    int repeat;
};
typedef struct param param;

struct funrepairparam{
    char updatesymbol;
    char rulesymbol;
    char rulecommentstr[20];
};
typedef struct funrepairparam funrepairparam;

//***************************************************************************//

//***************************************************************************//

//STEP1 script ノ 簍衡猭
int is_tableend(char * tabletempstr){
   char endstr[5]="end";
   int i=0,j=0;
   while(tabletempstr[i]!='\0' && endstr[j]!='\0'){
        if(tabletempstr[i]!=' '){
            if(tabletempstr[i] == endstr[j]){
                 //printf("the Tablefd of pointer is %c\n",tabletempstr[i]);
                if(j==2)
                    break;

            }
            else{
                break;
            }


            j++;
        }
        i++;
    }
    if(j==2)
        return 1;
    else if(j<2)
        return -1;
}

void searchkeyposarr(char* buf,char key,int* outputindexlen){
    int index=0,state=0,p=0,i=0;

    //scriptkeyposarr
    for(i=0;i<100;i++){

        //printf("%d\n",p);
        if(buf[i] == key  ){
            //printf("%d\n",p);
            index++;
            scriptkeyposarr[index-1]=i;
        }
        if(index==20){
           break;
        }
    }
    *outputindexlen=index;
    /*if(*outputindexlen != 0){
        return scriptkeyposarr;
    }*/
    /*else if(*outputindexlen == 0){
         printf("not script keyword symbol :\n");
    }*/

}

int searchkeypos(char* buf,char key){
    int index=0,state=0,p=0;
    while(1){
        if(buf[p] == 0){
            state=-1;
            //printf("%d\n",p);
            break;
        }
        else{
            //printf("%d\n",p);
            if(buf[++p] == key  ){
                //printf("%d\n",p);
                index=p;
                break;
            }
        }
    }
    if(state != -1){
        return index;
    }
    else{
        return state;
    }

}
///////////////////////////////////////////////////////////////
//STEP2 矪瞶PTN 癸莱簍衡猭
//a^b;b Ωよ计
int pow(int a,int b){
   if(b==0)
        return 1;

    return pow(a,b-1)*a;
}
param intarrtointvalue(copysymbol* table,int tableline){
    int i=0,j=0;
    int value=0;
    param valueparam;
    valueparam.pin=0;
    valueparam.repeat=0;
           // for(i=0;i<tableline-1;i++){
                  for(j=0;j<table[tableline].pinlen;j++) {
                        if((table[tableline].pinlen-j-1)>0)
                            valueparam.pin=pow(10, table[tableline].pinlen-j-1)* table[tableline].pin[j] + valueparam.pin;
                        else if((table[tableline].pinlen-j-1)==0)
                            valueparam.pin=table[tableline].pin[j] + valueparam.pin;

                       // printf("%d\n",table[tableline].pinlen)  ;
                       // printf("copy1[%d].pin[j] %d\n",j,table[tableline].pin[j])  ;
                    }
                    for(j=0;j<table[tableline].repeatlen;j++) {
                        if((table[tableline].repeatlen-j-1)>0)
                            valueparam.repeat=pow(10, table[tableline].repeatlen-j-1) * table[tableline].repeat[j] + valueparam.repeat;
                        else if((table[tableline].repeatlen-j-1)==0)
                            valueparam.repeat=table[tableline].repeat[j] + valueparam.repeat;

                       // printf("%d\n",table[tableline].repeatlen)  ;
                       // printf("copy1[%d].repeat[j] %d\n",j,table[tableline].repeat[j])  ;
                    }
                   // printf("\n")  ;
           // }
           // printf("\n")  ;
    return valueparam;
}

int getmodekeyposcount(repairpinparamarr* table,int tableline){
    int count=0;
    int j=0;
     for(j=0;j<table[tableline].modelen;j++) {
        if(table[tableline].mode[j]=='_'){
            count++;
        }
    }

    return count;

}



int ParsingMatchstr(repairpinparamarr *strToken,int tableline){
    //funnametable
    int j=0,i=0;
    int matchpassflag=0;
    int matchpassflagindex=-1;
    for(i=0;i<FUNCOUNT;i++){
        for(j=0;j<MAXChARCOUNT;j++){
            if(strToken[tableline].mode[j]==funnametable[i][j] && funnametable[i][j]!='\0' ){
                matchpassflag=1;
            }
            else if(funnametable[i][j]=='\0' ){
                break;
            }
            else{
                matchpassflag=0;
                break;
            }
        }
        if(matchpassflag==1){
           matchpassflagindex=i;
           break;
        }
    }
    if(matchpassflagindex >=0 && matchpassflag==1){
        return matchpassflagindex;
    }
    else {
         return -1;
    }
}
 funrepairparam getfunparameter(repairpinparamarr* strToken, int modekeyposcount,int tableline){
    funrepairparam param;
    int i=0,count=0,index=0,flag=0,flag2count=0,flag3count=0,j=0;
    const char buffchk[3]="IS";
    int actionflag=0;
    param.rulesymbol='-';
    param.rulecommentstr[0]='-';
    param.rulecommentstr[1]='\0';
    param.updatesymbol='-';

    if(modekeyposcount>0){

       for(i=0;i < strToken[tableline].modelen;i++){

           if(strToken[tableline].mode[i]=='_'){
                count++;
                modekeyposcount--;
                if(count==1 && modekeyposcount>=0){
                    //index=i;
                    flag=1;
                }
                else if(count==2 && modekeyposcount>=0){
                    flag=2;
                }
                else if(count==3 && modekeyposcount>=0){
                    flag=3;
                }


           }
           else if(flag==1 ){
                //flag=0;
                param.updatesymbol=strToken[tableline].mode[i];

           }
           else if(flag==2 ){
                //flag=0;
                flag2count++;
                if(flag2count<20){
                    param.rulecommentstr[flag2count-1]=strToken[tableline].mode[i];
                }
                else{
                    param.rulecommentstr[0]='*';
                }

           }
           else if(flag==3 ){
                flag3count++;

                if(strToken[tableline].mode[i]==buffchk[flag3count-1] && buffchk[flag3count-1]!='\0'){
                      actionflag=1;
                     //printf("flag:%d;;I:%d;TOKEN:%c\n",actionflag,flag3count,strToken[tableline].mode[i]) ;
                    continue;
                }
                else if(strToken[tableline].mode[i]!=buffchk[flag3count-1] && buffchk[flag3count-1]!='\0'){
                    actionflag=0;
                    param.rulesymbol='*';
                    break;
                }
                else if(actionflag==1 ){
                    param.rulesymbol=strToken[tableline].mode[i];
                    break;
                }
           }
           if(flag==2){
              param.rulecommentstr[flag2count]='\0';
           }

      }

    }

    return param;
}

int searchkeyword(char* keyword,char* source){
    int i,j;
    char symbol=0;
    symbol=*source;
    int count=0;
    int keypos=0;
    int matchflag=0;
    int startflag=1;
    int count1=0;


        //--------------------------
        while(symbol!=0){
            if(symbol==keyword[count1] && keyword[count1]!='\0' ){
                matchflag=1;
                if(startflag==1){
                    keypos=count;
                    startflag=0;
                }
                count1++;
            }
            else{
                if(keyword[count1]!='\0'){
                    matchflag=0;
                    startflag=1;
                    count1=0;
                }
            }

            source=source+1;
            symbol=*source;
            count++;

        }
        //---------------------------
        if( matchflag==1 ){

            return keypos;
        }
        else{
             return -1;
        }
        //------------------------------
}


void insert(char* source,char* character,int pos){
    char temp1;
    char temp2;
    int length=0;
    int i=0,j=0;

    while(source[j]!=0 && j<1000){
        j++;
    }
    length=j;
    temp2=source[pos];
    for(i=0;(i<length-pos) && (length-pos)>=0;i++){
        temp1=source[pos+i+1];
        source[pos+1+i]=temp2;
        temp2=temp1;
    }
    source[pos]=character[0];
    return ;
}

void deletpin(char* source,int pos){
    char temp1;
    char temp2;
    int length=0;
    int i=0,j=0;

    while(source[j]!=0 && j<1000){
        j++;
    }
    length=j;
    //temp2=source[pos];
    for(i=0;(i<length-pos) && (length-pos)>=0;i++){
        temp1=source[pos+i+1];
        source[pos+i]=temp1;
    }

    return;
}

void copypastepin(char* source,int sourcepinsymbol,int destination){
    char copysymbol;
    copysymbol=*(source+sourcepinsymbol);
    insert(source,&copysymbol,destination);
}

void replacesymbol(char* source ,int pos,char symbol){

    *(source+pos)=symbol;
}
void replacesymbol_ISsymbol(char* source ,int pos,char symbol,char trigger){
    if(*(source+pos)==trigger){
        *(source+pos)=symbol;
    }
}

void trigsymbolz(char* source,char* keyword,int pos){

    if(searchkeyword(keyword,source)!=-1){
        insert(source,"Z",pos);
    }
    else{
        insert(source,"X",pos);
    }

}

void trigsymbolz_ISsymbol(char* source,char* keyword,int pos1,int pos2,char symbol,char trigger){


    if(searchkeyword(keyword,source)!=-1){
        if(*(source+pos1)==trigger){
            insert(source,"Z",pos2);
            replacesymbol(source ,pos1,symbol);
        }
        else{
            insert(source,"X",pos2);
        }
    }
    else{
        insert(source,"X",pos2);
    }



}


int possymbolmatch(char* source ,int pos,char symbol){
    char match;
    match=*(source+pos);
    //printf("match:%c\n",match);
    if(match==symbol){
        return 1;
    }
    else{
        return -1;
    }

}

void ParsingModestrToken(repairpinparam strToken,int modekeyposcount,char* Rbuffer,int Vectorbegin,void* data){
    /*
    enum scriptmode{
     DELETEPIN =1     ,
     INSERTPIN        ,
     COPYPASTEPIN     ,
     REPLAYSYMBOLPIN  ,
     TRIGSYMBOLZPIN
} SELMODE;
*/
//㏑砏玥:functionname_updatesymbol_rulekeywordstring_rulekeysymbol
funrepairparam *funparam;
char tempchar=strToken.funparm2;
funrepairparam* tempcharptr=0;
    if(modekeyposcount==0){
        //fucntion->normal mode
        switch(strToken.mode){
            case 0:
                deletpin(Rbuffer,Vectorbegin+strToken.funparm1); //OK
                break;
            case 1:
                insert(Rbuffer,&tempchar,Vectorbegin+strToken.funparm1); //OK
                break;
            case 2:
                copypastepin(Rbuffer,Vectorbegin+strToken.funparm1,Vectorbegin+strToken.funparm2); //OK
                break;
            case 3:
                replacesymbol(Rbuffer ,Vectorbegin+strToken.funparm1,strToken.funparm2);   //OK
                break;
            case 4:
                trigsymbolz(Rbuffer,"//",Vectorbegin+strToken.funparm1); //OK , keyword:"//"
                break;
            default:
                printf("cann't recongnize function name");
                break;
        }
    }
    else if(modekeyposcount==1){
        //fucntion->pattern爹秆闽龄 action 把计
        switch(strToken.mode){
            case 1:
                funparam=(funrepairparam*)data;
                tempchar=funparam->updatesymbol;
                insert(Rbuffer,&tempchar,Vectorbegin+strToken.funparm1);    //OK
                break;
            case 3:
                funparam=(funrepairparam*)data;
                tempchar=funparam->updatesymbol;
                //printf("symbol:%c\n",tempchar);
                replacesymbol(Rbuffer ,Vectorbegin+strToken.funparm1,tempchar); //OK
                break;
            default:
                printf("cann't recongnize function name");
                break;
        }
    }
    else if(modekeyposcount==2){
        //fucntion->pattern爹秆闽龄 action 把计籔耞symbol碞牟祇赣mode把计
        switch(strToken.mode){
            case 4:
                funparam=(funrepairparam*)data;
                //printf("symbol:%s\n",funparam->rulecommentstr);
                trigsymbolz(Rbuffer,funparam->rulecommentstr,Vectorbegin+strToken.funparm1); //OK
                break;
            default:
                printf("cann't recongnize function name");
                break;
        }
    }
    else if(modekeyposcount==3){
        //fucntion->pattern爹秆闽龄 action 把计籔耞symbol碞牟祇赣mode把计
        switch(strToken.mode){
            case 3:
                funparam=(funrepairparam*)data;
                replacesymbol_ISsymbol(Rbuffer,Vectorbegin+strToken.funparm1,funparam->updatesymbol,funparam->rulesymbol); //OK
                break;
            case 4:
                funparam=(funrepairparam*)data;
                trigsymbolz_ISsymbol(Rbuffer,funparam->rulecommentstr,Vectorbegin+strToken.funparm1,Vectorbegin+strToken.funparm2,funparam->updatesymbol,funparam->rulesymbol); //OK
                break;
            default:
                printf("cann't recongnize function name");
                break;
        }
    }
    else{
       printf("cann't recongnize function name");
    }

    return ;
}

repairpinparam arrtovalue2(repairpinparamarr* table,int tableline){
    int i=0,j=0;
    int value=0;
    repairpinparam valueparam;
    valueparam.funparm1=0;
    valueparam.funparm2=0;
    valueparam.mode=0;
    int modeparsingkeypos=0;
    //modeparsingkeypos=getmodekeyposcount(table,tableline);
           // for(i=0;i<tableline-1;i++){
                  for(j=0;j<table[tableline].funparm1len;j++) {
                        if((table[tableline].funparm1len-j-1)>0)
                            valueparam.funparm1=pow(10, table[tableline].funparm1len-j-1)* table[tableline].funparm1[j] + valueparam.funparm1;
                        else if((table[tableline].funparm1len-j-1)==0)
                            valueparam.funparm1=table[tableline].funparm1[j] + valueparam.funparm1;

                       // printf("%d\n",table[tableline].pinlen)  ;
                       // printf("copy1[%d].pin[j] %d\n",j,table[tableline].pin[j])  ;
                    }
                    for(j=0;j<table[tableline].funparm2len;j++) {
                        if((table[tableline].funparm2len-j-1)>0)
                            valueparam.funparm2=pow(10, table[tableline].funparm2len-j-1) * table[tableline].funparm2[j] + valueparam.funparm2;
                        else if((table[tableline].funparm2len-j-1)==0)
                            valueparam.funparm2=table[tableline].funparm2[j] + valueparam.funparm2;

                       // printf("%d\n",table[tableline].repeatlen)  ;
                       // printf("copy1[%d].repeat[j] %d\n",j,table[tableline].repeat[j])  ;
                    }

                    valueparam.mode=ParsingMatchstr(table,tableline);

    //printf("key:_ :%d",getmodekeyposcount(table,tableline));
    return valueparam;
}



int repeatinsert(char* source,char* character,int pos,int repeat){
    int i=0;
    if(pos!=-1)
        for(i=0;i<repeat;i++){
            insert(source,character,pos+i);
        }

    return -1;
}




void getPTNvectorpos(char* source,int* vectorbigin,int* vectorend){//眔SYMBOL VECTOR癬籔Ю

    int begin;
    int i=0,j=0;
    int flag=0;
    int charsflag=0;
    begin=searchkeypos(source,'%');
    //printf("%d\n",begin);
    while(begin!=-1){
        if(source[i+begin+1]==' ' || source[i+begin+1]=="\/" || source[i+begin+1]=='\t' || source[i+begin+1]=='\n'){
            if(charsflag==0){
                flag=1;
            }
            else if(charsflag==1){
                flag=2;
            }

        }
        else{
            if(flag>0)
                charsflag=1;

            if(source[i+begin+1]!=';'){
                 if(flag==1 && j==0)
                    *vectorbigin=i+begin+1;
                vector[j]=source[i+begin+1];
            }
            else{
                *vectorend=*vectorend-1;
            }
            j++;
        }

        if(flag==2){
            *vectorend=i+begin+1+*vectorend;
            flag=0;
            break;
        }
        i++;
    }
    if(begin==-1){
        *vectorend=-1;
        *vectorbigin=-1;
    }
    return;
}
///////////////////////////////////////////////////////////////

//***************************************************************************//


int getstrlength(char* source,const int maxnum){
    int j=0;
    while(source[j]!=0 && j<maxnum){
        j++;
    }
    return j;
}

char* getbetweenchrs(char* buf,int begin ,int end){
    int p=begin,state=0,count=0;

    while(1){
        if( begin == -1 || end == -1){
            buf2[0]=0;
            break;
        }
        if(buf[p] == 0){
            state=-1;
            printf("%d\n",p);
            break;
        }
        else{
            //printf("%d\n",p);
            if( p<end ){
                    p++;
                //printf("%d\n",p);
                if(buf[p]==' '){
                }
                else{
                    buf2[count++]=buf[p];
                }
            }
            else{
                //buf2[count]='\n';
                break;
            }
        }
    }
    return buf2;
}


char* getPTNvector(char* source){
    int begin=0,flag=0;
    int i=0,j=0;

    begin=searchkeypos(source,'%');
    while( 1){
        if(source[i+begin+1]==' ' || source[i+begin+1]=="////"){
            flag++;
        }
        else{
            if(source[i+begin+1]!=';'){
                vector[j]=source[i+begin+1];
            }
            j++;
        }
        if(flag==2){
            flag=0;
            break;
        }
        i++;
    }
    return vector;
}


int loadpintable(char* tabletempstr,copysymbol copy1,int counter){

    return 0;
}

int main(int argc,char* argv[]){

    FILE* Rfd,*Wfd,*Tablefd;
    copysymbol copy1[300];
    char Rbuffer[1000];
    //param transparam;
    repairpinparam transparam;
    char* Wbuffer;
    int index=0;
    int begin;
    int end;
    int i=0,j=0,t=0,p=0,y=0,files=2,e=0;
    int pins=2;
    int repeat=3;
    int count=0,counter=0;
    int vectorbigin=0;
    int vectorend=0;
    char str;
    char wfdname[1000]={0};
    char tabletempstr[100]; //script read string buffer max count
    int r=0;
    int Readkeycount=0;
    int insertrep=0;
    int testdata=0;
    repairpinparamarr repairpinparamtable[1000];//script 瞏既300
    int modekeycount=0;
    funrepairparam funparam;
    //*******************************************************************************************//
    //弄batch 郎把计﹍て笆
    while(files < argc){
        memset(wfdname,0,1000);
        printf("files %d \n",files);
        printf("argc %d \n",argc);
        strcat(wfdname,"ptn\\");
        strcat(wfdname,argv[files]);
        memset(tabletempstr,0,100); //script read string buffer initial process
        Rfd=fopen(argv[files],"r");
        Wfd=fopen(wfdname,"w");
        Tablefd=fopen(argv[1],"r");

        //*******************************************************************************************//
        //script process flow
        if(Tablefd == NULL){ //PKscript is not exist
            printf("the PKscript file is no exist \n");
            printf("the PKscript file fd of pointer is %X\n",Tablefd);
        }
        else{ //PKscript is exist
            printf("the PKscript file is  exist\n");
            printf("the PKscript file fd of pointer is %X\n",Tablefd);
            while(fgets(tabletempstr,100,Tablefd)!=NULL){ //read string is total 100 character
                    if(counter >= 1 && is_tableend(tabletempstr) == -1){// PKscript instruction parameter 磞瓃Τ絛瞅

                        //printf("PKscript instruction parameter content:%s\n",tabletempstr);
                        while( t<100 ){ //羆PKscript –︽矪瞶程じ100
                             if(tabletempstr[t]=='\0'){ //狦赣︽instruction弄じ挡才腹'\0'玥挡
                                //printf("stringendpos: %d\n",t);
                                break;
                             }
                             if(tabletempstr[t]!=' ' && tabletempstr[t]!='\n'){
                                //眖PKscriptず甧耝把计籔家Α
                                searchkeyposarr(tabletempstr,':',&testdata);
                                //printf("keypos:%d;keylen:%d\n",testdata);

                                if(t < scriptkeyposarr[0] && testdata!=0){
                                    if(tabletempstr[t]!='*'){
                                        repairpinparamtable[counter-1].funparm1[p]=tabletempstr[t]-'0';//材把计皚(じ锣计)
                                        //printf("%d",t);
                                        //printf("char %c",tabletempstr[t]);
                                        printf("funparm1[%d] : %d\n",p,tabletempstr[t]-'0');
                                        p++;//材把计羆皚计ヘ(じ锣计)
                                    }
                                   /* else iftabletempstr[t]=='*'{
                                         copy1[counter-1].pin[p]

                                    }*/
                                 }
                                 else if(t > scriptkeyposarr[0] && t < scriptkeyposarr[1] && testdata!=0){
                                     if(tabletempstr[t]!='*'){
                                        repairpinparamtable[counter-1].funparm2[y]=tabletempstr[t]-'0';//材把计皚(じ锣计)

                                        //printf(": %d\n",t);
                                        printf("funparm2[%d] : %d\n",y,tabletempstr[t]-'0');
                                        //printf("%c",tabletempstr[t]);
                                        y++;//材把计羆皚计ヘ
                                     }
                                 }
                                 else if(t > scriptkeyposarr[1] && testdata!=0){
                                     if(tabletempstr[t]!='*'){
                                        repairpinparamtable[counter-1].mode[e]=tabletempstr[t];//材把计皚(SELECTMODE)

                                        //printf(": %d\n",t);
                                        printf("mode[%d] : %c\n",e,tabletempstr[t]);

                                        //printf("%c",tabletempstr[t]);
                                        e++;//材把计羆皚计ヘ
                                     }
                                 }

                                 //////////////////////////////////////////////

                              }

                        t++;//–Ω弄じ暗矪瞶
                        }
                    repairpinparamtable[counter-1].funparm1len=p;
                    //printf(" funparm1len : %d\n",p);
                    repairpinparamtable[counter-1].funparm2len=y;
                    //printf(" funparm2len : %d\n",y);
                    repairpinparamtable[counter-1].modelen=e;
                    //printf(" modelen : %d\n",e);

                    }
                    if(searchkeypos(tabletempstr,':') >=0 ){
                        if(counter>0){
                            printf("PKscript instruction parameter content:%s\n",tabletempstr);
                            printf("funparm1len : %d\n",p);
                            printf("funparm2len : %d\n",y);
                            printf("modelen : %d\n",e);
                            printf("instruction number is %d\n",counter);
                        }
                        counter++;
                    }
            //printf("instruction number is %d\n",counter-1);
            memset(Tmpbuffer,0,100);
            y=0;
            t=0;
            p=0;
            e=0;
            }
            fclose(Tablefd);
        }

        //*******************************************************************************************/

        //*******************************************************************************************//
        //ptn process flow
        if(Rfd == NULL){
            printf("the KYEC PTN file is no exist \n");
            printf("the KYEC PTN Rfd of pointer is %X\n",Rfd);

        }
        else{
            printf("the KYEC PTN file is  exist\n");
            printf("the KYEC PTN Rfd of pointer is %X\n",Rfd);
            while(fgets(Rbuffer,1000,Rfd)!=NULL){
                count++;

                begin=searchkeypos(Rbuffer,'%');
               // printf("%% : %d\n",begin);
                strcpy(Tmpbuffer,Rbuffer);
                getPTNvectorpos(Rbuffer,&vectorbigin,&vectorend);

                if(vectorbigin!=-1 && vectorend!=-1){

                    for(r=0;r < counter-1;r++){
                        transparam=arrtovalue2(repairpinparamtable,r);
                        modekeycount=getmodekeyposcount(repairpinparamtable,r);
                        //printf("SELMOD:%d\n",transparam.mode);
                        //printf("modekeycount:%d\n",modekeycount);
                        funparam=getfunparameter(repairpinparamtable, modekeycount,r);
                        //printf("funparam.rulecommentstr:%s\n",funparam.rulecommentstr);
                        //printf("funparam.rulesymbol:%c\n",funparam.rulesymbol);
                        //printf("funparam.updatesymbol:%c\n",funparam.updatesymbol);


                        ParsingModestrToken(transparam,modekeycount,Rbuffer,vectorbigin,&funparam);
                        //for(insertrep=0;insertrep<transparam.funparm2;insertrep++)
                        //    insert(Rbuffer,"0",vectorbigin+transparam.funparm1);

                    }
                }
                fputs(Rbuffer,Wfd);
                memset(Rbuffer,0,1000);

            }
            fclose(Rfd);
            fclose(Wfd);
        }
        //*******************************************************************************************//
        files++;
        counter=0;
        memset(Rbuffer,0,1000);
    }
    //*******************************************************************************************//
system("pause");
return 0;

}
