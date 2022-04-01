#include <stdio.h>
#include <stdlib.h>

struct Data
{
    unsigned int x;
    size_t i;
};

struct Node
{
    struct Data data;
    struct Node *next;
};

/*void add_end(struct Node **end, struct Node **start, struct Data data)
{
    struct Node *buf = malloc(sizeof(struct Node));
    buf->data = data;//(*buf).data = data;
    buf->next = (*end);
    (*end) = buf;
    if(*start == NULL)
    {
        (*start) = buf;
    }
}*/

void add_start(struct Node **end, struct Node **start, struct Data data)
{
    struct Node *buf = malloc(sizeof(struct Node));//???
    buf->data = data;
    buf->next = NULL;
    if(*start == NULL)
    {
        (*end) = buf;
        (*start) = buf;
    }
    else
    {
        (*start)->next = buf;
        (*start) = buf;
    }
}

struct Node *del_end(struct Node **start, struct Node **end)
{
    if(*end != NULL)
    {
        struct Node *next = (*end)->next;
        if((*end) != (*start))
        {
            free(*end);
            (*end) = next;
        }
        else
        {
            free(*end);
            (*end) = NULL;
            (*start) = NULL;
        }
        return next;
    }
    else
        return NULL;
}

void print_data(struct Node *end)
{
    printf("\n=============================================\n");
    struct Node *buf = end;
    while(buf != NULL)
    {
        printf("|%zu %u|\n", buf->data.i, buf->data.x);
        buf = buf->next;
    }
    printf("=============================================\n");
}


void qs(struct Node **a, size_t n)
{
    if(n <= 1)
        return;

    struct Data op = (*(a[n/2])).data;
    size_t ind_op = n/2;

    size_t i = 0;
    size_t j = n-1;
    while (i != j)
    {
        while((*(a[i])).data.x > op.x || ((*(a[i])).data.x == op.x && (*(a[i])).data.i < op.i)) i++;
        while(((*(a[j])).data.x < op.x || ((*(a[j])).data.x == op.x && (*(a[j])).data.i > op.i)) && j > ind_op) j--;

        if (i == j)
            break;

        if(i == ind_op)
            ind_op = j;
        else if(j == ind_op)
            ind_op = i;

        struct Data buffer = (*(a[i])).data;
        (*(a[i])).data = (*(a[j])).data;
        (*(a[j])).data = buffer;
    }

    qs(a, ind_op);
    qs(a + ind_op + 1, n - ind_op - 1);
}

long long bs(struct Node **ptr, unsigned int x, long long l, long long r)
{
    printf("bs\n");
    while (l <= r) {
        long long m = (l + r) / 2;
        if ((*(ptr[m + 1])).data.x > x) {
            l = m + 1;
        } else if ((*(ptr[m])).data.x > x) {
            return m + 1;
        } else {
            r = m - 1;
        }
    }
    return l;
}

void set(size_t i, unsigned int v, struct Node **ptr)
{

}

int main() {
    struct Node *start = NULL;
    struct Node *end = NULL;

    size_t n, m;

    scanf("%zu%zu", &n, &m);

    struct Node **ptr = malloc((n+1)*sizeof (struct Node *));
    if(ptr == NULL)
    {
        printf("Not memory\n");
        return 1;
    }

    struct Data buffer;
    for(size_t i = 0; i < n; i++)
    {
        scanf("%u", &buffer.x);
        buffer.i = i;
        add_start(&end, &start, buffer);
        ptr[i] = start;
    }
    buffer.x = 0;
    buffer.i = n;
    add_start(&end, &start, buffer);
    ptr[n++] = start;

    print_data(end);

    qs(ptr, n);

    print_data(end);

    for(int i = 0; i < m; i++)
    {
        int c;
        scanf("%d", &c);
        switch (c) {
            case 1:
            {
                printf("C is not 1 or 2\n");
                print_data(end);
                break;
            }
            case 2:
            {
                unsigned int x;
                size_t l;
                scanf("%u%zu", &x, &l);
                long long res = bs(ptr, x, ptr[l]->data.i, n-2);
                printf("%lld\n", res);
                if(res == 0 && res == n-1)
                {
                    printf("-1\n");
                }
                else
                {
                    printf("%lld\n", ptr[res-1]->data.i);
                }
                break;
            }
            default:
            {
                printf("C is not 1 or 2\n");
                return 2;
            }
        }
    }

    free(ptr);

    while(del_end(&start, &end));
    return 0;
}
