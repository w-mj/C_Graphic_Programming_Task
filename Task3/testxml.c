#include "xml.h"
#include <stdio.h>

int main(void)
{
    xmlTree x;
    x = loadXML("charater.xml");
    printXML(x, 0);
    puts(getValue(x, "AnimatedActor/Content/Animations/Animation/Loop"));
    destroyXMLTree(x);
    return 0;
}
