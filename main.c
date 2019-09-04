#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_xml.h"
#include "xml_work.h"

int main()
{
    char *xml_file = "eg_XML.xml";
    char *find_file = "address.linkman";
    int hashname[3] = {0};
    int i = 0;
    int tmp = 3;
    x_tree_t *tree_root = NULL;
    char *strings = NULL;

    tree_root = lode_xml_to_tree(xml_file, strlen(xml_file));

    rountToHash(tree_root, find_file, strlen(find_file), hashname, &tmp);

    //for(i; i<3; i++){
   //     printf("%d\n", hashname[i]);
    //}

    printf("%d\n", hashname[0]);

    return 0;
}
