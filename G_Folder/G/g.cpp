#include <cstdio>

//#define _DEBUG
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ADD(x, y) ((x) + (y))

class Tree
{
private:
    typedef unsigned long long type_tree;
#define TREE_SP "llu"
#define MAX_VAL (0xffffffffffffffff)
#define OP ADD
#define VALUE sum

public:
    explicit Tree(size_t n)
    {
        for(la = 1; la < n; la*=2);
        data = new Element [rl = 2*la];
        for (size_t i = 0; i < rl; i++)
        {
            data[i].fd.asv = -1;
            data[i].fd.added = -1;
            data[i].fd.VALUE = 0;
            data[i].left = i - la;
            data[i].right = i - la + 1;
        }
        this->n = n;
        //current_add = 0;
    }
    ~Tree()
    {
        delete[] data;
    }
private: 
    typedef struct assign_t
    {
        unsigned char type;
        type_tree val;
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
    size_t n;
    //size_t current_add;

    void down (size_t el)
    {
        if (el >= la)
        {
            data[el].fd.asv = -1;
            data[el].fd.added = -1;
            return;
        }
        if(data[el].fd.asv != -1)
        {
            data[el*2].fd.asv =  data[el].fd.asv;
            data[el*2].fd.VALUE =  data[el].fd.asv * (data[el*2].right - data[el*2].left);
            data[el*2 + 1].fd.asv =  data[el].fd.asv;
            data[el*2 + 1].fd.VALUE =  data[el].fd.asv * (data[el*2 + 1].right - data[el*2 + 1].left);
            data[el].fd.asv = -1;
            return;
        }
        if(data[el].fd.added != -1)
        {
            data[el*2].fd.added =  data[el].fd.added;
            data[el*2].fd.VALUE +=  data[el].fd.added * (data[el*2].right - data[el*2].left);
            data[el*2 + 1].fd.added =  data[el].fd.added;
            data[el*2 + 1].fd.VALUE +=  data[el].fd.added * (data[el*2 + 1].right - data[el*2 + 1].left);
            data[el].fd.added = -1;
            return;
        }
    }

    type_tree sum_assign_rq(size_t el, size_t l, size_t r)
    {
        /*if(data[el].fd.asv != -1)
        {
            return data[el].fd.asv * (r - l);
        }*/

        down(el);


        if(data[el].left == l && data[el].right == r)
        {
            //printf("sum += %zu %llu\n", el, data[el].fd.sum);
            /*if(data[el].fd.asv != -1)
            {
                return data[el].fd.asv * (data[el].right - data[el].left);
            }*/
            return data[el].fd.VALUE;
        }

        type_tree result;
        if(l < data[el*2].right && r > data[el*2 + 1].left)
        {
            type_tree sum_left = sum_assign_rq(el*2, MAX(l, data[el*2].left), MIN(r, data[el*2].right));
            type_tree sum_right = sum_assign_rq(el*2 + 1, MAX(l, data[el*2 + 1].left), MIN(r, data[el*2 + 1].right));
            result = OP(sum_left, sum_right);
        }
        else if(l < data[el*2].right)
        {
            result = sum_assign_rq(el*2, MAX(l, data[el*2].left), MIN(r, data[el*2].right));
        }
        else //if (r > data[el*2 + 1].left)
        {
            result =  sum_assign_rq(el*2 + 1, MAX(l, data[el*2 + 1].left), MIN(r, data[el*2 + 1].right));
        }
        data[el].fd.VALUE = OP(data[el*2].fd.VALUE, data[el*2 + 1].fd.VALUE);
        return result;
    }

    void set_assign_rq(size_t el, size_t l, size_t r, type_tree v)
    {
        down(el);

        if(data[el].left == l && data[el].right == r)
        {
            data[el].fd.asv = (long long)v;
            data[el].fd.sum = v*(data[el].right - data[el].left);
            return;
        }

        

        /*if(v < data[el].fd.min)
        {
            data[el].fd.min = v;
        }*/

        if(l < data[el*2].right && r > data[el*2 + 1].left)
        {
            set_assign_rq(el*2, MAX(l, data[el*2].left), MIN(r, data[el*2].right), v);
            set_assign_rq(el*2 + 1, MAX(l, data[el*2 + 1].left), MIN(r, data[el*2 + 1].right), v);
        }
        else if(l < data[el*2].right)
        {
            set_assign_rq(el*2, MAX(l, data[el*2].left), MIN(r, data[el*2].right), v);
        }
        else //if (r > data[el*2 + 1].left)
        {
            set_assign_rq(el*2 + 1, MAX(l, data[el*2 + 1].left), MIN(r, data[el*2 + 1].right), v);
        }

        data[el].fd.VALUE = OP(data[el*2].fd.VALUE, data[el*2 + 1].fd.VALUE);
    }
    
    void add_assign_rq(size_t el, size_t l, size_t r, type_tree v)
    {
        down(el);
        if(data[el].left == l && data[el].right == r)
        {
            /*if(data[el].fd.asv != -1)
            {
                data[el].fd.sum = data[el].fd.asv*(data[el].right - data[el].left);
                data[el].fd.asv 
            }*/

            data[el].fd.added = (long long)v;
            data[el].fd.sum += v*(data[el].right - data[el].left);
            
            return;
        }

        /*if(v < data[el].fd.min)
        {
            data[el].fd.min = v;
        }*/

        if(l < data[el*2].right && r > data[el*2 + 1].left)
        {
            add_assign_rq(el*2, MAX(l, data[el*2].left), MIN(r, data[el*2].right), v);
            add_assign_rq(el*2 + 1, MAX(l, data[el*2 + 1].left), MIN(r, data[el*2 + 1].right), v);
        }
        else if(l < data[el*2].right)
        {
            add_assign_rq(el*2, MAX(l, data[el*2].left), MIN(r, data[el*2].right), v);
        }
        else //if (r > data[el*2 + 1].left)
        {
            add_assign_rq(el*2 + 1, MAX(l, data[el*2 + 1].left), MIN(r, data[el*2 + 1].right), v);
        }

        data[el].fd.VALUE = OP(data[el*2].fd.VALUE, data[el*2 + 1].fd.VALUE);
    }

public:
    void print()
    {
#ifdef _DEBUG
        printf("\n=============================================\n");
    for (size_t i = 1; i < rl; i++)
        printf("|%2zu %3" TREE_SP " %2lld %2lld = %zu %zu|\n", i, data[i].fd.VALUE, data[i].fd.asv, data[i].fd.added, data[i].left, data[i].right);
    printf("=============================================\n");
#endif
    }
    void ini()
    {
        for(size_t i = la - 1; i > 0; i--)
        {
            data[i].fd.VALUE = 0;//OP(data[i*2].fd.VALUE, data[i*2 + 1].fd.VALUE);
            data[i].fd.asv = -1;
            data[i].fd.added = -1;

            data[i].left = data[i*2].left;
            data[i].right = data[i*2+1].right;
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