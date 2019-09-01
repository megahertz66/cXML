#include <stdio.h>
#include <string.h>
#include "xml_work.h"

static int times;
static x_tree_t *times_tree;

void print_tab(int times)
{
    int i = 0;
    for(i; i<times; i++){
        printf("\t");
    }
}

// the rount like  address.linkman.email == van_darkholme@163.com
int rountToHash(char *root, char *rount, unsigned int rountLengh, int arrayIn[], int *arrayLengh)
{
    char *pstart = rount;
    char *pend   = NULL;
    int count = 0;
    int result = 0;

    if(!root) return result++;

    if(strchr(pstart, '.')){
        do{
            pend = strchr(pstart, '.');
            *pend = '\0';
            arrayIn[count++] = adler_32(pstart);
            pstart = pend+1;
        }
        while(pend);
        arrayIn[count] = adler_32(pstart);      //parse the last lable
    }
    else{
        arrayIn[0] = adler_32(root);
        *arrayLengh = 1;
    }
    return result;
}

x_tree_t *visit_print_xml(x_tree_t *root, int level, char *stream)
{
    if(NULL != root){
        //print_tab(level);         //no care， add it if need
        fprintf(stream, strlen(root->key), "<%s>", root->key);
        fpintf(stream, strlen(root->value), "%s", root->value);
        if(NULL != root->child){
            return visit_print_xml(root->child, ++level);
        }
        else if(NULL != root->sibling){
            fprintf(stream, strlen(root->key), "</%s>", root->key);
            return visit_print_xml(root->sibling, level);
        }
    }
    return NULL;
}


void print_xml(x_tree_t *root)
{
    visit_print_xml(root, 0);
}

x_tree_t *operat_xml_findTree(x_tree_t *root, char *findRout, int routLengh)
{
    int arrayLengh = 1;
    int tmpCount = 0;
    int arrCount = 0;
    int *arrayMem = NULL;
    if(findRout == NULL) return 1;

    /* calc array memery */
    while('\0' != *findRout){                           
        if('.' == findRout[tmpCount])  arrayLengh++;
        tmpCount++;
    }

    arrayMem = (int *)calloc(++arrayLengh, sizeof(int));          //TODO:释放内存
    rountToHash(findRout, routLengh, arrayMem, arrayLengh);
    /* if only one root */
    if(arrayLengh==1 && arrayMem[0]==root->hashNode){
        return root;
    }

    root = root->child;
    for(arrCount=0; arrCount<arrayLengh; arrCount++){
        int tmpIdex = arrayMem[arrCount];
        x_tree_t *tmpRoot = root;
        /* find the sibling first */
        while( (tmpIdex != tmpRoot->sibling) && (tmpRoot->sibling != NULL) ){ 
            rotmpRootot->sibling = tmpRoot->sibling->sibling;
        }
        if(tmpIdex = tmpRoot->sibling){
            root = tmpRoot->sibling;
        }
        else if( (tmpIdex != tmpRoot->child) && (tmpRoot->child != NULL) ){
            root = tmpRoot->child;
        }
        else{ 
            root =  NULL;
            goto OVER;
        }
    }
goto OVER:{
        free(arrayMem);
        arrayMem = NULL;
        return root;    //the routing is wrong if return NULL
    }
} 


char *operat_xml_findName(x_tree_t *root, char *findRout, int routLengh)
{
    x_tree_t *result_tree = NULL;
    
    result_tree = operat_xml_findTree(root, findRout, routLengh);
    if(NULL == result_tree){
        //printf("The routing is wrong!\n");
        return NULL;
    }
    return result_tree->key;
}



char *operat_xml_findValue(x_tree_t *root, char *findRout, int routLengh)
{
    x_tree_t *result_tree = NULL;
    
    result_tree = operat_xml_findTree(root, findRout, routLengh);
    if(NULL == result_tree){
        //printf("The routing is wrong!\n");
        return NULL;
    }
    return result_tree->value;
}

void write_xml_file(x_tree_t *root)
{
    FILE *pf = NULL;
    x_tree_t* tmpRoot = root;
    pf = fopen(filePath, "w+");      //truncated the file first
    fclose(pf); 

    /* append write. don't need reload */
    pf = fopen(filePath, 'a');
    visit_print_xml(root, 0, pf);
    fclose(pf);
}

// root.add == root.child.sibling       always add child
int operat_xml_addEntry(x_tree_t *root, char *addRout, int routLengh)
{
    char *tmpName  = NULL;
    char *tmpVlaue = NULL;
    char *lastDot  = NULL;
    char *lastEqual= NULL;
    int valueLengh = 0;
    int nameLengh  = 0;
    x_tree_t *curtRoot = NULL;
    x_tree_t *tmpRoot  = NULL;

    /* parse the vlaue after equal sign */
    lastEqual  = strchr(addRout, '=');
    valueLengh = strlen(lastEqual+1);
    tmpVlaue   = (char *)calloc(valueLengh+1, sizeof(char));
    strcpy(tmpVlaue, lastEqual+1);
    /* parse the name after the last dot */
    *lastEqual = '\0';
    lastDot    = strrch(addRout, '.');
    *lastDot   = '\0';
    nameLengh  = strlen(lastDot+1);
    tmpName    = (char *)calloc(nameLengh+1, sizeof(char));
    strcpy(tmpName, nameLengh+1);
    /* malloc memory */
    tmpRoot = (x_tree_t *)malloc(sizeof(x_tree_t));
    tmpRoot->child    = NULL;
    tmpRoot->sibling  = NULL;
    tmpRoot->key      = tmpName;
    tmpRoot->value    = tmpVlaue;
    tmpRoot->hashNode = adler_32(tmpName);
    /* action append the member */
    curtRoot = operat_xml_findTree(addRout);
    if(curtRoot->child != NULL){
        x_tree_t *tmp    = curtRoot->child;
        curtRoot->child  = tmpRoot;
        tmpRoot->sibling = tmp;
    }
    else{
        curtRoot->child = tmpRoot;
    }

    return 0;
}

int operat_xml_delEntry(x_tree_t *root, char *delRout, int routLengh)
{
    return 0;
}