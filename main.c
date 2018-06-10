#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define  SIZE 1000
struct  Stack{
    int top;
    unsigned capacity;
    int* array;
};
typedef struct Variables{
    char name;
    int value;
}variable;
// Stack Operations
struct Stack* createStack( unsigned capacity )
{
    struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
    if (!stack)
        return NULL;
    stack->top = -1;
    stack->capacity = capacity;
    stack->array = (int*) malloc(stack->capacity * sizeof(int));
    if (!stack->array)
        return NULL;
    return stack;
}
int isEmpty(struct Stack* stack)
{
    return stack->top == -1 ;
}
int pop(struct Stack* stack)
{
    if (!isEmpty(stack))
        return stack->array[stack->top--] ;
    return '\0';
}
void push(struct Stack* stack, int op)
{
    stack->array[++stack->top] = op;
}
// A utility function to check if the given character is operand
int is_operator(char symbol)
{
    if(symbol == '^' || symbol == '*' || symbol == '/' || symbol == '+' || symbol =='-')
        return 1;
    else
        return 0;
}
// A utility function to return precedence of a given operator
// Higher returned value means higher precedence
int prec(char ch) {
    switch (ch)
    {
        case '+':
        case '-':
            return 1;

        case '*':
        case '/':
            return 2;

        case '^':
            return 3;
    }
    return -1;
}
void reset(char array[SIZE]){
    int m=0;
    while(array[m]!='\0')
    {
        array[m]='\0';
        m++;
    }
}
void partition(char line[256],char func[SIZE],char s[2]){
    char *token;
    int i=0;
    int j=1;
    int k=0;
    /* get the first token */
    token = strtok(line, s);
    /* walk through other tokens */
    while( token != NULL ) {
        if(s=="="&& i==1){
            while (token[j]!='\n' && token[j]!='\0') {
                if(token[j]!=';' && token[j+1]!=';'){
                    func[k]=token[j];
                    k++;
                    j++;
                }
                else
                    j++
               ;
            }
        }
        i++;
        token = strtok(NULL, s);
    }
}
int searchValue(variable v[10],char name){
    int i=-1;
    int true=0;
    while(v[i].name!='\0' && true==0){
        i++;
        if(v[i].name==name) {
            true = 1;
        }
    }
    if(true==0)
        return -1;
    else
        return i;
}
int isNumber(int i, char postfixExp[SIZE]) {
    int j=0;
    while(postfixExp[i]!=' '&& postfixExp[i]!='\n'&& isdigit(postfixExp[i]))
    {
        j++;
        i++;
    }
    return j;
}
int infixToPostfix(char infixExp[SIZE],char postfixExp[SIZE])
{
    // Create a stack of capacity equal to expression size
    struct Stack* stack = createStack(SIZE);
    if(!stack) // See if stack was created successfully
        return -1 ;
    int i=0;
    int j=0;
    char item;
    int decrease=0;
    char x;
    int n=0;
    printf("\n------INFIX TO POSTFIX------\n");
    item=infixExp[i];         /* initialize before loop*/
    while(item != '\n' && item != '\0')        /* run loop till end of infix expression */
    {
        if(item == '(')
        {
            push(stack,item);
        }
        else if(item==' ')
        {
            if(postfixExp[j-1]!=' ')
            {
                postfixExp[j]=item;
                j++;
            }
        }
        else if(isdigit(item))
        {
            decrease=isNumber(i,infixExp);
            while(decrease>0)
            {
                postfixExp[j]=infixExp[i];
                j++;
                i++;
                decrease--;
            }
            postfixExp[j]=infixExp[i];
        }
        else if(isalpha(item))
        {
            postfixExp[j] = item;              /* add operand symbol to postfix expr */
            j++;
        }
        else if(is_operator(item) == 1)        /* means symbol is operator */
        {
            x=pop(stack);
            while(is_operator(x) == 1 && prec(x)>= prec(item))
            {
                postfixExp[j] = x;                  /* so pop all higher precendence operator and */
                j++;
                x = pop(stack);                       /* add them to postfix expresion */
            }
            push(stack,x);
            /* because just above while loop will terminate we have
            oppped one extra item
            for which condition fails and loop terminates, so that one*/

            push(stack,item);                 /* push current oprerator symbol onto stack */
        }
        else if(item == ')')         /* if current symbol is ')' then */
        {
            x = pop(stack);                   /* pop and keep popping until */
            while(x != '(')                /* '(' encounterd */
            {
                postfixExp[j] = x;
                j++;
                x = pop(stack);
            }
        }
        i++;
        int t=0;
        if(item!=' ')
        {
            printf("Postfix : ");
            while (t<j)
            {
                if(postfixExp[t]!=' ')
                {
                    printf("%c",postfixExp[t]);
                }
                t++;
            }
            printf("\t \tStack : ");
            if(stack->top==-1){
                printf("Empty");
            }
            else{
                for(n=1;n<(stack->top)+1;n++){
                    printf("%c",stack->array[n]);
                }
            }
            printf("\n");
        }
        item = infixExp[i]; /* go to next symbol of infix expression */
    } /* while loop ends here */
    x=pop(stack);
    while(x!='\0'){
        postfixExp[j]=x;
        j++;
        x=pop(stack);
    }
    printf("POSTFIX OUTPUT :");
    int c=0;
    while (c<j)
    {
        if(postfixExp[c]!=' ')
        {
            printf("%c",postfixExp[c]);
        }
        c++;
    }
    postfixExp[j] = '\n'; /* add sentinel else puts() fucntion */
    /* will print entire postfix[] array upto SIZE */
}
int evaluatePostfix(char* postfixExp,variable v[10])
{
    // Create a stack of capacity equal to expression size
    struct Stack* stack = createStack(SIZE);
    int i;
    int c;
    int index;
    int decrease=0;
    char number[4];
    int k=0;
    // See if stack was created successfully
    if (!stack) return -1;
    printf("\n------POSTFIX EVALUATE------");
    // Scan all characters one by one
    for (i = 0; postfixExp[i]+1; ++i)
    {
        // If the scanned character is an operand (number here),
        // push it to the stack.
        if (isdigit(postfixExp[i]))
        {
            decrease=isNumber(i,postfixExp);
            for ( k = 0; k <decrease ; ++k) {
                number[k]=postfixExp[i];
                i++;
            }
            i--;
            push(stack, atoi(number));
            reset(number);
            c=-1;
            printf("\n Stack: ");
            while(c<stack->top && postfixExp[i]!=' ')
            {
                    c++;
                    printf("%d ",stack->array[c]);
            }
        }
        else if (isalpha(postfixExp[i])){
            index=searchValue(v,postfixExp[i]);
            push(stack,v[index].value);
            c=-1;
            printf("\n Stack: ");
            while(c<stack->top && postfixExp[i]!=' ')
            {
                    c++;
                    printf("%d ",stack->array[c]);

            }
        }
        else if(stack->top==0 &&postfixExp[i]!=' ' ){

            return pop(stack);
        }
            //  If the scanned character is an operator, pop two
            // elements from stack apply the operator
        else if(postfixExp[i]!=' ')
        {
            int val1 = pop(stack);
            int val2 = pop(stack);
            switch (postfixExp[i])
            {
                case '+': push(stack, (val2 + val1)); break;
                case '-': push(stack, (val2 - val1)); break;
                case '*': push(stack, (val2 * val1)); break;
                case '/': push(stack, (val2/val1));   break;
            }
            c=-1;
            printf("\n Stack: ");
            while(c<stack->top && postfixExp[i]!=' ')
            {
                if(stack->top==-1)
                    printf("Empty");
                else
                {
                    c++;
                    printf("%d ",stack->array[c]);
                }
            }
        }
    }
    printf("%d",pop(stack));
}
int main() {
    FILE *fp;
    fp = fopen("input.txt", "r");
    if(fp==NULL){
        printf("Error! Opening File");
        exit(1);
    }
    int i=0;
    char line[256];
    char infix[SIZE];
    variable v[10]={'\0','\0'};
    int n=0;
    int index;
    char postfix[SIZE];
    while(fgets(line, sizeof(line), fp)!=NULL){
        index=searchValue(v,line[0]);
        if(index==-1){
            index=n;
            v[index].name=line[0];
            n++;
        }
        partition(line,infix,"=");
        infixToPostfix(infix,postfix);
        v[index].value=evaluatePostfix(postfix,v);
        printf("\n");
        printf("%c = %d",v[index].name,v[index].value);
        reset(infix);
        reset(postfix);
        printf("\n");
    }
    for (i = 0; i <n ; ++i) {
        printf("%c = %d\t",v[i].name,v[i].value);
    }
    fclose(fp);
    return 0;
}
