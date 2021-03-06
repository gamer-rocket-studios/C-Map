#include "c_map.h"

//Made by Dustin Priest "Rocket"
//Hope this proves useful!

size_t map__edata_size_count = 0;
size_t map__edata_size_max = 4096;
char* map__edata;

size_t map__index_size_count = 0;
size_t map__index_size_max = 128;
size_t map__index_size = sizeof(struct ElementIndexer);
struct ElementIndexer* map__index;

size_t map__idata_size_count = 0;
size_t map__idata_size_max = 8 * 128;
char* map__idata;


void* map__expandData(char* original, size_t oSize, size_t newSize)
{
  char* buffer = (char*) malloc(newSize);
  
  for(size_t i = 0; i < oSize;i++)
    buffer[i] = original[i];
    
  free(original);
  return buffer;
}

void map__transferData(char* sender, char* recipient, size_t sstart, size_t rstart, size_t size)
{
  for(size_t i = 0; i < size;i++)
    recipient[i + rstart] = sender[i + sstart];
}

int map__compare(char* o1, char* o2, size_t size, size_t o1start, size_t o2start)
{
  for(size_t i = 0; i < size;i++)
    if(o1[i + o1start] != o2[i + o2start]) return 0;
    
  return 1;
}

int map__allocateElement(size_t byteSize)
{
  while((map__edata_size_count + byteSize) >= map__edata_size_max)
    map__edata = (char*) map__expandData(map__edata, map__edata_size_count, map__edata_size_max *= 2);
    
  size_t result = map__edata_size_count;
  map__edata_size_count += byteSize;
  
  return result;
}

struct ElementIndexer map__allocateIndex(char* data, size_t byteSize)
{
  struct ElementIndexer info;
  
  while((map__idata_size_count + byteSize) >= map__idata_size_max)
    map__idata = (char*) map__expandData(map__idata, map__idata_size_count, map__idata_size_max *= 2);
  
  info.iStart = map__idata_size_count;
  map__idata_size_count += byteSize;
  
  info.byteSize = byteSize;
  map__transferData(data, map__idata, 0, info.iStart, byteSize);
  
  info.eStart = map__allocateElement(1);
  info.elementByteSize = 1;
  return info;
}

int map_keyExists(void* key, size_t keyByteSize)
{
  for(size_t i = 0; i < map__index_size_count;i++)
  {
    struct ElementIndexer index = map__index[i];
    if(index.byteSize != keyByteSize) continue;
    
    char* indexKey = (char*) key;
    if(map__compare(indexKey, map__idata, keyByteSize, 0, index.iStart))
      return i;
  }
  
  return -1;
}

void map_insertKey(void* key, size_t keyByteSize)
{
  if(map__index_size_count == map__index_size_max)
  {
    size_t byteSize = map__index_size * map__index_size_max;
    size_t newByteSize = (map__index_size_max *= 2) * map__index_size;
    map__index = (struct ElementIndexer*) map__expandData((char*) map__index, byteSize, newByteSize);
  }
  
  struct ElementIndexer index = map__allocateIndex((char*) key, keyByteSize);
  map__index[map__index_size_count++] = index;
}

int map_insertElement(void* key, void* element, size_t keySize, size_t elementSize)
{
  int index = map_keyExists(key, keySize);
  if(index == -1) return 0;
  
  struct ElementIndexer* keyIndex = &map__index[index];
  if(keyIndex->elementByteSize < elementSize)
  {
    keyIndex->eStart = map__allocateElement(elementSize);
    keyIndex->elementByteSize = elementSize;
  }
  
  map__transferData((char*) element, map__edata, 0, keyIndex->eStart, elementSize);
  return 1;
}

InfoPackage map_getElement(int keyIndex, size_t datatypeSize)
{
  InfoPackage result;
  if(keyIndex == -1)
  {
    result.length = keyIndex;
    return result;
  }
  
  struct ElementIndexer info = map__index[keyIndex];
  result.length = info.elementByteSize / datatypeSize;
  
  char* buffer = (char*) malloc(info.elementByteSize);
  map__transferData(map__edata, buffer, info.eStart, 0, info.elementByteSize);
  result.data = buffer;
  
  return result;
}

void map_init()
{
  map__edata = (char*) malloc(4096);
  map__index = (struct ElementIndexer*) malloc(map__index_size * 128);
  map__idata = (char*) malloc(8 * 128);
}
