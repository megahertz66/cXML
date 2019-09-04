/**
* @file xml-DOM.h
* @brief xml-DOM生成树
* @author Hert Zhao & Alex Wang
* @date 2019-8-27
* @version A001
*/

#ifndef PARSE_XML
#define PARSE_XML

#include <stdio.h>

extern char *filePath;

/**
* @brief XML树标签成员结构
*/
typedef
struct xmlTree{
	int hashNode;	///< 标签名字的哈希值
	char *key;		///< 标签的名字
	char *value;		///< 标签中包含的内容
	struct xmlTree *sibling;	///< 同级标签
	struct xmlTree *child;		///< 次级标签
}x_tree_t;


/**
* 解析字符串
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
x_tree_t *parse_xml(char *root);

/**
* 读取文件到内存
* @param[int fd]  			xml文件描述符
* @return char* 			指向内存中字符串的char* 指针
* @ref xml-DOM.h
* @note
*/
char *load_xml_file(int fd);

/**
* 释放内存文件
* @param[char *needFree]  	需要释放的文件内存
* @return int	 			0
* @ref xml-DOM.h
* @note
*/
int remove_xml_file(char *needFree);

/**
* 解析注释
* @param[int fd]  			指向可能发生xml注释的字符串开始
* @return char* 			返回注释结束位置的后一个字符
* @ref xml-DOM.h
* @note
*/
char *parse_note(char *start);

/**
* 保存value
* @param[char *maybeValue]  		指向可能是value的位置
* @param[char *positon] 			返回注释结束位置的后一个字符
* @return int						读入长度
* @ref xml-DOM.h
* @note
*/
int save_value(char *positon, int posSize, char *maybeValue);

x_tree_t *lode_xml_to_tree(char *filePath, int pathLengh);


#endif // PARSE_XML
