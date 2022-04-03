#include <cstdio>

//#define _DEBUG
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ADD(x, y) ((x) + (y))
#define SET(x, y) (y)

class Tree
{
private:
    typedef unsigned long long type_tree;
#define TREE_SP "llu"
#define NEU_VAL (0x0)
#define OP ADD
#define VALUE sum

public:
    explicit Tree(size_t n)
    {
        for(la = 1; la < n; la*=2);
        data = new Element [rl = 2*la];
        for (size_t i = 0; i < rl; i++)
        {
            data[i].assign.type = Type{NON};
            data[i].VALUE = NEU_VAL;
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
        NON,
        SET,
        ADD
    } Type;
    typedef struct assign_t
    {
        type_tree val;
        Type type;
    } Assign;
    typedef struct element_t
    {
        Assign assign;
        type_tree VALUE;
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
        Element *left__node = &data[lel(el)];
        Element *right_node = &data[rel(el)];


        switch (cur_node->assign.type) {
            case Type {SET}:
            {
                left__node->VALUE = SET(left__node->VALUE, cur_node->assign.val * (left__node->right - left__node->left));
                right_node->VALUE = SET(right_node->VALUE, cur_node->assign.val * (right_node->right - right_node->left));
                left__node->assign = cur_node->assign;
                right_node->assign = cur_node->assign;
                cur_node->assign = Assign {0, Type {NON}};
                break;
            }
            case Type {ADD}:
            {
                left__node->VALUE = ADD(left__node->VALUE, cur_node->assign.val * (left__node->right - left__node->left));
                right_node->VALUE = ADD(right_node->VALUE, cur_node->assign.val * (right_node->right - right_node->left));
                left__node->assign.val = ADD(left__node->assign.val, cur_node->assign.val);
                right_node->assign.val = ADD(right_node->assign.val, cur_node->assign.val);

                if (left__node->assign.type == Type {SET})
                    left__node->assign.type =  Type {SET};
                else
                    left__node->assign.type =  Type {ADD};

                if (right_node->assign.type == Type {SET})
                    right_node->assign.type =  Type {SET};
                else
                    right_node->assign.type =  Type {ADD};

                cur_node->assign = Assign {0, Type {NON}};
                break;
            }
            default:{}
        }
    }

    type_tree sum_assign_rq(size_t el, size_t l, size_t r)
    {
        down(el);
        if(data[el].left == l && data[el].right == r)
        {
            return data[el].VALUE;
        }

        type_tree result = NEU_VAL;
        if(l < data[lel(el)].right)
        {
            result = OP(result, sum_assign_rq(lel(el), MAX(l, data[lel(el)].left), MIN(r, data[lel(el)].right)));
        }
        if (r > data[rel(el)].left)
        {
            result = OP(result, sum_assign_rq(rel(el), MAX(l, data[rel(el)].left), MIN(r, data[rel(el)].right)));
        }
        //data[el].VALUE = OP(data[lel(el)].VALUE, data[rel(el)].VALUE);
        return result;
    }

    void set_assign_rq(size_t el, size_t l, size_t r, type_tree v)
    {
        down(el);

        if(data[el].left == l && data[el].right == r)
        {
            //printf("set %zu\n", el);
            data[el].assign = Assign {v, Type {SET}};
            data[el].VALUE = v*(data[el].right - data[el].left);
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

        data[el].VALUE = OP(data[lel(el)].VALUE, data[rel(el)].VALUE);
    }
    
    void add_assign_rq(size_t el, size_t l, size_t r, type_tree v)
    {
        down(el);
        if(data[el].left == l && data[el].right == r)
        {
            //printf("add %zu\n", el);
            data[el].assign = Assign {v, Type {ADD}};
            data[el].VALUE += v*(data[el].right - data[el].left);
            
            return;
        }

        if(l < data[lel(el)].right)
        {
            add_assign_rq(lel(el), MAX(l, data[lel(el)].left), MIN(r, data[lel(el)].right), v);
        }
        if (r > data[rel(el)].left)
        {
            add_assign_rq(rel(el), MAX(l, data[rel(el)].left), MIN(r, data[rel(el)].right), v);
        }

        data[el].VALUE = OP(data[lel(el)].VALUE, data[rel(el)].VALUE);
    }

public:
    void print()
    {
#ifdef _DEBUG
        printf("\n=============================================\n");
        size_t i;
        for (i = 1; i < la; i++)
            printf("|%2zu %3" TREE_SP " %2zu %2lld = %zu %zu|\n", i, data[i].VALUE, data[i].assign.type, data[i].assign.val, data[i].left, data[i].right);
        for (; i < rl; i++)
            printf("|%2zu %3" TREE_SP " = %zu %zu|\n", i, data[i].VALUE, data[i].left, data[i].right);
    printf("=============================================\n");
#endif
    }
    void ini()
    {
        for(size_t i = la - 1; i > 0; i--)
        {
            data[i].VALUE = NEU_VAL;//OP(data[lel(i)].VALUE, data[rel(i)].VALUE);
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
    type_tree sum_assign(size_t l, size_t r)
    {
        return sum_assign_rq(1, l, r);
    }
    void set_assign(size_t l, size_t r, type_tree v)
    {
        return set_assign_rq(1, l, r, v);
    }
    void add_assign(size_t l, size_t r, type_tree v)
    {
        return add_assign_rq(1, l, r, v);
    }
};

int main()
{
    size_t n, m;
    scanf("%zu %zu\n", &n, &m);

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
                size_t l, r;
                unsigned long long v;
                scanf("%zu%zu%llu", &l, &r, &v);
                tree.set_assign(l, r, v);
                break;
            }
            case 2:
            {
                size_t l, r;
                unsigned long long v;
                scanf("%zu%zu%llu", &l, &r, &v);
                tree.add_assign(l, r, v);
                break;
            }
            case 3:
            {
                size_t l, r;
                scanf("%zu%zu", &l, &r);
                printf("%llu\n", tree.sum_assign(l, r));
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