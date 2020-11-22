#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

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

int main() {

    // number of atoms
    int n;
    scanf("%d\n", &n);
    fflush(stdin);

    // propositional logic formula
    char formula[MAX];
    fgets(formula, MAX, stdin); // has a trailing \n character, ignore it later

    // truth values
    char tv[MAX];
    fgets(tv, MAX, stdin); // has a trailing \n character, ignore it later

    struct Stack *operatorStack = createStack(10);
    struct Stack *operandStack = createStack(10);

    int charSize = strlen(formula) - 1; // ignoring the trailing \n character
    for (int i = 0; i < charSize; i++) {
        if (formula[i] == ' ' || formula[i] == '\n') { continue; }
        char tempChar = formula[i];
        if (tempChar == '~' || tempChar == '^' || tempChar == 'V' || tempChar == '>' || tempChar == '(') {
            push(operatorStack, tempChar);
        } else if (tempChar == ')') {
            char cursor = pop(operatorStack);
            if (cursor == '^' || cursor == 'V' || cursor == '>') {
                // binary operator
                char operand2 = pop(operandStack);
                char operand1 = pop(operandStack);
                int truthValue1 = (operand1 == 'T');
                int truthValue2 = (operand2 == 'T');
                int truthValueFinal;
                if (cursor == '^') {
                    truthValueFinal = truthValue1 & truthValue2;
                } else if (cursor == 'V') {
                    truthValueFinal = truthValue1 | truthValue2;
                } else if (cursor == '>') {
                    truthValueFinal = !truthValue1 | truthValue2;
                }
                char operandFinal = (truthValueFinal == 1) ? 'T' : 'F';
                push(operandStack, operandFinal);
            } else {
                // unary operator
                char operand = pop(operandStack);
                int truthValue = (operand == 'T');
                // negation is the only uninary operator
                int truthValueFinal = !truthValue;
                char operandFinal = (truthValueFinal == 1) ? 'T' : 'F';
                push(operandStack, operandFinal);
            }
            pop(operatorStack); // pop the '('
        } else {
            char correspondingTruthValue = tv[tempChar - '0' - 1];
            push(operandStack, correspondingTruthValue);
        }
    }
    printf("%c\n", pop(operandStack));
    return 0;
}