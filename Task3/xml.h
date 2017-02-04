#ifndef _XML_H_
#define _XML_H_

#define MAX_PROFILES 128
#define MAX_SUBLABLES 128
#define MAX_LABLE_NAME 256
#define MAX_PROFILE_NAME 128
#define MAX_PROFILE_VALUE 128


struct Content{
    char lableName[MAX_LABLE_NAME];
    int subLableNumber;
    int profilesNumber;
    struct Content* subLable[MAX_SUBLABLES];
    char *profileNameList[MAX_PROFILES];
    char *profileValueList[MAX_PROFILES];
};
typedef struct Content * xmlTree;

xmlTree loadXML(const char *fname);
void destroyXMLTree(xmlTree a);
xmlTree getSubTree(xmlTree parent, char *path);
char * getValue(xmlTree tree, char *path0);
void destroyValue(char * a);
void printXML(xmlTree a ,int root);

#endif
