#include <stdio.h>
#include <string.h>
#include "stack.h"

extern void *xmlStack[];      // 0~50
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
    if(size == 0) return 50;
    else return 50 - size;
}

int lenStack(void)
{

    return size;
}
