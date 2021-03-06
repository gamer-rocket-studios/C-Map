#ifndef C_MAP_HEADER
#define C_MAP_HEADER

#include <stdlib.h>
#include <string.h>

struct ElementIndexer
{
  size_t iStart;
  size_t byteSize;
  
  size_t eStart;
  size_t elementByteSize;
};

typedef struct MapReturnPackage
{
  void* data;
  int length;
} InfoPackage;

/**
* Returns the key element index if found (0 or greater).
* Returns -1 if the key does not exist.
*/
int map_keyExists(void* key, size_t keyByteSize);

/**
* Inserts a key index into the map. DOES NOT CHECK IF KEY ALREADY EXISTS,
* it is up to the programmer to check this using the map_keyExists() function.
*
* Structs, variables, and even arrays can be used as keys. As long as the correct
* byteSize is provided.
*/
void map_insertKey(void* key, size_t keyByteSize);

/**
* Returns 0 if the insertion was unsuccessful.
* This function does check if the key exists.
* It is possible to transfer arrays of data, as long
* as the correct byteSize is provided. Both the key and
* the element can be stack allocated. This function will
* copy the data over.
*/
int map_insertElement(void* key, void* element, size_t keySize, size_t elementSize);

/**
* Call the map_keyExists function in the
* first argument.
*
* The length variable inside the InfoPackage
* will have a value of -1 if the element
* retrieval was unsuccessful.
*
* Only provide the datatype/struct size.
* If you are expecting an array of elements,
* still only provide the size of the element.
*
* The data pointer variable returned is
* heap allocated and must be freed when 
* no longer in use.
*/
InfoPackage map_getElement(int keyIndex, size_t datatypeSize);

void map_init();

#endif
