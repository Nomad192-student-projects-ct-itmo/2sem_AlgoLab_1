#include <cstdio>

//#define _DEBUG
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

class Tree
{
private:
    typedef bool type_tree;
#define TREE_SP "d"
#define NEU_VAL (0x0)
    typedef struct element_t
    {
        size_t n_unit;
        size_t left;
        size_t right;
    } Element;
    size_t rl;
    size_t la;
    Element *data;
    size_t n;
    size_t current_add;

public:
    explicit Tree(size_t n)
    {
        for(la = 1; la < n; la*=2);
        data = new Element [rl = 2*la];
        for (size_t i = 0; i < rl; i++)
        {
            data[i].n_unit = 0;
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
    void calc_el(size_t i)
    {
        Element l = data[i*2];
        Element r = data[i*2 + 1];
        data[i].n_unit = l.n_unit + r.n_unit;

        data[i].left = data[i*2].left;
        data[i].right = data[i*2+1].right;
    }
    void ini()
    {
        for(size_t i = la - 1; i > 0; i--)
        {
            calc_el(i);
        }
    }

public:
    void print()
    {
#ifdef _DEBUG
        printf("\n=============================================\n");
        for (size_t i = 1; i < rl; i++)
            printf("|%zu %zu = %zu %zu|\n", i, data[i].n_unit, data[i].left, data[i].right);
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

        data[la + (current_add++)].n_unit = v;

        if(current_add == n)
            ini();
    }

    void set(size_t ind)
    {
        if(current_add < n)
        {
            printf("Error: tree is not full\n");
            return;
        }
        data[la + ind].n_unit = !(data[la + ind].n_unit);

        for (size_t j = (la + ind) / 2; j > 0; j /= 2)
        {
            calc_el(j);
        }
    }

    size_t find(size_t k)
    {
        size_t ind = 1;
        while(ind < la)
        {
            if(data[ind*2].n_unit >= (k+1))
            {
                ind*=2;
            }
            else
            {
                k -= data[ind*2].n_unit;
                ind = ind*2 + 1;
            }
        }
        return ind - la;
    }
};

int main()
{
    size_t n, m;
    scanf("%zu %zu", &n, &m);

    Tree tree(n);

    for (size_t i = 0; i < n; i++)
    {
        int buffer;
        scanf("%d", &buffer);
        tree.add(buffer == 1);
    }
    tree.print();

    for(int i = 0; i < m; i++)
    {
        int c;
        scanf("%d", &c);
        switch (c) {
            case 1:
            {
                size_t ind;
                scanf("%zu", &ind);
                tree.set(ind);
                break;
            }
            case 2:
            {
                size_t k;
                scanf("%zu", &k);
                printf("%zu\n", tree.find(k));
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