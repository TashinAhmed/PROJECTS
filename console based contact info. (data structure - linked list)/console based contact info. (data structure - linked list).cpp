
//creating contact info. (console based)
//created by tashin ahmed
//AFTER DELETE ERROR (X)
//FIRST DELETE ERROR (X)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>                    // for representing EOF & unsigned chars
#include <time.h>
void insert();                        // for inserting data's
void display();                       // for displaying
void delete();                        // for deleting a contact
void mod();                           // for editing a contact
int  search();                        // for searching a contact
bool acNumb(int);                     // for modify through ac No.
int  update();                        // for permission to update

struct list{                          // CONTACT LISTED.
    int  contactNumb;                 // for the contact No.
    char name[20];
    char phoneNumb[15];
    char email[20];
    struct list *next;
};
typedef struct list node;
  int count=0;                        // declared globally; used in multiple functions
  node *start,*prev,*curr;            //                     ;;

int main()
{
  char ch;
  start=NULL;
    do{
      fflush(stdin);
      puts ("\nWelcome To The Contact Database\n--------------------------------");
      puts ("  1 - Add a new contact\n  2 - Delete contact");
      puts ("  3 - List all contacts\n  4 - Modify contact");
      puts ("  5 - Find a contact by name\n--------------------------------");
      puts ("  Q - Save and quit\n\n       Your choice:");
      ch=toupper(getchar());
      switch(ch){
        case'1' :  puts("Add a new contact\n");        fflush(stdin);insert(); break;
        case'2' :  puts("Delete a contact\n");         delete();  break;
        case'3' :  puts("List all contacts\n");        display(); break;
        case'4' :  puts("Modify a contact\n");         mod();  break;
        case'5' :  puts("Find a contact by name\n");   search(); break;
        case'Q' :  puts("Save and quit\n");
        default :  break;
      }
    } while(ch!='Q');
    return 0;
}

void insert(){
    if(start==NULL){
        start=new node();
        count++;
        printf("contact number\t%d:\n",count);     start->contactNumb=count;
        printf("Enter contact name :");            gets(start->name);
        printf("Enter contact Phone number :");    gets(start->phoneNumb);
        printf("Enter contact email :");           gets(start->email);
        printf("contact added!");
        start->next=NULL;
        prev=start;
    }

    else{
        curr=new node();
        count++;
        printf("contact number\t%d:\n",count);     curr->contactNumb=count;
        printf("Enter contact name :");            gets(curr->name);
        printf("Enter contact Phone number :");    gets(curr->phoneNumb);
        printf("Enter contact email :");           gets(curr->email);
        printf("contact added!");
        curr->next=NULL;
        prev->next=curr;
        prev=curr;
    }
}

void display(){
    if(start==NULL)
        puts("There are no contacts to display!");
    else{
        printf("Acct#\t\tName\t\tPhone\t\tEmail\n");
        puts("------------------------------------------------------");
        curr=start;
        do{
          printf("%d:\t\t %s\t\t %s\t\t %s\n",curr->contactNumb,curr->name,curr->phoneNumb,curr->email);
        }while((curr=curr->next)!=NULL);
    }
}

void delete(){
    int rec;
    node *temp1;
    if(start==NULL){
        puts("There are no contacts to delete!");
        return;
    }
    disp();    // for displaying the whole status again.
    printf("Enter contact account number to delete: ");
    scanf("%d",&rec);
    curr=start;
    while(curr!=NULL){
        if(curr->cno==rec){
            if(curr==start){
                start=curr->next;
                free(curr);
                printf("contact %d deleted!\n",rec);
                return;
            }
            else{
                temp1->next=curr->next;
                free(curr);
                printf("contact %d deleted!\n",rec);
                return;
            }
        }
        else{
            temp1=curr;
            curr=curr->next;
        }
    }
    printf("contact %d not found!\n",rec);
}

void mod(){
    int rec, res;
    if(start==NULL){
        puts("There are no contacts to modify!");
        return;
    }
    display();
    printf("Enter A/C No. you want to modify/change: ");
    scanf("%d",&rec);
    res=acNumb(rec);
    if(res==true){
        printf("Contact %d:\n",curr->contactNumb);
        printf("Name: %s\n",curr->name);
        if(update()) {gets(curr->name);}
        printf("Phone: %s\n",curr->phoneNumb);
        if(update()) {gets(curr->phoneNumb);}
        printf("Email: %s\n",curr->email);
        if(update()) {gets(curr->email);}
        return;
    }
    printf("contact %d was not found!\n",rec);
}

int search(){
    char srn[20];
    if(start==NULL){
        puts("No contacts to find!");
        return 1;
    }
    printf("Enter contact name: ");
    fflush(stdin);
    gets(srn);
    curr=start;
    while(curr!=NULL){
        if(strcmp(curr->nm,srn)==0){
           printf("Acct#\t\tName\t\tPhone\t\tEmail\n");
           puts("------------------------------------------------------");
           printf("%d:\t\t %s\t\t %s\t\t %s\n",curr->cno,curr->nm,curr->ph,curr->em);
           return 0;
        }
        else
            curr=curr->next;
    }
    printf("contact %s was not found!\n",srn);
    return 1;
}

bool acNumb(int recn){
    int rec;
    rec=recn;
    curr=start;
    while(curr!=NULL){
         if(curr->cno==rec){
            return true;
        }
        else{
            curr=curr->next;
        }
    }
    return false;
}

int update(){
    char ch;
    fflush(stdin);
    printf("Update? (Y to update any other key to not)");
    ch=toupper(getchar());
    fflush(stdin);
    if(ch=='Y'){
        printf("Enter new value: ");
        return 1;
    }
    else
        return 0;
}
