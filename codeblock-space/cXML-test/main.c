#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "hash.h"
#include "parse_xml.h"
#include "stack.h"

x_tree_t *root = NULL;

int main()
{
    long Fsize = 0;
    char *string = NULL;
    int fp = open("eg_XML.xml", O_RDONLY);

    string = load_xml_file(fp);
//    printf("%s\n", string);
    root = parse_xml(string);
    printf("%s\n", root->child->child->sibling->value);
    return 0;
}
