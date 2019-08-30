#include "parse_xml.h"
#include "hash.h"
#include "stack.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


char *load_xml_file(int fd)
{
    int tmp_fd = fd;
    long Fsize = 0;
    char *fileMem = NULL;
    char *tmp = NULL;

    /*read all of file*/
    Fsize = lseek(tmp_fd, 0, SEEK_END);
    lseek(tmp_fd, 0, SEEK_SET);

    /*ֱput file into memery*/
    fileMem = (char *)malloc(sizeof(char)*Fsize+1);
    read(tmp_fd, fileMem, Fsize);

    return fileMem;
}

/* unused*/
char *parse_note(char *start)
{
    int i = 0;
    char *p_tmp = start;
    if( (*p_tmp=='<') && (*(p_tmp+ 1)=='!') && (*(p_tmp+ 2)=='-') && (*(p_tmp+ 3)=='-') ){
        for(i=3; i<500; i++){
            if( (*(p_tmp)+ i=='>') && (*(p_tmp+ i-1)=='-') && (*(p_tmp+ i-2)=='-')){
                return p_tmp+i+1;
            }
        }
    }
    return NULL;
}

/* 是否需要去空白？*/
int save_value(char *positon, int posSize, char *maybeValue)
{
    int i = 0;
    char *p_tmp = NULL;

    while(i < posSize-1){
        if(*(maybeValue) == '<'){
            break;
        }
        positon[i++] = *maybeValue++;
    }
    positon[i] = '\0';
    return i;
}


/*
    弹栈与下一个压栈为兄弟，入栈与下一个压栈为父子
    TODO: 如果要兼容文件中存在 '\0' 的情况是不是将 while(*pf != '\0') 结合文件长度判断
*/
x_tree_t *parse_xml(char *root)
{
    char *pf = root;
    char tmpName[30] = {0};     //max label name
    char *tmpValue = (char *)calloc(1024, sizeof(char));    //save the value
    x_tree_t *treeRoot = NULL;      //result rootks
    x_tree_t *tmpSibling = NULL;    //record the sibling
    while(*pf != '\0'){
        if('<' == *pf){
            int i = 0;
            int valLengh = 0;
            int tmpHash = 0;
            char *xmlName = (char *)calloc(31, sizeof(char));
            char *tmpXmlName = xmlName;
            pf++;
            if(*pf == '/')  pf++;   //ignore the '/'
            while(*pf != '>'){
                *xmlName++ = *pf++;    //ignore the '<' and copy the lable
                if(*(pf) == '>'){
                    char * saveValue = NULL;
                    *xmlName = '\0';     //make string
                    tmpHash = adler_32(tmpXmlName);    //hash the lable name
                    /* nearby the same lable */
                    //此处if 和else if的条件有些多余，为了逻辑清晰，暂时保留。
                    if( (topStack() != NULL) && ((x_tree_t *)topStack())->hashNode == tmpHash ){
                        free(xmlName);      //repeat the lable name
                        if(lenStack() == 1){
                            treeRoot =  (x_tree_t *)popStack();  //it's tree root
                        }
                        else popStack();
                    }
                    /* nearby the diff lable */
                    else if( (topStack() == NULL) || ( ((x_tree_t *)topStack())->hashNode != tmpHash) ){
                        x_tree_t *tmpLable = (x_tree_t *)malloc(sizeof(x_tree_t));
                        /* get the content. now pf=='>' , 此处可以确定pf位于value位置 将pf移动置 '>' 位置*/
                        valLengh = save_value(tmpValue, 1023, pf+1);           
                        saveValue = (char *)calloc(valLengh, sizeof(char)+1);
                        strcpy(saveValue, tmpValue);

                        tmpLable->key = xmlName;
                        tmpLable->hashNode = tmpHash;
                        tmpLable->child = NULL;
                        tmpLable->sibling = NULL;
                        tmpLable->value = saveValue;
                        // find his father and sibling
                        if(topStack() != NULL){
                            /* 
                             * 判断父亲是否有儿子，如果有就不要在认爸爸了。
                             * 如果父亲有儿子，就认儿子当兄弟，如果儿子有兄弟就认儿子的儿子当兄弟。
                             * "认贼作父"
                             */
                            if( ( (x_tree_t *)topStack() )->child == NULL){
                                ( (x_tree_t *)topStack() )->child = tmpLable;
                            }
                            else{
                                while( ( (x_tree_t *)topStack() )->child->sibling != NULL){
                                    ( (x_tree_t *)topStack() )->child->sibling = \
                                    ( (x_tree_t *)topStack() )->child->sibling->sibling;
                                }
                                ( (x_tree_t *)topStack() )->child->sibling = tmpLable;
                            }
                        }
                        pushStack((void *)tmpLable);    //push lable in stack
                    }
                }
            }
        }
        pf++;
    }
    return treeRoot;
}

// the rount like  address.linkman.email == van_darkholme@163.com
int rountToHash(char *rount, unsigned int rountLengh, int arrayIn[], int arrayLengh)
{
    char *pstart = rount;
    char *pend   = NULL;
    int count = 0;
    int result = 0;

    if(!rount) return result++;

    if(strchr(pstart, '.')){
        do{
            pend = strchr(pstart, '.');
            *pend = '\0';
            arrayIn[count++] = adler_32(pstart);
            pstart = pend+1;
        }
        while(pend && (count<arrayLengh));
        arrayIn[count] = adler_32(pstart);      //parse the last lable
        arrayLengh = count;
    }
    else{
        arrayIn[0] = adler_32(rount);
        arrayLengh = 1;
    }
    return result;
}


int xml_show(char *treeOut, unsigned int treeLengh)
{
    return 0;
}




int xml_find(char *routeIn, unsigned int rountLengh, char *treeOut, unsigned int treeLengh)
{
    return 0;
}













