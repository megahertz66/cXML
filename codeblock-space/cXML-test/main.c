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
    int fp = open("eg_XML.xml", O_RDONLY);

    //printf("%s\n", load_xml_file(fp));
    root = parse_xml(load_xml_file(fp));
    printf("%s\n", root->child->child->value);
    return 0;
}
