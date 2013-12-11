struct post {
    char usrName[50];
    char msg[500];
    int visi;
};
typedef struct post Post;


struct postList {
    Post post;
    struct postList *next;
};
typedef struct postList PostList;

PostList* plInicialize();
PostList* plInsert(PostList*, char[50], char[500], int);
PostList* destroi_posts(PostList* l);
