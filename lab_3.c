#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

int get_priority(char op) {
    switch (op) {
        case '(': return 0;
        case ')': return 1;
        case '+': return 2;
        case '-': return 2;
        case '*': return 3;
        case '/': return 3;
        case '^': return 4;
        case '~': return 5; 
        default:  return -1;
    }
}
typedef struct Word
{
    char type;
    char *sigh;
    int size;
} Word;

typedef struct Node{
    struct Node *right;
    struct Node *left;
    Word *value;
} Node;

Node *create_node(Word *op){
    Node *node = malloc(sizeof(Node));
    if(!node){
        printf("Error memory\n");
        return NULL;
    }
    node->right = NULL;
    node->left = NULL;
    node->value = op;
    return node;
}

void add_operator_to_tree(Node ***stack_tree, Node *node, int *size) {
    Node **stack = *stack_tree;
    int top = *size - 1;
    node->right = stack[top];
    node->left  = stack[top - 1];
    (*size)--;
    Node **new_stack = realloc(stack, (*size) * sizeof(Node*));
    if (*size > 0 && !new_stack) {
        printf("Memory error\n");
        free(node);
        (*size)++;
        return; 
    }
    new_stack[*size - 1] = node;
    *stack_tree = new_stack;
}

void add_un_minus_to_tree(Node ***stack_tree, Node *node, int *size) {
    Node **stack = *stack_tree;
    int top = *size - 1;
    node->right = stack[top];
    node->left = NULL;
    stack[*size - 1] = node;
    *stack_tree = stack;
}

void add_num_to_tree_stack(Node ***stack_tree, Node *node, int size) {
    Node **stack = *stack_tree;
    Node **new_stack = realloc(stack, size * sizeof(Node*));
    if (!new_stack && size > 0) { 
        printf("Memory error\n"); 
        return; 
    }
    new_stack[size - 1] = node;
    *stack_tree = new_stack;
}

Node *Tree(Word **stack, int len){
    Node **tree_stack = NULL;
    int size = 0;
    for (int i = 0; i < len; i++){
        if(stack[i]->type == 'o'){
            Node *node = create_node(stack[i]);
            int pr = get_priority(stack[i]->sigh[0]);
            if (pr == 5){
                add_un_minus_to_tree(&tree_stack, node, &size);
            }else{
                add_operator_to_tree(&tree_stack, node, &size);
            }
        }else{
            Node *node = create_node(stack[i]);
            size++;
            add_num_to_tree_stack(&tree_stack, node, size);
        }
    }
    if (size != 1) {
        printf("Error: invalid expression tree\n");
        for (int i = 0; i < size; i++) {
            free(tree_stack[i]);
        }
        free(tree_stack);
        return NULL;
    }
    Node *root = tree_stack[0];
    free(tree_stack);
    return root;
}


void delete_word(Word *op) {
    if (op) {
        free(op->sigh);
        free(op);
    }
}

void print_word(Word *op) {
    if (!op) return;
    for (int i = 0; i < op->size; i++){
        char s = op->sigh[i];
        printf("%c", op->sigh[i]);
    }
    return;
}

Word **Dey(Word **stack, int len, int *new_len){
    Word **end_stack = malloc(sizeof(Word*)*len);
    int end_size = 0;
    Word **help_stack = malloc(sizeof(Word*)*len);
    int help_size = 0;
    for (int i = 0; i < len; i++){
        if(stack[i]->type != 'o'){
            end_stack[end_size] = stack[i];
            end_size++;
        }else{
            int pr = get_priority(stack[i]->sigh[0]);
            if (help_size == 0){
                help_stack[help_size] = stack[i];
                help_size++;
            }
            else if (pr == 0){
                help_stack[help_size] = stack[i];
                help_size++;
            }else if(pr == 1){
                while (get_priority(help_stack[help_size-1]->sigh[0]) != 0){
                    end_stack[end_size] = help_stack[help_size-1];
                    help_size = help_size-1;
                    end_size++;
                }
                help_size = help_size-1;
            }else if(pr <= get_priority(help_stack[help_size-1]->sigh[0])){
                while (pr <= get_priority(help_stack[help_size-1]->sigh[0])){
                    end_stack[end_size] = help_stack[help_size-1];
                    help_size--;
                    end_size++;
                    if(help_size == 0){
                        break;
                    }
                }
                help_stack[help_size] = stack[i];
                help_size++;
            }else if(pr > get_priority(help_stack[help_size-1]->sigh[0])){
                help_stack[help_size] = stack[i];
                help_size++;
            }
        }
    }
    while (help_size > 0){
        end_stack[end_size] = help_stack[help_size-1];
        help_size = help_size-1;
        end_size++;
    }
    free(help_stack);
    *new_len = end_size;
    return end_stack;

}

int is_num(char n) {
    switch (n) {
        case '0': return 0;
        case '1': return 0;
        case '2': return 0;
        case '3': return 0;
        case '4': return 0;
        case '5': return 0;
        case '6': return 0;
        case '7': return 0;
        case '8': return 0;
        case '9': return 0; 
        default:  return -1;
    }
}

Word* create_word(char type) {
    Word *op = (Word*)malloc(sizeof(Word));
    if (!op) {
        printf("Memory error\n");
        return NULL;
    }
    op->type = type;
    op->sigh = NULL;
    op->size = 0;
    return op;
}

void add(Word *op, char ch){
    char *new_sigh = (char*)realloc(op->sigh, sizeof(char)*(op->size+1));
    if (new_sigh == NULL){
        printf("Memory error\n");
        return;
    }
    op->size++;
    op->sigh = new_sigh;
    op->sigh[op->size -1] = ch;
    return;
}

int from_char_to_int(int size, char *sigh){
    int num = 0;
    for(int i=size-1; i >= 0; i--){
        int n = sigh[i] - '0';
        if(n*10*(size-1-i) == 0){
            num = n;
        }else{
            num = num + n*(int)pow(10.0,(double)(size-1-i));
        }
    }
    return num;
}

char *revers(char *sigh, int len){
    char *new_sigh = malloc(sizeof(char)*len);
    if(new_sigh == NULL){
        printf("Memory error\n");
        return NULL;
    }
    for (int i = 0; i < len; i++){
        new_sigh[len-1-i] = sigh[i];
    }
    free(sigh);
    return new_sigh;
}

void free_tree(Node *node) {
    if (!node) return;
    free_tree(node->left);
    free_tree(node->right);
    delete_word(node->value);
    free(node);
}

void change_root(Node *root, int total){
    Node *right = root->right;
    Node *left = root->left;
    int size = 0;
    if (right->value->size >= left->value->size){
        size = right->value->size;
    }else{
        size = left->value->size;
    }
    free_tree(root->left);
    free_tree(root->right);
    root->left = NULL;
    root->right = NULL;
    int len = 0;
    char *n_sigh = malloc(sizeof(char*)*size);
    while (total != 0){
        len++;
        if (len > size){
            char *new_sigh = realloc(n_sigh, sizeof(char*)*len);
            if (new_sigh == NULL){
                printf("Memory error\n");
                return;
            }
            n_sigh = new_sigh;
        }
        n_sigh[len-1] = total%10 +'0';
        total = total/10;
    }
    n_sigh = revers(n_sigh, len);
    if(n_sigh == NULL){
        printf("Memory error\n");
        return;
    }
    root->value->size = len;
    root->value->type = 'n';
    free(root->value->sigh);
    root->value->sigh = n_sigh;
    return;
}

void count_tree(Node *root){
    if(root == NULL){
        return;
    }
    if(root->value->sigh[0] == '+'){
        Node *right = root->right;
        Node *left = root->left;
        if(right->value->type == 'n' && left->value->type == 'n'){
            int num1 = from_char_to_int(right->value->size, right->value->sigh);
            int num2 = from_char_to_int(left->value->size, left->value->sigh);
            int total = num1+num2;
            change_root(root, total);
        }
        else if(right->value->type == 'n' && left->value->type == 'o' && left->value->sigh[0] == '~'){
            if(left->right->value->type == 'n'){
                int num1 = from_char_to_int(right->value->size, right->value->sigh);
                int num2 = from_char_to_int(left->right->value->size, left->right->value->sigh);
                int total = num1-num2;
                if (total < 0){
                    change_root(root, -1*total);
                    Word *minus = create_word('n');
                    for(int i = 0; i < root->value->size; i++){
                        add(minus, root->value->sigh[i]); 
                    }
                    Node *new_node = create_node(minus);
                    free_tree(root->right);
                    root->right = new_node;
                    root->value->sigh[0] = '~';
                    root->value->type = 'o';
                    root->value->size = 1;
                }else{
                    change_root(root, total);
                }
            }
        }
        else if(left->value->type == 'n' && right->value->type == 'o' && right->value->sigh[0] == '~'){
            if(right->right->value->type == 'n'){
                int num2 = from_char_to_int(left->value->size, left->value->sigh);
                int num1 = from_char_to_int(right->right->value->size, right->right->value->sigh);
                int total = num2 - num1;
                if (total < 0){
                    change_root(root, -1*total);
                    Word *minus = create_word('n');
                    for(int i = 0; i < root->value->size; i++){
                        add(minus, root->value->sigh[i]); 
                    }
                    Node *new_node = create_node(minus);
                    root->right = new_node;
                    root->value->sigh[0] = '~';
                    root->value->type = 'o';
                    root->value->size = 1;
                }else{
                change_root(root, total);}
            }
        }
    }
    count_tree(root->right);
    count_tree(root->left);
}

void print_tree(Node *root, int level, char side){
    if(root == NULL){
        return;
    }
    for (int i = 0; i < level; i++){
        printf("   ");
    }
    if(level > 0){
        printf("%c: ", side);
    }
    print_word(root->value);
    printf("\n");

    print_tree(root->right, level+1, 'R');
    print_tree(root->left, level+1, 'L');
}

void normal_print(Node *root, int d){
    if (root == NULL){
        return;
    }
    if (d != 0 && root->right !=NULL && root->left != NULL){
        printf("(");
    }
    normal_print(root->left, d+1);
    print_word(root->value);
    normal_print(root->right, d+1);
    if(d!= 0 && root->right !=NULL && root->left != NULL){
        printf(")");
    }

}
void free_root(Node *root){
    if (root == NULL){
        return;
    }
    free(root->value->sigh);
    free(root->value);
    free_root(root->left);
    free_root(root->right);
}

void main(){
    Word **stack = NULL;   
    int len = 0;
    char ch;

    printf("Enter expression: ");
    ch = getchar();
    int bracket = 0;
    while (ch  != '\n' && ch != EOF)
    {
        if(get_priority(ch) != -1){
            if(len == 0 && get_priority(ch) != 5){
                printf("Error: uncorrect expression\n");
                return;
            }
            else if(len != 0){
                if(stack[len-1]->type == 'o' && get_priority(stack[len-1]->sigh[0])!= 1){
                    if(get_priority(ch) != 5 && get_priority(ch) != 0 && get_priority(ch) != 1){
                        printf("Error: uncorrect expression\n");
                        return;
                    }
                }
            }
            if (get_priority(ch) == 0){
                bracket ++;
            }
            if(get_priority(ch) == 1){
                bracket--;
            }
            if(bracket < 0){
                printf("Error: uncorrect expression");
                return;
            }
            Word *word = create_word('o');
            add(word, ch);
            len++;
            Word **new_stack = realloc(stack, sizeof(Word*)*len);
            if (new_stack == NULL){
                printf("Memery error\n");
                free(stack);
                return;
            }
            stack = new_stack;
            stack[len-1] = word;
            ch = getchar();
        }else{
            Word *word = create_word('n');
            while(get_priority(ch) == -1 && ch != '\n' && ch != EOF){
                if (is_num(ch) == -1){
                    word->type = 'l';
                }
                add(word, ch);
                ch = getchar();
            }
            len++;
            Word **new_stack = realloc(stack, sizeof(Word*)*len);
            if (new_stack == NULL){
                printf("Memery error\n");
                free(stack);
                return;
            }
            stack = new_stack;
            stack[len-1] = word;
        }
    }
    if (bracket != 0){
        printf("Error: uncorrect expression\n");
        return;
    }

    printf("\nExpression:\n");
    for (int i = 0; i < len; i++) {
        print_word(stack[i]);
    }
    int new_len = 0;
    Word **new_stack = Dey(stack, len, &new_len);

    free(stack);
    stack = new_stack;
    printf("\nPostfix expression:\n");
    for (int i = 0; i < new_len; i++) {
        print_word(stack[i]);
        printf(" ");
    }
    Node *root = Tree(stack, new_len);
    if (!root) {
        for (int i = 0; i < new_len; i++){ 
            delete_word(stack[i]);
        }
        free(stack);
        return;
    }

    printf("\nTree:\n");
    print_tree(root, 0, ' ');

    count_tree(root);
    printf("\nTree after count:\n");
    print_tree(root, 0, ' ');

    printf("\nNormal expression after count:\n");
    normal_print(root, 0);

    free_root(root);
    free(stack);
    return;
    
}
