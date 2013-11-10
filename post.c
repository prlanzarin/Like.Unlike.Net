#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "post.h"

// inicializa ponteiro da lista
PostList* plInicialize() {
    return NULL;
}

// insere na lista
PostList* plInsert(PostList* pList, char usrName[50], char msg[500], int visi) {
    PostList* newPostList;

    newPostList = (PostList*) malloc(sizeof(PostList));
    strncpy(newPostList->post.msg, msg, 500);
    strncpy(newPostList->post.usrName, usrName, 50);
    newPostList->post.visi = visi;

    newPostList->next = pList;
    pList = newPostList;

    return pList;
}
