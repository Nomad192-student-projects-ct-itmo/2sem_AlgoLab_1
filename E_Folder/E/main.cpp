#include <cstdio>

//#define _DEBUG
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

class Tree
{
private:
    typedef unsigned long long type_tree;
#define TREE_SP "llu"
#define MIN_VAL (0x0)

public:
    explicit Tree(size_t n)
    {
        for(la = 1; la < n; la*=2);
        data = new Element [rl = 2*la];
        for (size_t i = 0; i < rl; i++)
        {
            data[i].max = MIN_VAL;
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
    typedef struct element_t
    {
        type_tree max;
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
            data[i].max = MAX(data[i*2].max, data[i*2 + 1].max);

            data[i].left = data[i*2].left;
            data[i].right = data[i*2+1].right;
        }
    }
    size_t rq_search(size_t el, type_tree x, size_t l)
    {
        //printf("rq_s %zu\n", el);
        if(el >= la)
        {
            if(data[el].left < l)
            {
                return n;
            }
            else
                return el - la;
        }

        if(l < data[el*2 + 1].left)
        {
            size_t res;
            if(data[el*2].max >= x)
            {
                res = rq_search(el*2, x, l);
                if(res < n)
                    return res;
            }
            if(data[el*2 + 1].max >= x)
                return rq_search(el*2 + 1, x, l);
            return n;
        }
        else
        {
            if(data[el*2 + 1].max >= x)
                return rq_search(el*2 + 1, x, l);
            return n;
        }
    }

public:
    void print()
    {
#ifdef _DEBUG
        printf("\n=============================================\n");
    for (size_t i = 1; i < rl; i++)
        printf("|%zu %" TREE_SP " = %zu %zu|\n", i, data[i].max, data[i].left, data[i].right);
    printf("=============================================\n");
#endif
    }
    void add(type_tree v)
    {
        if(current_add >= n)
        {
            printf("Error: tree is full\n");
            return;
        }

        data[la + current_add++].max = v;

        if(current_add == n)
            ini();
    }
    void set(size_t ind, type_tree v)
    {
        if(current_add < n)
        {
            printf("Error: tree is not full\n");
            return;
        }
        data[la + ind].max = v;

        for (size_t j = (la + ind) / 2; j > 0; j /= 2)
        {
            data[j].max = MAX(data[j*2].max, data[j*2 + 1].max);
        }
    }
    size_t search(type_tree x, size_t l)
    {
        if(current_add < n)
        {
            printf("Error: tree is not full\n");
            return n;
        }
        if(data[1].max >= x)
            return rq_search(1, x, l);
        return n;
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
            case 1: {
                size_t ind;
                unsigned long long v;
                scanf("%zu%llu", &ind, &v);
                tree.set(ind, v);
                tree.print();
                break;
            }
            case 2: {
                unsigned long long x;
                size_t l;
                scanf("%llu%zu", &x, &l);
                size_t res = tree.search(x, l);
                if(res >= n)
                {
                    printf("-1\n");
                }
                else
                {
                    printf("%zu\n", res);
                }

                break;
            }
            default: {
                printf("C is not 1 or 2\n");
                return 2;
            }
        }
    }

    return 0;
}