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

int Consulta(int x, UserTree* tree);
UserTree* Remove(UserTree* tree, char name[]);
void Destroi(UserTree* tree);

// R-N, auxiliares
UserTree* VerificaRN(UserTree* t, char name[]);


UserTree* RotacaoSimplesEsq(UserTree* tree);
UserTree* RotacaoSimplesDir(UserTree* tree);
void Desenha(UserTree* tree, int nivel);
UserTree* Maior(UserTree* tree);
UserTree* Menor(UserTree* tree);

