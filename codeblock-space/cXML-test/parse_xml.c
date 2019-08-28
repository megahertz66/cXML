#include "parse_xml.h"
#include "hash.h"
#include "stack.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


char *load_xml_file(FILE *fd)
{
    FILE *tmp_fd = fd;
    long Fsize = 0;
    char *fileMem = NULL;

    /*read all of file*/
    fseek(tmp_fd, 0, SEEK_END);
    Fsize = ftell(tmp_fd);
    rewind(tmp_fd);

    /*ֱput file into memery*/
    fileMem = (char *)calloc(Fsize+1, sizeof(char));
    fprintf(tmp_fd, "%s", fileMem);

    return fileMem;
}


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
        if(*(maybeValue+1) == '<'){
            p_tmp = parse_note(maybeValue+1);
            if(p_tmp){
                maybeValue = p_tmp;
            }
            else{
                return i;
            }
        }
        positon[i++] = (*maybeValue)++;
    }
    positon[i] = '\0';
    return i;
}


/*
    弹栈与下一个压栈为兄弟，入栈与下一个压栈为父子
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
            if(*(pf+1) == '/')  pf++;   //ignore the '/'
            while(*pf != '>'){
                char * saveValue = NULL;
                tmpName[i++] = *(++pf);    //ignore the '<' and copy the lable
                if(*(pf+1) == '>'){
                    char *xmlName = (char *)calloc(31, sizeof(char));
                    tmpName[i] = '\0';     //make string
                    strcpy(xmlName, tmpName);
                    tmpHash = adler_32(xmlName);    //hash the lable name
                    /* nearby the same lable */
                    //此处if 和else if的条件有些多余，为了逻辑清晰，暂时保留。
                    if( (topStack() != NULL) && ((x_tree_t *)topStack())->hashNode == tmpHash ){
                        free(xmlName);      //repeat the lable name
                        // save his sibling 质疑！
                        tmpSibling = (x_tree_t *)topStack();
                        if(lenStack() == 1){
                            treeRoot =  (x_tree_t *)popStack();  //it's tree root
                        }
                        else popStack();
                    }
                    /* nearby the diff lable */
                    else if( (topStack() == NULL) || ( ((x_tree_t *)topStack())->hashNode != tmpHash) ){
                        x_tree_t *tmpLable = (x_tree_t *)malloc(sizeof(x_tree_t));

                        /* get the content. now pf=='>' , 此处可以确定pf位于value位置*/
                        valLengh = save_value(tmpValue, 1024, pf);
                        saveValue = (char *)calloc(valLengh, sizeof(char)+1);
                        strcpy(saveValue, tmpValue);

                        tmpLable->key = xmlName;
                        tmpLable->hashNode = tmpHash;
                        tmpLable->child = NULL;
                        tmpLable->sibling = NULL;
                        tmpLable->value = saveValue;
                        // find his father and sibling
                        if(topStack() != NULL){
                            ( (x_tree_t *)topStack() )->child = tmpLable;
                            if(tmpSibling != NULL){
                                tmpSibling->sibling = tmpLable;
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
