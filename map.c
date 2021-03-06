#include "map.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h> 

#define INVALID_SIZE -1

typedef struct Map_Node_t {
  char* key;
  char* value;
  struct Map_Node_t* next;
} *Map_Node;

struct Map_t {
  Map_Node elements;
  Map_Node iterator;
};

//-------------------------------------------------------------------//
/*HELPER FUNCTION DECLARATIONS*/

/**
*	createMapNode: Creates a new Map_Node.
*
* @param key - The key of the new node
* @param data - the value of the new node
* @return
* 	NULL if memory allocation failed
* 	new Map_Node if successful
*/
static Map_Node createMapNode(const char* key, const char* data);

/**
*	updateValue: updates the value of a node.
* assumes the node is NOT null.
*
* @param ptr - The node to update
* @param data - the new value of the node
* @return
* 	MAP_OUT_OF_MEMORY if memory allocation for the new value failed
* 	MAP_SUCCESS if allocation was successful
*/
static MapResult updateValue(Map_Node ptr, const char* data);

/**
*	destroyMapNode: frees the memory of the key and value, 
* then frees the node itself.
* MUST make sure removing the node won't ruin the list.
*
* @param ptr - The node to destroy
*/
static void destroyMapNode(Map_Node ptr);

//-------------------------------------------------------------------//
/*HELPER FUNCTIONS*/

static Map_Node createMapNode(const char* key, const char* data) {
  Map_Node ptr = malloc(sizeof(*ptr));
  if(ptr == NULL) {
    return NULL;
  }

  char* key_copy = malloc(strlen(key)+1);
  if(key_copy == NULL) {
    free(ptr);
    return NULL;
  }
  strcpy(key_copy, key);

  char* data_copy = malloc(strlen(data)+1);
  if(data_copy == NULL) {
    free(key_copy);
    free(ptr);
    return NULL;
  }
  strcpy(data_copy, data);

  ptr->key = key_copy;
  ptr->value = data_copy;
  ptr->next = NULL;

  return ptr;
}

static MapResult updateValue(Map_Node ptr, const char* data) {
  assert(ptr);
  assert(ptr->key && ptr->value);

  char* data_copy = malloc(strlen(data)+1);
  if(data_copy == NULL) {
    return MAP_OUT_OF_MEMORY;
  }

  strcpy(data_copy, data);
  free(ptr->value);
  ptr->value = data_copy;

  return MAP_SUCCESS;
}

static void destroyMapNode(Map_Node ptr) {
  assert(ptr);
  assert(ptr->key && ptr->value);

  free(ptr->key);
  free(ptr->value);
  free(ptr);
}

//-------------------------------------------------------------------//
/*ADT FUNCTIONS*/

//1
Map mapCreate()
{
    Map new_map = malloc(sizeof(*new_map));
    if(!new_map)
    {
      return NULL;
    }
    new_map->elements = NULL;
    new_map->iterator = NULL;
    return new_map;
}


//2
void mapDestroy(Map map) {
  if(map) {
    mapClear(map);
    free(map);
  }
}

//3
Map mapCopy(Map map) {
  if(map == NULL) {
    return NULL;
  }

  map->iterator = NULL;

  if(map->elements == NULL) {
    assert(map->iterator == NULL);
    return mapCreate();
  }

  Map new_map = mapCreate();
  if(new_map == NULL) {
    return NULL;
  }

  Map_Node ptr = map->elements;
  MapResult result;
  while(ptr) {
    result = mapPut(new_map, ptr->key, ptr->value);
    if(result == MAP_OUT_OF_MEMORY) {
      mapDestroy(new_map);
      return NULL;
    }
    ptr = ptr->next;
  }

  return new_map;
}

//4
int mapGetSize(Map map) {
  if(map == NULL) {
    return INVALID_SIZE;
  }

  if(map->elements == NULL) { //empty map
    assert(map->iterator == NULL);
    return 0;
  }

  int counter = 0;
  Map_Node ptr = map->elements;

  while(ptr) {
    counter++;
    ptr = ptr->next;
  }

  return counter;
}

//5
bool mapContains(Map map, const char* key)
{
  if(map == NULL || key == NULL)
  {
    return false;
  }

  Map_Node ptr = map->elements;

  while(ptr != NULL)
  {
    if(!strcmp(ptr->key, key))
    {
      return true;
    }
    else
    {
      ptr = ptr -> next;
    }
    
  }
  return false;
}

//6
MapResult mapPut(Map map, const char* key, const char* data) {
  if(map == NULL || key == NULL || data == NULL) {
    return MAP_NULL_ARGUMENT;
  }

  map->iterator = NULL;

  if(map->elements == NULL) { //empty map
    map->elements = createMapNode(key, data);
    if(map->elements == NULL) {
      return MAP_OUT_OF_MEMORY;
    }

    return MAP_SUCCESS;
  }

  Map_Node ptr = map->elements;

  while(ptr) {
    if(!strcmp(ptr->key, key)) { //key exists, update value and return
      return updateValue(ptr, data);
    }

    if(ptr->next == NULL) { //reached end of list
      break;
    }

    ptr = ptr->next;
  }
  
  assert(!mapContains(map, key));
  ptr->next = createMapNode(key, data); //create new key-value pair at end of list
  if(ptr->next == NULL) {
    return MAP_OUT_OF_MEMORY;
  }

  return MAP_SUCCESS;
}

//7
char* mapGet(Map map, const char* key)
{
  if(map == NULL || key == NULL) {
    return NULL;
  }
  
  Map_Node ptr = map->elements;

  while(ptr != NULL)
  {
    if(!strcmp(ptr->key, key))
    {
      return ptr->value;
    }
    else
    {
      ptr = ptr->next;
    }
  }
  return NULL;
}

//8
MapResult mapRemove(Map map, const char* key) {
  if(map == NULL || key == NULL) {
    return MAP_NULL_ARGUMENT;
  }

  map->iterator = NULL;

  Map_Node ptr = map->elements;

  if(ptr == NULL) { //empty map
    return MAP_ITEM_DOES_NOT_EXIST;
  }

  if(!strcmp(ptr->key, key)) { //need to remove the first key
    map->elements = ptr->next;
    destroyMapNode(ptr);
    return MAP_SUCCESS;
  }

  while(ptr->next) {
    Map_Node toDelete = ptr->next;
    
    if(!strcmp(toDelete->key, key)) { 
      ptr->next = toDelete->next;
      destroyMapNode(toDelete);
      return MAP_SUCCESS;
    }

    ptr = ptr->next;
  }

  return MAP_ITEM_DOES_NOT_EXIST;
}

//9
char* mapGetFirst(Map map)
{
  if(map == NULL) {
    return NULL;
  }

  if(map->elements == NULL) {
    return NULL;
  }

  map->iterator = map->elements;
  return map->iterator->key;
}

//10
char* mapGetNext(Map map) {
  if(map == NULL) {
    return NULL;
  }

  if(map->iterator == NULL) {
    return NULL;
  }

  if(map->iterator->next == NULL) {
    return NULL;
  }

  map->iterator = map->iterator->next;
  return map->iterator->key;
}

//11
MapResult mapClear(Map map)
{
  if(map == NULL)
  {
    return MAP_NULL_ARGUMENT;
  }

  if(map->elements == NULL) {
    assert(map->iterator == NULL);
    return MAP_SUCCESS;
  }

  Map_Node ptr = map->elements;

  while(ptr)
  {
    Map_Node nextNode = ptr->next;
    destroyMapNode(ptr);
    ptr = nextNode;
  }

  map->elements = NULL;
  map->iterator = NULL;

  return MAP_SUCCESS;
}