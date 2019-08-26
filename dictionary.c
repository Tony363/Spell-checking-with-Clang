#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dictionary.h"

typedef struct node
{
    bool is_word;
    struct node *children[27];
} node;

node *first;
int word_count = 0;

int get_index(char c);
node *create_init_node();
bool unload_helper(node *node_ptr);


bool check(const char *word)
{
    node *node_ptr = first;
    for (int i = 0,  len = strlen(word); i <= len; i++)
    {
        if (word[i] == '\0')
        {
            if (node_ptr->is_word == true)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        int index = get_index(word[i]);// [......t]
        if (node_ptr->children[index] == NULL)
        {
            return false;
        }
        node_ptr = node_ptr->children[index];


    }
    return false;
}

bool load(const char *dictionary)
{
    FILE *dict_ptr = fopen(dictionary, "r");
    if (dict_ptr == NULL)
    {
        fprintf(stderr, " File does not exist! \n");
        return false;
    }
    //GLOBAL first gets a new_and_initialized node
    first = create_init_node();//NBM node* first = create_init_node();

    // initialize c to get char from dicitonary and check until we reach EOF
    //node* first = create_init_node();

    node *node_ptr = first;
    for (char c = fgetc(dict_ptr); c != EOF; c = fgetc(dict_ptr))
    {
        // if the letter is not a newline char then we need to load this letter in our trie
        if (c != '\n')
        {
            int index = get_index(c);
            if (node_ptr->children[index] == NULL)
            {
                node_ptr->children[index] = create_init_node();
            }
            node_ptr = node_ptr->children[index];
        }
        else
        {
            //mark the end of the word
            node_ptr-> is_word = true;
            //increment word_count coz we found a complete word from dict_ptr
            word_count++;
            //go back to homebase
            node_ptr = first;


        }
    }
    return true;
}
//returns the index corresponding to the character entered
int get_index(char c)
{
    if (c == '\'') //last index
    {
        return 26;
    }
    else if (c >= 'a' && c <= 'z')
    {
        return c - 'a';
    }
    else if (c >= 'A' && c <= 'Z')
    {
        return c - 'A';
    }//error index
    return -1;
}

//returns a pointer to a node with initial values : false and all NULL
node *create_init_node()
{
    node *node_ptr = malloc(sizeof(node));
    node_ptr->is_word = false;
    for (int i = 0; i < 27; i++)
    {
        node_ptr->children[i] = NULL;
    }
    return node_ptr;
}

unsigned int size(void)
{
    return word_count;
}

bool unload(void)
{

    if (unload_helper(first))
        ;
    {
        return true;
    }
    return false;
}

bool unload_helper(node *node_ptr)
{

    for (int i = 0; i < 27; i++)
    {

        if (node_ptr->children[i] != NULL)
        {
            unload_helper(node_ptr->children[i]);
        }

    }
    // now you know that everyting is emptied/null
    free(node_ptr);
    return true;
}
