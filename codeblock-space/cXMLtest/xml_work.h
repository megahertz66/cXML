#ifndef XML_WOEK
#define XML_WORK

#include "parse_xml.h"


/**
* @brief 宏值
* @note  FIND_NAME 要查找的是name，以此类推
*/
typedef
enum TOOL_NUM{
    FIND_NAME = 0, FIND_VALUE, ADD_ENTRY, DEL_ENTRY
}tool_num;


/**
* 打印制定数量的TA
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
void print_tab(int times, FILE *stream);

/**
* 路径转hash值
* @param[char *maybeValue]  		指向可能是value的位置
* @param[char *positon] 			返回注释结束位置的后一个字符
* @return int						读入长度
* @ref xml-DOM.h
* @note
*/
int rountToHash(x_tree_t *root, char *rount, unsigned int rountLengh, int arrayIn[], int *arrayLengh);

/**
* 遍历打印XML元素
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
x_tree_t *visit_print_xml(x_tree_t *root, int level, FILE *stream);

/**
* 遍历打印XML元素,对外提供
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
void print_xml(x_tree_t *root);

/**
* 查找指定路径下的内容
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
x_tree_t *operat_xml(x_tree_t *root, char *findRout, tool_num whatFind);

/**
* 查找指定路径下的元素指针
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
x_tree_t *operat_xml_findTree(x_tree_t *root, char *findRout, int routLengh);

/**
* 遍历路径
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
int visit_xml(char *findRout);

/**
* 找到对应的XML元素名称
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
char *operat_xml_findName(x_tree_t *root, char *findRout, int routLengh);

/**
* 找到对应的XML元素中间的值
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
char *operat_xml_findValue(x_tree_t *root, char *findRout, int routLengh);

/**
* 写入文件
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
void write_xml_file(x_tree_t *root);

/**
* 向指定位置添加XML元素
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
int operat_xml_addEntry(x_tree_t *root, char *addRout, int routLengh);

/**
* 向指定位置删除XML元素
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
int operat_xml_delEntry(x_tree_t *root, char *delRout, int routLengh);


#endif
