#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"
#include "xml_work.h"



void print_tab(int times, FILE *stream)
{
    int i = 0;
    fprintf(stream, "%c", '\n');
    for(i; i<times; i++){
        fprintf(stream, "%c", '\t');
    }
    return;
}

//TODO: 不要更改输入的变量。重新review code
// the rount like  address.linkman.email == van_darkholme@163.com
int rountToHash(x_tree_t *root, char *rount, unsigned int rountLengh, int arrayIn[], int *arrayLengh)
{
    char *pstart = NULL;
    char *pend   = NULL;
    int count = 0;
    int result = 0;

    pstart = (char *)calloc(rountLengh+1, sizeof(char));
    strcpy(pstart, rount);

    if(!rount) return result = 0;

    if(strchr(pstart, '.')){
        do{
            pend = strchr(pstart, '.');
            if(!pend){
                break;
            }
            *pend = '0';
            arrayIn[count++] = adler_32(pstart);
            pstart = pend+1;
        }
        while(pend);
        arrayIn[count] = adler_32(pstart);      //parse the last lable
    }
    else{
        arrayIn[0] = adler_32(pstart);
        *arrayLengh = 1;
    }
    free(pstart);
    return result;
}

//TODO:泛型指针
x_tree_t *visit_print_xml(x_tree_t *root, int level, FILE *stream)
{
    if(NULL != root){
        fprintf(stream, "<%s>", root->key);
        fprintf(stream, "%s", root->value);
        if(NULL != root->child){
            return visit_print_xml(root->child, ++level, stream);
        }
        fprintf(stream, "</%s>", root->key);
        print_tab(level, stream);
        if(NULL != root->sibling){
            return visit_print_xml(root->sibling, level, stream);
        }
    }
    return NULL;
}


void print_xml(x_tree_t *root)
{
    visit_print_xml(root, 0, NULL);
}

x_tree_t *operat_xml_findTree(x_tree_t *root, char *findRout, int routLengh)
{
    int arrayLengh = 1;
    int tmpCount = 0;
    int arrCount = 0;
    int *arrayMem = NULL;
    if(findRout == NULL) return NULL;

    /* calc array memery */
    while('\0' != findRout[tmpCount]){
        if(findRout[tmpCount] == '.')  arrayLengh++;
        tmpCount++;
    }

    arrayMem = (int *)calloc(++arrayLengh, sizeof(int));          //TODO:释放内存
    rountToHash(root, findRout, routLengh, arrayMem, &arrayLengh);
    /* if only one root */
    if( (arrayLengh==1) && (arrayMem[0]==root->hashNode) ){
        return root;
    }
    if(arrayMem[0] != root->hashNode){
        return NULL;
    }

    for(arrCount=0; arrCount<arrayLengh; arrCount++){
        int tmpIdex = arrayMem[arrCount];
        x_tree_t *tmpRoot = root;
        /* find the sibling first */
        while( (tmpIdex != tmpRoot->sibling->hashNode) && (tmpRoot->sibling != NULL) ){
            tmpRoot->sibling = tmpRoot->sibling->sibling;
        }
        if(tmpIdex == tmpRoot->sibling->hashNode){
            root = tmpRoot->sibling;
        }
        else if( (tmpIdex == root->child->hashNode) && (tmpRoot->child != NULL) ){
            root = root->child;
        }
        else{
            root =  NULL;       //the routing is wrong if return NULL
            break;
        }
    }
    free(arrayMem);
    arrayMem = NULL;
    return root;
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


// TODO: 看看有没有儿子，要是有就打value 要么就打子集
char *operat_xml_findValue(x_tree_t *root, char *findRout, int routLengh)
{
    x_tree_t *result_tree = NULL;

    result_tree = operat_xml_findTree(root, findRout, routLengh);
    if(NULL == result_tree){
        //printf("The routing is wrong!\n");
        return NULL;
    }
    if(NULL != result_tree->child){
        return result_tree->value;
    }
    return NULL;
    //TODO:返回给调用者字符串指针？
}

void write_xml_file(x_tree_t *root)
{
    FILE *pf = NULL;
    pf = fopen(filePath, "w+");      //truncated the file first
    fclose(pf);

    /* append write. don't need reload */
    pf = fopen(filePath, "a");
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
    lastDot    = strrchr(addRout, '.');
    *lastDot   = '\0';
    nameLengh  = strlen(lastDot+1);
    tmpName    = (char *)calloc(nameLengh+1, sizeof(char));
    strcpy(tmpName, lastDot+1);
    /* malloc memory */
    tmpRoot = (x_tree_t *)malloc(sizeof(x_tree_t));
    tmpRoot->child    = NULL;
    tmpRoot->sibling  = NULL;
    tmpRoot->key      = tmpName;
    tmpRoot->value    = tmpVlaue;
    tmpRoot->hashNode = adler_32(tmpName);
    /* action append the member */
    curtRoot = operat_xml_findTree(root, addRout, strlen(addRout));
    if(curtRoot->child  != NULL){
        x_tree_t *tmp    = curtRoot->child;
        curtRoot->child  = tmpRoot;
        tmpRoot->sibling = tmp;
    }
    else{
        curtRoot->child = tmpRoot;
    }

    return 0;
}

//  这个递归应该可以删除吧。
void recursive_del(x_tree_t *root)
{
    if(NULL != root)
	{
        if(NULL != root->child){
            recursive_del(root->child);
        }
        if(NULL != root->sibling){
            recursive_del(root->sibling);
        }
        free(root);
        root = NULL;
    }
	return ;
}

// 如果有孩子， if有没有兄弟， 如果有则指向其自身的指针，指向其下一个兄弟， 然后free
// 如果没有兄弟，则将指向自身的指针置空，然后free

int operat_xml_delEntry(x_tree_t *root, char *delRout, int routLengh)
{
    x_tree_t *tmpXTree = NULL;
	x_tree_t *lastXTree = NULL;
	char *lastRout = NULL;
	char *lastDot = NULL;
    char *tmpRout = NULL;
	int lastLen = 0;
	int tmpCnt = 0;
	if('\0' == *delRout)
	{
		return -1;
	}
	//find last node
    tmpRout = (char *)calloc(routLengh+1, sizeof(char));
    strcpy(tmpRout, delRout);

    tmpXTree = operat_xml_findTree(root, tmpRout, routLengh);
	if(NULL == tmpXTree)return -1;

	lastDot = strrchr(tmpRout, '.');
	*lastDot = '\0';

	lastXTree = operat_xml_findTree(root, tmpRout, routLengh);
	//if path is not root, and path have sibling
	if((NULL != lastXTree) && (NULL != tmpXTree->sibling))
	{
		if(lastXTree->child == tmpXTree)
		{
			lastXTree->child = tmpXTree->sibling;
		}
		else if(lastXTree->sibling == tmpXTree)
		{
			lastXTree->sibling = tmpXTree->sibling;
		}
	}
	recursive_del(tmpXTree);

    return 0;
}









