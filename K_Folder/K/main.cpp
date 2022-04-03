#include <cstdio>

//#define _DEBUG
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ADD(x, y) ((x) + (y))
#define SET(x, y) (y)

class Sparse_Table
{
private:
    typedef unsigned long st_type;
#define TREE_SP "lu"
#define NEU_VAL (0xFFFFFFFF)
#define OP MIN
#define VALUE min
#define WIDE 8;
#define PRINT_FORMAT "8lu"

public:
    explicit Sparse_Table(size_t n, st_type first, st_type f(st_type))
    {
        size_t la = 1;
        depth = 1;
        for(; la < n; la*=2, depth++);

        data = new Element*[depth];
        for (size_t i = 0; i < depth; i++)
        {
            data[i] = new Element[n];
        }
        this->n = n;
        ini(first, f);
    }
    ~Sparse_Table()
    {
        for(size_t i = 0; i < depth; i++)
            delete[] data[i];
        delete[] data;
    }
private:
    typedef struct element_t
    {
        st_type VALUE;
    } Element;

    size_t depth;
    Element **data;
    size_t n;

    void ini(st_type first, st_type f(st_type))
    {
        data[0][0].VALUE  = first;
        st_type prev = first;
        for (size_t j = 1; j < n; j++)
        {
            data[0][j].VALUE  = f(prev);
            prev = data[0][j].VALUE;
        }

        for (size_t i = 1, la = 1; i < depth; i++, la*=2)
            for (size_t j = 0; j < n; j++)
                data[i][j].VALUE = OP(data[i - 1][j].VALUE, data[i - 1][MIN(j + la, n)].VALUE);

    }

    void print_char_line(size_t wide, char symbol)
    {
        printf("\n");
        for(size_t k = 0; k < wide * n + n + 4; k++)
        printf("%c", symbol);
        printf("\n");
    }

    void print_data_line(size_t i)
    {
        printf("%2-zu = ", i);
        for (size_t j = 0; j < n; j++)
            printf("%-" PRINT_FORMAT " ", data[i][j].VALUE);
    }

public:
    void print()
    {
#ifdef _DEBUG
        size_t wide = WIDE;
        print_char_line(wide, '=');
        size_t i = 1;
        for (; i < depth; i++)
        {
            print_data_line(depth - i);
            print_char_line(wide, '-');
        }
        print_data_line(depth - i);
        print_char_line(wide, '=');
#endif
    }
    st_type min (size_t u, size_t v)
    {
        size_t cur_depth = 1;
        size_t depth_len = 2;
        for (; depth_len < (v - u + 1); depth_len*=2, cur_depth++);
        depth_len/=2; cur_depth--;
#ifdef _DEBUG
        printf("%zu %zu = %zu %zu\n", u, v, cur_depth, depth_len);
#endif
        return MIN(data[cur_depth][u-1].VALUE, data[cur_depth][v - depth_len].VALUE);
    }
};

unsigned long counting_next_element (unsigned long prev)
{
    return (23*prev + 21563) % 16714589;
}

void counting_next_request (size_t u_prev, size_t v_prev, unsigned long r_prev, size_t i, size_t n, size_t *u_next, size_t *v_next)
{
    *u_next = ((17*u_prev + 751 + r_prev + 2*i) % n) + 1;
    *v_next = ((13*v_prev + 593 + r_prev + 5*i) % n) + 1;
}

int main()
{
    size_t n, m;
    unsigned long first;
    scanf("%zu %zu %lu", &n, &m, &first);

    Sparse_Table sparse_table(n, first, counting_next_element);
    sparse_table.print();

    size_t u, v;
    scanf("%zu %zu", &u, &v);

    unsigned long end_result = sparse_table.min(MIN(u, v), MAX(u, v));
#ifdef _DEBAG
    printf("%zu %zu %lu\n", u, v, end_result);
#endif
    for(size_t i = 1; i < m; i++)
    {
        counting_next_request(u, v, end_result, i, n, &u, &v);
        end_result = sparse_table.min(MIN(u, v), MAX(u, v));
#ifdef _DEBAG
        printf("%zu %zu %lu\n", u, v, end_result);
#endif
    }
#ifndef _DEBAG
    printf("%zu %zu %lu\n", u, v, end_result);
#endif

    return 0;
}