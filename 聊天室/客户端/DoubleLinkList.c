#include "DoubleLinkList.h"
#include <stdio.h>
#include<stdlib.h>
//画图最好用
DLNode* CreatNode(ElementType element)
{
    DLNode*newNode=(DLNode*)malloc(sizeof(DLNode));
    if (newNode==NULL)
    {
       printf("creat Node malloc error\n");
       return NULL;
    }
    newNode->data=element;
    newNode->next=NULL;
    newNode->prev=NULL;
    return newNode;
}
void InitDList(DLlist*list)
{
    list->head=list->tail=CreatNode(NULL);
    list->len=0;
}
void InsertDLlistTail(DLlist *list, ElementType element)
{
    DLNode *newNode = CreatNode(element);
    if(newNode == NULL)
    {
        return;
    }
    list->tail->next = newNode;//让尾节点的next=newNode
    newNode->prev = list->tail;//让newNode的前一个节点=尾节点
    list->tail = newNode;//让尾节点=newNode
    list->len++;//list->len++;
}

void TravelDLlist(DLlist *list, void (*funcptr)(ElementType))
{
    if(funcptr == NULL)
       {return;} 
    
    DLNode *travelPoint = list->head->next;
    while(travelPoint != NULL)
    {
        funcptr(travelPoint->data);
        travelPoint = travelPoint->next;
    }
}
void InsertDLlistHead(DLlist*list,ElementType element)
{
  if (list->len==0)
    {
        InsertDLlistTail(list,element);
    }
    else
    {
        DLNode*newNode=CreatNode(element);
          if (newNode==NULL)
          {
            return;
          }
           newNode->next=list->head->next;//让newNode的next=原来头节点的next
           newNode->prev=list->head;//让newNode的前一个=头节点
           list->head->next->prev=newNode;//让原来head节点的后一个的前一个=newNode,(list->head->next->prev=newNode
           list->head->next=newNode;//让head节点的后一个=newNode
           list->len++;
    }
}
void RemoveByIndex(DLlist*list ,int index,void(*freeptr)(ElementType))
{
    if (index<0||index>=list->len)
    {
       printf("非法位置:%d len :%d\n",index,list->len);
       return ;
    }
    if (index==list->len-1)//删除尾节点
    {
        DLNode*Prev=list->tail->prev;//定义尾节点的前一个节点为Prev
        Prev->next=NULL;//让Prev的下一个节点为NULL
        if (freeptr!=NULL)
        {
            freeptr(list->tail->data);
        }
        free(list->tail);//free掉list的尾节点
        list->tail= Prev;//让尾节点=Prev
        list->len--;
    }
    else
    {
         DLNode*travelPoint=list->head;
         for (int i = 0; i < index; i++)
        {
          travelPoint=travelPoint->next;
        }
        DLNode *freeNode =travelPoint->next;//定义freeNode为travelpoint的下一个节点,要删的就是freeNode
        travelPoint->next=freeNode->next;//令freeNode的下一个节点为travelpoint的下一个节点
        freeNode->next->prev=travelPoint;//(画图)，让这个链子不会断掉
        if (freeptr!=NULL)
        {
            freeptr(freeNode->data);
        }
        free(freeNode);
        list->len--;
    }
}
void RemoveByElement(DLlist*list,ElementType element,bool(*Operptr)(ElementType,ElementType),void(*freeptr)(ElementType))
{

    DLNode*travelpoint=list->head;
    int index=0;
    while (travelpoint->next!=NULL)
    {
        if (Operptr(travelpoint->next->data,element)==true)
        {
            RemoveByIndex(list,index,freeptr);
        }
        else
        {
            index++;
            travelpoint=travelpoint->next;
        }
            
    }

}
ElementType*FindByIndex(DLlist*list,int index)
{
    
    if (index<0||index>=list->len)
    {
        printf("非法位置\n");
        return NULL;
    }
    DLNode*travelPoint=list->head->next;
    for (int i = 0; i <index ; i++)
    {
        travelPoint=travelPoint->next;
    }
    return &travelPoint->data;
}
DLlist FindByElement(DLlist*list,ElementType element,bool(*Operptr)(ElementType,ElementType))
{
    INITDLLIST(result);
    int index=0;
    DLNode*travelpoint=list->head;
    while (travelpoint->next!=NULL)
    {
        if (Operptr(travelpoint->next->data,element)==true)
        {
            InsertDLlistTail(&result,travelpoint->next->data);
        }
        travelpoint=travelpoint->next;
        
    }
    return result;
}
void Swap(ElementType* value1,ElementType *value2)
{
    ElementType temp = *value1;
    *value1 = *value2;
    *value2 = temp;
}
void FastSort(DLNode*start,DLNode* end,bool (*funcptr)(void *,void *))
{
    if(start == end)
    {
        return ;
    }
    DLNode*p1 = start;
    DLNode*p2 = start->next;
    while(p2 != end)
    {
        if(funcptr(p2->data,start->data) == true)
        {
            p1 = p1->next;
            Swap(&p1->data,&p2->data);
        }
        p2 = p2->next;
    }
    //将p1指的数与基准值换位
    Swap(&start->data,&p1->data);
    //经过以上过程，指针p1在比基准值小的数列末位，指针p2在比基准值大的数列末位
    FastSort(start,p1,funcptr);
    FastSort(p1->next,end,funcptr);
}

void DLlistSort(DLlist*list,bool(*funcptr)(ElementType,ElementType))
{
    FastSort(list->head->next,NULL,funcptr);
}
void ClearDLlist(DLlist*list,void(*Clearptr)(ElementType))//clear掉双向链表
{
    while (list->head!=NULL)
    {
        DLNode*next=list->head->next;
        if (list->head->data!=NULL&&Clearptr!=NULL)
        {
            Clearptr(list->head->data);
        }
        free(list->head);
        list->head=next;
        
    }
    list->len=0;
}





    
   




















    


    
