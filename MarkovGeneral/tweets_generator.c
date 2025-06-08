#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "markov_chain.h"
#include "linked_list.h"

#define INVALID_ARGC "Usage: invalid amount of arguments"
#define INVALID_FILE "Error: this program can't find the given file"
#define LEN_WORD 100
#define LEN_LINE 1000
#define DOT '.'
#define ARG5 5
#define ARG4 4


static bool is_last_word(void * data)
{
  if(!data){return false;}
  char * word = (char *) data;
  int len = (int)strlen (word);
  char last_char = word[len - 1];
  if (last_char == DOT){return true;}
  return false;
}

static void free_str(void * str)
{
    if(!str){ return;}
    char * str1 = (char *)str;
    free(str1);
}

static void* copy_str(void * src)
{
  if(!src){return NULL;}
  char * str1 = (char *) src;
  int len = (int)strlen (str1);
  char * str2 = malloc (len*(sizeof (char ))+1);
  if(!str2)
  {
      free_str(src);
      return NULL;}
  strcpy(str2, str1);
  return str2;
}



static int comp_str(void * str1, void *str2)
{
  char * string_1 = (char *)str1;
  char * string_2 = (char *)str2;
  return strcmp (string_1,string_2);
}

static void print_str(void *data)
{
  char * string = (char*) data;
  printf ("%s",string);
  if(!is_last_word (string)){ printf (" ");}
}



static int valid_file(const char * path, FILE ** p_file)
{
  * p_file = fopen (path, "r");
  if (* p_file == NULL){return EXIT_FAILURE;}
  else {return EXIT_SUCCESS;}
}



static int fill_database(FILE *fp, int num_words, MarkovChain *markov_chain)
{

  char line[LEN_LINE];
  char *cur_word;

  Node *p_last_node = NULL;

  while (fgets(line, sizeof(line), fp) != NULL) {

    cur_word = strtok(line, " \n\r");

    while (cur_word != NULL &&  num_words != 0) {

      // Process the current word

      Node *p_cur_node = add_to_database(markov_chain, cur_word);
      if (p_cur_node == NULL) {
          return EXIT_FAILURE;
      }

      if (p_last_node != NULL) {
        if (!add_node_to_counter_list(p_last_node->data,
                                      p_cur_node->data,markov_chain))
        {
          return EXIT_FAILURE;
        }
      }
      p_last_node = p_cur_node;

      if (markov_chain->is_last(cur_word)) {
        p_last_node = NULL;
      }

      num_words--;

      cur_word = strtok(NULL, " \r\n");
    }
  }

  fclose(fp);
  return EXIT_SUCCESS;
}

static MarkovChain * new_markov_chain()
{
  MarkovChain * markov_chain = malloc (sizeof (MarkovChain));
  if(!markov_chain){
      return NULL;}

  LinkedList *linked_list = malloc (sizeof (LinkedList));
  if(!linked_list){
      return NULL;}

  *linked_list = (LinkedList) {NULL, NULL, 0};
  markov_chain->database = linked_list;

  markov_chain->print_func = &print_str;
  markov_chain->comp_func = &comp_str;
  markov_chain->copy_func = &copy_str;
  markov_chain->is_last = &is_last_word;
  markov_chain->free_data = &free_str;

  return markov_chain;
}


int main(int argc, char *argv[])
{
  int num_words_to_read;
  if (argc == ARG4)
  { num_words_to_read = -1; }
  if (argc == ARG5)
  { num_words_to_read = str_to_int (argv[4]); }
  if (argc != ARG4 && argc != ARG5)
  {
    fprintf (stdout, INVALID_ARGC);
    return EXIT_FAILURE;
  }
  unsigned int seed = (unsigned int)str_to_int(argv[1]);
  int num_tweets = str_to_int (argv[2]);

  FILE *file;
  if (valid_file (argv[3], &file) == EXIT_FAILURE)
  {
    fprintf (stdout, INVALID_FILE);
    return EXIT_FAILURE;
  }
  srand (seed); //sets random function with given seed
  MarkovChain *p_markov_chain = new_markov_chain();

  if (!p_markov_chain)
  {
    return fail_alloc (&p_markov_chain);
  }

  int alloc = fill_database (file, num_words_to_read, p_markov_chain);
  if (alloc == 1)
  {
    return fail_alloc (&p_markov_chain);
  }
  for (int i = 1; i <= num_tweets; i++)
  {
    printf ("Tweet %d: ", i);
    generate_random_sequence (p_markov_chain, NULL, LEN_WORD);
  }
  free_markov_chain (&p_markov_chain);
  return EXIT_SUCCESS;}