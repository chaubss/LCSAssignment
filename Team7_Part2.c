/* Logic Homework 3, Part 1 */
/* Team Number: 7 */
/* Team Members:
1. Aagam Shah 2019A7PS1320H
2. Aryan Chaubal 2019A7PS0130H
3. Prathmesh Srivastava 2019A7PS1322H
4. Samarth Jain 2019A7PS0179H
*/
 
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>       //this program uses pow(). Please execute this with -lm tag if using linux
 
#define MAX 100
 
char tv[MAX];
int val[1030][11]={0},lastval=-1;
//array val[][] stores all the valuations which evaluate to false. variable lastval stores the index of the last valuation stored in val[][].
 
 
//function to take a given integer, convert it into binary, then store the truth values in tv[]
void toBinary(int n,int lim)
{
    int inBinary[10]={0},index=0;
    while(n>0)                      //convert to binary
    {
        int temp = n%2;
        inBinary[index]=temp;
        index++;
        n/=2;
    }
    if(index<lim)               //filling empty bits with 0
    {
        while(index<lim)
        {
            inBinary[index]=0;
            index++;
        }
    }
    for(int i=index-1;i>=0;i--)     //inverting the binary representation formed and storing truth values
    {
        if(inBinary[i]==0)
        {
            tv[index-i-1]='F';
        }
        else
        {
            tv[index-i-1]='T';
        }
        
    }
}
 
//function to store the valuations for which the formula evaluates to false
void store(int n){
    lastval++;      //incrementing index before storing each valuation
    for(int i=0;i<n;i++)
    {
        if(tv[i]=='T')
        {
            val[lastval][i]=1;
        }
        else{
            val[lastval][i]=0;
        }
    }
}
 
//function to generate CNF from stored valuations
void generatecnf(int n){
    printf("(");
    if(lastval==-1)     //printing TOP if formula is valid
        printf("T");
    for(int i=0;i<=lastval;i++)
    {
        printf("(");
        for(int j=0;j<n;j++)
        {
            if(val[i][j]==0)
            {
                printf("%d",j+1);   //if value of literal is false, we print it as-is
            }
            else
            {
                printf("(~%d)",j+1);  //if value of literal is true, we print its negation
            }
            if(j<n-1)
            {
                printf("V");        //printing disjunction after each literal except the last literal in the clause
            }
        }
        printf(")");
        if(i<lastval)
        {
            printf("^");        //printing conjunction after each clause except the last one
        }
    }
    printf(")");
}
 
 
// Stack Definition, taken from https://www.geeksforgeeks.org/stack-data-structure-introduction-program/
// A structure to represent a stack
struct Stack {
    char top;
    unsigned capacity;
    char *array;
};
 
// function to create a stack of given capacity. It initializes size of
// stack as 0
struct Stack *createStack(unsigned capacity) {
    struct Stack *stack = (struct Stack *)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = '-';
    stack->array = (char *)malloc(stack->capacity * sizeof(char));
    return stack;
}
 
// Stack is full when top is equal to the last index
int isFull(struct Stack *stack) {
    return (stack->top == (stack->capacity - 1));
}
 
// Stack is empty when top is equal to -1
int isEmpty(struct Stack *stack) {
    return (stack->top == '-');
}
 
// Function to add an item to stack.  It increases top by 1
void push(struct Stack *stack, char item) {
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
}
 
// Function to remove an item from stack.  It decreases top by 1
char pop(struct Stack *stack) {
    if (isEmpty(stack))
        return '-';
    return stack->array[stack->top--];
}
 
// Function to return the top from stack without removing it
char peek(struct Stack *stack) {
    if (isEmpty(stack))
        return '-';
    return stack->array[stack->top];
}
 
// This method will return the correct truth value after running the algorithm
char evalFormula(int n, char formula[MAX], char tv[MAX]) {
 
    // Create the stacks for operator and operand
    struct Stack *operatorStack = createStack(MAX);
    struct Stack *operandStack = createStack(MAX);
 
    int charSize = strlen(formula) - 1; // ignoring the trailing \n character
    for (int i = 0; i < charSize; i++) {
        if (formula[i] == ' ' || formula[i] == '\n') { continue; } // ignore any whitespace or new line characters
        char tempChar = formula[i]; // a variable holding the current character
        if (tempChar == '~' || tempChar == '^' || tempChar == 'V' || tempChar == '>' || tempChar == '(') {
            // If it is an operator, push it into the operator stack
            push(operatorStack, tempChar);
        } else if (tempChar == ')') {
            // Run process for when character is detected as a ')'
            char cursor = pop(operatorStack); // This is the current operator
            if (cursor == '^' || cursor == 'V' || cursor == '>') {
                // This is a binary operator, in this case, pop two values from the operand stack
                char operand2 = pop(operandStack); // operand2 is first because it was added last, and the stack will pop the last element
                char operand1 = pop(operandStack);
                int truthValue1 = (operand1 == 'T'); // Get the corresponding bit as in integer, 1 for T and 0 for F
                int truthValue2 = (operand2 == 'T');
                int truthValueFinal;
                // run the bitwise operations based on value of cursor
                if (cursor == '^') {
                    truthValueFinal = truthValue1 & truthValue2;
                } else if (cursor == 'V') {
                    truthValueFinal = truthValue1 | truthValue2;
                } else if (cursor == '>') {
                    truthValueFinal = !truthValue1 | truthValue2; // equivalent to implies
                }
                // Get the character T or F corresponding to 1 or 0
                char operandFinal = (truthValueFinal == 1) ? 'T' : 'F';
                push(operandStack, operandFinal); // Push it to the operator stack
            } else {
                // In case it wasn't a binary operator, it will be a unary operator
                char operand = pop(operandStack); // Pop only one operand this time since its unary
                int truthValue = (operand == 'T');
                // Negation is the only uninary operator
                int truthValueFinal = !truthValue;
                char operandFinal = (truthValueFinal == 1) ? 'T' : 'F';
                push(operandStack, operandFinal);
            }
            pop(operatorStack); // Pop the last '(' corresponding to this operation
        } else {
            // Assuming correct input, in all other cases, this will be an operand
            char correspondingTruthValue = tv[tempChar - '0' - 1]; // Getting the correct integer from character's ASCII value
            // Push the corresponding truth value into the operand stack
            push(operandStack, correspondingTruthValue);
        }
    }
    // In the end, the answer will be the only element left in the operand stack, pop and return it
    return(pop(operandStack));
}
 
int main() {
 
    // variable n stores the number of atoms
    //isvalid stores 1 if the formula is valid
    //issatisfiable stores1 if the formula is satisfiable
    int n,isvalid=1,issatisfiable=0;
    scanf("%d\n", &n);
 
    // Propositional Logic Formula
    char formula[MAX];
    fgets(formula, MAX, stdin); // fgets leaves a trailing \n character, ignore it later
 
    //iterating over all possible valuations. there are 2^n valuations for a formula with n atoms
    for(int i=0;i<pow(2,n);i++){
        toBinary(i,n);//converting current state of i to binary and getting the valuation of atoms
        char answer = evalFormula(n, formula, tv);//evaluating the formula for the found valuation
        
        
        if(answer=='F'){
            store(n);       //if valution evaluatesto false, storing it for generating cnf
            isvalid=0;      //if any one valuation is false, the formula becomes invalid
        }
        if(answer=='T')
        {
            issatisfiable=1;    //if any one valuationis true, the formula becomes satisfiable
        }
    }
    
    //printing satisfiaility
    if(issatisfiable)
        printf("Formula is satisfiable.\n");
    else
        printf("Formula is not satisfiable.\n");
        
    //printing validity
    if(isvalid)
        printf("Formula is valid.\n");
    else
        printf("Formula is not valid.\n");
    
    //printing the equivalent CNF of the formula 
    generatecnf(n);
    printf("\n");
    
    return 0;
}