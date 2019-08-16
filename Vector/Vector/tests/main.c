#include <stdlib.h>
#include <stdio.h>
#include <vector/vector.h>
#include <assert.h>
#include <time.h>

// Simple tests and benchmarks

typedef struct object
{
	int x;
	double y;
	long z;
} object;

void print_obj(object* obj)
{
	printf("object[x=%i,y=%f,z=%i]\n", obj->x, obj->y, obj->z);
}

int objcmp(object* o1, object* o2)
{
	if(o1->x != o2->x)
		return 0;
	if(o1->y != o2->y)
		return 0;
	if(o1->z != o2->z)
		return 0;

	return 1;
}

void simple_test()
{
	int n = 1000;

	vector* vec = vec_create(sizeof(object));

	for(int i = 0;i < n;++i)
	{
		object obj = { i, i*2.5, i*10 };
		vec_push_back(vec, &obj);
	}

	vec_shrink_to_fit(vec);

	assert(vec->size == n);

	for(int i = 0;i < n;++i)
	{
		object obj = { i, i*2.5, i*10 };
		assert(vec_find(vec, &obj, 0) != VEC_NPOS);
	}

	for(int i = 0;i < n;i++)
	{
		object obj = { i, i*2.5, i*10 };
		object* obj2 = vec_at(vec, i);
		assert(objcmp(&obj, obj2) == 0);
	}

	for(int i = 0;i < n;i++)
	{
		object obj = { i, i*2.5, i*10 };
		object obj2;
		vec_at_cp(vec, i, &obj2);
		assert(objcmp(&obj, &obj2) == 0);
	}

	for(int i = 0;!vec_empty(vec);i++)
	{
		vec_erase(vec, vec->size-1);
		assert(vec->size == n - (i+1));
	}

	vec_free(vec);
}

void time_push_back(vector* vec, int n)
{
	vec_clear(vec);

	clock_t start = clock();

	for(int i = 0;i < n;++i)
	{
		vecf_push_back(vec, i);
	}

	clock_t end = clock();

	printf("Push Back time: %f ms\n", (end-start) / (CLOCKS_PER_SEC / 1000.0));
}

void time_push(vector* vec, int n)
{
	vec_clear(vec);

	clock_t start = clock();

	for(int i = 0;i < n;++i)
	{
		vecf_insert(vec, 0, i);
	}

	clock_t end = clock();

	printf("Push time: %f ms\n", (end-start) / (CLOCKS_PER_SEC / 1000.0));
}

void time_find(vector* vec, int n)
{
	clock_t start = clock();

	for(int i = 0;i < n;++i)
	{
		if(vecf_find(vec, n/2, 0) == VEC_NPOS)
		{
			puts("ERROR");
		}
	}

	clock_t end = clock();

	printf("Find time: %f ms\n", (end-start) / (CLOCKS_PER_SEC / 1000.0));
}

void time_pop_back(vector* vec, int n)
{
	clock_t start = clock();

	for(int i = 0;i < n;++i)
	{
		vec_pop_back(vec);
	}

	clock_t end = clock();

	printf("Pop Back time: %f ms\n", (end-start) / (CLOCKS_PER_SEC / 1000.0));
}

void time_pop(vector* vec, int n)
{
	clock_t start = clock();

	for(int i = 0;i < n;++i)
	{
		vec_erase(vec, 0);
	}

	clock_t end = clock();

	printf("Pop time: %f ms\n", (end-start) / (CLOCKS_PER_SEC / 1000.0));
}

int cmp_float(void* a, void* b, uint data_size)
{
	return *(float*)a == *(float*)b;
}

int main()
{
	simple_test();

	const int n = 100000;

	vector vec;
	vecf_init(&vec);

	time_push(&vec, n);
	time_push_back(&vec, n);
	time_find(&vec, n);
	time_pop_back(&vec, n);
	// We need to fill the vector again
	for(int i = 0;i < n;++i)
	{
		vecf_push_back(&vec, i);
	}
	time_pop(&vec, n);

	free(vec.buffer);

	system("pause");
	return 0;
}
