#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


typedef struct bucket
{
    char * key;
    void * data;
    struct bucket * next;
}bucket_t;


typedef struct hash_table
{
    int size;
    bucket_t **entries;
}ht_t;

#if 0
unsigned int hash(const char *key,ht_t * table ) {
    int TABLE_SIZE = table->size;
    unsigned int hash_value = 0;
    unsigned int prime = 31;  // Простое число для умножения

    while (*key) {
        hash_value = (hash_value * prime + *key) % TABLE_SIZE;
        key++;
    }

    return hash_value;
}
#endif


// Djb2 hash function
unsigned long hash(char *str,ht_t * table) {
        int NUM_BUCKETS=table->size;
        unsigned long hash = 5381;
        int c;
        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        return hash % NUM_BUCKETS;

}


ht_t * create_hash_table( unsigned size ){
       ht_t * hashtable = malloc(sizeof(ht_t));
       hashtable->size = size;
       hashtable->entries = malloc(sizeof(bucket_t*)*size);
       for ( int i =0;i<size;++i ){
            hashtable->entries[i] = NULL;
       }
       return hashtable;
}


int counting_collisions( ht_t * hashtable ){
    unsigned size = hashtable->size;
    unsigned collis = 0;
    for (int i =0;i<size;++i){
        bucket_t * buck = hashtable->entries[i];
        while (buck!=NULL){
            if (buck->next!=NULL){
                collis+=1;
            }
            buck = buck->next;
        }
    }
    return collis;
}


void ht_set(ht_t * hash_table , char * key,void * value)
{
    unsigned slot = hash(key,hash_table);

    bucket_t * buck = hash_table->entries[slot];

    if (buck==NULL){
        buck = malloc(sizeof(bucket_t));
        buck->key = strdup(key);
        buck->data =strdup(value);
        buck->next = NULL;
        hash_table->entries[slot] = buck;
        return;
    }
    
    while(buck!=NULL){
        if (strcmp(key,buck->key)==0){
            free(buck->data);
            buck->data = strdup(value);
            return;
        }
        buck = buck->next;
    }
    bucket_t * new_bucket = malloc(sizeof(bucket_t));
    new_bucket->data = strdup(value);
    new_bucket->key = strdup(key);
    new_bucket->next = hash_table->entries[slot];
    hash_table->entries[slot] = new_bucket;
}


void free_hash_table(ht_t * table){
    int size = table->size;
    for (int i =0;i<size;++i){
        bucket_t * buck = table->entries[i];

        while (buck!=NULL){
            bucket_t * tmp = buck->next;

            free(buck->data);
            free(buck->key);
            free(buck);
            buck = tmp;
        }
    }
    free(table->entries);
    free(table);
}


void print_hash_table(ht_t * hashtable){
    for (int i =0;i<hashtable->size;++i){
        printf("%d:",i+1);
        if (hashtable->entries[i]==NULL){
            printf("->NULL\n");
        }
        bucket_t * buck = hashtable->entries[i];
        while(buck!=NULL){
            printf("key{%s}|value{%s}->",(char *)buck->key,(char*)buck->data);
            buck = buck->next;
        }
        printf("\n");
    }
}


char * get_by_key(char * key,ht_t * table){
    if (key==NULL || table==NULL)return NULL;
    int index = hash(key,table);
        bucket_t * buck = table->entries[index];
        while(buck!=NULL){
            if (strcmp(key,buck->key)==0){
                return (char *)buck->data;
            }
            buck = buck->next;
        }
    return NULL;
}

int main() {
    ht_t *table = create_hash_table(10);

    ht_set(table, "apple", "red");
    ht_set(table, "banana", "yellow");
    ht_set(table, "grape", "purple");
    ht_set(table, "orange", "orange");
    ht_set(table, "watermelon", "green");
    ht_set(table, "strawberry", "red");
    ht_set(table, "cherry", "red");
    ht_set(table, "lemon", "yellow");
    ht_set(table, "lime", "green");
    ht_set(table, "blueberry", "blue");
    ht_set(table, "blackberry", "black");
    ht_set(table, "pear", "green");
    ht_set(table, "peach", "pink");
    ht_set(table, "plum", "purple");
    ht_set(table, "mango", "orange");


    char * get_apple = get_by_key("huj",table);
    assert(get_apple==NULL);
    
    int collisions = counting_collisions(table);
    printf("In this table:%d collisions\n",collisions);
    print_hash_table(table);
    free_hash_table(table);
    
}