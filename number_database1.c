#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define BUFFER_SIZE 128
#define opc 7

int gen_rand()
{
 return RAND_MAX == 0x7fffffff ? rand() : rand() << 16 | rand() << 1 | rand() & 1;
}
int *nums;
long int n, nt, r1, r2, ops, range;

int search(int target);
int cmpint(const void * a, const void * b); 
int find(int target);
int succ(int target);
int pred(int target);
int minv();
int maxv();
bool add(int value);
bool delete(int value);

void user_interface();

int main(int argc, char * argv[])
{double factor = 0.1;
 if (argc != 4)
   {printf("Command line arguments are : n r1 r2\n");
    exit(0);
   }
 n = atol(argv[1]);
 nt = n * (1 + factor);
 ops = n / 10;
 r1 = atoi(argv[2]);
 r2 = atol(argv[3]);
 range = r2 - r1 + 1;
 nums = malloc(nt * sizeof(int));
 printf("n = %ld, r1 = %ld, r2 = %ld, Memory used = %f Mbytes\n", 
           n, r1, r2, (nt * sizeof(int) / 1000000.0));
 for (int i = 0; i < nt; i++)
   nums[i] = gen_rand() % range + r1;  
 qsort(nums, nt, sizeof(int), cmpint);
 for (int i = 10; i < nt - 10; i++)
   if (i % 10 == 1 && nums[i - 1] != nums[i + 1])
     nums[i] = -1;
 user_interface();
 return 0; 
}

// user interface;
void user_interface() {
    int choice;
    int value;

    while (1) {
        printf("\nOptions:\n");
        printf("1. Find value\n");
        printf("2. Add value\n");
        printf("3. Delete value\n");
        printf("4. Successor\n");
        printf("5. Predecessor\n");
        printf("6. Minimum value\n");
        printf("7. Maximum value\n");
        printf("8. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value to find: ");
                scanf("%d", &value);
                int result = find(value);
                if (result > 0) {
                    printf("Value %d found %d times.\n", value, result);
                } else {
                    printf("Value %d not found.\n", value);
                }
                break;

            case 2:
                printf("Enter value to add: ");
                scanf("%d", &value);
                if (add(value)) {
                    printf("Value %d added successfully.\n", value);
                } else {
                    printf("Value %d could not be added.\n", value);
                }
                break;

            case 3:
                printf("Enter value to delete: ");
                scanf("%d", &value);
                if (delete(value)) {
                    printf("Value %d deleted successfully.\n", value);
                } else {
                    printf("Value %d not found and could not be deleted.\n", value);
                }
                break;

            case 4:
                printf("Enter value to find successor: ");
                scanf("%d", &value);
                int successor = succ(value);
                if (successor != -1) {
                    printf("Successor of %d is %d.\n", value, successor);
                } else {
                    printf("No successor found for %d.\n", value);
                }
                break;

            case 5:
                printf("Enter value to find predecessor: ");
                scanf("%d", &value);
                int predecessor = pred(value);
                if (predecessor != -1) {
                    printf("Predecessor of %d is %d.\n", value, predecessor);
                } else {
                    printf("No predecessor found for %d.\n", value);
                }
                break;

            case 6:
                int minValue = minv();
                if (minValue != -1) {
                    printf("Minimum value is %d.\n", minValue);
                } else {
                    printf("No minimum value found.\n");
                }
                break;

            case 7:
                int maxValue = maxv();
                if (maxValue != -1) {
                    printf("Maximum value is %d.\n", maxValue);
                } else {
                    printf("No maximum value found.\n");
                }
                break;

            case 8:
                return;  // Exit the user interface loop

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// base code
int search(int target)
{int low = 0, high = nt - 1;
 if (target < nums[0])
   return 0;
 int mid = (high + low) / 2;
 while (low <= high)
   {while (mid > 0 && nums[mid] == -1) mid--;
    if (nums[mid] == target)
      {int i = mid;
       while (i > 0 && nums[i - 1] == target)
         i--;
       return i; 
      }
    else if (target < nums[mid])
      {high = mid - 1;
       while (high > 0 && nums[high] == -1) high--;
      }
    else  
      {low = mid + 1;
       while (low < nt && nums[low] == -1) low++;
      }
    mid = (high + low) / 2;
   }    
 return -mid;
}

int cmpint(const void * a, const void * b) 
{return ( *(int*)a - *(int*)b );
}

int find(int target)
{int i = search(target), j;
 if (i < 0)
   return 0;
 if (i == 0 && nums[0] != target)
   return 0;  
 for (j = i + 1; j < nt; j++)
   if (nums[j] != target)
     break;
 return j - i;        
}

int succ(int target)
{int i = search(target);
 if (i < 0 || i >= nt - 1)
   return -1;
 for (; i < nt; i++)
   if (nums[i] != -1 && nums[i] != target)  
     return nums[i];
 return -1;      
}

int pred(int target)
{int i = search(target);
 if (i <= 0)
   return -1;
 for (i--; i >= 0; i--)
   if (nums[i] != -1 && nums[i] != target)  
     return nums[i];
 return -1;      
}

bool add(int value)
{int i = abs(search(value));
 if (i > 0 && nums[i - 1] == -1)
   {nums[i - 1] = value;
    return true;
   }  
 i++;  
 int j = i;
 while (j < nt - 1 && nums[j] != -1) j++;
 if (j == nt)
   return false;
 while (j > i)
   {nums[j] = nums[j - 1];
    j--;
   }
 nums[i] = value;  
 return true;
}

bool delete(int value)
{int i = search(value);
 if (i < 0)  
   return false;
 nums[i] = -1;
 return true;      
}

int minv()
{for (int i = 0; i < nt; i++)
   if (nums[i] != -1)
     return nums[i];
 return -1;       
}

int maxv()
{for (int i = nt - 1; i >= 0; i--)
   if (nums[i] != -1)
     return nums[i];
 return -1;       
}
