#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_xml.h"
#include "xml_work.h"

int main()
{
    char *xml_file = "eg_XML.xml";
    char find_file[] = "address.linkman";
    int hashname[3] = {0};
    int i = 0;
    int tmp = 3;
    x_tree_t *tree_root = NULL;
    x_tree_t *tmp_tree = NULL;
    char *strings = NULL;

    tree_root = lode_xml_to_tree(xml_file, strlen(xml_file));

    tmp_tree = operat_xml_findTree(tree_root, find_file, strlen(find_file));

    printf("%s\n", tmp_tree->key);

    return 0;
}
