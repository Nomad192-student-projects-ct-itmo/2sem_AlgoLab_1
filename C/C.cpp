#include <cstdio>

//#define _DEBUG
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

class Tree
{
private:
    typedef long long type_tree;
#define TREE_SP "lld"
#define NEU_VAL (0x0)

public:
    explicit Tree(size_t n)
    {
        for(la = 1; la < n; la*=2);
        data = new Element [rl = 2*la];
        for (size_t i = 0; i < rl; i++)
        {
            data[i].sum = NEU_VAL;
            data[i].pref = NEU_VAL;
            data[i].suff = NEU_VAL;
            data[i].ans = NEU_VAL;
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
        type_tree sum;
        type_tree pref;
        type_tree suff;
        type_tree ans;
        size_t left;
        size_t right;
    } Element;

    size_t rl;
    size_t la;
    Element *data;
    size_t n;
    size_t current_add;
    void calc_el(size_t i)
    {
        Element l = data[i*2];
        Element r = data[i*2 + 1];
        data[i].sum = l.sum + r.sum;
        data[i].pref = MAX(l.pref, l.sum + r.pref);
        data[i].suff = MAX(r.suff, r.sum + l.suff);
        data[i].ans = MAX(MAX(l.ans, r.ans), l.suff + r.pref);

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
        printf("|%zu %" TREE_SP " = %zu %zu|\n", i, data[i].sum, data[i].left, data[i].right);
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

        data[la + current_add].sum = v;
        data[la + current_add].pref = v;
        data[la + current_add].suff = v;
        data[la + (current_add++)].ans = v;

        if(current_add == n)
            ini();
    }
    type_tree max_sum()
    {
        return data[1].ans > 0 ? data[1].ans : 0;
    }

    void set(size_t ind, type_tree v)
    {
        if(current_add < n)
        {
            printf("Error: tree is not full\n");
            return;
        }
        data[la + ind].sum = v;
        data[la + ind].pref = v;
        data[la + ind].suff = v;
        data[la + ind].ans = v;

        for (size_t j = (la + ind) / 2; j > 0; j /= 2)
        {
            calc_el(j);
        }
    }
};

int main()
{
    size_t n, m;
    scanf("%zu %zu\n", &n, &m);

    Tree tree(n);

    for (int i = 0; i < n; i++)
    {
        long long buffer;
        scanf("%lld", &buffer);
        tree.add(buffer);
    }
    tree.print();
    printf("%lld\n", tree.max_sum());
    for(int i = 0; i < m; i++)
    {
        size_t ind;
        long long v;
        scanf("%zu%lld", &ind, &v);
        tree.set(ind, v);
        printf("%lld\n", tree.max_sum());
        tree.print();
    }

    return 0;
}