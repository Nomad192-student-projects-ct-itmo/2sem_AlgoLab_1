#include <cstdio>
#include <vector>
#include <algorithm>

/*
5 5
4 15
2 9
3 13
1 7
5 20
 */

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
            data[i].sort = new std::vector <Sub_Element>;
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

    typedef struct sub_element_t
    {
        type_tree x;
        size_t i;
        friend bool operator < (const sub_element_t &fst, const sub_element_t &snd)
        {
            return (fst.x < snd.x);
        }
    } Sub_Element;
    /*typedef struct comp_t
    {
        bool operator() (const Sub_Element &fst, const Sub_Element &snd)
        {
            return (fst.x < snd.x);
        }
    } Comp;*/
    typedef struct element_t
    {
        std::vector <Sub_Element> *sort;
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

        data[i].left = data[i*2].left;
        data[i].right = data[i*2+1].right;



        std::merge(l.sort->begin(), l.sort->end(), r.sort->begin(), r.sort->end(), std::inserter(*data[i].sort, data[i].sort->begin()));

        /*
        size_t length = data[i].right - data[i].left;
        int c = 0, k = 0;
        for (size_t j = 0; j < length; j++)
        {
            if(k == length/2 || (c < length/2 && (l.sort[c].x > r.sort[k].x || (l.sort[c].x == r.sort[k].x && l.sort[c].i < r.sort[k].i))))
            {
                data[i].sort[j].x = l.sort[c].x;
                data[i].sort[j].i = l.sort[c].i;
                data[i].sort->insert({x, i});
                c++;
            }
            else
            {
                data[i].sort[j].x = r.sort[k].x;
                data[i].sort[j].i = r.sort[k].i;
                k++;
            }
        }*/
    }
    void ini()
    {
        for(size_t i = la - 1; i > 0; i--)
        {
            calc_el(i);
        }
    }

    long long rq_search_lower(size_t el, type_tree x, size_t l)
    {
        if(data[el].left == l)
        {
#ifdef _DEBUG
            printf("bl_0  |%zu - %zu %zu| l=%zu\n", el, data[el].left, data[el].right, l);
#endif
            auto result = std::lower_bound(data[el].sort->begin(), data[el].sort->end(), Sub_Element{x, 0});
            if(result == data[el].sort->end())
                return -1;
            /*if(result == data[el].sort->begin() && x < (*data[el].sort->begin()).x)
                return -2;*/
            if(x == (*result).x)
                return data[el].sort->begin() - result - 2;
            return result - data[el].sort->begin();
        }
        if(l < data[el*2 + 1].left)
        {
#ifdef _DEBUG
            printf("bl_2  |%zu - %zu %zu| l=%zu\n", el, data[el].left, data[el].right, l);
#endif
            long long res1 = rq_search_lower(el*2, x, l);
            long long res2 = rq_search_lower(el*2 + 1, x, data[el*2 + 1].left);
            if(res1 == -1)
            {
                return res2;
            }
            if(res2 == -1)
            {
                return res1;
            }

            return MIN(res1, res2);
        }
        else
        {
#ifdef _DEBUG
            printf("bl_1  |%zu - %zu %zu| l=%zu\n", el, data[el].left, data[el].right, l);
#endif
            return rq_search_lower(el*2 + 1, x, l);
        }
    }
    long long rq_search_upper(size_t el, type_tree x, size_t l)
    {
        if(data[el].left == l)
        {
#ifdef _DEBUG
            printf("bl_0  |%zu - %zu %zu| l=%zu\n", el, data[el].left, data[el].right, l);
#endif
            auto result = std::upper_bound(data[el].sort->begin(), data[el].sort->end(), Sub_Element{x, 0});
            if(result == data[el].sort->end())
                return -1;
            /*if(result == data[el].sort->begin() && x < (*data[el].sort->begin()).x)
                return -2;*/
            if(x == (*result).x)
                return data[el].sort->begin() - result - 2;
            return result - data[el].sort->begin();
        }
        if(l < data[el*2 + 1].left)
        {
#ifdef _DEBUG
            printf("bl_2  |%zu - %zu %zu| l=%zu\n", el, data[el].left, data[el].right, l);
#endif
            long long res1 = rq_search_upper(el*2, x, l);
            long long res2 = rq_search_upper(el*2 + 1, x, data[el*2 + 1].left);
            if(res1 == -1)
            {
                return res2;
            }
            if(res2 == -1)
            {
                return res1;
            }

            return MIN(res1, res2);
        }
        else
        {
#ifdef _DEBUG
            printf("bl_1  |%zu - %zu %zu| l=%zu\n", el, data[el].left, data[el].right, l);
#endif
            return rq_search_upper(el*2 + 1, x, l);
        }
    }

public:
    void print()
    {
#ifdef _DEBUG
        printf("\n=============================================\n");
        for (size_t i = 1; i < rl; i++)
        {
            printf("i=%-3zu: l=%-3zu r=%-3zu\t", i, data[i].left, data[i].right);
            /*for (size_t j = 0; j < data[i].right - data[i].left; j++)
            {
                printf("|%-2" TREE_SP " ", data[i].sort[j].x);
            }*/
            for (Sub_Element j : *data[i].sort)
            {
                printf("|%-2" TREE_SP " ", j.x);
            }
            printf("\n\t\t\t");
            for (Sub_Element j : *data[i].sort)
            {
                printf("|%-2" TREE_SP " ", j.i);
            }
            printf("\n---------------------------------------------\n");
        }
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

        Sub_Element b = {v,current_add};

        data[la + current_add].sort->push_back(b);

        current_add ++;

        if(current_add == n)
            ini();
    }
    /*long long bs(Sub_Element *ptr, unsigned long x, long long l, long long r)
    {
        while (l <= r) {
            long long m = (l + r) / 2;
            if (ptr[m + 1].x >= x) {
                l = m + 1;
            } else if (ptr[m].x >= x) {
                return m + 1;
            } else {
                r = m - 1;
            }
        }
        return l;
    }*/

    /*void set(size_t i, type_tree v)
    {
        if(current_add < n)
        {
            printf("Error: tree is not full\n");
            return;
        }
        type_tree cur = (*(data[la + i].sort->begin())).x;
#ifdef _DEBUG
        printf("set - %" TREE_SP "\n", cur);
#endif

        data[la + i].sort->erase(data[la + i].sort->begin());
        data[la + i].sort->insert(Sub_Element {v, i});

        for (size_t j = (la + i) / 2; j > 0; j /= 2)
        {
            data[j].sort->erase(data[j].sort->find(Sub_Element {cur, i}));
            data[j].sort->insert(Sub_Element {v, i});
        }

    }*/

    long long search_lower(type_tree x, size_t l)
    {
        if(current_add < n)
        {
            printf("Error: tree is not full\n");
            return -1;
        }
        return rq_search_lower(1, x, l);
    }
    long long search_upper(type_tree x, size_t l)
    {
        if(current_add < n)
        {
            printf("Error: tree is not full\n");
            return -1;
        }
        return rq_search_upper(1, x, l);
    }
};

int main()
{
    size_t n, m;
    scanf("%zu %zu\n", &n, &m);

    Tree tree_start(n);
    Tree tree_end(n);

    for (size_t i = 0; i < n; i++)
    {
        long long st, en;
        scanf("%lld %lld", &st, &en);
        tree_start.add(MIN(st,en));
        tree_end.add(MAX(st,en));
    }
    tree_start.print();
    tree_end.print();

    for (size_t i = 0; i < m; i++)
    {
        long long buf;
        scanf("%lld", &buf);
        long long st = tree_start.search_upper(buf, 0), en = tree_end.search_lower(buf, 0);
        if(st == -1) st = n;
        else if(st < 0) st = st*-1 - 1;

        if(en == -1) en = n;
        else if(en < 0) en = en*-1 - 2;
        //printf("%lld %lld %llu\n", st, en, st - en);
        printf("%llu ", st - en);
    }

    /*
    for (size_t i = 0; i < m; i++)
    {
        int c;
        scanf("%d", &c);
        switch (c) {
            case 1:
            {
                size_t ind;
                long long v;
                scanf("%zu%lld", &ind, &v);
                tree.set(ind, v);
                tree.print();
                break;
            }
            case 2:
            {
                long long x;
                size_t l;
                scanf("%lld%zu", &x, &l);
                printf("%lld\n", tree.search(x, l));

                break;
            }
            default:
            {
                printf("C is not 1 or 2\n");
                return 2;
            }
        }
    }
*/

    return 0;
}