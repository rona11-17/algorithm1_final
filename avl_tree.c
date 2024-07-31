#include <stdio.h>
#include <stdlib.h>

int n, tree_count, avl_count, perfect_count, permutation[10], tree[20][20];

struct tree_node {
    struct tree_node *left, *right;
    struct tree_node *next, *avl_next, *perfect_next;
    int value;
};

struct tree_node *memo_head, *memo_tail, *avl_memo_head, *avl_memo_tail, *perfect_memo_head, *perfect_memo_tail;



/*生成した木の番兵をリストでつなぐ(キューの考え方)*/
void insert_tree_memo(struct tree_node *p) {
    p->next = NULL;
    if (memo_head == NULL) {
        memo_head = p;
    } else {
        memo_tail->next = p;
    }
    memo_tail = p;
}

void insert_avl_tree_memo(struct tree_node *p) {
    p->avl_next = NULL;
    if (avl_memo_head == NULL) {
        avl_memo_head = p;
    } else {
        avl_memo_tail->avl_next = p;
    }
    avl_memo_tail = p;
}

void insert_perfect_tree_memo(struct tree_node *p) {
    p->perfect_next = NULL;
    if (perfect_memo_head == NULL) {
        perfect_memo_head = p;
    } else {
        perfect_memo_tail->perfect_next = p;
    }
    perfect_memo_tail = p;
}

/*二つの二分木が重複しているか判定するプログラム(詳細)*/
int check_value_duplication(struct tree_node *p, struct tree_node *q)
{
    if (p == NULL && q == NULL) {
        return 1;
    } else if (p != NULL && q != NULL) {
        if (p->value == q->value) {
            int left = check_value_duplication(p->left, q->left);
            int right = check_value_duplication(p->right, q->right);

            return left && right;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

/*2つの二分木が重複しているか判定するプログラム*/
int tree_duplication_check(struct tree_node *head, struct tree_node *q)
{
    struct tree_node *p;
    
    for (p = head; p != NULL; p = p->next) {
        if (check_value_duplication(p, q) == 1) {
            return 1;
        }
    }
    return 0;
}

/*木の成長プログラム*/
struct tree_node *insert_tree(int value, struct tree_node *p)
{
    if (p == NULL) {
        struct tree_node *q;
        q = (struct tree_node *)malloc(sizeof(struct tree_node));
        q->value = value;
        q->left = q->right = NULL;
        return q;
    } else {
        if (value < p->value) {
            p->left = insert_tree(value, p->left);
            return p;
        } else {
            p->right = insert_tree(value, p->right);
            return p;
        }
    }
}

/*木の生成プログラム*/
void create_tree()
{
    int i;
    struct tree_node *head = (struct tree_node*)malloc(sizeof(struct tree_node));
    head->value = 0;
    head->left = head->right = NULL;
    
    for (i = 0; i < n; i++) {
        insert_tree(permutation[i], head);
    }

    if (tree_duplication_check(memo_head, head) == 0) {
        insert_tree_memo(head);
        tree_count++;
    } else {
        free(head);
    }
}


/*順列出力*/
void print_ans() 
{
    int i;

    for (i = 0; i < n; i++) {
        printf("%d ", permutation[i]);
    }
    printf("\n");
}

/*順列生成で使用する重複チェック*/
int check_duplication(int m, int i)
{
    int index;

    for (index = 0; index < m; index++) {
        if(permutation[index] == i + 1) {
            return 1;
        }
    }
    return 0;
}

/*順列生成*/
void permutations(int m)
{
    int i;

    if (m == n - 1) {
        for (i = 0; i < n; i++) {
            if (check_duplication(m, i) == 0) {
                permutation[m] = i + 1;
                break;
            }
        }
        print_ans();
        create_tree();
    } else {
        for (i = 0; i < n; i++) {
            if (check_duplication(m, i) == 0) {
                permutation[m] = i + 1;
                permutations(m + 1);
            }
        }
    }
}

/*ノードの総数を返す*/
int tree_node_count(struct tree_node *head)
{
    int right, left;

    if (head->right == NULL && head->left == NULL) {
        return 0;
    } else if (head->value == 0) {
        return tree_node_count(head->right);
    } else if (head->right == NULL) {
        return tree_node_count(head->left) + 1;
    } else if (head->left == NULL) {
        return tree_node_count(head->right) + 1;
    } else {
        right = tree_node_count(head->right);
        left = tree_node_count(head->left);
        
        return left + right + 2;
    }
}

/*木の高さを返す*/
int tree_height(struct tree_node *head)
{
    int right, left;

    if (head->right == NULL && head->left == NULL) {
        return 0;
    } else if (head->value == 0) {
        return tree_height(head->right);
    } else if (head->right == NULL) {
        return tree_height(head->left) + 1;
    } else if (head->left == NULL) {
        return tree_height(head->right) + 1;
    } else {
        right = tree_height(head->right);
        left = tree_height(head->left);
        if (left >= right) {
            return left + 1;
        } else {
            return right + 1;
        }
    }
}

/*avl木の判定*/
int avl_check(struct tree_node *head)
{
    int left, right;

    if (head->value == 0) {
        avl_check(head->right);
    } else if (head->right == NULL && head->left == NULL) {
        return 0;
    } else if (head->right != NULL && head->left == NULL) {
        if (tree_height(head->right) == 0) {
            return 0;
        } else {
            return 1;
        }
    } else if (head->right == NULL && head->left != NULL) {
        if (tree_height(head->left) == 0) {
            return 0;
        } else {
            return 1;
        }
    } else {
        left = avl_check(head->left);
        right = avl_check(head->right);

        if (left == 1 || right == 1) {
            return 1;
        } else {
            if (abs(tree_height(head->left) - tree_height(head->right)) <= 1) {
                return 0;
            } else {
                return 1;
            }
        }
    }
}

/*完全二分木の判定*/
int perfect_check(struct tree_node *head)
{
    int left, right;

    if (head->right == NULL && head->left == NULL) {
        return 0;
    } else if (head->value == 0) {
        return perfect_check(head->right);
    } else if (head->right == NULL) {
        if (tree_node_count(head->left) == 0) {
            return 0;
        } else {
            return 1;
        }
    } else if (head->left == NULL) {
        if (tree_node_count(head->right) == 0) {
            return 0;
        } else {
            return 1;
        }
    } else {
        left = perfect_check(head->left);
        right = perfect_check(head->right);

        if (left == 1 || right == 1) {
            return 1;
        } else {
            if (abs(tree_node_count(head->left) - tree_node_count(head->right)) <= 1) {
                return 0;
            } else {
                return 1;
            }
        }
    }
}

/*木の可視化プログラム*/
void print_tree()
{
    int i, j;

    for (i = 0; i < 28; i++) {
        for (j = 0; j < 55; j++) {
            if (tree[i][j] == 0) {
                printf(" ");
            } else if (tree[i][j] == -1) {
                printf("/");
            } else if (tree[i][j] == -2) {
                printf("\\");
            } else {
                printf("%d", tree[i][j]);
            }
        }
        printf("\n");
    }
    for (i = 0; i < 28; i ++) {
        for (j = 0; j < 55; j++) {
            tree[i][j] = 0;
        }
    }
}

/*木の様子を二次元配列に格納*/
void store_tree(struct tree_node *head, int x, int y)
{
    int i, height = tree_height(head);
    
    tree[x][y] = head->value;
    if (head->value == 0) {
        store_tree(head->right, x, y);
    } else if (head->right == NULL && head->left != NULL) {
        for (i = 0; i < height; i++) {
            tree[x + (i + 1)][y - (i + 1)] = -1;
        }
        store_tree(head->left, x + (height + 1), y - (height + 1));
    } else if (head->right != NULL && head->left == NULL) {
        for (i = 0; i < height; i++) {
            tree[x + (i + 1)][y + (i + 1)] = -2;
        }
        store_tree(head->right, x + (height + 1), y + (height + 1));
    } else if (head->right != NULL && head->left != NULL) {
        for (i = 0; i < height; i++) {
            tree[x + (i + 1)][y - (i + 1)] = -1;
            tree[x + (i + 1)][y + (i + 1)] = -2;
        }
        store_tree(head->left, x + (height + 1), y - (height + 1));
        store_tree(head->right, x + (height + 1), y + (height + 1));
    }
}

/*平均算出*/
float average(int n, struct tree_node *memo_head) {
    float ave = 0.0;
    struct tree_node *p;

    for (p = memo_head; p != NULL; p = p->next) {
        ave += tree_height(p);
    }
    ave /= n;
    return ave;
}

float avl_average(int n, struct tree_node *memo_head) {
    float ave = 0.0;
    struct tree_node *p;

    for (p = memo_head; p != NULL; p = p->avl_next) {
        ave += tree_height(p);
    }
    ave /= n;
    return ave;
}

float perfect_average(int n, struct tree_node *memo_head) {
    float ave = 0.0;
    struct tree_node *p;

    for (p = memo_head; p != NULL; p = p->perfect_next) {
        ave += tree_height(p);
    }
    ave /= n;
    return ave;
}
/*分散算出*/
float variance(int n, struct tree_node *memo_head) {
    float var = 0.0, loss = 0.0;
    struct tree_node *p;

    for (p = memo_head; p != NULL; p = p->next) {
        loss = (float)(tree_height(p) - average(n, memo_head));
        var += loss * loss;
    }
    var /= n;
    return var;
}

float avl_variance(int n, struct tree_node *memo_head) {
    float var = 0.0, loss = 0.0;
    struct tree_node *p;

    for (p = memo_head; p != NULL; p = p->avl_next) {
        loss = (float)(tree_height(p) - avl_average(n, memo_head));
        var += loss * loss;
    }
    var /= n;
    return var;
}

float perfect_variance(int n, struct tree_node *memo_head) {
    float var = 0.0, loss = 0.0;
    struct tree_node *p;

    for (p = memo_head; p != NULL; p = p->perfect_next) {
        loss = (float)(tree_height(p) - perfect_average(n, memo_head));
        var += loss * loss;
    }
    var /= n;
    return var;
}

/*相異なる木の結果出力*/
void different_tree_print()
{
    struct tree_node *p;
    if (n <= 5) {
        for (p = memo_head; p != NULL; p = p->next) {
            store_tree(p, 0, 14);
            print_tree();
        }
    }
    
    printf("%d\n", tree_count);
    printf("%f\n", average(tree_count, memo_head));
    printf("%f\n", variance(tree_count, memo_head));
}

/*avl木の結果出力*/
void avl_tree_print()
{
    struct tree_node *p;
    
    for (p = memo_head; p != NULL; p = p->next) {
        if (avl_check(p) == 0) {
            if (n <= 5) {
                store_tree(p, 0, 27);
                print_tree();
            }
            insert_avl_tree_memo(p);
            avl_count++;
        }
    }
    printf("%d\n", avl_count);
    printf("%f\n", avl_average(avl_count, avl_memo_head));
    printf("%f\n", avl_variance(avl_count, avl_memo_head));
}

/*完全二分木の結果出力*/
void perfect_tree_print()
{
    struct tree_node *p;

    for (p = memo_head; p != NULL; p = p->next) {
        if (perfect_check(p) == 0) {
            if (n <= 5) {
                store_tree(p, 0, 27);
                print_tree();
            }
            insert_perfect_tree_memo(p);
            perfect_count++;
        }
    }
    printf("%d\n", perfect_count);
    printf("%f\n", perfect_average(perfect_count, perfect_memo_head));
    printf("%f\n", perfect_variance(perfect_count, perfect_memo_head));
}

/*malloc領域の削除*/
struct tree_node  *delete_tree_node(struct tree_node *head)
{
    if (head == NULL) {
        return NULL;
    }

    head->left = delete_tree_node(head->left);
    head->right = delete_tree_node(head->right);
    free(head);
    
    return NULL;
}

void delete_tree()
{
    struct tree_node *p;

    for (p = memo_head; p != NULL;) {
        struct tree_node *tmp = p;
        p = p->next;
        delete_tree_node(p);
    }
}

int main() 
{
    struct tree_node *p;
    tree_count = avl_count = perfect_count = 0;
    memo_head = memo_tail = NULL;
    avl_memo_head = avl_memo_tail = NULL;
    perfect_memo_head = perfect_memo_tail = NULL; 

    printf("input n: ");
    scanf("%d", &n);

    permutations(0);

    different_tree_print();
    // avl_tree_print();
    // perfect_tree_print();

    delete_tree();

    return 0;
}