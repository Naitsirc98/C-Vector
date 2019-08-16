#pragma once
#define VEC_NPOS -1

typedef unsigned int uint;

typedef void* (*alloc_function)(uint size, uint count);
typedef void* (*realloc_function)(void* old_buffer, uint old_size, uint new_size);
typedef int (*equal_function)(void* a, void* b, uint data_size);

typedef struct vector
{
	void* buffer; // Data storage
	uint data_size; // Size of each element, in bytes
	uint size; // Number of elements in the vector
	uint capacity; // Size of the vector storage, in bytes
	alloc_function alloc_func; // Function used to allocate a new buffer
	realloc_function realloc_func; // Function used to realloc the vector buffer
	equal_function equal_func; // Function used to compare values of the vector
} vector;

// Please notice that this vector implementation only manages data by value, that is, 
// it does not delete data dynamically allocated

// Allocates a new vector dynamically and initializes it
vector* vec_create(uint data_size);
// Free the vector and its data storage
void vec_free(vector* vec);
// Initializes the vector with default parameters
void vec_init(vector* vec, uint data_size);
// Requests that the vector capacity be at least enough to contain n elements
void vec_reserve(vector* vec, uint new_size);
// Resizes the container so that it contains n elements
void vec_resize(vector* vec, uint new_size);
// Resizes the container so that it contains n elements, and the new elements are initialized as copies of val
void vec_resize_val(vector* vec, uint new_size, const void* val);
// Returns the maximum number of elements that the vector can hold
uint vec_max_size(vector* vec);
// Returns whether the vector is empty (size = 0)
uint vec_empty(vector* vec);
// Requests the container to reduce its capacity to fit its size
void vec_shrink_to_fit(vector* vec);
// Returns the first position of the element in the vector, starting the search from offset.
// If the element is not found, VEC_NPOS is returned
uint vec_find(vector* vec, void* element, uint offset);
// Returns the last position of the element in the vector, starting the search from size-1-offset.
// If the element is not found, VEC_NPOS is returned
uint vec_find_last(vector* vec, void* element, uint offset);
// Return 0 if the element is not stored in the vector
uint vec_has(vector* vec, void* element);
// Returns a pointer to the element at pos in the vector
void* vec_at(vector* vec, uint pos);
// Returns a copy of the element at pos in the vector. The copy is stored in element
void* vec_at_cp(vector* vec, uint pos, void* element);
// Returns a reference to the first element in the vector
void* vec_front(vector* vec);
// Returns a copy of the first element in the vector. The copy is stored in element
void* vec_front_cp(vector* vec, void* element);
// Returns a reference to the last element in the vector
void* vec_back(vector* vec);
// Returns a copy of the last element in the vector. The copy is stored in element
void* vec_back_cp(vector* vec, void* element);
// Add element at the end. The value is copied to the vector
void vec_push_back(vector* vec, void* element);
// Removes the last element in the vector, effectively reducing the container size by one
void vec_pop_back(vector* vec);
// The vector is extended by inserting new elements before the element at the specified position, 
// effectively increasing the container size by the number of elements inserted
void vec_insert(vector* vec, uint pos, void* element);
// Set the element given at pos. Returns the old element at pos
void vec_replace(vector* vec, uint pos, void* element);
// Removes from the vector the element at pos
void vec_erase(vector* vec, uint pos);
// Removes from the vector the range of elements [first, last)
void vec_erase_range(vector* vec, uint first, uint last);
// Removes all elements from the vector, leaving the container with a size of 0. This does not affect capacity
void vec_clear(vector* vec);
// Compares 2 vectors, returning 0 if v1 equals v2, that is, if both vectors have the same number of elements,
// and their values are equal too. It uses vector.cmp_func to compare the values
int vec_cmp(vector* v1, vector* v2);
// Copy count elements, from v1 to v2, in the range v1[v1_off, v1_off+count) to v2[v2_off, v2_off+count)
// and returns v2
vector* vec_cpy(vector* v1, vector* v2, uint v1_off, uint count, uint v2_off);
// Duplicates the vector, in the range [offset, offset+count)
vector* vec_dup(vector* vec, uint offset, uint count);


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

// =========================== CHAR =================================

vector* vecc_create();
void vecc_init(vector* vec);
void vecc_resize_val(vector* vec, uint new_size, char val);
uint vecc_find(vector* vec, char element, uint offset);
uint vecc_find_last(vector* vec, char element, uint offset);
uint vecc_has(vector* vec, char element);
char* vecc_at(vector* vec, uint pos);
char vecc_at_cp(vector* vec, uint pos);
char* vecc_front(vector* vec);
char vecc_front_cp(vector* vec);
char* vecc_back(vector* vec);
char vecc_back_cp(vector* vec);
void vecc_push_back(vector* vec, char element);
void vecc_insert(vector* vec, uint pos, char element);
void vecc_replace(vector* vec, uint pos, char element);

// =========================== UNSIGNED CHAR =========================

vector* vecuc_create();
void vecuc_init(vector* vec);
void vecuc_resize_val(vector* vec, uint new_size, unsigned char val);
uint vecuc_find(vector* vec, unsigned char element, uint offset);
uint vecuc_find_last(vector* vec, unsigned char element, uint offset);
uint vecuc_has(vector* vec, unsigned char element);
unsigned char* vecuc_at(vector* vec, uint pos);
unsigned char vecuc_at_cp(vector* vec, uint pos);
unsigned char* vecuc_front(vector* vec);
unsigned char vecuc_front_cp(vector* vec);
unsigned char* vecuc_back(vector* vec);
unsigned char vecuc_back_cp(vector* vec);
void vecuc_push_back(vector* vec, unsigned char element);
void vecuc_insert(vector* vec, uint pos, unsigned char element);
void vecuc_replace(vector* vec, uint pos, unsigned char element);

// =========================== SHORT =========================

vector* vecs_create();
void vecs_init(vector* vec);
void vecs_resize_val(vector* vec, uint new_size, short val);
uint vecs_find(vector* vec, short element, uint offset);
uint vecs_find_last(vector* vec, short element, uint offset);
uint vecs_has(vector* vec, short element);
short* vecs_at(vector* vec, uint pos);
short vecs_at_cp(vector* vec, uint pos);
short* vecs_front(vector* vec);
short vecs_front_cp(vector* vec);
short* vecs_back(vector* vec);
short vecs_back_cp(vector* vec);
void vecs_push_back(vector* vec, short element);
void vecs_insert(vector* vec, uint pos, short element);
void vecs_replace(vector* vec, uint pos, short element);

// =========================== UNSIGNED SHORT =========================

vector* vecus_create();
void vecus_init(vector* vec);
void vecus_resize_val(vector* vec, uint new_size, unsigned short val);
uint vecus_find(vector* vec, unsigned short element, uint offset);
uint vecus_find_last(vector* vec, unsigned short element, uint offset);
uint vecus_has(vector* vec, unsigned short element);
unsigned short* vecus_at(vector* vec, uint pos);
unsigned short vecus_at_cp(vector* vec, uint pos);
unsigned short* vecus_front(vector* vec);
unsigned short vecus_front_cp(vector* vec);
unsigned short* vecus_back(vector* vec);
unsigned short vecus_back_cp(vector* vec);
void vecus_push_back(vector* vec, unsigned short element);
void vecus_insert(vector* vec, uint pos, unsigned short element);
void vecus_replace(vector* vec, uint pos, unsigned short element);

// =========================== INT =========================

vector* veci_create();
void veci_init(vector* vec);
void veci_resize_val(vector* vec, uint new_size, int val);
uint veci_find(vector* vec, int element, uint offset);
uint veci_find_last(vector* vec, int element, uint offset);
uint veci_has(vector* vec, int element);
int* veci_at(vector* vec, uint pos);
int veci_at_cp(vector* vec, uint pos);
int* veci_front(vector* vec);
int veci_front_cp(vector* vec);
int* veci_back(vector* vec);
int veci_back_cp(vector* vec);
void veci_push_back(vector* vec, int element);
void veci_insert(vector* vec, uint pos, int element);
void veci_replace(vector* vec, uint pos, int element);

// =========================== UNSIGNED INT =========================

vector* vecui_create();
void vecui_init(vector* vec);
void vecui_resize_val(vector* vec, uint new_size, unsigned int val);
uint vecui_find(vector* vec, unsigned int element, uint offset);
uint vecui_find_last(vector* vec, unsigned int element, uint offset);
uint vecui_has(vector* vec, unsigned int element);
unsigned int* vecui_at(vector* vec, uint pos);
unsigned int vecui_at_cp(vector* vec, uint pos);
unsigned int* vecui_front(vector* vec);
unsigned int vecui_front_cp(vector* vec);
unsigned int* vecui_back(vector* vec);
unsigned int vecui_back_cp(vector* vec);
void vecui_push_back(vector* vec, unsigned int element);
void vecui_insert(vector* vec, uint pos, unsigned int element);
void vecui_replace(vector* vec, uint pos, unsigned int element);

// =========================== LONG =========================

vector* vecl_create();
void vecl_init(vector* vec);
void vecl_resize_val(vector* vec, uint new_size, long val);
uint vecl_find(vector* vec, long element, uint offset);
uint vecl_find_last(vector* vec, long element, uint offset);
uint vecl_has(vector* vec, long element);
long* vecl_at(vector* vec, uint pos);
long vecl_at_cp(vector* vec, uint pos);
long* vecl_front(vector* vec);
long vecl_front_cp(vector* vec);
long* vecl_back(vector* vec);
long vecl_back_cp(vector* vec);
void vecl_push_back(vector* vec, long element);
void vecl_insert(vector* vec, uint pos, long element);
void vecl_replace(vector* vec, uint pos, long element);

// =========================== UNSIGNED LONG =========================

vector* vecul_create();
void vecul_init(vector* vec);
void vecul_resize_val(vector* vec, uint new_size, unsigned long val);
uint vecul_find(vector* vec, unsigned long element, uint offset);
uint vecul_find_last(vector* vec, unsigned long element, uint offset);
uint vecul_has(vector* vec, unsigned long element);
unsigned long* vecul_at(vector* vec, uint pos);
unsigned long vecul_at_cp(vector* vec, uint pos);
unsigned long* vecul_front(vector* vec);
unsigned long vecul_front_cp(vector* vec);
unsigned long* vecul_back(vector* vec);
unsigned long vecul_back_cp(vector* vec);
void vecul_push_back(vector* vec, unsigned long element);
void vecul_insert(vector* vec, uint pos, unsigned long element);
void vecul_replace(vector* vec, uint pos, unsigned long element);

// =========================== FLOAT =========================

vector* vecf_create();
void vecf_init(vector* vec);
void vecf_resize_val(vector* vec, uint new_size, float val);
uint vecf_find(vector* vec, float element, uint offset);
uint vecf_find_last(vector* vec, float element, uint offset);
uint vecf_has(vector* vec, float element);
float* vecf_at(vector* vec, uint pos);
float vecf_at_cp(vector* vec, uint pos);
float* vecf_front(vector* vec);
float vecf_front_cp(vector* vec);
float* vecf_back(vector* vec);
float vecf_back_cp(vector* vec);
void vecf_push_back(vector* vec, float element);
void vecf_insert(vector* vec, uint pos, float element);
void vecf_replace(vector* vec, uint pos, float element);

// =========================== DOUBLE =========================

vector* vecd_create();
void vecd_init(vector* vec);
void vecd_resize_val(vector* vec, uint new_size, double val);
uint vecd_find(vector* vec, double element, uint offset);
uint vecd_find_last(vector* vec, double element, uint offset);
uint vecd_has(vector* vec, double element);
double* vecd_at(vector* vec, uint pos);
double vecd_at_cp(vector* vec, uint pos);
double* vecd_front(vector* vec);
double vecd_front_cp(vector* vec);
double* vecd_back(vector* vec);
double vecd_back_cp(vector* vec);
void vecd_push_back(vector* vec, double element);
void vecd_insert(vector* vec, uint pos, double element);
void vecd_replace(vector* vec, uint pos, double element);