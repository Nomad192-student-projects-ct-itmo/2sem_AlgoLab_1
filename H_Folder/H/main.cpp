#include <cstdio>

//#define _DEBUG
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

class Tree
{
private:
    typedef unsigned long long type_tree;
    typedef struct value_t
    {
        type_tree min;
        type_tree max;
    } Value;
    static Value update(Value one, Value two) {
        return {MIN(one.min, two.min), MAX(one.max, two.max)};
    }
#define TREE_SP "llu"
#define NEU_VAL {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}
#define NEU_TYPE_VAL 0xFFFFFFFFFFFFFFFF
//#define MAX_VAL (0xFFFFFFFFFFFFFFFF)
#define OP update

public:
    explicit Tree(size_t n)
    {
        for(la = 1; la < n; la*=2);
        data = new Element [rl = 2*la];
        for (size_t i = 0; i < rl; i++)
        {
            data[i].assign = Assign {0, Type {NON}};
            data[i].value = NEU_VAL;
            data[i].left = i - la;
            data[i].right = i - la + 1;
        }
        //this->n = n;
        //current_add = 0;
    }
    ~Tree()
    {
        delete[] data;
    }
private:
    typedef enum type_t
    {
        NON = 0,
        DEL = 1,
    } Type;
    typedef struct assign_t
    {
        type_tree val;
        Type type;
    } Assign;
    typedef struct element_t
    {
        Assign assign;
        Value value;
        size_t left;
        size_t right;
    } Element;

    size_t rl;
    size_t la;
    Element *data;
    //size_t n;
    //size_t current_add;

    static size_t lel(size_t el) {return el*2;}
    static size_t rel(size_t el) {return el*2 + 1;}

    void down (size_t el)
    {
        Element *cur_node = &data[el];
        if (el >= la)
        {
            cur_node->assign = Assign {0, Type {NON}};
            return;
        }
        Element *leftt_node = &data[lel(el)];
        Element *right_node = &data[rel(el)];


        switch (cur_node->assign.type) {
            case Type {DEL}:
            {
                leftt_node->value = NEU_VAL;
                right_node->value = NEU_VAL;
                leftt_node->assign = cur_node->assign;
                right_node->assign = cur_node->assign;
                cur_node->assign = Assign {0, Type {NON}};
                break;
            }
            default:{}
        }
    }

    void set_assign_rq(size_t el, size_t l, size_t r, type_tree v)
    {
        down(el);

        if(data[el].left == l && data[el].right == r)
        {
            //printf("set %zu\n", el);
            data[el].assign = Assign {0, Type {NON}};
            data[el].value = {v, v};
            return;
        }

        if(l < data[lel(el)].right)
        {
            set_assign_rq(lel(el), MAX(l, data[lel(el)].left), MIN(r, data[lel(el)].right), v);
        }
        if (r > data[rel(el)].left)
        {
            set_assign_rq(rel(el), MAX(l, data[rel(el)].left), MIN(r, data[rel(el)].right), v);
        }

        data[el].value = OP(data[lel(el)].value, data[rel(el)].value);
    }

    size_t del_assign_rq(size_t el, size_t l, size_t r, type_tree v)
    {
        down(el);
        if(data[el].left == l && data[el].right == r)
        {
            if(data[el].value.min > v)
                return 0;
            if(data[el].value.max <= v)
            {
                data[el].assign = Assign {0, Type {DEL}};
                data[el].value = NEU_VAL;
                return data[el].right - data[el].left;
            }
        }

        size_t res = 0;

        if(l < data[lel(el)].right)
        {
            res += del_assign_rq(lel(el), MAX(l, data[lel(el)].left), MIN(r, data[lel(el)].right), v);
        }
        if (r > data[rel(el)].left)
        {
            res += del_assign_rq(rel(el), MAX(l, data[rel(el)].left), MIN(r, data[rel(el)].right), v);
        }

        data[el].value = OP(data[lel(el)].value, data[rel(el)].value);
        return res;
    }

public:
    void print()
    {
#ifdef _DEBUG
        printf("\n=============================================\n");
        size_t i;
        for (i = 1; i < la; i++)
            printf("|%2zu %" TREE_SP " %" TREE_SP " t=%-2d v=%-2lld = %zu %zu|\n", i, data[i].value.min, data[i].value.max, data[i].assign.type, data[i].assign.val, data[i].left, data[i].right);
        for (; i < rl; i++)
            //printf("|%2zu %" TREE_SP " %" TREE_SP " t=%-2d v=%-2lld = %zu %zu|\n", i, data[i].value.min, data[i].value.max, data[i].assign.type, data[i].assign.val, data[i].left, data[i].right);
            printf("|%2zu %3" TREE_SP " %3" TREE_SP " = %zu %zu|\n", i, data[i].value.min, data[i].value.max, data[i].left, data[i].right);
    printf("=============================================\n");
#endif
    }
    void ini()
    {
        for(size_t i = la - 1; i > 0; i--)
        {
            data[i].value = NEU_VAL;//OP(data[lel(i)].VALUE, data[rel(i)].VALUE);
            data[i].assign = Assign {0, Type {NON}};

            data[i].left    = data[lel(i)].left;
            data[i].right   = data[rel(i)].right;
        }
    }
    /*void add(type_tree v)
    {
        if(current_add >= n)
        {
            printf("Error: tree is full\n");
            return;
        }

        data[la + current_add].fd.min = v;
        data[la + current_add].fd.asv = -1;

        if(++current_add == n)
            ini();
    }*/
    void set_assign(size_t l, size_t r, type_tree v)
    {
        return set_assign_rq(1, l, r, v);
    }
    size_t del_assign(size_t l, size_t r, type_tree v)
    {
        return del_assign_rq(1, l, r, v);
    }
};

int main()
{
    size_t n, m;
    scanf("%zu %zu", &n, &m);

    Tree tree(n);

    /*for (int i = 0; i < n; i++)
    {
        unsigned long long buffer;
        scanf("%llu", &buffer);
        tree.add(buffer);
    }*/

    tree.ini();
    tree.print();

    for(int i = 0; i < m; i++)
    {
        int c;
        scanf("%d", &c);
        switch (c) {
            case 1:
            {
                size_t it;
                unsigned long long h;
                scanf("%zu%llu", &it, &h);
                tree.set_assign(it, it+1, h);
                break;
            }
            case 2:
            {
                size_t l, r;
                unsigned long long p;
                scanf("%zu%zu%llu", &l, &r, &p);
                printf("%zu\n", tree.del_assign(l, r, p));
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