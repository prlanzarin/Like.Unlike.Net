#include <stdio.h>
#include <stdlib.h>
#include "structuser.h"

// R-N
UserTree* Insere(UserTree* tree, char name[]) {

UserTree* x = tree;
User* aUser;

            if(tree == NULL)  {
                 // alocação de espaço
                 NodoNULL = (UserTree*) malloc(sizeof(UserTree));
                 x = (UserTree*) malloc(sizeof(UserTree));
                 //inicializações
                 // NULL
                 NodoNULL->red = 0; // null é preto;
                 NodoNULL->esq = NodoNULL;
                 NodoNULL->dir = NodoNULL;
                 NodoNULL->aUser = NULL;
                 NodoNULL->pai = NodoNULL;
                 // Raiz
                 aUser = (User*) malloc(sizeof(User));  // aloca espaço para novo usuário
                 strncpy(aUser->name, name, NAME_SIZE);  // nome do novo usuario
                 aUser->like = NULL;                      // inicializa lista like
                 aUser->unlike = NULL;                  // inicializa lista unlike
                 x->aUser = aUser;
                 x->esq = x->dir = NodoNULL;
                 x->red = 0;
                 x->pai = NodoNULL;
                 return x;
            }

            UserTree* p= x->pai; // pai
            UserTree* v= p->pai; // vo


            while( x != NodoNULL )  /* desce na árvore */
            {
              v = p;        // atualiza ponteiro do pai
              p = x;        // atualiza ponteiro do vo
              if( strncmp(x->aUser->name, name, NAME_SIZE) > 0 ) x = x->esq;
              else x = x->dir;
            };

            if(x != NodoNULL) return tree; // Nodo Ja Existe

            x = (UserTree*) malloc(sizeof(UserTree));
            aUser = (User*) malloc(sizeof(User));  // aloca espaço para novo usuário
            strncpy(aUser->name, name, NAME_SIZE);  // nome do novo usuario
            aUser->like = NULL;                      // inicializa lista like
            aUser->unlike = NULL;                  // inicializa lista unlike
            x->aUser = aUser;
            x->esq = x->dir = NodoNULL;
            x->pai = p;
            x->red = 1;

            if( strncmp(p->aUser->name, name, NAME_SIZE) > 0)
                p->esq = x;
            else
                p->dir = x;

            // Nodo Foi Inserido mas pode ter modificado as regras então temos que verificar.

            return VerificaRN(tree, name);
};

// procura um usuário pelo nome
// retorna 1 se achar, se não retorna 0
int Consulta(char name[NAME_SIZE], UserTree* t){
    int cmp;

    if( t == NodoNULL ) return 0;
    cmp = strncmp(name, t->aUser->name, NAME_SIZE);     // compara nome do usuário com palavra da busca
    if(cmp == 0) return 1;
    else if( cmp < 0 ) return Consulta( x, t->esq );
        else if( cmp > 0 ) return Consulta( x, t->dir );
            else return 0;
}

UserTree* Remove(UserTree* tree, char name[]);

void Destroi(UserTree* tree) {

}

// R-N, auxiliares
UserTree* VerificaRN(UserTree* t, char name[]);


/* rotaciona para a esquerda */
UserTree* RotacaoSimplesEsq(UserTree* t) {
    UserTree* aux;

    aux = t->dir;
    t->dir = aux->esq;
    aux->esq = t;

    return aux;   /* nova raiz */

}

/* rotaciona para a direita */
UserTree* RotacaoSimplesDir(UserTree* t) {
    UserTree* aux;

    aux = t->esq;
    t->esq = aux->dir;
    aux->dir = t;

    return aux;  /* nova raiz */
}

void Desenha(UserTree* t, int nivel) {
    int x;

    if (t !=NodoNULL)
    {
        for (x=1; x<=nivel; x++)
        printf("=");
        if(t->red) printf("%s Red\n", t->aUser->name);
        else printf("%s Black\n", t->aUser->name);
        if (t->esq != NodoNULL) Desenha(t->esq, (nivel+1));
        if (t->dir != NodoNULL) Desenha(t->dir, (nivel+1));
    }
    else printf("Arvore Vazia\n");
}

// percorre a árvore para a direita (direção do maior valor)
UserTree* Maior(UserTree* t) {
    while (t != NodoNULL) t = t->dir;
    return t->pai;
}

// percorre a árvore para a esquerda (direção do menor valor)
UserTree* Menor(UserTree* t) {
    while (t != NodoNULL) t = t->esq;
    return t->pai;
}

