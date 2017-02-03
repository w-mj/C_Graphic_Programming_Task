#include "xml.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char *strcpp(char *target, char *source, char stopSign)
{
    while (*source != '\0' && *source != stopSign) {
        *target = *source;
        target++;
        source++;
    }
    *target = '\0';
    return source;
}

xmlTree initLable(const char *lableName)
{
    xmlTree ans = (xmlTree)malloc(sizeof(struct Content));
    strcpy(ans -> lableName, lableName);
    ans -> subLableNumber = 0;
    ans -> profilesNumber = 0;
    for (int i = 0; i < MAX_SUBLABLES; i++) {
        ans -> subLable[i] = NULL;
    }
    memset(ans -> profileNameList, 0, sizeof(ans -> profileNameList));
    memset(ans -> profileValueList, 0, sizeof(ans -> profileValueList));
    return ans;
}

char *getWord(char *target, char * source)
{
    while (isalpha(*source)) {
        *target = *source;
        target++;
        source++;
    }
    *target = '\0';
    return source;
}
char *getValue(char *target, char *source)
{
    while ( *source != '"') // 直到找到双引号为止
        source++;
    return strcpp(target, source+1, '"');
}
bool openLable(char *str)
{
    if (*str == '<' )
        if (strncmp(str, "<!--", 4) == 0)
            return false;
        else if (strncmp(str, "</", 2) == 0 || strncmp(str, "< /", 3) == 0)
            return false;
        else
        return true;
    else 
        return false;
}
// nextLable 返回标签起始的‘<',closeLable返回标签结束的’/‘
char *nextLable(char * str)
{
    while (!openLable(str)) {
        str++;
    } 
    return str;
}

//进入函数时，**start = '<' ，离开函数时，**start == '/'
xmlTree createSubTree(char **start)
{
    char *seek = *start; // 

    char name[MAX_LABLE_NAME];
    seek = getWord(name, seek + 1); // 找到下一个单词的的同时本身也做偏移
    xmlTree currentNode = initLable(name);
    //printf("%s-", name);
    while(1) {
        //for (int i = 0 ;i < 50; i++)
        //    putchar(*(seek+i));
        //printf("###\n");
        if (*seek == '/') // 判断这个子树的标签是否关闭
            break;
        
        if (isalpha(*seek)) {
            currentNode -> profileNameList[currentNode -> profilesNumber] = (char *)malloc(sizeof(char) * MAX_PROFILE_NAME);
            currentNode -> profileValueList[currentNode -> profilesNumber] = (char *)malloc(sizeof(char) * MAX_PROFILE_VALUE);
            seek = getWord(currentNode -> profileNameList[currentNode -> profilesNumber], seek);
            seek = getValue(currentNode -> profileValueList[currentNode -> profilesNumber], seek);
            
            /*
            if (strcmp(currentNode -> profileNameList[currentNode -> profilesNumber], "NullId") == 0 &&
                strcmp(currentNode -> profileValueList[currentNode -> profilesNumber], "99") == 0)
                printf("xxxxxxxxxxxx\n%s", seek);
                */
            printf("%s=%s\n", currentNode -> profileNameList[currentNode -> profilesNumber] , currentNode -> profileValueList[currentNode -> profilesNumber]); 
            
            currentNode -> profilesNumber += 1; // 添加属性
        }
        else if (openLable(seek)) {
            printf("abcdefg\n");
            currentNode -> subLable[currentNode -> subLableNumber] = createSubTree(&seek);
            currentNode -> subLableNumber += 1;
            while (*seek != '>')
                seek++;
        }
        seek++;
    }
    *start = seek;
    //printf("     %c\n", **start);
    return currentNode;
}
xmlTree loadXML(const char *fname)
{
    FILE *fp = NULL;
    fp = fopen(fname, "rb");
    fseek(fp, 0, SEEK_END);
    int fsize = ftell(fp);
    rewind(fp);
    
    char *fstr = (char *)malloc(sizeof(char) * fsize);
    fread(fstr, 1, fsize, fp); // 把文件整个读入字符串
    char *seek = fstr; // 保留首地址free用
    
    xmlTree result = initLable(fname); // 设置根节点为文件名
    
    while (*seek != '\0') {
        if (openLable(seek)) {
            result -> subLable[result -> subLableNumber] = createSubTree(&seek);
            result -> subLableNumber += 1;
        }
        seek++;
    }
    free(fstr);
    fclose(fp);
    return result;

}

void destroyXMLTree(xmlTree a)
{
    for (int i = 0; i < a -> subLableNumber; i++)
        destroyXMLTree(a -> subLable[i]);
    for (int i = 0; i < a -> profilesNumber; i++) {
        free(a -> profileNameList[i]);
        free(a -> profileValueList[i]);
    }
    free(a);
}

void pb(int a)
{
    for (int i = 0; i < a; i++) 
        printf("\t");
}
void printXML(xmlTree a, int root)
{
    pb(root);
    printf("%s\n", a -> lableName);
    for (int i = 0; i < a -> profilesNumber; i++) {
        pb(root);
        printf("%s---%s\n", a -> profileNameList[i], a -> profileValueList[i]);
    }
    for (int i = 0; i < a -> subLableNumber; i++) {
        printXML(a -> subLable[i] , root + 1);
    }
}
