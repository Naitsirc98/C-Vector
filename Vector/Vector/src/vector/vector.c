#include "vector\vector.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

void* alloc_buffer(uint size, uint count)
{
	return malloc(size* count);
}

void* realloc_buffer(void* old_buffer, uint old_size, uint new_size)
{
	return realloc(old_buffer, new_size);
}

int equal_func(void* a, void* b, uint data_size)
{
	assert(a != NULL);
	assert(b != NULL);

	return memcmp(a, b, data_size) == 0;
}



vector* vec_create(uint data_size)
{
	vector* vec = (vector*)malloc(sizeof(vector));

	vec_init(vec, data_size);

	return vec;
}

void vec_free(vector* vec)
{
	assert(vec != NULL);

	free(vec->buffer);
	free(vec);
}

void vec_init(vector* vec, uint data_size)
{
	assert(vec != NULL);

	vec->data_size = data_size;
	vec->size = 0;
	vec->capacity = 0;
	vec->alloc_func = alloc_buffer;
	vec->realloc_func = realloc_buffer;
	vec->equal_func = equal_func;
	vec->buffer = NULL;
}

void vec_reserve(vector* vec, uint new_size)
{
	assert(vec != NULL);

	if(new_size > vec->size)
	{
		const uint new_capacity = new_size* vec->data_size;
		vec->buffer = vec->realloc_func(vec->buffer, vec->capacity, new_capacity);
		vec->capacity = new_capacity;
	}
}

void vec_resize(vector* vec, uint new_size)
{
	assert(vec != NULL);

	vec_reserve(vec, new_size);

	vec->size = new_size;
}

void vec_resize_val(vector* vec, uint new_size, const void* val)
{
	assert(vec != NULL);
	assert(val != NULL);

	const uint old_size = new_size;

	vec_resize(vec, new_size);

	if(new_size > old_size)
	{
		const uint data_size = vec->data_size;
		char* buffer = vec->buffer;
		for(int i = old_size;i < new_size;++i)
		{
			memcpy(buffer+i*data_size, val, data_size);
		}
	}
}

uint vec_max_size(vector* vec)
{
	assert(vec != NULL);

	return vec->capacity / vec->data_size;
}

int vec_empty(vector* vec)
{
	assert(vec != NULL);

	return vec->size == 0;
}

void vec_shrink_to_fit(vector* vec)
{
	assert(vec != NULL);

	if(vec->size < vec_max_size(vec))
	{
		const uint new_capacity = vec->size* vec->data_size;
		vec->buffer = vec->realloc_func(vec->buffer, vec->capacity, new_capacity);
		vec->capacity = new_capacity;
	}
}

uint vec_find(vector* vec, void* element, uint offset)
{
	assert(vec != NULL);
	assert(offset < vec->size);

	if(element == NULL)
		return VEC_NPOS;

	const uint size = vec->size;
	const uint data_size = vec->data_size;
	const uint limit = size * data_size;
	const equal_function equal_func = vec->equal_func;
	const char* buffer = vec->buffer;

	for(uint i = offset*data_size;i < limit;i += data_size)
	{
		// printf("find %i...\n", i);
		if(equal_func(buffer+i, element, data_size))
		{
			return i / data_size;
		}
	}

	return VEC_NPOS;
}

uint vec_find_last(vector* vec, void* element, uint offset)
{
	assert(vec != NULL);
	assert(offset < vec->size);

	if(element == NULL)
		return VEC_NPOS;

	const uint size = vec->size;
	const uint data_size = vec->data_size;
	const uint limit = size * data_size;
	const equal_function equal_func = vec->equal_func;
	const char* buffer = vec->buffer;

	for(uint i = (size-1)*data_size - offset*data_size;i >= 0;i -= data_size)
	{
		if(equal_func(buffer+i, element, data_size))
		{
			return i / data_size;
		}
	}

	return VEC_NPOS;
}

uint vec_has(vector* vec, void* element)
{
	assert(vec != NULL);

	return vec_find(vec, element, 0) != VEC_NPOS;
}

void* vec_at(vector* vec, uint pos)
{
	assert(vec != NULL);
	assert(pos < vec->size);

	return (char*)vec->buffer + pos* vec->data_size;
}

void* vec_at_cp(vector* vec, uint pos, void* element)
{
	assert(vec != NULL);
	assert(pos < vec->size);
	assert(element != NULL);

	return memcpy(element, (char*)vec->buffer+pos*vec->data_size, vec->data_size);
}

void* vec_front(vector* vec)
{
	return vec_at(vec, 0);
}

void* vec_front_cp(vector* vec, void* element)
{
	return vec_at_cp(vec, 0, element);
}

void* vec_back(vector* vec)
{
	assert(vec != NULL);

	return vec_at(vec, vec->size-1);
}

void* vec_back_cp(vector* vec, void* element)
{
	assert(vec != NULL);

	return vec_at_cp(vec, vec->size-1, element);
}

void vec_push_back(vector* vec, void* element)
{
	assert(vec != NULL);

	vec_insert(vec, vec->size, element);
}

void vec_pop_back(vector* vec)
{
	assert(vec != NULL);

	vec_erase(vec, vec->size-1);
}

void vec_insert(vector* vec, uint pos, void* element)
{
	assert(vec != NULL);
	assert(pos <= vec->size);
	assert(element != NULL);

	const uint size = vec->size;
	const uint offset = pos* vec->data_size;
	
	if(size == 0)
	{
		vec_reserve(vec, 1);
	}
	else if(size == vec_max_size(vec))
	{
		vec_reserve(vec, size* 2);
	}
	
	if(pos < size)
	{
		// Shift buffer elements from pos to the right
		memmove((char*)vec->buffer+(pos+1)*vec->data_size, (char*)vec->buffer+offset,
			(size-pos)*vec->data_size);
	}

	memcpy((char*)vec->buffer+offset, element, vec->data_size);

	++vec->size;
}

void vec_replace(vector* vec, uint pos, void* element)
{
	assert(vec != NULL);
	assert(pos < vec->size);
	assert(element != NULL);

	void* ptr = vec_at(vec, pos);
	memcpy(ptr, element, vec->data_size);
}

void vec_erase(vector* vec, uint pos)
{
	assert(vec != NULL);
	assert(pos < vec->size);

	if(pos < vec->size-1)
	{
		// Shift buffer elements from pos+1 to the left
		memmove((char*)vec->buffer+pos*vec->data_size, (char*)vec->buffer+(pos+1)*vec->data_size,
			(vec->size-pos+1)*vec->data_size);
	}

	--vec->size;
}

void vec_erase_range(vector* vec, uint first, uint last)
{
	assert(vec != NULL);
	assert(first <= last);
	assert(last <= vec->size);

	if(last < vec->size)
	{
		// Shift buffer elements from pos+1 to the left
		memmove((char*)vec->buffer+first*vec->data_size, (char*)vec->buffer+(last)*vec->data_size,
			(vec->size-last)*vec->data_size);
	}

	vec->size -= last-first;
}

void vec_clear(vector* vec)
{
	assert(vec != NULL);

	vec->size = 0;
}

int vec_cmp(vector* v1, vector* v2)
{
	assert(v1 != NULL);

	if(v2 == NULL)
		return v1->size;

	if(v1->size != v2->size) 
	{
		return v1->size - v2->size;
	}
	
	if(v1->data_size != v2->data_size)
	{
		return v1->data_size - v2->data_size;
	}

	const uint data_size = v1->data_size;
	const uint limit = v1->size* data_size;
	const equal_function equal_func = v1->equal_func;
	const char* v1_buf = v1->buffer;
	const char* v2_buf = v2->buffer;

	for(uint i = 0;i < limit;i += data_size)
	{
		const uint cmp = equal_func(v1_buf+i, v2_buf+i, data_size);

		if(cmp != 0)
			return cmp;
	}
	
	return 0;
}

vector* vec_cpy(vector* v1, vector* v2, uint v1_off, uint count, uint v2_off)
{
	assert(v1 != NULL);
	assert(v2 != NULL);
	assert(v1_off < v1->size);
	assert(count <= v1->size - v1_off);
	assert(v2_off < v2->size);

	memcpy((char*)v1->buffer+v1_off*v1->data_size, (char*)v2->buffer+v2_off*v2->data_size, count*v1->data_size);
}

vector* vec_dup(vector* vec, uint offset, uint count)
{
	assert(vec != NULL);
	assert(offset < vec->size);
	assert(count <= vec->size - offset);

	vector* copy = vec_create(vec->data_size);
	vec_reserve(copy, vec_max_size(vec));
	vec_cpy(vec, copy, 0, vec->size, 0);
	copy->alloc_func = vec->alloc_func;
	copy->realloc_func = vec->realloc_func;
	copy->equal_func = vec->equal_func;
	copy->size = vec->size;

	return copy;
}

// =========================== VECTOR VALUE-TYPE SPECIALIZATIONS ===================================
// 
// Function name types:
//
// char: vecc
// unsigned char: vecuc
// short: vecs
// unsigned short: vecus
// int: veci
// unsigned int: vecui
// long: vecl
// unsigned long: vecul
// float: vecf
// double: vecd
// 
// 

// =========================== CHAR =================================

int charcmp(char* a, char* b, uint data_size)
{
	return *a == *b;
}

vector* vecc_create()
{
	return vec_create(sizeof(char));
}

void vecc_init(vector* vec)
{
	vec_init(vec, sizeof(char));
	vec->equal_func = charcmp;
}

void vecc_resize_val(vector* vec, uint new_size, char val)
{
	vec_resize_val(vec, new_size, &val);
}

uint vecc_find(vector* vec, char element, uint offset)
{
	return vec_find(vec, &element, offset);
}

uint vecc_find_last(vector* vec, char element, uint offset)
{
	return vec_find_last(vec, element, offset);
}

uint vecc_has(vector* vec, char element)
{
	return vec_has(vec, &element);
}

char* vecc_at(vector* vec, uint pos)
{
	return (char*)vec_at(vec, pos);
}

char vecc_at_cp(vector* vec, uint pos)
{
	return *vecc_at(vec, pos);
}

char* vecc_front(vector* vec)
{
	return (char*)vec_front(vec);
}

char vecc_front_cp(vector* vec)
{
	return *vecc_front(vec);
}

char* vecc_back(vector* vec)
{
	return (char*)vec_back(vec);
}

char vecc_back_cp(vector* vec)
{
	return *vecc_back(vec);
}

void vecc_push_back(vector* vec, char element)
{
	vec_push_back(vec, &element);
}

void vecc_insert(vector* vec, uint pos, char element)
{
	vec_insert(vec, pos, &element);
}

void vecc_replace(vector* vec, uint pos, char element)
{
	vec_replace(vec, pos, &element);
}


// =========================== UNSIGNED CHAR =================================

int ucharcmp(unsigned char* a, unsigned char* b, uint data_size)
{
	return *a == *b;
}

vector* vecuc_create()
{
	return vec_create(sizeof(unsigned char));
}

void vecuc_init(vector* vec)
{
	vec_init(vec, sizeof(unsigned char));
	vec->equal_func = ucharcmp;
}

void vecuc_resize_val(vector* vec, uint new_size, unsigned char val)
{
	vec_resize_val(vec, new_size, &val);
}

uint vecuc_find(vector* vec, unsigned char element, uint offset)
{
	return vec_find(vec, &element, offset);
}

uint vecuc_find_last(vector* vec, unsigned char element, uint offset)
{
	return vec_find_last(vec, element, offset);
}

uint vecuc_has(vector* vec, unsigned char element)
{
	return vec_has(vec, &element);
}

unsigned char* vecuc_at(vector* vec, uint pos)
{
	return (unsigned char*)vec_at(vec, pos);
}

unsigned char vecuc_at_cp(vector* vec, uint pos)
{
	return *vecuc_at(vec, pos);
}

unsigned char* vecuc_front(vector* vec)
{
	return (unsigned char*)vec_front(vec);
}

unsigned char vecuc_front_cp(vector* vec)
{
	return *vecuc_front(vec);
}

unsigned char* vecuc_back(vector* vec)
{
	return (unsigned char*)vec_back(vec);
}

unsigned char vecuc_back_cp(vector* vec)
{
	return *vecuc_back(vec);
}

void vecuc_push_back(vector* vec, unsigned char element)
{
	vec_push_back(vec, &element);
}

void vecuc_insert(vector* vec, uint pos, unsigned char element)
{
	vec_insert(vec, pos, &element);
}

void vecuc_replace(vector* vec, uint pos, unsigned char element)
{
	vec_replace(vec, pos, &element);
}


// =========================== SHORT =================================

int shortcmp(short* a, short* b, uint data_size)
{
	return *a == *b;
}

vector* vecs_create()
{
	return vec_create(sizeof(short));
}

void vecs_init(vector* vec)
{
	vec_init(vec, sizeof(short));
	vec->equal_func = shortcmp;
}

void vecs_resize_val(vector* vec, uint new_size, short val)
{
	vec_resize_val(vec, new_size, &val);
}

uint vecs_find(vector* vec, short element, uint offset)
{
	return vec_find(vec, &element, offset);
}

uint vecs_find_last(vector* vec, short element, uint offset)
{
	return vec_find_last(vec, element, offset);
}

uint vecs_has(vector* vec, short element)
{
	return vec_has(vec, &element);
}

short* vecs_at(vector* vec, uint pos)
{
	return (short*)vec_at(vec, pos);
}

short vecs_at_cp(vector* vec, uint pos)
{
	return *vecs_at(vec, pos);
}

short* vecs_front(vector* vec)
{
	return (short*)vec_front(vec);
}

short vecs_front_cp(vector* vec)
{
	return *vecs_front(vec);
}

short* vecs_back(vector* vec)
{
	return (short*)vec_back(vec);
}

short vecs_back_cp(vector* vec)
{
	return *vecs_back(vec);
}

void vecs_push_back(vector* vec, short element)
{
	vec_push_back(vec, &element);
}

void vecs_insert(vector* vec, uint pos, short element)
{
	vec_insert(vec, pos, &element);
}

void vecs_replace(vector* vec, uint pos, short element)
{
	vec_replace(vec, pos, &element);
}


// =========================== UNSIGNED SHORT =================================

int ushortcmp(unsigned short* a, unsigned short* b, uint data_size)
{
	return *a == *b;
}

vector* vecus_create()
{
	return vec_create(sizeof(unsigned short));
}

void vecus_init(vector* vec)
{
	vec_init(vec, sizeof(unsigned short));
	vec->equal_func = ushortcmp;
}

void vecus_resize_val(vector* vec, uint new_size, unsigned short val)
{
	vec_resize_val(vec, new_size, &val);
}

uint vecus_find(vector* vec, unsigned short element, uint offset)
{
	return vec_find(vec, &element, offset);
}

uint vecus_find_last(vector* vec, unsigned short element, uint offset)
{
	return vec_find_last(vec, element, offset);
}

uint vecus_has(vector* vec, unsigned short element)
{
	return vec_has(vec, &element);
}

unsigned short* vecus_at(vector* vec, uint pos)
{
	return (unsigned short*)vec_at(vec, pos);
}

unsigned short vecus_at_cp(vector* vec, uint pos)
{
	return *vecus_at(vec, pos);
}

unsigned short* vecus_front(vector* vec)
{
	return (unsigned short*)vec_front(vec);
}

unsigned short vecus_front_cp(vector* vec)
{
	return *vecus_front(vec);
}

unsigned short* vecus_back(vector* vec)
{
	return (unsigned short*)vec_back(vec);
}

unsigned short vecus_back_cp(vector* vec)
{
	return *vecus_back(vec);
}

void vecus_push_back(vector* vec, unsigned short element)
{
	vec_push_back(vec, &element);
}

void vecus_insert(vector* vec, uint pos, unsigned short element)
{
	vec_insert(vec, pos, &element);
}

void vecus_replace(vector* vec, uint pos, unsigned short element)
{
	vec_replace(vec, pos, &element);
}


// =========================== INT =================================

int intcmp(int* a, int* b, uint data_size)
{
	return *a == *b;
}

vector* veci_create()
{
	return vec_create(sizeof(int));
}

void veci_init(vector* vec)
{
	vec_init(vec, sizeof(int));
	vec->equal_func = intcmp;
}

void veci_resize_val(vector* vec, uint new_size, int val)
{
	vec_resize_val(vec, new_size, &val);
}

uint veci_find(vector* vec, int element, uint offset)
{
	return vec_find(vec, &element, offset);
}

uint veci_find_last(vector* vec, int element, uint offset)
{
	return vec_find_last(vec, element, offset);
}

uint veci_has(vector* vec, int element)
{
	return vec_has(vec, &element);
}

int* veci_at(vector* vec, uint pos)
{
	return (int*)vec_at(vec, pos);
}

int veci_at_cp(vector* vec, uint pos)
{
	return *veci_at(vec, pos);
}

int* veci_front(vector* vec)
{
	return (int*)vec_front(vec);
}

int veci_front_cp(vector* vec)
{
	return *veci_front(vec);
}

int* veci_back(vector* vec)
{
	return (int*)vec_back(vec);
}

int veci_back_cp(vector* vec)
{
	return *veci_back(vec);
}

void veci_push_back(vector* vec, int element)
{
	vec_push_back(vec, &element);
}

void veci_insert(vector* vec, uint pos, int element)
{
	vec_insert(vec, pos, &element);
}

void veci_replace(vector* vec, uint pos, int element)
{
	vec_replace(vec, pos, &element);
}


// =========================== UNSIGNED INT =================================

int uintcmp(unsigned int* a, unsigned int* b, uint data_size)
{
	return *a == *b;
}

vector* vecui_create()
{
	return vec_create(sizeof(unsigned int));
}

void vecui_init(vector* vec)
{
	vec_init(vec, sizeof(unsigned int));
	vec->equal_func = uintcmp;
}

void vecui_resize_val(vector* vec, uint new_size, unsigned int val)
{
	vec_resize_val(vec, new_size, &val);
}

uint vecui_find(vector* vec, unsigned int element, uint offset)
{
	return vec_find(vec, &element, offset);
}

uint vecui_find_last(vector* vec, unsigned int element, uint offset)
{
	return vec_find_last(vec, element, offset);
}

uint vecui_has(vector* vec, unsigned int element)
{
	return vec_has(vec, &element);
}

unsigned int* vecui_at(vector* vec, uint pos)
{
	return (unsigned int*)vec_at(vec, pos);
}

unsigned int vecui_at_cp(vector* vec, uint pos)
{
	return *vecui_at(vec, pos);
}

unsigned int* vecui_front(vector* vec)
{
	return (unsigned int*)vec_front(vec);
}

unsigned int vecui_front_cp(vector* vec)
{
	return *vecui_front(vec);
}

unsigned int* vecui_back(vector* vec)
{
	return (unsigned int*)vec_back(vec);
}

unsigned int vecui_back_cp(vector* vec)
{
	return *vecui_back(vec);
}

void vecui_push_back(vector* vec, unsigned int element)
{
	vec_push_back(vec, &element);
}

void vecui_insert(vector* vec, uint pos, unsigned int element)
{
	vec_insert(vec, pos, &element);
}

void vecui_replace(vector* vec, uint pos, unsigned int element)
{
	vec_replace(vec, pos, &element);
}


// =========================== LONG =================================

int longcmp(long* a, long* b, uint data_size)
{
	return *a == *b;
}

vector* vecl_create()
{
	return vec_create(sizeof(long));
}

void vecl_init(vector* vec)
{
	vec_init(vec, sizeof(long));
	vec->equal_func = longcmp;
}

void vecl_resize_val(vector* vec, uint new_size, long val)
{
	vec_resize_val(vec, new_size, &val);
}

uint vecl_find(vector* vec, long element, uint offset)
{
	return vec_find(vec, &element, offset);
}

uint vecl_find_last(vector* vec, long element, uint offset)
{
	return vec_find_last(vec, element, offset);
}

uint vecl_has(vector* vec, long element)
{
	return vec_has(vec, &element);
}

long* vecl_at(vector* vec, uint pos)
{
	return (long*)vec_at(vec, pos);
}

long vecl_at_cp(vector* vec, uint pos)
{
	return *vecl_at(vec, pos);
}

long* vecl_front(vector* vec)
{
	return (long*)vec_front(vec);
}

long vecl_front_cp(vector* vec)
{
	return *vecl_front(vec);
}

long* vecl_back(vector* vec)
{
	return (long*)vec_back(vec);
}

long vecl_back_cp(vector* vec)
{
	return *vecl_back(vec);
}

void vecl_push_back(vector* vec, long element)
{
	vec_push_back(vec, &element);
}

void vecl_insert(vector* vec, uint pos, long element)
{
	vec_insert(vec, pos, &element);
}

void vecl_replace(vector* vec, uint pos, long element)
{
	vec_replace(vec, pos, &element);
}


// =========================== UNSIGNED LONG =================================

int ulongcmp(unsigned long* a, unsigned long* b, uint data_size)
{
	return *a == *b;
}

vector* vecul_create()
{
	return vec_create(sizeof(unsigned long));
}

void vecul_init(vector* vec)
{
	vec_init(vec, sizeof(unsigned long));
	vec->equal_func = ulongcmp;
}

void vecul_resize_val(vector* vec, uint new_size, unsigned long val)
{
	vec_resize_val(vec, new_size, &val);
}

uint vecul_find(vector* vec, unsigned long element, uint offset)
{
	return vec_find(vec, &element, offset);
}

uint vecul_find_last(vector* vec, unsigned long element, uint offset)
{
	return vec_find_last(vec, element, offset);
}

uint vecul_has(vector* vec, unsigned long element)
{
	return vec_has(vec, &element);
}

unsigned long* vecul_at(vector* vec, uint pos)
{
	return (unsigned long*)vec_at(vec, pos);
}

unsigned long vecul_at_cp(vector* vec, uint pos)
{
	return *vecul_at(vec, pos);
}

unsigned long* vecul_front(vector* vec)
{
	return (unsigned long*)vec_front(vec);
}

unsigned long vecul_front_cp(vector* vec)
{
	return *vecul_front(vec);
}

unsigned long* vecul_back(vector* vec)
{
	return (unsigned long*)vec_back(vec);
}

unsigned long vecul_back_cp(vector* vec)
{
	return *vecul_back(vec);
}

void vecul_push_back(vector* vec, unsigned long element)
{
	vec_push_back(vec, &element);
}

void vecul_insert(vector* vec, uint pos, unsigned long element)
{
	vec_insert(vec, pos, &element);
}

void vecul_replace(vector* vec, uint pos, unsigned long element)
{
	vec_replace(vec, pos, &element);
}


// =========================== FLOAT =================================

int floatcmp(float* a, float* b, uint data_size)
{
	return *a == *b;
}

vector* vecf_create()
{
	return vec_create(sizeof(float));
}

void vecf_init(vector* vec)
{
	vec_init(vec, sizeof(float));
	vec->equal_func = floatcmp;
}

void vecf_resize_val(vector* vec, uint new_size, float val)
{
	vec_resize_val(vec, new_size, &val);
}

uint vecf_find(vector* vec, float element, uint offset)
{
	return vec_find(vec, &element, offset);
}

uint vecf_find_last(vector* vec, float element, uint offset)
{
	return vec_find_last(vec, &element, offset);
}

uint vecf_has(vector* vec, float element)
{
	return vec_has(vec, &element);
}

float* vecf_at(vector* vec, uint pos)
{
	return (float*)vec_at(vec, pos);
}

float vecf_at_cp(vector* vec, uint pos)
{
	return *vecf_at(vec, pos);
}

float* vecf_front(vector* vec)
{
	return (float*)vec_front(vec);
}

float vecf_front_cp(vector* vec)
{
	return *vecf_front(vec);
}

float* vecf_back(vector* vec)
{
	return (float*)vec_back(vec);
}

float vecf_back_cp(vector* vec)
{
	return *vecf_back(vec);
}

void vecf_push_back(vector* vec, float element)
{
	vec_push_back(vec, &element);
}

void vecf_insert(vector* vec, uint pos, float element)
{
	vec_insert(vec, pos, &element);
}

void vecf_replace(vector* vec, uint pos, float element)
{
	vec_replace(vec, pos, &element);
}


// =========================== DOUBLE =================================

int doublecmp(double* a, double* b, uint data_size)
{
	return *a == *b;
}

vector* vecd_create()
{
	return vec_create(sizeof(double));
}

void vecd_init(vector* vec)
{
	vec_init(vec, sizeof(double));
	vec->equal_func = doublecmp;
}

void vecd_resize_val(vector* vec, uint new_size, double val)
{
	vec_resize_val(vec, new_size, &val);
}

uint vecd_find(vector* vec, double element, uint offset)
{
	return vec_find(vec, &element, offset);
}

uint vecd_find_last(vector* vec, double element, uint offset)
{
	return vec_find_last(vec, &element, offset);
}

uint vecd_has(vector* vec, double element)
{
	return vec_has(vec, &element);
}

double* vecd_at(vector* vec, uint pos)
{
	return (double*)vec_at(vec, pos);
}

double vecd_at_cp(vector* vec, uint pos)
{
	return *vecd_at(vec, pos);
}

double* vecd_front(vector* vec)
{
	return (double*)vec_front(vec);
}

double vecd_front_cp(vector* vec)
{
	return *vecd_front(vec);
}

double* vecd_back(vector* vec)
{
	return (double*)vec_back(vec);
}

double vecd_back_cp(vector* vec)
{
	return *vecd_back(vec);
}

void vecd_push_back(vector* vec, double element)
{
	vec_push_back(vec, &element);
}

void vecd_insert(vector* vec, uint pos, double element)
{
	vec_insert(vec, pos, &element);
}

void vecd_replace(vector* vec, uint pos, double element)
{
	vec_replace(vec, pos, &element);
}
