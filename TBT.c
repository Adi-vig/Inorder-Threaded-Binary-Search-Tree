#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct node{
    int val;
    int lbit;
    int rbit;

    struct node* left;
    struct node* right;

};

// struct node* head;  

struct node* getNode(int val){
    struct node* temp= (struct node*)malloc(sizeof(struct node));
    temp->val  = val;
    temp->lbit = 0;
    temp->rbit = 0;
    temp->left = NULL;
    temp->right= NULL;
    return temp;
}

struct node* insert(struct  node* head , int key){
    struct node* temp = getNode(key);

    struct node* p;



    if(head->left == head) {
        head->left = temp;
        temp->left = head;
        temp->right = head;
        return head;
    }

    // p=head;
    p= head->left;   //idk mostlyyyyy

    while(1){
        // printf("adf");
        if(key < p->val && p->lbit==1)
            p=p->left;
        
        else if(key > p->val && p->rbit==1)
            p=p->right;
        else 
            break;
    }

    if(key < p->val )
    {
        p->lbit = 1;
        // rearrange the thread before linking new node
        temp->left = p->left;       //inorder predecessor (previous)
        // thread again 
        temp->right = p;            //inorder successor (next)

        // linking new node
        p->left = temp;       

    }

    else if( key > p->val )
    {
        p->rbit = 1;

        // rearrange the thread after linking new node
        temp->right = p->right;     //inorder successor (next)
        temp->left = p;             //inorder predecessor (previous)

        // link the new node
        p->right = temp;
        
    }

    return head;
}



    //             10
    //    5                    15
    //  3   8            13           20
    //               12     14          22





struct node* inorderPredecessor(struct node* p){
    if(p->lbit==0)return p->left;
    else if(p->lbit==1){
        p=p->left;
        while(p->rbit==1)p=p->right;
    }
    return p;
}






struct node* inorderSuccessor(struct node*p){
    if(p->rbit==0)return p->right;
    else if (p->rbit==1)
    {
        p=p->right;
        while(p->lbit==1)p=p->left; 
    }
    return p;
}



void inorder(struct node* head){
    struct node* p;
    p=head->left;
    while(p->lbit==1)p=p->left;

    while(p!=head){
        // printf("currotn p = %d\n", p->val);
        printf(" %d", p->val);
        p=inorderSuccessor(p);

    }
}








void preorder(struct node* head){
    struct node* p;
    p=head->left;

    while(p!=head){
        printf("%d ", p->val);

        if(p->lbit==1){
            p=p->left;
        }
        else if(p->rbit==1){
            p=p->right;
        }
        // p is leaf
        else if(p->rbit==0){
            while(p->rbit==0)p=p->right;
            p=p->right;
        }
        
    }    
}




struct node* findParent(struct node* p){
    struct node* child= p;
    //ancestor of child 
    while(p->rbit==1)p=p->right;
    p=p->right;

    if(p->left==child)return p;

    p=p->left;
    while(p->right != child){
        p=p->right;
    }
    return p;
}

struct node* postSuccessor(struct node* p){
    struct node* cur= p;
    struct node* parent = findParent(cur);
    // printf("suc %d\n", parent->val);

    if(parent->right==cur)return parent;

    else {
        while(p->rbit==1)p=p->right;
        p=p->right;

        if(p->rbit==1){
            p=p->right;    
            while(!(p->rbit==0 && p->lbit==0)){
                if(p->lbit==1)p=p->left;
                else if(p->rbit==1)p=p->right;  
            }
        }
        // printf("suc %d\n", p->val);

    }
        return p;

}


void postorder(struct node* head)
{
    struct node* p = head->left;
    struct node* temp=p;
    while(!(p->rbit==0 && p->lbit==0)){
        if(p->lbit==1){
            p=p->left;
        }
        else if(p->rbit==1)p=p->right;  
    }
    printf(" %d", p->val);

    while(p!=head->left){
        // printf(" hello\n");
        p=postSuccessor(p);
        printf(" %d", p->val);
    }
}








// ********************************* DELETE ********************************

struct node* delTwoChild(struct node* head, struct node* par, struct node* ptr);
struct node* delOneChild(struct node* head, struct node* par, struct node* ptr);
struct node* delNoChild (struct node* head, struct node* par, struct node* ptr);



// Deletes a key from threaded BST with given root and
// returns new root of BST.
struct node* delThreadedBST(struct node* head, int key)
{
	// Initialize parent as NULL and ptrent
	// node as root.
	struct node *par = head, *ptr = head->left;

    //ptr is child and par is parent 

	// Set true if key is found
	bool found = 0;

	// Search key in BST : find Node and its
	// parent.
	while (ptr != head) {
		if (key == ptr->val) {
			found = 1;
			break;
		}
		par = ptr;
		if (key < ptr->val) {
			if (ptr->lbit == 1)
				ptr = ptr->left;
			else
				break;
		}
		else {
			if (ptr->rbit == 1)
				ptr = ptr->right;
			else
				break;
		}
	}

	if (found == 0)
		printf("key not present in tree\n");

	// Two Children
	else if (ptr->lbit == 1 && ptr->rbit == 1)
		head = delTwoChild(head, par, ptr);

    // No children
    else if(ptr->lbit == 0 && ptr->rbit == 0)
        head = delNoChild(head, par, ptr);

	// One child
    else 
        head = delOneChild(head, par, ptr);
    

	return head;
}







struct node* delTwoChild(struct node* head, struct node* par, struct node* ptr){
    // find inorder successor of ptr and its parent 
    struct node* parSuc = ptr; 
    struct node* suc = ptr->right;

    //leftmost child of right subtree of ptr 
    while(suc->lbit==1){
        parSuc=suc;
        suc = suc->left;
    }


    //replace key 
    ptr->val = suc->val;

    //calling delete to successor node
    //inorder successor will have only 2 cases (no child / one child)

    //No child
    if(suc->lbit==0 && suc->rbit==0)
        head = delNoChild(head , parSuc , suc);

    // only one child to inorder successor
    else        
        head = delOneChild(head, parSuc , suc);
    
    
    return head;

}



struct node* delOneChild(struct node* head, struct node* par, struct node* ptr){

    //set child of ptr (node to be deleted)
    struct node* child;
    if(ptr->lbit==1)child = ptr->left;
    else child = ptr->right;


    struct node* p= inorderPredecessor(ptr);
    struct node* s= inorderSuccessor(ptr);

    //ptr is left child of par
    // else if()
    if(ptr == par->left){
        par->left = child;
    }
    //ptr is right child of par
    else {
        par->right = child;
    }

    // if ptr has left subtree 
    if(ptr->lbit ==1)
        p->right = s;

    
    //if ptr has right subtree
    else if(ptr->rbit ==1)
        s->left =p;

    free(ptr);

    return head;



}




struct node* delNoChild (struct node* head, struct node* par, struct node* ptr){
    //if root is to be deleted and it has no children
    if(ptr == head->left){          // or par == head
        ptr = NULL;
        // return head;
    }

    // ptr is left child of par
    else if(ptr == par->left){
        par->lbit=0;
        par->left = ptr->left;  // reconnect thread to inorder predecessor (previous )
    }

    // ptr is right child of par
    else if(ptr == par->right){
        par->rbit = 0;
        par->right = ptr->right;
    }
    free(ptr);
    return head;

}

int main(){

    struct node* head;
    head = (struct node*)malloc(sizeof(struct node));
    // head->val = -1;
    head->lbit= 0; 
    head->rbit= 1;     //convention for all cases
    head->right=head; //convention for all cases
    head->left=head;  //head->left should point to root 




    insert(head,10);
    insert(head,5);
    insert(head,15);
    insert(head,20);
    insert(head,13);
    insert(head,14);
    insert(head,12);
    insert(head,8);
    insert(head,3);
    insert(head, 22);
    insert(head, 21);
    
    
    inorder(head);
    printf("\n********************************\n");
    // preorder(head);
    printf("\n********************************\n");
    // postorder(head);         // there is no postorder here

    // head = delThreadedBST(head, 20);
    inorder(head);
    printf("\n********************************\n");
    postorder(head); 

    return 0;
}

 //             10
    //    5                    15
    //  3   8            13           20
    //               12     14           22
    //                                21                                   
