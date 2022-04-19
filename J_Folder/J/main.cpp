#include <cstdio>

//#define _DEBUG
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

class Tree
{
public:
    typedef enum op_type_t
    {
        NON,
        ADD_F,
    } OP_Type;
private:
#define TREE_SP "lld"
#define NEU_TYPE_VAL 0x0
#define NEU_VAL {NEU_TYPE_VAL, NEU_TYPE_VAL, 1}

    typedef long long tree_type;
    typedef struct value_t
    {
        tree_type sum;
        tree_type w_sum;
        tree_type sum_w;
    } Value;
    typedef struct assign_t
    {
        OP_Type type;
        tree_type val;
    } Assign;
    typedef struct element_t
    {
        Assign assign;
        Value value;
        size_t left;
        size_t right;
    } Element;
    static Value update(Value one, Value two, size_t len_one) {
        return Value{one.sum + two.sum,
                     one.w_sum + two.w_sum + (two.sum * ((tree_type)len_one)),
                     one.sum_w + two.sum_w + ((tree_type)len_one * (tree_type)len_one)};
    }
    static void operation(Element *el, Assign assign)
    {
        switch (assign.type)
        {
            case OP_Type{ADD_F}:
            {
#ifdef _DEBUG
                printf("l=%zu r=%zu ADD_F\n", el->left, el->right);
#endif
                el->value.sum += assign.val * (tree_type)(el->right - el->left);
                el->value.w_sum += assign.val * el->value.sum_w;
                break;
            }
            case OP_Type{NON}:{return;}
        }
        el->assign.type = assign.type;
        el->assign.val += assign.val;
    }
    void ini()
    {
        for(size_t i = la; --i; )
        {
            data[i].value = update(data[lel(i)].value, data[rel(i)].value, data[lel(i)].right - data[lel(i)].left);
            data[i].assign = Assign {OP_Type {NON}};

            data[i].left    = data[lel(i)].left;
            data[i].right   = data[rel(i)].right;
        }
    }

public:
    explicit Tree(size_t n)
    {
        for(la = 1; la < n; la*=2);
        data = new Element [rl = 2*la];
        for (size_t i = la+n; i < rl; i++)
        {
            data[i].assign.type = OP_Type{NON};
            data[i].value = NEU_VAL;
            data[i].left = i - la;
            data[i].right = i - la + 1;
        }
        this->n = n;
        current_add = 0;
    }
    void ini_next(tree_type val)
    {
        if(current_add >= n)
            return;

        data[la + current_add].assign    = Assign {NON, NEU_TYPE_VAL};
        data[la + current_add].value     = Value {val, val, 0x1};
        data[la + current_add].left      = current_add;
        data[la + current_add].right     = current_add+1;

        if(++current_add == n)
            ini();
    }
    ~Tree()
    {
        delete[] data;
    }
private:
    size_t rl;
    size_t la;
    Element *data;
    size_t n;
    size_t current_add;

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
        Element *leftt_node = &data[lel(el)];
        Element *right_node = &data[rel(el)];
#ifdef _DEBUG
        printf("down %zu\n", el);
#endif
        operation(leftt_node, cur_node->assign);
        operation(right_node, cur_node->assign);
        cur_node->assign = Assign {OP_Type {NON}};
    }

    void assign_rq(size_t el, size_t l, size_t r, Assign assign)
    {
        down(el);

        if(data[el].left == l && data[el].right == r)
        {
#ifdef _DEBUG
            printf("assign_rq - %zu\n", el);
#endif
            operation(&data[el], assign);
            return;
        }

        if(l < data[lel(el)].right)
        {
            assign_rq(lel(el), MAX(l, data[lel(el)].left), MIN(r, data[lel(el)].right), assign);
        }
        if (r > data[rel(el)].left)
        {
            assign_rq(rel(el), MAX(l, data[rel(el)].left), MIN(r, data[rel(el)].right), assign);
        }

        data[el].value = update(data[lel(el)].value, data[rel(el)].value, data[lel(el)].right - data[lel(el)].left);
    }

    tree_type w_sum_rq(size_t el, size_t l, size_t r, tree_type w)
    {
        down(el);

        if(data[el].left == l && data[el].right == r)
        {
#ifdef _DEBUG
            printf("w_sum_rq - %zu\n", el);
#endif
            return data[el].value.w_sum + data[el].value.sum * (w - 1);
        }

        tree_type result = 0;

        if(l < data[lel(el)].right)
        {
            result += w_sum_rq(lel(el), MAX(l, data[lel(el)].left), MIN(r, data[lel(el)].right), w);
            w += (tree_type) (MIN(r, data[lel(el)].right) - MAX(l, data[lel(el)].left));
        }
        if (r > data[rel(el)].left)
        {
            result += w_sum_rq(rel(el), MAX(l, data[rel(el)].left), MIN(r, data[rel(el)].right), w);
        }

        data[el].value = update(data[lel(el)].value, data[rel(el)].value, data[lel(el)].right - data[lel(el)].left);

        return result;
    }

    void print_val(size_t i)
    {
        printf("%4" TREE_SP " ", data[i].value.sum);
        printf("%4" TREE_SP " ", data[i].value.w_sum);
        printf("%4" TREE_SP " ", data[i].value.sum_w);
    }

    static const char *type_assign_to_string(OP_Type op_type)
    {
        switch (op_type) {
            case OP_Type{ADD_F}: return "ADD_F";
            case OP_Type{NON}  : return "NON";
        }
        return "";
    }

    void print_assign(size_t i)
    {
        printf(" %5s %4" TREE_SP, type_assign_to_string(data[i].assign.type), data[i].assign.val);
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
            print_assign(i);
            printf(" = %2zu %2zu|\n", data[i].left, data[i].right);
        }

        for (; i < rl; i++)
        {
            printf("|%2zu", i);
            print_val(i);
            //print_assign(i);
            printf(" = %2zu %2zu|\n", data[i].left, data[i].right);
        }
        printf("=============================================\n");
#endif
    }

    void add_assign(size_t l, size_t r, tree_type v)
    {
        return assign_rq(1, l, r, Assign{ADD_F, v});
    }
    tree_type w_sum(size_t l, size_t r)
    {
        return w_sum_rq(1, l, r, 1);
    }
};

class Test_Tree
{
private:
    typedef long long tree_type;
    size_t n;
    size_t current_add;
    tree_type *data;
public:
    explicit Test_Tree(size_t n)
    {
        this->n = n;
        data = new tree_type [n];
        current_add = 0;
    }
    void ini_next(tree_type val)
    {
        if(current_add >= n)
            return;

        data[current_add++] = val;
    }
    void add_assign(size_t l, size_t r, tree_type v)
    {
        while(l < r)
            data[l++] += v;
    }
    tree_type w_sum(size_t l, size_t r)
    {
        tree_type result = 0;
        for(tree_type i = 1; l < r; i++)
            result += i*data[l++];
        return result;
    }
};

#include <cstdlib>
#define TEST
#define TEST_LEN_ARR (10)
#define TEST_OP_N (5)
#define TEST_N (100000)
#define TEST_MAX_VAL (100)

int main()
{
#ifdef TEST
    long long arr[TEST_LEN_ARR];
    size_t t[TEST_OP_N];
    long long lf[TEST_OP_N];
    long long rg[TEST_OP_N];
    long long val[TEST_OP_N];

    for (size_t length = 1; length < TEST_LEN_ARR; length++)
    {
        printf("    test len %d\n", length);
        for(size_t n_attempt = 0; n_attempt < TEST_N; n_attempt++)
        {
            for(int c_j = 0; c_j < length; c_j++)
            {
                arr[c_j] = rand() % (TEST_MAX_VAL + 1);
            }

            size_t n = length;
            Tree        norm_tree(n);
            Test_Tree   test_tree(n);
            for(size_t i = 0; i < n; i++)
            {
                norm_tree.ini_next(arr[i]);
                test_tree.ini_next(arr[i]);
            }


            size_t m = (rand() % TEST_OP_N) + 1;


            for(size_t i = 0; i < m; i++)
            {
                size_t type = (rand() %2) + 1;
                t[i] = type;
                size_t l = (rand() % length) + 1, r = (rand() % length) + 1;
                switch (type) {
                    case 1:
                    {
                        long long v = rand() % (TEST_MAX_VAL + 1) * (rand() % 2 ? 1 : -1);
                        lf[i] = MIN(l, r);
                        rg[i] = MAX(l, r);
                        val[i] = v;
                        norm_tree.add_assign(MIN(l, r) - 1, MAX(l, r), v);
                        test_tree.add_assign(MIN(l, r) - 1, MAX(l, r), v);
                        break;
                    }
                    case 2:
                    {
                        lf[i] = MIN(l, r);
                        rg[i] = MAX(l, r);
                        val[i] = 0;

                        long long norm_tree_res = norm_tree.w_sum(MIN(l, r) - 1, MAX(l, r));
                        long long test_tree_res = test_tree.w_sum(MIN(l, r) - 1, MAX(l, r));
                        if(norm_tree_res != test_tree_res)
                        {
                            printf("Error test:\n");
                            printf("%zu %zu\n", n, m);
                            for(size_t j = 0; j < length; j++)
                                printf("%lld ", arr[j]);
                            printf("\n");
                            for(size_t j = 0; j < i; j++)
                            {
                                printf("%zu %zu %zu", t[j], lf[j], rg[j]);
                                if(t[j] == 1)
                                    printf(" %lld", val[j]);
                                printf("\n");
                            }
                            printf("norm tree=%lld, correct=%lld", norm_tree_res, test_tree_res);
                            return 1;
                        }
                        break;
                    }
                    default: printf("Error type %zu\n", type); return -1;
                }
            }
        }

    }
    printf("end tests\n");
#else
    size_t n, m;
    scanf("%zu %zu", &n, &m);
    Tree tree(n);

    for(size_t i = n; i--;)
    {
        long long v;
        scanf("%lld", &v);
        tree.ini_next(v);
    }
    tree.print();

    for(size_t i = m; i--;)
    {
        size_t type, l, r;
        scanf("%zu %zu %zu", &type, &l, &r);
        l--;
        switch (type) {
            case 1:
            {
                long long v;
                scanf("%lld", &v);
                tree.add_assign(l, r, v);
                break;
            }
            case 2:
            {
                printf("%lld\n", tree.w_sum(l, r));
                break;
            }
            default: return -1;
        }
        tree.print();
    }
#endif

    return 0;
}