struct post {
    char string[500];
    int visi;
};
typedef struct post Post;


struct postStack {
    Post post;
    struct postStack *next;
};
typedef struct postStack PostStack;
