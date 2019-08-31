#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "hash.h"
#include "parse_xml.h"
#include "stack.h"

x_tree_t *root = NULL;

int main()
{
    char *string = NULL;

    FILE *fp = fopen ("eg_XML.xml", "rb" );
    string = load_xml_file_v2(fp);
    root = parse_xml(string);
    printf("%s\n", root->child->child->sibling->value);
    return 0;
}
