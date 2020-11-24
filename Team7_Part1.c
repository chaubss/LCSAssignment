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

#define MAX 100

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

    // Number of atoms
    int n;
    scanf("%d\n", &n);

    // Propositional Logic Formula
    char formula[MAX];
    fgets(formula, MAX, stdin); // fgets leaves a trailing \n character, ignore it later

    // Truth Values
    char tv[MAX];
    fgets(tv, MAX, stdin);

    // Run the method we created evalFormula to get the answer
    char answer = evalFormula(n, formula, tv);
    printf("%c\n", answer);
    
    return 0;
}