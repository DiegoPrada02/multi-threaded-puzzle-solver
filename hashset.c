#include "solver.h"
hashset set_init() {
  hashset h = {(node **)malloc(BINS * sizeof(node *)), 0, BINS};
  for (int i = 0; i < BINS; i++)
    h.table[i] = NULL;
  return h;
}
static int hash(char *key, int bins) {
  unsigned hashval = 0;
  for (int i = 0; i < strlen(key); i++)
    hashval = 31 * hashval + key[i];
  return hashval % bins;
}
static void rehash(hashset*);
int insert(hashset *h, char *val) {
	if(search(*h, val)) // I found the value
		return 0;// duplicate
	if (h->size >= h->bins)//load factor >= 100%
		rehash(h);
	int index = hash(val, h->bins);
	node *new_element = (node *)malloc(sizeof(node));
	new_element->next = h->table[index];
	new_element->value = strdup(val);
	h->table[index] = new_element;
	h->size++;
	return 1;
}
static int next_prime(int min) {
  while (1) {
    int prime = 1;
    for (int div = 2; div <= sqrt(min); div++)
      if (min % div == 0) {//divisible by div
        prime = 0;//not prime
        break;
      }
    if (prime)
      return min;
    else
      min++;
  }
  return min;
}
static void rehash(hashset *h) {
	int next_size = next_prime(2 * h->bins);
	//fprintf(stderr, "Warning: rehashing from size %d to size %d\n", 
		//h->bins, next_size); 
	fflush(stderr);
	node **oldtable = h->table;
	int old_bins = h->bins;
	h->table = (node **)malloc(next_size * sizeof(node *));
	for (int i = 0; i < next_size; i++)
		h->table[i] = NULL;
	h->size = 0;
	h->bins = next_size;
	for (int i = 0; i < old_bins; i++){
		for (node *it = oldtable[i]; it; it = it->next){
			insert(h, it->value);
		}
	}
}
int search(hashset h, char *val) {
  int index = hash(val, h.bins);
  for (node *iterator = h.table[index]; iterator; iterator = iterator->next)
  {
	if (!strcmp(iterator->value, val))
		return 1;
  }
  return 0;
}
