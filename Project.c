#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX 100

typedef struct treeNode
{
    char val;
    struct treeNode* left;
    struct treeNode* right;
} treeNode;

treeNode* stack[MAX];
int top=-1;

void push(treeNode* node) { stack[++top]=node; }

treeNode* pop()
{
    if(top==-1) return NULL;
    return stack[top--];
}

treeNode* createNode(char val)
{
    treeNode* node=(treeNode*)malloc(sizeof(treeNode));
    node->val=val;
    node->left=NULL;
    node->right=NULL;
    return node;
}

int isOp(char c)
{
    if(c=='+'||c=='-'||c=='*'||c=='/') return 1;
    else return 0;
}

int precedence(char c)
{
    if(c=='+'||c=='-') return 1;
    if(c=='/'||c=='*') return 2;
    return 0;
}

treeNode* buildFromPostfix(const char* postfix)
{
    top=-1;
    for(int i=0;postfix[i]!='\0';i++)
    {
        if(postfix[i]==' '||postfix[i]=='\n') continue;
        treeNode* node=createNode(postfix[i]);
        if(isOp(postfix[i]))
        {
            node->right=pop();
            node->left=pop();
        }
        push(node);
    }
    return pop();
}

treeNode* buildFromPrefix(const char* prefix)
{
    top=-1;
    int len=strlen(prefix);
    for(int i=len-1;i>=0;i--)
    {
        if(prefix[i]==' '||prefix[i]=='\n') continue;
        treeNode* node=createNode(prefix[i]);
        if(isOp(prefix[i]))
        {
            node->left=pop();
            node->right=pop();
        }
        push(node);
    }
    return pop();
}

treeNode* buildFromInfix(const char* infix)
{
    char postfix[MAX],opStack[MAX];
    int opTop=-1,j=0;
    for(int i=0;infix[i]!='\0';i++)
    {
        char c=infix[i];
        if(c==' '||c=='\n') continue;
        if(isalnum(c)) postfix[j++]=c;
        else if(c=='(') opStack[++opTop]=c;
        else if(c==')')
        {
            while(opTop>=0&&opStack[opTop]!='(')
            {
                postfix[j++]=opStack[opTop--];
            }
            opTop--;
        }
        else if(isOp(c))
        {
            while(opTop>=0&&precedence(opStack[opTop])>=precedence(c))
            {
                postfix[j++]=opStack[opTop--];
            }
            opStack[++opTop]=c;
        }
    }
    while(opTop>=0) postfix[j++]=opStack[opTop--];
    postfix[j]='\0';
    return buildFromPostfix(postfix);
}

void printInfix(treeNode* root)
{
    if(root==NULL) return;
    if(isOp(root->val)) printf("(");
    printInfix(root->left);
    printf("%c",root->val);
    printInfix(root->right);
    if(isOp(root->val)) printf(")");
}

void printPrefix(treeNode* root)
{
    if(root==NULL) return;
    printf("%c",root->val);
    printPrefix(root->left);
    printPrefix(root->right);
}

void printPostfix(treeNode* root)
{
    if(root==NULL) return;
    printPostfix(root->left);
    printPostfix(root->right);
    printf("%c",root->val);
}

int evaluate(treeNode* root)
{
    if(root==NULL) return 0;
    if(root->val>='0'&&root->val<='9') return root->val-'0';
    int left=evaluate(root->left);
    int right=evaluate(root->right);
    if(root->val=='+') return left+right;
    if(root->val=='-') return left-right;
    if(root->val=='*') return left*right;
    if(root->val=='/')
    {
        if(right!=0) return left/right;
        return 0;
    }
    return 0;
}

void freeTree(treeNode* root)
{
    if(root==NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main()
{
    char expression[MAX];
    int inputType,choice;
    printf("===========================================\n");
    printf("   BASICEXPRESSION CONVERTER & EVALUATOR   \n");
    printf("===========================================\n");
    printf("Select Input Format:\n");
    printf(" 1. Infix   (e.g., (3+5)*4 )\n");
    printf(" 2. Prefix  (e.g., *+354 )\n");
    printf(" 3. Postfix (e.g., 35+4* )\n");
    printf("Choice (1-3): ");
    scanf("%d",&inputType);
    getchar();
    printf("Enter Expression: ");
    fgets(expression,MAX,stdin);
    expression[strcspn(expression,"\n")]=0;
    treeNode* root=NULL;
    if(inputType==1) root=buildFromInfix(expression);
    else if(inputType==2) root=buildFromPrefix(expression);
    else if(inputType==3) root=buildFromPostfix(expression);
    else
    {
        printf("Invalid input type!\n");
        return 1;
    }
    while(1)
    {
        printf("\n-------------------------------------------\n");
        printf("1. Output as Infix\n");
        printf("2. Output as Prefix\n");
        printf("3. Output as Postfix\n");
        printf("4. Evaluate Result\n");
        printf("5. Show All Conversions & Exit\n");
        printf("Choose option (1-5): ");
        scanf("%d",&choice);
        printf("\nRESULT: ");
        switch(choice)
        {
            case 1: printInfix(root); printf("\n"); break;
            case 2: printPrefix(root); printf("\n"); break;
            case 3: printPostfix(root); printf("\n"); break;
            case 4: printf("%d\n",evaluate(root)); break;
            case 5:
                printf("\n -> Infix   : "); printInfix(root);
                printf("\n -> Prefix  : "); printPrefix(root);
                printf("\n -> Postfix : "); printPostfix(root);
                printf("\n -> Result  : %d\n",evaluate(root));
                freeTree(root);
                printf("\nMemory freed. Goodbye!\n");
                return 0;
            default: printf("Invalid choice!\n");
        }
        
    }
    return 0;
}