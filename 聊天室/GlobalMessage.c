#include "GlobalMessage.h"

CInfo*CreatClientInfo(const char *account,int sock)
{
    CInfo*c=(CInfo*)malloc(sizeof(CInfo));
    if (c==0)
    {
        return NULL;

    }
    strcpy(c->account,account);
    c->sock=sock;
    return c;
}
void ClearClientInfo(void*c)
{
    CInfo*cp=(CInfo*)c;
    free(cp);
}
bool IsNameEqual(void*ptr1,void*ptr2)
{
    CInfo *c=(CInfo*)ptr1;
    char*name=(char*)ptr2;
    return strcmp(c->account,name)==0;

}