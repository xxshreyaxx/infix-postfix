#include <iostream>
#include <cstdlib>
#define maxSize 50
char infix[] = "a+b*c+d";
char prefixExp[] = "++a*bcd";
int curr = 0;
class node{
public:
    char data;
    node *left;
    node *right;
};
class Stack{
private:
    int top;
    char array[maxSize]{};
public:
    Stack (){
        top = -1;
    }
    bool isEmpty () const{
        if (top == -1)
            return true;
        else
            return false;
    }
    bool isFull () const{
        if (top == 49)
            return true;
        else
            return false;
    }
    void pop (){
        top--;
    }
    void push (char c){
        if (isFull ())
            return;
        else{
            top++;
            array[top] = c;
        }
    }
    char getTopVal (){
        return array[top];
    }
    int getStackSize () const{
        return top;
    }
};

int isOperator (int pos, const char string[]){
    if (string[pos] == '+' || string[pos] == '*' || string[pos] == '~' || string[pos] == '>' || string[pos] == ')' || string[pos] == '(')
        return 1;
    else
        return 0;
}

int checkPrecedence (char a){
    switch (a){
        case '~':
            return 4;
        case '*':
            return 3;
        case '+':
            return 2;
        case '>':
            return 1;
        case '(':
            return 0;
        case ')':
            return 0;
    }
}

int getLen (const char string[]){
    int count = 0;
    while (string[count] != NULL && count != maxSize - 1){
        count++;
    }
    return count;
}

void reverseString (char string[]){
    char temp;
    int len = getLen (string);
    for (int i = 0; i < len / 2; i++){
        temp = string[i];
        string[i] = string[len - i - 1];
        string[len - i - 1] = temp;
    }
}

void infixToPrefix (char infix[])
{
    int len = getLen (infix);
    reverseString (infix);
    Stack stack;
    char prefix[maxSize];
    int count = 0;
    for (int i = 0; i < len; i++){
        if (infix[i] == '(')
            infix[i] = ')';
        else if (infix[i] == ')')
            infix[i] = '(';
    }
    for (int i = 0; i < len; i++){
        if (isOperator (i, infix) == 0){
            prefix[count] = infix[i];
            count++;
        }
        else if (infix[i] == '(')
            stack.push (infix[i]);
        else if (infix[i] == ')'){
            while (stack.getTopVal () != '(' && !stack.isEmpty ()){
                stack.pop ();
                prefix[count] = infix[i];
                count++;
            }
            if (stack.getTopVal () == '(')
                stack.pop ();
        }
        else if (isOperator (i, infix) == 1){
            if (stack.isEmpty ())
                stack.push (infix[i]);
            else{
                int precIn = checkPrecedence (infix[i]);
                int precStack = checkPrecedence (stack.getTopVal ());
                if (precIn == precStack)
                    stack.push (infix[i]);
                else if (precIn == precStack && infix[i] == '~'){
                    while ((checkPrecedence (infix[i]) ==checkPrecedence (stack.getTopVal ()))&& (infix[i] == '~')){
                        prefix[count] = stack.getTopVal ();
                        count++;
                        stack.pop ();
                    }
                    stack.push (infix[i]);
                }
                else if (checkPrecedence (infix[i]) >checkPrecedence (stack.getTopVal ())){
                    stack.push (infix[i]);
                }
                else
                {
                    while ((!stack.isEmpty ())&& (checkPrecedence (infix[i]) <checkPrecedence (stack.getTopVal ()))){
                        prefix[count] = stack.getTopVal ();
                        count++;
                        stack.pop ();
                    }
                    stack.push (infix[i]);
                }
            }
        }
    }
    while (!stack.isEmpty ()){
        prefix[count] = stack.getTopVal ();
        count++;
        stack.pop ();
    }
    reverseString (prefix);
    printf ("Prefix: %s\n", prefix);
}

node * parseTree(){
    int len = getLen(prefixExp);
    if(curr >= len)
        return nullptr;
    struct node * newNode = (node*)malloc(sizeof(node));
    newNode->data = prefixExp[curr];
    if(prefixExp[curr] == '>' || prefixExp[curr] == '+' || prefixExp[curr] == '*'){
        ++curr;
        newNode->left = parseTree();
        newNode->right = parseTree();
        return newNode;
    }
    else if(prefixExp[curr] == '~'){
        ++curr;
        newNode->left = parseTree();
        return newNode;
    }
    else{
        newNode->left = nullptr;
        newNode->right = nullptr;
        curr++;
        return newNode;
    }
}
void inorderTraversal (struct node *root){
    if (root == nullptr)
        return;
    {
        inorderTraversal (root->left);
        printf ("%c", (root->data));
        inorderTraversal (root->right);
    }
}

int max (int a, int b){
    if(a > b)
        return a;
    else
        return b;
}

int height (struct node *root){
    if (root == nullptr)
        return -1;
    else
        return max (height (root->left), height (root->right)) + 1;
}
void makeTree() {
    struct node* root;
    root = parseTree();
    printf("\n");
    inorderTraversal(root);
    printf("\n");
    printf("root node: %c \n", root->data);
    printf("height of tree: %d \n", height(root));
}
int solve(int a, int b, char op){
    if(op == '+')
        return a||b;
    if(op == '*')
        return a&&b;
    if(op == '>')
        return (((!a)&&(!b))+b);
}

int truthValue(struct node* root){
    if(root!=nullptr){
        if((root->data) == '0' || (root->data) == '1')
            return (root->data)-'0';
        int leftV = truthValue(root->left);
        int rightV = truthValue(root->right);
        return solve(leftV,rightV,root->data);
    }
}
int main (){
    printf("Infix: %s\n",infix);
    infixToPrefix(infix);
    node *s = nullptr;
    printf("Infix after in order traversing: ");
    makeTree ();
}
