#include <cstdio>

//#define _DEBUG
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

class Tree
{
private:
    typedef unsigned long long type_tree;
#define TREE_SP "llu"
#define MAX_VAL (0xffffffffffffffff)

public:
    explicit Tree(size_t n)
    {
        for(la = 1; la < n; la*=2);
        data = new Element [rl = 2*la];
        for (size_t i = 0; i < rl; i++)
        {
            data[i].sum = 0;
            data[i].min = MAX_VAL;
            data[i].n_min = 1;
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
        long long assigned_value;
        type_tree sum;
        type_tree min;
        size_t n_min;
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
            data[i].min = MIN(data[i*2].min, data[i*2 + 1].min);

            if(data[i*2].min == data[i*2 + 1].min)
            {
                data[i].n_min = data[i*2].n_min + data[i*2 + 1].n_min;
            }
            else
            {
                data[i].n_min = data[i*2].min < data[i*2 + 1].min ? data[i*2].n_min : data[i*2 + 1].n_min;
            }

            data[i].left = data[i*2].left;
            data[i].right = data[i*2+1].right;
        }
    }
    type_tree sum_req(size_t cur_elm, size_t l, size_t r)
    {
#ifdef _DEBUG
        printf("sum - %zu %zu %zu | %zu %zu\n", cur_elm, data[cur_elm].left, data[cur_elm].right, l, r);
#endif
        if(data[cur_elm].left == l && data[cur_elm].right == r)
        {
#ifdef _DEBUG
            printf("r = %" TREE_SP "\n", data[cur_elm].sum);
#endif
            return data[cur_elm].sum;
        }


        type_tree result = 0;

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
    typedef struct min_answer
    {
        type_tree min;
        size_t n_min;
    } Min_Answer;
    Min_Answer min_req(size_t cur_elm, size_t l, size_t r)
    {
#ifdef _DEBUG
        printf("min - %zu %zu %zu | %zu %zu\n", cur_elm, data[cur_elm].left, data[cur_elm].right, l, r);
#endif
        if(data[cur_elm].left == l && data[cur_elm].right == r)
        {
#ifdef _DEBUG
            printf("r = %" TREE_SP " %zu\n", data[cur_elm].min, data[cur_elm].n_min);
#endif
            return Min_Answer{data[cur_elm].min, data[cur_elm].n_min};
        }


        Min_Answer result;

        if(l < data[cur_elm*2].right && r > data[cur_elm*2 + 1].left)
        {
            result = min_req(cur_elm*2, MAX(l, data[cur_elm*2].left), MIN(r, data[cur_elm*2].right));
            Min_Answer buffer = min_req(cur_elm*2 + 1, MAX(l, data[cur_elm*2 + 1].left), MIN(r, data[cur_elm*2 + 1].right));
            if(buffer.min == result.min)
                result = Min_Answer {result.min, result.n_min + buffer.n_min};
            else
                result = Min_Answer {MIN(result.min, buffer.min), result.min < buffer.min ? result.n_min : buffer.n_min};
        }
        else
        {
            if(l < data[cur_elm*2].right)
            {
                result = min_req(cur_elm*2, MAX(l, data[cur_elm*2].left), MIN(r, data[cur_elm*2].right));
            }
            if (r > data[cur_elm*2 + 1].left)
            {
                result = min_req(cur_elm*2 + 1, MAX(l, data[cur_elm*2 + 1].left), MIN(r, data[cur_elm*2 + 1].right));
            }
        }
        return result;
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
        data[la + (current_add++)].min = v;

        if(current_add == n)
            ini();
    }
    type_tree sum(size_t l, size_t r)
    {
        if(current_add < n)
        {
            printf("Error: tree is not full\n");
            return 0;
        }
        return sum_req(1, l, r);
    }
    void min(size_t l, size_t r, type_tree *min, size_t *n_min)
    {
        Min_Answer answer = min_req(1, l, r);
        *min = answer.min;
        *n_min = answer.n_min;
    }
    void set(size_t ind, type_tree v)
    {
        if(current_add < n)
        {
            printf("Error: tree is not full\n");
            return;
        }
        data[la + ind].sum = v;
        data[la + ind].min = v;

        for (size_t j = (la + ind) / 2; j > 0; j /= 2)
        {
            data[j].sum = data[j * 2].sum + data[j * 2 + 1].sum;
            data[j].min = MIN(data[j*2].min, data[j*2 + 1].min);

            if(data[j*2].min == data[j*2 + 1].min)
            {
                data[j].n_min = data[j*2].n_min + data[j*2 + 1].n_min;
            }
            else
            {
                data[j].n_min = data[j*2].min < data[j*2 + 1].min ? data[j*2].n_min : data[j*2 + 1].n_min;
            }
        }
    }
    void assign(size_t l, size_t r, type_tree v)
    {

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
                size_t ind;
                unsigned long long v;
                scanf("%zu%llu", &ind, &v);
                tree.set(ind, v);
                break;
            }
            case 2:
            {
                size_t l, r;
                scanf("%zu%zu", &l, &r);
                unsigned long long min;
                size_t n_min;
                tree.min(l, r, &min, &n_min);
                printf("%llu %zu\n", min, n_min);
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