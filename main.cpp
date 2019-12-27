#include <iostream>
#include<bits/stdc++.h>
using namespace std;

#define ALPHA_SIZE 26

struct Node{
    bool isEOW;
    Node *next[ALPHA_SIZE];
};
Node* createNode(){
    Node* nn=(Node*)malloc(sizeof(Node));
    nn->isEOW=false;
    for(int i=0;i<ALPHA_SIZE;i++)
        nn->next[i]=NULL;
    return nn;

}
void insertWord(Node *root, char str[]){
    Node* temp=root;
    for(int i=0;str[i];i++){
        int idx=str[i]-'a';
        if(temp->next[idx]==NULL)
            temp->next[idx]=createNode();
        temp=temp->next[idx];
    }
    temp->isEOW=true;

}
bool isWord_found(Node *root, char str[]){
    Node* temp=root;
    for(int i=0;str[i];i++){
        int idx=str[i]-'a';
        if(temp->next[idx]==NULL)
            return false;
        temp=temp->next[idx];
    }
    return temp->isEOW;

}
int showmenu()
{
    cout<<"\n"<<"MENU"<<"\n"<<"--------"<<"\n";
    cout<<"1.Insert Word"<<"\n";
    cout<<"2.Auto suggest Word"<<"\n";
    cout<<"3.Print all words Word"<<"\n";
    cout<<"4.Exit"<<"\n";
    cout<<"\n";
    cout<<"Enter your choice: ";
    int c;
    cin>>c;
    return c;


}
char path[100];
void printWord(Node *root,int pi){
    if(root->isEOW==true)
        printf("%s\n",path);
    for (int i=0;i<ALPHA_SIZE;i++)
    {
        if(root->next[i]!=NULL){
            path[pi]=i+'a';
            path[pi+1]='\0';
            printWord(root->next[i],pi+1);
        }

    }
}
void autoSuggest(Node* root,char pre[])
{
    Node* temp=root;
    for(int i=0;pre[i];i++)
    {
        int idx=pre[i]-'a';
        if(temp->next[idx]==NULL){
            cout<<"no word matched with this content";
            return ;}
        temp=temp->next[idx];
    }
    strcpy(path,pre);
    printWord(temp,strlen(pre));
}
void fill(Node* root){
FILE* fp=fopen("dict.txt","r");
char str[100];
while(!feof(fp)){
    fscanf(fp,"%s",str);
    insertWord(root,str);
}
fclose(fp);
}

int main()
{
    Node* root=createNode();
    char str[100];
    while(true)
    {

        int ch=showmenu();
        switch(ch){
    case 1:
        cout<<"Enter a word:";
        fill(root);
        break;
    case 2:
        printWord(root,0);
        break;
    case 3:
        cout<<"enter a word to search:";
        scanf("%s",str);
        if(isWord_found(root,str))
            cout<<"    Found";
        else
            cout<<"     Not Found";
        break;
    case 4:
        return 0;
    default:
        cout<<"Bad choice! Try again.\n";

        }
    }
    return 0;
}
