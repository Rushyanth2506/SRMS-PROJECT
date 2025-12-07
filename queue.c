#include <stdio.h>
#include <stdlib.h>

#define MAX 5

int stack[MAX];
int top = -1;


void push(int value) {
    if (top == MAX - 1) {
        printf("\nStack Overflow! Cannot push.\n");
        return;
    }
    top++;
    stack[top] = value;
    printf("%d pushed to stack.\n", value);
}


void pop() {
    if (top == -1) {
        printf("\nStack Underflow! Cannot pop.\n");
        return;
    }
    printf("%d popped from stack.\n", stack[top]);
    top--;
}


void display() {
    if (top == -1) {
        printf("\nStack is Empty!\n");
        return;
    }
    printf("\nStack elements:\n");
    for (int i = top; i >= 0; i--) {
        printf("| %d |\n", stack[i]);
    }
}

int main() {
    int choice, value;

    while (1) {
        printf("\n\n--- STACK SIMULATION ---\n");
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Display\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter value to push: ");
            scanf("%d", &value);
            push(value);
            break;

        case 2:
            pop();
            break;

        case 3:
            display();
            break;

        case 4:
            exit(0);

        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
