#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#define LIST_MAXLEN_TEXT 100
#define MAX_WORD_LEN 40
#define MAX_LINE_LEN 100

typedef struct node_t {
    char text[LIST_MAXLEN_TEXT];
    struct node_t  *next;
} node_t;

typedef struct index_node {
    
    char word[MAX_WORD_LEN];
    char capped_word[MAX_WORD_LEN];
    char sentence[MAX_LINE_LEN];
    struct index_node  *next;

} index_node;

node_t *new_node(char *);
node_t *add_front(node_t *, node_t *);
node_t *add_end(node_t *, node_t *);
node_t *peek_front(node_t *);
node_t *remove_front(node_t *);
void    apply(node_t *, void(*fn)(node_t *, void *), void *arg);

index_node *new_index_node(char *, char *, char *);
index_node *add_back(index_node *, index_node *);
index_node *add_inorder(index_node *, index_node *);
index_node *remove_first(index_node *);




#endif
