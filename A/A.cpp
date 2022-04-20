#include <cstdio>

//#define _DEBUG
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

class Tree
{
public:
    explicit Tree(size_t n)
    {
        for(la = 1; la < n; la*=2);
        data = new Element [rl = 2*la];
        for (size_t i = 0; i < rl; i++)
        {
            data[i].sum = 0;
            data[i].left = i - la;
            data[i].right = i - la + 1;
        }
        this->n = n;
        current_add = 0;
    }
    ~Tree()
    {
        delete[] data;
    }
private:
    typedef unsigned long long ull;
    typedef struct element_t
    {
        ull sum;
        ull min;
        size_t left;
        size_t right;
    } Element;
    size_t rl;
    size_t la;
    Element *data;
    size_t n;
    size_t current_add;
    void ini()
    {
        for(size_t i = la - 1; i > 0; i--)
        {
            data[i].sum = data[i*2].sum + data[i*2 + 1].sum;
            data[i].left = data[i*2].left;
            data[i].right = data[i*2+1].right;
        }
    }
    ull sum_req(size_t cur_elm, size_t l, size_t r)
    {
#ifdef _DEBUG
        printf("sum - %zu %zu %zu | %zu %zu\n", cur_elm, data[cur_elm].left, data[cur_elm].right, l, r);
#endif
        if(data[cur_elm].left == l && data[cur_elm].right == r)
        {
#ifdef _DEBUG
            printf("r = %llu\n", data[cur_elm].sum);
#endif
            return data[cur_elm].sum;
        }


        ull result = 0;

        if(l < data[cur_elm*2].right)
        {
            result += sum_req(cur_elm*2, MAX(l, data[cur_elm*2].left), MIN(r, data[cur_elm*2].right));
        }
        if (r > data[cur_elm*2 + 1].left)
        {
            result += sum_req(cur_elm*2 + 1, MAX(l, data[cur_elm*2 + 1].left), MIN(r, data[cur_elm*2 + 1].right));
        }

        return result;
    }

public:
    void print()
    {
#ifdef _DEBUG
        printf("\n=============================================\n");
    for (size_t i = 1; i < rl; i++)
        printf("|%zu %llu = %zu %zu|\n", i, data[i].sum, data[i].left, data[i].right);
    printf("=============================================\n");
#endif
    }
    void add(ull v)
    {
        if(current_add >= n)
        {
            printf("Error: tree is full\n");
            return;
        }

        data[la + (current_add++)].sum = v;

        if(current_add == n)
            ini();
    }
    ull sum(size_t l, size_t r)
    {
        if(current_add < n)
        {
            printf("Error: tree is not full\n");
            return 0;
        }
        return sum_req(1, l, r);
    }
    void set(size_t ind, ull v)
    {
        if(current_add < n)
        {
            printf("Error: tree is not full\n");
            return;
        }
        data[la + ind].sum = v;

        for (size_t j = (la + ind) / 2; j > 0; j /= 2)
            data[j].sum = data[j * 2].sum + data[j * 2 + 1].sum;
    }
};

int main()
{
    size_t n, m;
    scanf("%zu %zu\n", &n, &m);

    Tree tree(n);

    for (int i = 0; i < n; i++)
    {
        unsigned long long buffer;
        scanf("%llu", &buffer);
        tree.add(buffer);
    }

    tree.print();

    for(int i = 0; i < m; i++)
    {
        int c;
        scanf("%d", &c);
        switch (c) {
            case 1:
            {
                int ind, v;
                scanf("%d%d", &ind, &v);
                tree.set(ind, v);
                break;
            }
            case 2:
            {
                int l, r;
                scanf("%d%d", &l, &r);
                printf("%llu\n", tree.sum(l, r));
                break;
            }
            default:
            {
                printf("C is not 1 or 2\n");
                return 2;
            }
        }
        tree.print();
    }

    return 0;
}