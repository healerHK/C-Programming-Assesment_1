#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define BUFFER_SIZE 128
#define MAX_SIZE 1000000000
#define opc 7

void stats();
int gen_rand()
{
    return RAND_MAX == 0x7fffffff ? rand() : rand() << 16 | rand() << 1 | rand() & 1;
}
int *nums;
long int n, nt, r1, r2, ops, range;

int search(int target);
int cmpint(const void *a, const void *b);
int find(int target);
int succ(int target);
int pred(int target);
int minv();
int maxv();
bool add(int value);
bool delete(int value);
bool resize(int percentage);

void user_interface();

int main(int argc, char *argv[])
{
    double factor = 0.1;
    if (argc != 4)
    {
        printf("Command line arguments are : n r1 r2\n");
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
void user_interface()
{
    char buffer[BUFFER_SIZE], text[16];
    while (true)
    {
        printf("Enter command: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        int number = 0, count = 0;
        if (strstr(buffer, "quit") && sscanf(buffer, "quit")==0)
        {
            printf("Program terminating.\n");
            break;
        }

        else if (sscanf(buffer, "find %d", &number) == 1)
        {
            count = find(number);
            int predecessor = pred(number);
            int successor = succ(number);
            if (count > 0)
            {
                printf("%d is found %d times.\n", number, count);
                printf("The predecessor of %d is %d.\n", number, predecessor);
                printf("The successor of %d is %d.\n", number, successor);
            }
            else
                printf("%d does not exist in the database.\n", number);
        }

        else if (sscanf(buffer, "add %d %d", &number, &count) == 2)
        {
            bool success = true;
            for (int i = 0; i < count; i++)
            {
                success = success && add(number);
            }
            if (success)
                printf("%d added  %d times to the table.\n",  number, count);
            else
                printf("Failed to %d added  %d times to the table.\n", number, count);
        }

        else if (sscanf(buffer, "add %d", &number) == 1)
        {
            bool success = add(number);
            if (success)
                printf("%d is added successfully.\n", number);
            else
                printf("Failed to add %d.\n", number);
        }

        else if (sscanf(buffer, "del %d %s", &number, text) == 2 &&
                 strncmp(text, "all", 3) == 0)
        {
            bool success = true;
            while (find(number) > 0)
            {
                success = success && delete (number);
            }
            if (success)
                printf("All instances of %d are deleted successfully.\n", number);
            else
                printf("Failed to delete all instances of %d.\n", number);
        }

        else if (sscanf(buffer, "del %d %s", &number,text) == 1)
        {
            bool success = delete (number);
            if (success)
                printf("%d is deleted successfully.\n", number);
            else
                printf("Failed to delete %d.\n", number);
        }

        else if (sscanf(buffer, "resize %d", &number) == 1)
        {
            bool success = resize(number);
            if (success)
                printf("The array is resized to %d%% of the current size, and the -1s are redistributed.\n", number);
            else
                printf("Failed to resize the array.\n");
        }

        else if (sscanf(buffer, "print %d %d", &number, &count) == 2)
        {
            int index = search(number);
            if (index < 0)
                printf("%d is not exist in the database.\n", number);
            else
            {
                int start = index - count;
                int end = index + count;
                if (start < 0)
                    start = 0;
                if (end >= nt)
                    end = nt - 1;
                printf("The contents of the array from %d numbers before %d to %d numbers after %d are:\n", count, number, count, number);
                for (int i = start; i <= end; i++)
                    printf("%d ", nums[i]);
                printf("\n");
            }
        }

        else if (strstr(buffer, "stats") && sscanf(buffer, "stats") == 0)
        {
            stats();
        }

        else
        {
            printf("Invalid command.\n");
        }
    }
}

void stats()
{
    double sum = 0, mean = 0, std = 0, median = 0;
    int min = INT_MAX, max = INT_MIN;
    int dup_count = 0, cons_count = 0, neg_count = 0;
    int longest_dup = 0, longest_cons = 0, longest_neg = 0;
    int current_dup = 0, current_cons = 0, current_neg = 0;
    int non_neg_count = 0;
    for (int i = 0; i < nt; i++)
    {
        if (nums[i] >= 0)
        {
            sum += nums[i];
            non_neg_count++;
        }

        if (nums[i] < min && nums[i] != -1)
            min = nums[i];
        if (nums[i] > max && nums[i] != -1)
            max = nums[i];

        if (i > 0 && nums[i] == nums[i - 1] && nums[i] != -1)
        {
            dup_count++;
            current_dup++;
            if (current_dup > longest_dup)
                longest_dup = current_dup;
        }
        else
            current_dup = 1;
        if (i > 0 && nums[i] == nums[i - 1] + 1 && nums[i] != -1)
        {
            cons_count++;
            current_cons++;
            if (current_cons > longest_cons)
                longest_cons = current_cons;
        }
        else
            current_cons = 1;
        if (nums[i] == -1)
        {
            neg_count++;
            current_neg++;
            if (current_neg > longest_neg)
                longest_neg = current_neg;
        }
        else
            current_neg = 0;
    }
    if (non_neg_count > 0)
        mean = sum / non_neg_count;

    for (int i = 0; i < nt; i++)
    {
        if (nums[i] >= 0)
            std += pow(nums[i] - mean, 2);
    }
    if (non_neg_count > 0)
        std = sqrt(std / non_neg_count);

    int *temp = malloc(nt * sizeof(int));
    memcpy(temp, nums, nt * sizeof(int));
    qsort(temp, nt, sizeof(int), cmpint);

    int mid = non_neg_count / 2;
    if (non_neg_count % 2 == 0)
        median = (temp[mid - 1] + temp[mid]) / 2.0;
    else
        median = temp[mid];

    free(temp);

    printf("Statistics for the %d numbers stored in nums:\n", non_neg_count);

    printf("a. Average = %.2f, Std Dev = %.2f, Median= %.2f\n", mean, std, median);

    printf("b. Min = %d, Max = %d, Value Usage: %.2f%%\n", min, max, non_neg_count * 100.0 / nt);

    printf("c. Max Dups: %d, Max Con Seq: %d, Max -1s Seq: %d\n", longest_dup, longest_cons, longest_neg);

    printf("d. %Memory for -1s: %.2f%%, -1s average interval: %.2f, -1s max interval: %d\n", neg_count * 100.0 / nt, nt * 1.0 / (neg_count + 1), longest_cons + longest_dup + 1);

    bool valid = true;
    for (int i = 0; i < nt - 1; i++)
    {
        if (nums[i] != -1 && nums[i + 1] != -1 && nums[i] > nums[i + 1])
        {
            valid = false;
            break;
        }
    }
    if (valid)
        printf("e.Numbers in database are in correct sequence\n");
    else 
        printf("e.Numbers in databse are not in correct sequence\n");
}
int search(int target)
{
    int low = 0, high = nt - 1;
    if (target < nums[0])
        return 0;
    int mid = (high + low) / 2;
    while (low <= high)
    {
        while (mid > 0 && nums[mid] == -1)
            mid--;
        if (nums[mid] == target)
        {
            int i = mid;
            while (i > 0 && nums[i - 1] == target)
                i--;
            return i;
        }
        else if (target < nums[mid])
        {
            high = mid - 1;
            while (high > 0 && nums[high] == -1)
                high--;
        }
        else
        {
            low = mid + 1;
            while (low < nt && nums[low] == -1)
                low++;
        }
        mid = (high + low) / 2;
    }
    return -mid;
}

int cmpint(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int find(int target)
{
    int i = search(target), j;
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
{
    int i = search(target);
    if (i < 0 || i >= nt - 1)
        return -1;
    for (; i < nt; i++)
        if (nums[i] != -1 && nums[i] != target)
            return nums[i];
    return -1;
}

int pred(int target)
{
    int i = search(target);
    if (i <= 0)
        return -1;
    for (i--; i >= 0; i--)
        if (nums[i] != -1 && nums[i] != target)
            return nums[i];
    return -1;
}

bool add(int value)
{
    int i = abs(search(value));
    if (i > 0 && nums[i - 1] == -1)
    {
        nums[i - 1] = value;
        return true;
    }
    i++;
    int j = i;
    while (j < nt - 1 && nums[j] != -1)
        j++;
    if (j == nt)
        return false;
    while (j > i)
    {
        nums[j] = nums[j - 1];
        j--;
    }
    nums[i] = value;
    return true;
}

bool delete(int value)
{
    int i = search(value);
    if (i < 0)
        return false;
    nums[i] = -1;
    return true;
}

int minv()
{
    for (int i = 0; i < nt; i++)
        if (nums[i] != -1)
            return nums[i];
    return -1;
}

int maxv()
{
    for (int i = nt - 1; i >= 0; i--)
        if (nums[i] != -1)
            return nums[i];
    return -1;
}

bool resize(int percentage)
{
    int new_size = nt * percentage / 100;
    if (new_size < n || new_size > MAX_SIZE)
        return false;

    int *new_nums = malloc(new_size * sizeof(int));
    int j = 0;
    for (int i = 0; i < nt; i++)
    {
        if (nums[i] >= 0)
        {
            new_nums[j] = nums[i];
            j++;
        }
    }
    int interval = new_size / (nt - n + 1);
    j = interval;
    while (j < new_size)
    {
        new_nums[j] = -1;
        j += interval + 1;
    }
    free(nums);
    nums = new_nums;
    nt = new_size;
    return true;
}
