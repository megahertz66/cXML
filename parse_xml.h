/**
* @file xml-DOM.h
* @brief xml-DOM生成树
* @author Hert Zhao & Alex Wang
* @date 2019-8-27
* @version A001
*/




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
* @brief 栈操作结构
*/
typedef
struct mainList{
	x_tree_t *node;		///< 标签成员结构
	int hashNum;		///< 标签名hash值
};

/**
* 读取文件
* @param[char *root]  	带解析xml的字符转格式
* @return 0， 			解析成功xml树根地址
* @ref xml-DOM.h
* @note
*/
x_tree_t *parse_xml(char *root);
