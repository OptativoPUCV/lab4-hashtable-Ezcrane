#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap 
{
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) 
{
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) 
{
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) 
    {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2)
{
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
  int aux = hash(key, map->capacity);
  while(map->buckets[aux]!=NULL && map->buckets[aux]->key!=NULL)
    {
      aux = (aux+1)%map->capacity;
    }
  if(map->buckets[aux]==NULL)
  {
    map->buckets[aux] = createPair(key, value);
    map->size++;
  }
  else
  {
    map->buckets[aux]->value = value;
  }
}

void enlarge(HashMap * map) 
{
  enlarge_called = 1; //no borrar (testing purposes)
  int old_capacity = map->capacity;
  Pair** old_array = map->buckets;
  map->capacity = 2 * old_capacity;
  map->buckets = (Pair**)calloc(map->capacity, sizeof(Pair*));
  map->size = 0;
  for(int i = 0; i < old_capacity; i++)
    {
      if(old_array[i] != NULL)
      {
        insertMap(map, old_array[i]->key, old_array[i]->value);
        free(old_array[i]);
      }
    }
  free(old_array);
}


HashMap * createMap(long capacity) 
{
  HashMap * map = (HashMap *)malloc(sizeof(HashMap) * capacity);
  if(map == NULL) return NULL;
  map->buckets = (Pair **)calloc(capacity, sizeof(Pair*));
  if(map->buckets == NULL) return NULL;
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  return map;
}

void eraseMap(HashMap * map,  char * key) 
{    
  int posicion = hash(key, map->capacity);
  while(map->buckets[posicion]!=NULL && !is_equal(map->buckets[posicion]->key, key))
    {
      posicion = (posicion+1)%map->capacity;
    }
  if(map->buckets[posicion]!=NULL && is_equal(map->buckets[posicion]->key, key))
  {
    map->buckets[posicion]->key = NULL;
    map->size--;
  }
}

Pair * searchMap(HashMap * map,  char * key) 
{   
  int posicion = hash(key, map->capacity);
  while(map->buckets[posicion]!=NULL && !is_equal(map->buckets[posicion]->key, key))
    {
      posicion = (posicion+1)%map->capacity;
    }
  if(map->buckets[posicion]!=NULL && is_equal(map->buckets[posicion]->key, key))
  {
    map->current = posicion;
    return map->buckets[posicion];
  }
  else return NULL;
}

Pair * firstMap(HashMap * map) 
{
  for(int i = 0; i < map->capacity; i++)
    {
      if(map->buckets[i]!=NULL)
      {
        map->current = i;
        {
          return map->buckets[i];
        }
      }
    }
}

Pair * nextMap(HashMap * map) 
{
  return map->buckets[map->current+1];
}
