#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define MAX 100

typedef struct node 
{
    int id; //ID of user
    int numfren; //number of friends of user
    char name[MAX]; //name of user
    int* friends; //friends of user as an array
    struct node* right;  //user to the right
    struct node* left; //user to the left
} node;

struct node* retUser(char str[MAX])
{
    char name[MAX];
    char ID[MAX];
    char strfriends[MAX];

    //copied ID
    char*token=strtok(str,",");
    strcpy(ID,token);

    //copied Name
    token=strtok(NULL,",");
    strcpy(name,token);

    //copied friends' ID
    token=strtok(NULL,",");
    strcpy(strfriends,token);
    //printf("%s\n",(strfriends));

    //creating user nodes
    int id=atoi(ID);
    struct node *user = malloc(sizeof(struct node));
    user->id = id;
    user->friends = (int*)malloc(MAX * sizeof(int));
    strcpy(user->name, name);


    //adding user's friend's IDs
    token = strtok(strfriends,"|");  
    int i=0;
    while( token != NULL ) {
        int temp=atoi(token);
        user->friends[i]=temp;
        i++;
        token = strtok(NULL,"|");
    }
    user->numfren=i;
    if(i==0){user->friends[i]=-1;}
    return user;
}

//search for user with id=key
struct node* search(int key, struct node *users)
{
    node* tempo = users;
    node* pntr = NULL;
    if(tempo == NULL) 
        return NULL;
    if(tempo->id == key) 
        return tempo;
    else{
        if(key < tempo->id){
            pntr = search(key, tempo->left);
        }
        else{
            pntr = search(key, tempo->right);
        }
    }
    return pntr;
}

//see document for explanattion
struct node*refineUser(struct node*user, struct node *users)
{
    user->left = NULL;
    user->right = NULL;
    while(search(user->id, users)!=NULL) {
        user->id++;
    }
    int n = 0;
    node* tempo;
    while(n < user->numfren){
        tempo = search(user->friends[n], users);
        if(tempo == NULL){
            int m = n;
            while(m < user->numfren-1){
                user->friends[m] = user->friends[m+1];
                m++;
            }
            user->numfren--;
        }
        else{
            tempo->friends[tempo->numfren] = user->id;
            tempo->numfren++;
            n++;
        }
    }
    if(user->numfren == 0){
        user->friends[0] = -1; 
    }
    return user;
}

//insert user with id
struct node* insertUser(struct node*root,int id,struct node*user)
{
    user->id = id;
    if(root==NULL) 
        return user;
    else{
        if(user->id >= root->id) 
            root->right = insertUser(root->right, id, user);
        else
            root->left = insertUser(root->left, id, user);
    }
    return root;
}

//prints friends list//debug
void friends(int id, struct node *users) 
{
    node* tempo = search(id, users);
    if(tempo == NULL) 
        return;
    if(tempo->numfren == 0) 
        printf("-1\n");
    int i = 0;
    while(i < tempo->numfren){
        printf("%d\n", tempo->friends[i]);
        i++;
    }
}

//find child node with minimum value (inorder successor) - helper to delete node
struct node *minValueNode(struct node *node) {
    if(node->right == NULL) 
        return NULL;
    node = node->right;
    while(node->left != NULL) 
        node = node->left;
    return node;
}

//deletes itself from its friend's nodes
struct node*deleteFriends(int key, struct node*users)
{
    node* temp = search(key, users);
    if(temp == NULL) return NULL;
    else{
        int i = 0;
        while(i<temp->numfren){
            node* fren = search(temp->friends[i], users);
            if(fren!=NULL){
                int j = 0;
                while(j<fren->numfren)
                {
                    if(fren->friends[j] != key) j++;
                    else
                    {
                        int k = j; 
                        while(k<fren->numfren){
                            fren->friends[k] = fren->friends[k+1];
                            k++;
                        }
                        fren->numfren--;
                    }
                }
            }
            i++;
        }
    }
    return users; 
}

// Deleting a node
struct node *deleteNode(struct node *root, int key) {
    if(root == NULL) 
        return root;
    if(key>root->id)
        root->right = deleteNode(root->right, key);
    else if(key<root->id)
        root->left = deleteNode(root->left, key);
    else{
        if(root->left == NULL && root->right == NULL){
            return NULL;
        }
        if(root->left== NULL){
            node* temp = root->right;
            free(root);
            return temp;
        }
        if(root->right == NULL){
            node* temp = root->left;
            free(root);
            return temp;
        }
        node* temp = minValueNode(root);
        root->id = temp->id;
        int i = 0;
        int* d = root->friends;
        root->friends = temp->friends;
        free(d);
        root->numfren = temp->numfren;
        strcpy(root->name, temp->name);
        root->right = deleteNode(root->right, temp->id); // deleting inorder successor
    }
    return root;
}

//Print USER's IDs in ascending order
void printInOrder(node* myusers) 
{
    if(myusers!=NULL){
        printInOrder(myusers->left);
        printf("%d %s\n", myusers->id, myusers->name);
        printInOrder(myusers->right);
    }
}


int main(int argc, char **argv)
{
    node *users = NULL;   
    while(1){
        int opt, id;
        fflush(stdin);
        scanf("%d",&opt);
        char str[MAX];
        switch (opt)
        {
            case 1:
                // Insert new user
                scanf("%s",str);
                struct node*tbins=retUser(str);
                tbins=refineUser(tbins, users);
                users=insertUser(users,tbins->id,tbins);
                break;
            case 2:
                // delete user
                scanf("%d", &id);
                deleteFriends(id, users);
                users=deleteNode(users, id);
                break;
            case 3:
                // search
                scanf("%d", &id);
                node* result=search(id,users);
                if(result==NULL) 
                    printf("USER NOT IN TREE.\n");
                else{
                    printf("%d\n",result->id);
                }
                break;
            case 4:
                // Print friends
                scanf("%d", &id);
                friends(id, users);
                break;
            case 5:
                // Print inorder
                printInOrder(users);
                break;
            case 6:
                // Exit
                exit(0);
                break;
            default: 
                printf("Wrong input! \n");
                break;
        }
    }
    return 0;
}