#include "markov_chain.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define DIGIT 10




int get_random_number (int max_number)
{
  return rand() %max_number;
}

MarkovNode *get_first_random_node(MarkovChain *markov_chain)
{
  if (markov_chain == NULL || markov_chain->database == NULL)
  {
    return NULL;
  }
  while (true)
  {
    int random = get_random_number (markov_chain->
        database->size);
    Node *curr_node = markov_chain->database->first;
    if(!curr_node) {return NULL;}
    for (int i = 0; i < random; i++)
    {
      curr_node = curr_node->next;
    }
    if (markov_chain->is_last(curr_node->data->data)== false)
    {
      return curr_node->data;
    }
  }}


MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
  if(state_struct_ptr == NULL){return NULL;}

  int counter = 0;
  for (int i = 0; i < state_struct_ptr->frequency_length; i++)
  {
    counter += (int)state_struct_ptr->counter_list[i].frequency;
  }
  int j = get_random_number (counter);
  for (int i = 0; i < state_struct_ptr->frequency_length; i++)
  {
    j -= (int)state_struct_ptr->counter_list[i].frequency;
    if (j < 0)
    { return state_struct_ptr->counter_list[i].markov_node; }
  }
  return NULL;
}


void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  if(markov_chain == NULL){return ;}
  int counter = 0;

  if(!first_node){
  first_node = get_first_random_node (markov_chain);}

  if(!first_node){ return;}

  markov_chain->print_func(first_node->data);
  counter++;

  while (counter < max_length)
  {
    MarkovNode *second_node = get_next_random_node (first_node);
    if(!second_node){ return;}
    markov_chain->print_func(second_node->data);

    if(markov_chain->is_last(second_node->data))
    {
      printf ("\n");
      return;
    }
    first_node = second_node;
    counter++;
  }
  printf ("\n");}


void free_markov_chain(MarkovChain **markov_chain)
{

    if(*markov_chain == NULL){ return;}
    if((*markov_chain)->database != NULL)
    {
      Node *current = (*markov_chain)->database->first;
      Node *next;

      while (current != NULL)
      {
        next = current->next;

        free(current->data->data);//free data
        free(current->data->counter_list);//free counter list
        free(current->data);//free markov node


        free(current);// free node

        current = next;
      }
    }
  free((*markov_chain)->database); //free linked list
  free (*markov_chain);//free markov chain
}

bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode
  *second_node, MarkovChain *markov_chain)
{
  if(markov_chain == NULL){return false;}
  NextNodeCounter * temp;

  for(int i = 0; i < first_node->frequency_length; i++)
  {
    MarkovNode * temp_markov =first_node->counter_list[i].markov_node;
    if(temp_markov == second_node)
    {
      if (!second_node){return false;}
      first_node->counter_list[i].frequency ++;
      return true;
    }
  }
  if (first_node->counter_list == NULL)
  {
    temp = malloc(sizeof(NextNodeCounter));
  } else
  {
    int freq_len = first_node->frequency_length;
    unsigned long wanted_len = (freq_len +1) * sizeof (NextNodeCounter);
    temp =realloc(first_node->counter_list,wanted_len);
  }
  if (!temp)
  {
    return false;
  }
  temp[first_node->frequency_length].frequency = 1;
  temp[first_node->frequency_length].markov_node = second_node;
  first_node->counter_list = temp;
  first_node->frequency_length ++;
  return true;
}

Node* get_node_from_database(MarkovChain *markov_chain, void *data_ptr)
{
  if(markov_chain == NULL){return NULL;}

  Node *p_node = markov_chain->database->first;
  if(!p_node){return NULL;}

  for (int i = 0; i < markov_chain->database->size; ++i)
  {
    if (markov_chain->comp_func(p_node->data->data,data_ptr)==0)
    {
      return p_node;
    }
    p_node = p_node->next;
  }
  return NULL;
}

MarkovNode * new_marko_node (MarkovChain * markov_chain_ptr, void *data_ptr)
{
  if(markov_chain_ptr == NULL){return NULL;}

  MarkovNode * new_markov = malloc (sizeof (MarkovNode));
  if(!new_markov)
  {
      return NULL;
  }

  void * data = markov_chain_ptr->copy_func(data_ptr);
    if(!data){return NULL;}

  *new_markov = (MarkovNode){data,NULL,0};

  return new_markov;
}

Node* add_to_database(MarkovChain *markov_chain, void *data_ptr)
{
  if(markov_chain == NULL){return NULL;}
  //the word is already in the list

  Node * p_node = get_node_from_database (markov_chain,data_ptr);
  if (p_node){return p_node;}


  //the word is not on the list
  MarkovNode * p_markov =  new_marko_node (markov_chain,data_ptr);
  if(!p_markov){return NULL;}

  if(add (markov_chain->database, p_markov) != 0){return NULL;}
  return markov_chain->database->last;
}

int str_to_int (char * str)
{
  int result = 0;
  int length = (int)strlen(str);
  for (int i = 0; i < length; i++)
  {
    result = result * DIGIT + (str[i] - '0');
  }
  return result;
}

int fail_alloc(MarkovChain ** markov_chain)
{
  fprintf (stdout,ALLOCATION_ERROR_MASSAGE);
  free_markov_chain (markov_chain);
  return EXIT_FAILURE;
}

