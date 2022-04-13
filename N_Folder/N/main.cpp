/*#include <stdio.h>

typedef unsigned char byte;

typedef struct time_t
{
    byte h;
    byte m;
    byte s;
} Time;

typedef struct ticket_office_t
{
     Time stt;
     Time end;
} Tic_off;

void to_scan(Tic_off *tic_off)
{
    scanf("%hhu %hhu %hhu %hhu %hhu %hhu",
          &tic_off->stt.h, &tic_off->stt.m, &tic_off->stt.s,
          &tic_off->end.h, &tic_off->end.m, &tic_off->end.s);
}

void to_print(Tic_off tic_off)
{
    printf("%hhu %hhu %hhu %hhu %hhu %hhu\n",
          tic_off.stt.h, tic_off.stt.m, tic_off.stt.s,
          tic_off.end.h, tic_off.end.m, tic_off.end.s);
}

short eq(Time first, Time second)
{
    if (first.h > second.h)
        return 1;
    else if (first.h < second.h)
        return -1;
    else
    {
        if (first.m > second.m)
            return 1;
        else if (first.m < second.m)
            return -1;
        else
        {
            if (first.s > second.s)
                return 1;
            else if (first.s < second.s)
                return -1;
            else
                return 0;
        }
    }
}

Time eq_more(Time first, Time second)
{
    switch (eq(first, second)) {
        case 1:return first;
        default:return second;
    }
}
Time eq_less(Time first, Time second)
{
    switch (eq(first, second)) {
        case 1:return second;
        default:return first;
    }
}
int main() {
    size_t n;
    scanf("%zu", &n);
    Tic_off full = {{0,0,0}, {23, 59, 59}};
    while(n--)
    {
        Tic_off buf;
        to_scan(&buf);
        to_print(buf);

        switch (eq(buf.end, buf.stt)) {
            case 1:
            {
                full.stt = eq_more(full.stt, buf.stt);
                full.end = eq_less(full.end, buf.end);
                break;
            }
            case -1:
            {
                full.stt = eq_more(full.stt, buf.stt);
                full.end = eq_less(full.end, buf.end);
                break;
            }
            case 0: break;
            default:
            {
                return -1;
            }
        }

    }
    to_print(full);
    return 0;
}
*/

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

typedef unsigned char byte;

typedef struct my_time_t
{
    byte h;
    byte m;
    byte s;
} Time;

typedef struct ticket_office_t
{
    Time stt;
    Time end;
} Tic_off;

void to_scan(Tic_off *tic_off)
{
    scanf("%hhu %hhu %hhu %hhu %hhu %hhu",
          &tic_off->stt.h, &tic_off->stt.m, &tic_off->stt.s,
          &tic_off->end.h, &tic_off->end.m, &tic_off->end.s);
}

void to_print(Tic_off tic_off)
{
    printf("%hhu %hhu %hhu %hhu %hhu %hhu\n",
           tic_off.stt.h, tic_off.stt.m, tic_off.stt.s,
           tic_off.end.h, tic_off.end.m, tic_off.end.s);
}

short eq(Time first, Time second)
{
    if (first.h > second.h)
        return 1;
    else if (first.h < second.h)
        return -1;
    else
    {
        if (first.m > second.m)
            return 1;
        else if (first.m < second.m)
            return -1;
        else
        {
            if (first.s > second.s)
                return 1;
            else if (first.s < second.s)
                return -1;
            else
                return 0;
        }
    }
}

int main()
{
    size_t n_sec = 86400;
    Tree tree(n_sec);
    tree.ini();
    tree.print();

    size_t n;
    scanf("%zu", &n);
    for(size_t i = n; i--;)
    {
        Tic_off buf;
        to_scan(&buf);
        size_t start = buf.stt.h * 3600 + buf.stt.m * 60 + buf.stt.s;
        size_t end = buf.end.h * 3600 + buf.end.m * 60 + buf.end.s;

        //to_print(buf);

        switch (eq(buf.end, buf.stt)) {
            case 1:
            {
                tree.add_assign(start, end, 1);
                //printf("%zu %zu\n", start, end);
                break;
            }
            case -1:
            {
                tree.add_assign(start, n_sec, 1);
                //printf("%zu %zu", start, n_sec);
                tree.add_assign(0, end, 1);
                //printf("and %zu %zu\n", 0, end);
                break;
            }
            case 0: n--; break;
            default:
            {
                return -1;
            }
        }
    }

    size_t n_sec_full = 0;
    if(n != 0)
    {
        for(size_t i = 0; i < n_sec; i++)
        {
            unsigned long long res = tree.sum_assign(i, i+1);
            if(res == n)
                n_sec_full++;

        }
        printf("%zu", n_sec_full);
    }
    else
        printf("%zu", n_sec);

    return 0;
}