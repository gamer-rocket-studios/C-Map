#include "c_map.h"

struct BadExample
{
  float x;
  float y;
};

typedef struct ExampleForExample
{
  char foo;
  long long bar;
} Example;

struct ExampleStruct
{
  int x;
  long long y;
  short z;
  Example fourthDimension;
};

void examplePrint(char* type, int result)
{
  char* did = "was";
  char* didNOT = "was NOT";
  char* resultString;
  
  if(result != -1)
    resultString= did;
  else
    resultString = didNOT;
    
  printf("The %s key %s found.\n", type, resultString);
}

int main(void)
{
  map_init();
  //c_map demonstration and information
  
  //You must first insert a key
  map_insertKey("example1", strlen("example1"));
  
  //Now you can fetch this key at any time.
  //Let's give it something to fetch.
  int valueint = 17;
  map_insertElement("example1", &valueint, strlen("example1"), sizeof(int));
  
  //Now we can grab this value.
  //This function always returns an InfoPackage
  //The first variable is your data.
  //The second is the element count, in case you stored an array in your key.
  
  //You always call the keyExists function in the first argument.
  InfoPackage info = map_getElement(map_keyExists("example1", strlen("example1")), sizeof(int));
  
  //The "length" variable in the InfoPackage struct will be set to -1 if the element fetch failed.
  
  //Now you need to "unpack" your package.
  int* dataint = (int*) info.data;
  printf("%d", dataint[0]);
  
  //All data you fetch is heap allocated. It must be freed once it is no longer in use.
  //Do NOT free the InfoPackage variable. Free the void*
  free(info.data);
  
  //You could also free dataint in this situation.
  
  
  
  
  
  //Now that the data is freed, you can re-insert an entity into the same key.
  //The size is dynamic, meaning you can freely insert whatever you want at any time.
  
  //For example, let's insert an integer array.
  int arrayExample[25] = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  arrayExample[13] = 17;
  arrayExample[24] = 9383;
  
  map_insertElement("example1", &arrayExample, strlen("example1"), sizeof(arrayExample));
  
  //You will be able to fetch this no problem, despite the data being 24 times larger than before.
  info = map_getElement(map_keyExists("example1", strlen("example1")), sizeof(int));
  dataint = (int*) info.data;
  printf("\nStart of integer array:\n");
  for(int i = 0; i < info.length;i++)
    printf("%d: %d\n", i, dataint[i]);
  printf("End of integer array.\n");
  
  free(dataint);
  
  
  
  
  
  //Of course you can add more keys.
  //You can use ANYTHING as a key. Structs, strings, datatypes, even arrays!
  //Floating point numbers did seem to work in testing. But floats and doubles can be inaccurate.
  //You are free to try using floats inside structs as keys, or floats themselves as keys...
  //More testing is needed.
  
  size_t stringLength = strlen("example2");
  map_insertKey("example2", stringLength);
  
  int integerKey = 4;
  map_insertKey(&integerKey, sizeof(int));
  
  map_insertKey(&arrayExample, sizeof(arrayExample));
  
  struct ExampleStruct exam = {6, 7938L, 2, {88, 7373838L}};
  map_insertKey(&exam, sizeof(struct ExampleStruct));
  
  Example structArray[3];
  map_insertKey(&structArray, sizeof(structArray));
  
  float badExample1 = 23.45f;
  map_insertKey(&badExample1, sizeof(float));
  
  struct BadExample badExample2 = {99.99f, 3};
  map_insertKey(&badExample2, sizeof(struct BadExample));
  
  
  //The following demonstration will show if each key exists or not.
  //The arrays and integers should work.
  //The floats seem to work here. But more testing is needed.
  
  examplePrint("string", map_keyExists("example2", stringLength));
  examplePrint("integer", map_keyExists(&integerKey, sizeof(int)));
  examplePrint("integer array", map_keyExists(&arrayExample, sizeof(arrayExample)));
  examplePrint("structure", map_keyExists(&exam, sizeof(struct ExampleStruct)));
  examplePrint("structure array", map_keyExists(&structArray, sizeof(structArray)));
  examplePrint("float", map_keyExists(&badExample1, sizeof(float)));
  examplePrint("float-containing structure", map_keyExists(&badExample2, sizeof(struct BadExample)));
  
  examplePrint("not-yet-added string", map_keyExists("example3that'sbad", strlen("example3that'sbad")));
  int notyet = 24;
  examplePrint("not-yet-added integer", map_keyExists(&notyet, sizeof(int)));
  long long notyetarray[4] = {9383838L, 838383828892L, 999L, 0L};
  examplePrint("not-yet-added long long integer", map_keyExists(&notyetarray, sizeof(notyetarray)));
  
  map_insertKey(&notyetarray, sizeof(notyetarray));
  examplePrint("now-added long long integer", map_keyExists(&notyetarray, sizeof(notyetarray)));
    
  
  return 0;
}

/* Console Output:
17
Start of integer array:
0: 0
1: 1
2: 0
3: 0
4: 0
5: 0
6: 0
7: 0
8: 0
9: 0
10: 0
11: 0
12: 0
13: 17
14: 0
15: 0
16: 0
17: 0
18: 0
19: 0
20: 0
21: 0
22: 0
23: 0
24: 9383
End of integer array.
The string key was found.
The integer key was found.
The integer array key was found.
The structure key was found.
The structure array key was found.
The float key was found.
The float-containing structure key was found.
The not-yet-added string key was NOT found.
The not-yet-added integer key was NOT found.
The not-yet-added long long integer key was NOT found.
The now-added long long integer key was found.

Process finished.
*/
