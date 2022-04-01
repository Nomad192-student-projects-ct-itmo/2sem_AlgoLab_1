#include <stdio.h>
#include <stdlib.h>

//#define _DEBUG

typedef unsigned long long ull;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct element_t
{
    ull sum;
    ull min;
    int left;
    int right;
} Element;

void print(Element *data, int size)
{
#ifdef _DEBUG
    printf("\n=============================================\n");
    for (int i = 0; i < size; i++)
        printf("|%d %llu = %d %d|\n", i, data[i].sum, data[i].left, data[i].right);
    printf("=============================================\n");
#endif
}

ull sum(Element *data, int cur_elm, int l, int r)
{
#ifdef _DEBUG
    printf("sum - %d %d %d | %d %d\n", cur_elm, data[cur_elm].left, data[cur_elm].right, l, r);
#endif
    if(data[cur_elm].left == l && data[cur_elm].right == r)
    {
#ifdef _DEBUG
        printf("r = %llu\n", data[cur_elm]);
#endif
        return data[cur_elm].sum;
    }


    ull result = 0;

    if(l < data[cur_elm*2].right)
    {
        result += sum(data, cur_elm*2, MAX(l, data[cur_elm*2].left), MIN(r, data[cur_elm*2].right));
    }
    if (r > data[cur_elm*2 + 1].left)
    {
        result += sum(data, cur_elm*2 + 1, MAX(l, data[cur_elm*2 + 1].left), MIN(r, data[cur_elm*2 + 1].right));
    }

    return result;
}

int main(void)
{
    int n, m;
    scanf("%d %d\n", &n, &m);

    int rs;
    for(rs = 1; rs < n; rs*=2);

    Element *data = malloc(rs * 2 * sizeof(Element));
    if(data == NULL)
    {
        printf("Not memory\n");
        return 1;
    }
    for (int i = 0; i < 2*rs; i++)
    {
        data[i].sum = 0;
        data[i].left = i - rs;
        data[i].right = i - rs + 1;
    }

    for (int i = 0; i < n; i++)
    {
        scanf("%llu", &data[rs + i].sum);
    }


    for(int i = rs - 1; i > 0; i--)
    {
        data[i].sum = data[i*2].sum + data[i*2 + 1].sum;
        data[i].left = data[i*2].left;
        data[i].right = data[i*2+1].right;
    }


    print(data, 2*rs);

    for(int i = 0; i < m; i++)
    {
        int c;
        scanf("%d", &c);
        switch (c) {
            case 1:
            {
                int ind, v;
                scanf("%d%d", &ind, &v);

                data[rs + ind].sum = v;

                for(int j = (rs+ind)/2; j > 0; j/=2)
                    data[j].sum = data[j*2].sum + data[j*2 + 1].sum;

                break;
            }
            case 2:
            {
                int l, r;
                scanf("%d%d", &l, &r);

                printf("%llu\n", sum(data, 1, l, r));

                break;
            }
            default:
            {
                printf("C is not 1 or 2\n");
                free(data);
                return 2;
            }
        }
        print(data, 2*rs);
    }

    free(data);
    return 0;
}