#ifndef SIMPLE_STACK
#define SIMPLE_STACK

void *xmlStack[50];

/**
* 初始化简单的栈
* @param[void]  
* @return void 			
* @ref stack.h
* @note 栈使用前调用
*/
void initStack(void);

/**
* 弹出栈顶元素
* @param[void]  	        
* @return void* 			返回 void* 指针
* @ref xml-DOM.h
* @note
*/
void *popStack();

/**
* 返回栈顶元素
* @param[void]  	        
* @return void* 			返回 void* 指针
* @ref xml-DOM.h
* @note
*/
void *topStack();

/**
* 元素压栈
* @param[void *value]  	    要压入元素的指针
* @return void* 			返回 void* 指针
* @ref xml-DOM.h
* @note
*/
void *pushStack(void *value);

/**
* 栈空间剩余大小
* @return int 			返回栈空间剩余大小
* @ref xml-DOM.h
* @note
*/
int laveStack(void);

/**
* 栈空间大小
* @return int 			返回栈空间大小
* @ref xml-DOM.h
* @note
*/
int lenStack(void);

#endif