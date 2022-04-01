#include <cstdio>

typedef unsigned int uint;
typedef unsigned long long int ull;

//define _DEBUG
#define NEUTRAL_ELEMENT (0)

class Sum_Tree
{
private:
    struct element
    {
        uint mean;
        size_t l;
        size_t r;
    };

    element *data;
    size_t size = 0;
    size_t real_size = 0;
    size_t ini_index = 0;

public:
    explicit Sum_Tree(size_t size)
    {
        this->size = size;
        calc_size();
#ifdef _DEBUG
        printf("size=%zu\n", this->size);
        printf("real size=%zu\n", this->real_size);
#endif
        data = new element[real_size];
    }
    void add(uint x)
    {
        if (ini_index >= size)
        {
            printf("The original tree array is fully initialized\n");
            return;
        }
        data[real_size/2 + ini_index].mean = x;
        data[real_size/2 + ini_index].l = ini_index;
        data[real_size/2 + ini_index].r = ini_index;
        ini_index++;
        if (ini_index == size)
        {
            recalculate();
        }
    }
    void set (size_t i, uint v)
    {
        size_t index = real_size/2 + i;
        uint prev = data[index].mean;
        data[index].mean = v;
        uint now  = data[index].mean;
        if (index%2==0)
            index--;
        while(index > 0)
        {
            index/=2;
#ifdef _DEBUG
            printf("i = %zu\n", index);
#endif
            data[index].mean -= prev;
            data[index].mean += now;
            if (index%2==0 && index != 0)
                index--;
        }
        data[0].mean -= prev;
        data[0].mean += now;
    }
    ull sum(size_t element, size_t l, size_t r)
    {
#ifdef _DEBUG
        printf("sum1 %zu %zu %zu\n", element, l, r);
        printf("sum2 %zu %zu\n", data[element].l, data[element].r);
        printf("sum3 %d %d %d\n\n",
               data[element].l == data[element].r,
               data[element].l == l,
               data[element].r == r);
#endif
        if(data[element].l == data[element].r || (data[element].l == l && data[element].r == r))
            return data[element].mean;

        ull result;
        if (r <= data[element*2+1].r)
        {
            result = sum(element*2+1, l, r);
        }
        else if (l >= data[element*2+2].l)
        {
            result = sum(element*2+2, l, r);
        }
        else
        {
            result = sum(element*2+1, l, data[element*2+1].r) + sum(element*2+2, data[element*2+2].l, r);
        }
        return result;
    }

    void print()
    {
#ifdef _DEBUG
        for(size_t i=0, j=1; i<real_size; i++)
        {
            if(i == (j-1))
            {
                printf("|");
                j*=2;
            }
            else
                printf(" ");

            printf("%u", data[i].mean);
        }
        printf("|\n");

        for(size_t i=0; i<real_size; i++)
        {
            printf("i=%zu, %zu %zu\n", i, data[i].l, data[i].r);
        }
#endif
    }
    ~Sum_Tree()
    {
        delete[] data;
    }

private:
    void calc_size()
    {
        for(real_size = 1; real_size < size; real_size*=2);
        real_size*=2;
        real_size--;
    }
    void recalculate()
    {
        while(ini_index < (real_size+1)/2)
        {
            data[real_size/2 + ini_index].mean = NEUTRAL_ELEMENT;
            data[real_size/2 + ini_index].l = ini_index;
            data[real_size/2 + ini_index].r = ini_index;
#ifdef _DEBUG
            printf("%d\n", ini_index);
#endif
            ini_index++;
        }


        for(size_t j = real_size; j/=2;)
            for(size_t i=j; i < j*2; i+=2)
            {
#ifdef _DEBUG
                printf("%d %d\n", data[i].l, data[i+1].r);
#endif
                data[i/2].mean = data[i].mean + data[i+1].mean;
                data[i/2].l = data[i].l;
                data[i/2].r = data[i+1].r;
            }

    }
};

int main() {

    size_t n, n_op;
    scanf("%zu %zu", &n, &n_op);

    Sum_Tree my_tree(n);
    uint buffer;

    for(int i = 0; i < n; i++)
    {
        scanf("%u", &buffer);
        my_tree.add(buffer);
    }
    my_tree.print();
    for(int i = 0; i < n_op; i++)
    {
        scanf("%d", &buffer);
        switch (buffer) {
            case 1:
            {
                size_t index;
                uint v;
                scanf("%zu %u", &index, &v);
                my_tree.set(index, v);
                my_tree.print();
                break;
            }
            case 2:
            {
                size_t l;
                size_t r;
                scanf("%zu %zu", &l, &r);
                printf("%llu\n", my_tree.sum(0, l, r-1));
                break;
            }
            default:
            {
                printf("Incorrect input\n");
            }
        }
    }

    return 0;
}
