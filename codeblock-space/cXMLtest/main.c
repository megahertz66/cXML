#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_xml.h"
#include "xml_work.h"

int main()
{
    char *xml_file = "eg_XML.xml";
    x_tree_t *tree_root = NULL;
    char *string = NULL;

    string = load_xml_file_v2("eg_XML.xml", strlen(xml_file));
    //printf("%s\n", string);

    tree_root = parse_xml(string);
    //tree_root = lode_xml_to_tree(xml_file, strlen(xml_file));

    printf("%s\n", tree_root->child->child->key);

    return 0;
}
