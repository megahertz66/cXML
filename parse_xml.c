#include "parse_xml.h"
#include "hash.h"
#include "stack.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

char *filePath = NULL;

/* 使用read存在兼容性问题。故使用 load_xml_file_v2 函数 */
char *load_xml_file(int fd)
{
    struct stat fileInfo;
    int tmp_fd = fd;
    long Fsize = 0;
    char *fileMem = NULL;

    /*read all of file*/
    fstat(tmp_fd, &fileInfo);
    Fsize = fileInfo.st_size;
    /*ֱput file into memery*/
    fileMem = (char *)calloc(1, sizeof(char)*Fsize);
    read(tmp_fd, fileMem, Fsize);

    return fileMem;
}


char *load_xml_file_v2(char *fileName, int nameLengh)
{
    FILE *fd;
    char *fileMem = NULL;
    long lSize;
    filePath = fileName;

    fileName[nameLengh+1] = '\0';
    fopen(fileName, "rb+");          //TODO: 对于打开错误的判断
    fseek (fd , 0 , SEEK_END);
    lSize = ftell (fd);
    rewind (fd);

    fileMem = (char *)calloc(1, sizeof(char)*lSize+1);
    fread (fileMem,1,lSize,fd);
    fclose(fd);

    return fileMem;
}

int remove_xml_file(char *needFree)
{
    free(needFree);
    needFree = NULL;
    return 0;
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
    char *tmpValue = (char *)calloc(1024, sizeof(char));    //save the value
    x_tree_t *treeRoot = NULL;      //result rootks
    int valLengh = 0;
    while(*pf != '\0'){
        if('<' == *pf){
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

                        pf += valLengh;
                        break;
                    }
                }
            }
        }
        pf++;
    }
    return treeRoot;
}


x_tree_t *lode_xml_to_tree(char *filePath, int pathLengh)
{
    char *xmlfile = NULL;
    x_tree_t *treeRoot = NULL;

    xmlfile = load_xml_file_v2(filePath, pathLengh);
    treeRoot = parse_xml(xmlfile);
    remove_xml_file(xmlfile);

    return treeRoot;
}















