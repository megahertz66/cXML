#include <stdio.h>
#include <string.h>
#include "stack.h"

#define MAXSTACKSIZE    51

void *xmlStack[MAXSTACKSIZE];      // 0~50
static int size = 0;

void initStack(void)
{
    int i = 0;
    for(i; i<50; i++){
        xmlStack[i] = NULL;
    }
    size = 0;
}

void *popStack()
{
    void *result = NULL;
    if(0 == size)   return NULL;
    result = xmlStack[size];
    xmlStack[size--] = NULL;
    return result;
}

void *topStack()
{
    return xmlStack[size];
}

void *pushStack(void *value)
{
    return xmlStack[++size] = value;       //第0个数组就不要了。
}

int laveStack(void)
{
    if(size == 0) return MAXSTACKSIZE;
    else return MAXSTACKSIZE - size;
}

int lenStack(void)
{

    return size;
}
