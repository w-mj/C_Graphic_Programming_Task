#include "xml.h"

int main(void)
{
    xmlTree x;
    x = loadXML("aa.anm2");
    printXML(x, 0);
    destroyXMLTree(x);
    return 0;
}
