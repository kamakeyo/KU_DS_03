#define BLACK 1
#define RED 0
#include <stdio.h>
#include <stdlib.h>

int bh = 0;
int total = 0;
int tr = 0;
int nb=0;
int miss=0;
int del=0;
int insert=0;


typedef struct Node*NodePtr;
struct Node
{
    int value;
    NodePtr p, LF, RT;
    int c;
};

NodePtr nil;

NodePtr node_alloc(int newvalue)
{
    NodePtr self = (NodePtr)malloc(sizeof(struct Node));
    self->value = newvalue;
    self->p = nil;
    self->LF = nil;
    self->RT = nil;
    return self;
}

NodePtr nil_init()
{
    nil = (NodePtr)malloc(sizeof(struct Node));
    nil->value = -1;
    nil->c = 1;
    nil->LF = nil->RT = NULL;
    
    return nil;
    
}
typedef struct RBT* RBTPtr;
struct RBT
{
    NodePtr root, NIL;
};


RBTPtr rbt_alloc()
{
    RBTPtr self = (RBTPtr)malloc(sizeof(struct RBT));
    self->NIL = nil;
    self->root = self->NIL;
    return self;
}


void LF_Rotate(RBTPtr self, NodePtr n)
{
    NodePtr y;
    y = n->RT;
    n->RT = y->LF;
    if (y->LF != self->NIL)
        y->LF->p = n;
    y->p = n->p;
    if (n->p == self->NIL)
        self->root = y;
    else if (n == n->p->LF)
        n->p->LF = y;
    else
        n->p->RT = y;
    y->LF = n;
    n->p = y;
}

void RT_Rotate(RBTPtr self, NodePtr n)
{
    NodePtr y;
    y = n->LF;
    n->LF = y->RT;
    if (y->RT != self->NIL)
        y->RT->p = n;
    y->p = n->p;
    if (n->p == self->NIL)
        self->root = y;
    else if (n == n->p->LF)
        n->p->LF = y;
    else
        n->p->RT = y;
    y->RT = n;
    n->p = y;
}


void rbt_insert_fixup(RBTPtr self, NodePtr n)
{
    NodePtr y;
    while (n->p->c == RED)
    {
        if (n->p == n->p->p->LF)
        {
            y = n->p->p->RT;
            if (y!=self->NIL && y->c == RED)
            {
                n->p->c = BLACK;
                y->c = BLACK;
                n->p->p->c = RED;
                n = n->p->p;
            }
            else
            {
                if (n == n->p->RT)
                {
                    n = n->p;
                    LF_Rotate(self, n);
                }
                n->p->c = BLACK;
                n->p->p->c = RED;
                RT_Rotate(self, n->p->p);
            }
        }
        else
        {
            y = n->p->p->LF;
            if (y->c == RED)
            {
                n->p->c = BLACK;
                y->c = BLACK;
                n->p->p->c = RED;
                n = n->p->p;
            }
            else
            {
                if (n == n->p->LF)
                {
                    n = n->p;
                    RT_Rotate(self, n);
                }
                n->p->c = BLACK;
                n->p->p->c = RED;
                LF_Rotate(self, n->p->p);
            }
        }
    }
    self->root->c = BLACK;
}
void rbt_transplant(RBTPtr self, NodePtr u, NodePtr v)
{
    if (u->p == self->NIL)
        self->root = v;
    else if (u == u->p->LF)
        u->p->LF = v;
    else
        u->p->RT = v;
    v->p = u->p;
}

void rbt_insert(RBTPtr self, NodePtr n)
{
    NodePtr y = self->NIL;
    NodePtr x = self->root;
    while (x != self->NIL)
    {
        y = x;
        if (n->value < x->value)
            x = x->LF;
        else
            x = x->RT;
    }
    n->p = y;
    if (y == self->NIL)
        self->root = n;
    else if (n->value < y->value)
        y->LF = n;
    else
        y->RT = n;
    n->LF = self->NIL;
    n->RT = self->NIL;
    n->c = RED;
    rbt_insert_fixup(self, n);
}

NodePtr tree_min(RBTPtr self, NodePtr x)
{
    while (x->LF!=self->NIL)
        x = x->LF;
    return x;
}
NodePtr tree_max(RBTPtr self, NodePtr x)
{
    while (x->RT != self->NIL)
        x = x->RT;
    return x;
}
void rbt_del_fixup(RBTPtr self, NodePtr n)
{
    NodePtr w;
    while (n != self->root && n->c == BLACK)
    {
        if (n == n->p->LF)
        {
            w = n->p->RT;
            if (w->c == RED)
            {
                w->c = BLACK;
                n->p->c = RED;
                LF_Rotate(self, n->p);
                w = n->p->RT;
            }
            if (w->LF->c == BLACK && w->RT->c == BLACK)
            {
                w->c = RED;
                n = n->p;
            }
            else
            {
                if (w->RT->c == BLACK)
                {
                    w->LF->c = BLACK;
                    w->c = RED;
                    RT_Rotate(self, w);
                    w = n->p->RT;
                }
                w->c = n->p->c;
                n->p->c = BLACK;
                w->RT->c = BLACK;
                LF_Rotate(self, n->p);
                n = self->root;
            }
        }
        else
        {
            w = n->p->LF;
            if (w->c == RED)
            {
                w->c = BLACK;
                n->p->c = RED;
                RT_Rotate(self, n->p);
                w = n->p->LF;
            }
            if (w->RT->c == BLACK && w->LF->c == BLACK)
            {
                w->c = RED;
                n = n->p;
            }
            else
            {
                if (w->LF->c == BLACK)
                {
                    w->RT->c = BLACK;
                    w->c = RED;
                    LF_Rotate(self, w);
                    w = n->p->LF;
                }
                w->c = n->p->c;
                n->p->c = BLACK;
                w->LF->c = BLACK;
                RT_Rotate(self, n->p);
                n = self->root;
            }
        }
        
    }
    n->c = BLACK;
}
void rbt_del(RBTPtr self, NodePtr n)
{
    NodePtr y, x;
    y = n;
    int y_original_color = y->c;
    if (n->LF == self->NIL)
    {
        x = n->RT;
        rbt_transplant(self, n, n->RT);
    }
    else if (n->RT == self->NIL)
    {
        x = n->LF;
        rbt_transplant(self, n, n->LF);
    }
    else
    {
        y = tree_min(self, n->RT);
        y_original_color = y->c;
        x = y->RT;
        if (y->p == n)
            x->p = y;
        else
        {
            rbt_transplant(self, y, y->RT);
            y->RT = n->RT;
            y->RT->p = y;
        }
        rbt_transplant(self, n, y);
        y->LF = n->LF;
        y->LF->p = y;
        y->c = n->c;
    }
    if (y_original_color == BLACK)
        rbt_del_fixup(self, x);
}
void rbt_print(RBTPtr self, NodePtr tree, int level)
{
    if (tree->RT != self->NIL)
        rbt_print(self, tree->RT, level + 1);
    for (int i = 0; i < level; i++)
        printf("   ");
    printf("%d-%s\n", tree->value, !(tree->c) ? "R" : "B");
    if (tree->LF != self->NIL)
        rbt_print(self, tree->LF, level + 1);
}
int rbt_count(RBTPtr self, NodePtr tree)
{
    if (tree==self->NIL)
        return 0;
    if (tree->LF != self->NIL)
        return rbt_count(self, tree->LF)+rbt_count(self, tree->RT)+1;
    else
        return rbt_count(self, tree->LF)+rbt_count(self, tree->LF);
}


void rbt_bh(RBTPtr self, NodePtr tree)
{
    if (tree == self->NIL)
    {
        return;
    }
    else
    {
        if (tree->c == BLACK) {
            bh++;
            rbt_bh(self, tree->RT);
        }
        else if (tree->c == RED)
            rbt_bh(self, tree->RT);
    }
}

struct node
{
    int data;
    struct node *next;
};
void push(struct node** head_ref, int new_data)
{
    struct node* new_node = (struct node*) malloc(sizeof(struct node));
    new_node->data = new_data;
    new_node->next = (*head_ref);
    (*head_ref) = new_node;
}
NodePtr rbt_search(RBTPtr self, NodePtr x, int value)
{
    if (x == self->NIL || x->value == value)
        return x;
    else if (value <= x->value)
        return rbt_search(self,x->LF, value);
    else
        return rbt_search(self,x->RT, value);
}
void inorder_traversal(RBTPtr self, NodePtr root) {
    if (root != self->NIL) {
        inorder_traversal(self, root->LF);
        printf("%d %s\n", root->value, !(root->c) ? "R" : "B");
        inorder_traversal(self, root->RT);
    }
}
void rbt_counter(RBTPtr self, NodePtr root) {
    if (root != self->NIL) {
        rbt_counter(self, root->LF);
        total++;
        rbt_counter(self, root->RT);
    }
}

NodePtr rbt_successor(RBTPtr self, NodePtr x)
{
    if (x->RT != self->NIL)
        return tree_min(self, x->RT);
    NodePtr y= x->p;
    while (y!= self->NIL && x==y->RT)
    {
        x=y;
        y=y->p;
    }
    return y;
}
NodePtr rbt_predecessor(RBTPtr self, NodePtr x)
{
    if (x->LF != self->NIL)
        return tree_max(self, x->LF);
    NodePtr y= x->p;
    while (y!= self->NIL && x==y->LF)
    {
        x=y;
        y=y->p;
    }
    return y;
}
int main()
{
    nil = nil_init();
    RBTPtr rbt = rbt_alloc();
    FILE *fp;
    int num;
    fp = fopen("/Users/SK/desktop/prjtest/test01.txt", "r");
    while (!feof(fp))
    {
        fscanf(fp, "%d", &num);
        if (num > 0)
        {
            rbt_insert(rbt, node_alloc(num));
            insert++;
            rbt_print(rbt,rbt->root,0);
        }
        else if (num < 0)
        {
            NodePtr node1 = rbt_search(rbt, rbt->root, num*-1);
            if (node1->value!=-1)
            {
                rbt_del(rbt, node1);
                del++;
            }
            else
                miss++;
        }
        else if (num == 0)
            break;
    }
    fclose(fp);
    FILE *fp2;
    fp2 = fopen("/Users/kamakeyo/desktop/자구/output.txt", "wb");
    FILE *fp1;
    int num1;
    NodePtr search;
    fp1 = fopen("/Users/kamakeyo/desktop/자구/output.txt", "r");

    while (!feof(fp1))
    {
        fscanf(fp1, "%d", &num1);
        search=rbt_search(rbt, rbt->root, num1);
        if (search->value == -1) {
            rbt_insert(rbt, node_alloc(num1));
            search=rbt_search(rbt, rbt->root, num1);
            NodePtr a = rbt_successor(rbt, search);
            NodePtr b = rbt_predecessor(rbt, search);
            
            if (b->value==-1)
            {
                printf("NIL  ");
                fprintf(fp2, "NIL  ");
            }
            else
            {
                printf("%d  ", b->value);
                fprintf(fp2, "%d  ", b->value);
            }
            printf("NIL  ");
            fprintf(fp2, "NIL  ");
            if (a->value == -1)
            {
                printf("NIL  \n\n");
                fprintf(fp2, "NIL  \n\n");
            }
            else
            {
                printf("%d  \n\n", a->value);
                fprintf(fp2, "%d  \n\n", a->value);
            }
            rbt_del(rbt, search);
        }
        else {
            NodePtr a = rbt_successor(rbt, search);
            NodePtr b = rbt_predecessor(rbt, search);
            
            if (b->value==-1)
            {
                printf("NIL  ");
                fprintf(fp2, "NIL  ");
            }
            else
            {
                printf("%d  ", b->value);
                fprintf(fp2, "NIL  ");
            }
            printf("%d  ", search->value);
            fprintf(fp2, "%d  ", search->value);
            if (a->value == -1)
            {
                printf("NIL  \n\n");
                fprintf(fp2, "NIL  \n\n");
            }
            else
            {
                printf("%d  \n\n", a->value);
                fprintf(fp2, "%d  \n\n", a->value);
            }
            
        }
    }
    fclose(fp1);
    fclose(fp2);
    rbt_print(rbt, rbt->root, 0);
    
    rbt_bh(rbt, rbt->root);
    rbt_counter(rbt, rbt->root);
    printf("total = %d\n", total);
    printf("insert= %d\n", insert);
    printf("nb= %d\n", rbt_count(rbt, rbt->root));
    printf("del= %d\n", del);
    printf("miss= %d\n", miss);
    printf("bh = %d\n", bh);
    inorder_traversal(rbt, rbt->root);
     
    return 0;
}
