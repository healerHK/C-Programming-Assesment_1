#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define BUFFER_SIZE 128
// basic stack
int *stack;
int stack_size = 0;
int top = -1;
void initialise(int size);
bool push(int number);
bool pop(int *number);
int size();

// user interface
void user_interface();

void ipush(int number, int count);
void ipop(int count);
void idelete(int number, int count);
void iprint(int count);
void iresize(int percent);
void isize();

int main()
{
    user_interface();
    if (stack)
        free(stack);
    return 0;
}

// Base stack code
void initialise(int size)
{
    if (stack_size != 0)
        free(stack);
    stack = (int *)malloc(size * sizeof(int));
    stack_size = size;
    top = -1;
}

bool push(int number)
{
    if (top + 1 < stack_size) // room to add number
    {
        top++;
        stack[top] = number;
        return true;
    }
    return false;
}

bool pop(int *number)
{
    if (top >= 0)
    {
        *number = stack[top];
        top--;
        return true;
    }
    return false;
}

int size()
{
    return top + 1;
}
// end stack

// begin user interface;
void user_interface()
{
    char buffer[BUFFER_SIZE], text[16];
    while (true)
    {
        printf("Enter command:");
        fgets(buffer, BUFFER_SIZE, stdin); // handles \0 so we can put 128 acc
        int number = -1, count = -1;
        if (sscanf(buffer, "init %d", &number) == 1)
            initialise(number);
        else if (sscanf(buffer,"quit")==0)
        {
            printf("Program terminating \n");
            break;
        }
        else if (stack_size == 0)
            printf("Stack not initialised \n");
        else if (sscanf(buffer, "push %d %d", &number, &count) == 2)
            ipush(number, count);
        else if (sscanf(buffer, "push %d", &number) == 1)
            ipush(number, 1);
        else if (sscanf(buffer, "pop %d", &count) == 1)
            ipop(count);
        else if (sscanf(buffer, "pop %s", text) == 1 && strncmp(text, "all", 3) == 0)
            ipop(-1); // -1 to signal all
        else if (strncmp(buffer, "pop", 3) == 0)
            ipop(1);
        else if (sscanf(buffer, "del %d %d", &number, &count) == 2)
            idelete(number, count);
        else if (sscanf(buffer, "del %d %s", &number, text) == 2 &&
                 strncmp(text, "all", 3) == 0)
            idelete(number, -1); // -1 to signal all
        else if (sscanf(buffer, "del %d", &number) == 1)
            idelete(number, 1);
        else if (sscanf(buffer, "print %d", &number) == 1)
            iprint(count);
        else if (strncmp(buffer, "print", 5) == 0)
            iprint(-1); // -1 to signal all
        else if (sscanf(buffer, "resize %d", &number) == 1)
            iresize(number);
        else if (strncmp(buffer, "size", 4) == 0)
            isize();
        else
            printf("invalid command \n");
    }
}

// user interface functions
void ipush(int number, int count)
{
    bool overflow = false;
    for (int i = 0; i < count && !overflow; i++)
        overflow = !push(number);
    if (overflow)
        printf("Stack full\n");
} 

void ipop(int count)
{
    if (count == -1) // pop all
        count = top + 1;
    int number;
    for (int i = 0; i < count && pop(&number); i++)
        printf("%d\n", number);
}

void idelete(int number, int count)
{
    if (count == -1)
        count = top + 1;
    int deleted = 0;
    int index = 0;
    for (int i = 0; i <= top; i++)
        if (deleted < count && stack[i] == number)
            deleted++;
        else
        {
            stack[index] = stack[i];
            index++;
        }
    top = index - 1;
}

void iprint(int count)
{
    if (top == -1)
        printf("stack is empty\n");
    else
    {
        if (count == -1)
            count = top + 1;
        for (int i = top; i >= top - count + 1; i--)
            printf("%d\n", stack[i]);
    }
}

void iresize(int percent)
{
    int new_stack_size = stack_size * ((double)percent / 100);
    if (new_stack_size < size())
    {
        printf("You would lose data, currently % elemnts stored \n", size());
        return;
    }
    int *new_stack = (int *)malloc(new_stack_size * sizeof(int));
    for (int i = 0; i <= top; i++)
        new_stack[i] = stack[i];
    free(stack);
    stack = new_stack;
    stack_size = new_stack_size; 
}

void isize()
{
    printf("Stack size = %d, in use %d\n", stack_size, size());
}