#include <cstdio>

//#define _DEBUG
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ADD(x, y) ((x) + (y))

class Tree
{
public:
    typedef enum op_type_t
    {
        NON,
        SET_B,
        SET_W,
    } OP_Type;
private:
#define TREE_SP "u"
#define NEU_VAL {0x0, 0x0, 0x0, 0x0, Dot_Type {N}, Dot_Type {N}}
#define NEU_TYPE_VAL 0u
    typedef unsigned int type_tree;
    typedef enum dot_type_t
    {
        B,
        W,
        N
    } Dot_Type;
    typedef struct value_t
    {
        type_tree b_len;
        type_tree w_len;
        type_tree b_n;
        type_tree w_n;
        Dot_Type l_td;
        Dot_Type r_td;
    } Value;
    typedef struct assign_t
    {
        OP_Type type;
    } Assign;
    typedef struct element_t
    {
        Assign assign;
        Value value;
        size_t left;
        size_t right;
    } Element;
    static Value update(Value one, Value two) {
        return {ADD(one.b_len, two.b_len),
                ADD(one.w_len, two.w_len),
                one.r_td == Dot_Type {B} && one.r_td == two.l_td ? ADD(one.b_n, two.b_n) - 1 : ADD(one.b_n, two.b_n),
                one.r_td == Dot_Type {W} && one.r_td == two.l_td ? ADD(one.w_n, two.w_n) - 1 : ADD(one.w_n, two.w_n),
                one.l_td,
                two.r_td};
    }
    static void operation(Element *el, OP_Type op_type)
    {
        type_tree len = el->right - el->left;
        type_tree n = 1u;
        type_tree other = NEU_TYPE_VAL;

        switch (op_type)
        {
            case OP_Type{SET_B}:
            {
#ifdef _DEBUG
                printf("l=%zu r=%zu B\n", el->left, el->right);
#endif
                el->value.b_len = len;
                el->value.w_len = other;
                el->value.b_n = n;
                el->value.w_n = other;
                el->value.r_td = Dot_Type {B};
                el->value.l_td = Dot_Type {B};
                break;
            }
            case OP_Type{SET_W}:
            {
#ifdef _DEBUG
                printf("l=%zu r=%zu W\n", el->left, el->right);
#endif
                el->value.b_len = other;
                el->value.w_len = len;
                el->value.b_n = other;
                el->value.w_n = n;
                el->value.r_td = Dot_Type {W};
                el->value.l_td = Dot_Type {W};
                break;
            }
            case OP_Type{NON}:{return;}
        }
        el->assign = {op_type};
    }


public:
    explicit Tree(size_t n)
    {
        for(la = 1; la < n; la*=2);
        data = new Element [rl = 2*la];
        for (size_t i = 0; i < rl; i++)
        {
            data[i].assign.type = OP_Type{NON};
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
            cur_node->assign = Assign {OP_Type {NON}};
            return;
        }
        Element *left__node = &data[lel(el)];
        Element *right_node = &data[rel(el)];
#ifdef _DEBUG
        printf("down %zu\n", el);
#endif
        operation(left__node, cur_node->assign.type);
        operation(right_node, cur_node->assign.type);
        cur_node->assign = Assign {OP_Type {NON}};
    }

    void set_assign_rq(size_t el, size_t l, size_t r, OP_Type op_type)
    {
        down(el);

        if(data[el].left == l && data[el].right == r)
        {
#ifdef _DEBUG
            printf("set - %zu\n", el);
#endif
            operation(&data[el], op_type);
            return;
        }

        if(l < data[lel(el)].right)
        {
            set_assign_rq(lel(el), MAX(l, data[lel(el)].left), MIN(r, data[lel(el)].right), op_type);
        }
        if (r > data[rel(el)].left)
        {
            set_assign_rq(rel(el), MAX(l, data[rel(el)].left), MIN(r, data[rel(el)].right), op_type);
        }

        data[el].value = update(data[lel(el)].value, data[rel(el)].value);
    }

    static char dot_type_to_char(Dot_Type dot_type)
    {
        switch (dot_type) {
            case Dot_Type{B}: return 'B';
            case Dot_Type{W}: return 'W';
            case Dot_Type{N}: return 'N';
        }
        return 0;
    }

    void print_val(size_t i)
    {
        printf("%3" TREE_SP " ", data[i].value.b_len);
        printf("%3" TREE_SP " ", data[i].value.w_len);
        printf("%3" TREE_SP " ", data[i].value.b_n);
        printf("%3" TREE_SP " ", data[i].value.w_n);
        printf("%3c ", dot_type_to_char(data[i].value.l_td));
        printf("%3c" , dot_type_to_char(data[i].value.r_td));
    }

    static const char * assign_to_string(Assign assign)
    {
        switch (assign.type) {
            case OP_Type{SET_B}: return "SET_B";
            case OP_Type{SET_W}: return "SET_W";
            case OP_Type{NON}  : return "NON";
        }
        return "";
    }

public:
    void print()
    {
#ifdef _DEBUG
        printf("\n=============================================\n");
        size_t i;
        for (i = 1; i < la; i++)
        {
            printf("|%2zu", i);
            print_val(i);
            printf(" %5s", assign_to_string(data[i].assign));
            printf(" = %2zu %2zu|\n", data[i].left, data[i].right);
        }

        for (; i < rl; i++)
        {
            printf("|%2zu", i);
            print_val(i);
            //printf(" %5s", assign_to_string(data[i].assign));
            printf(" = %2zu %2zu|\n", data[i].left, data[i].right);
        }
        printf("=============================================\n");
#endif
    }
    void ini()
    {
        for(size_t i = la - 1; i > 0; i--)
        {
            data[i].value = update(data[lel(i)].value, data[rel(i)].value);
            data[i].assign = Assign {OP_Type {NON}};

            data[i].left    = data[lel(i)].left;
            data[i].right   = data[rel(i)].right;
        }
    }
    typedef struct n_b_result_t
    {
        type_tree n;
        type_tree length;
    } N_B_Result;

    N_B_Result n_b()
    {
        size_t el = 1;
        return N_B_Result {data[el].value.b_n, data[el].value.b_len};
    }

    void set_assign(char type, size_t l, size_t r)
    {
#ifdef _DEBUG
        printf("set %c %zu %zu", type, l, r);
#endif
        OP_Type op_type;
        switch (type) {
            case 'W': op_type = OP_Type {SET_W}; break;
            case 'B': op_type = OP_Type {SET_B}; break;
            default:{return;}
        }
#ifdef _DEBUG
        printf(" %5s\n", assign_to_string(Assign {op_type}));
#endif
        return set_assign_rq(1, l, r, op_type);
    }
};


int main()
{
    size_t n_dot = 1000000;
    size_t shift = n_dot/2;
    Tree tree(n_dot);
    tree.ini();
    tree.print();

    size_t n;
    scanf("%zu", &n);
    for(size_t i = n; i--;)
    {
        char type;
        int x, l;
        scanf("\n%c %d %u", &type, &x, &l);
        if(type != 'W' && type != 'B')
        {
            printf("Error type=\"%c\"\n", type);
            return 1;
        }
        unsigned int left = MIN(x, x + l) + shift;
        unsigned int right = MAX(x, x + l) + shift;
        tree.set_assign(type, left, right);
        Tree::N_B_Result n_b_result = tree.n_b();
        printf("%u %u\n", n_b_result.n, n_b_result.length);
        tree.print();
    }

    return 0;
}