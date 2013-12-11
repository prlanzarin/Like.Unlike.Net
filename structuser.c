#include <stdio.h>
#include <stdlib.h>
#include "structuser.h"
#include <string.h>

// R-N
UserTree* Insere(UserTree* t, char name[])
{

    UserTree* x = t;
    User *aUser;

    if(t == NULL)  {
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
        v = p; p = x;
        if( strncmp(x->aUser->name, name, NAME_SIZE) > 0 ) x = x->esq;
        else x = x->dir;
    };

    if(x != NodoNULL) return t; // Nodo Ja Existe

    x = (UserTree*) malloc(sizeof(UserTree));
    aUser = (User*) malloc(sizeof(User));  // aloca espaço para novo usuário
    strncpy(aUser->name, name, NAME_SIZE);  // nome do novo usuario
    aUser->like = NULL;                      // inicializa lista like
    aUser->unlike = NULL;                  // inicializa lista unlike
    x->aUser = aUser;
    x->esq = x->dir = NodoNULL;
    x->pai = p;
    x->red = 0;

    if( strncmp(p->aUser->name, name, NAME_SIZE) > 0)
        p->esq = x;
    else
        p->dir = x;

    // Nodo Foi Inserido mas pode ter modificado as regras então temos que verificar.

    return VerificaRN(t, name);
};

UserTree* InserePonteiro(UserTree* tree, User *amigo)
{

    UserTree* x = tree;
    User* aUser;
    if(tree == NULL)
    {
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
        aUser = amigo;   // nome do novo usuario
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
        if( strncmp(x->aUser->name, amigo->name, NAME_SIZE) > 0 ) x = x->esq;
        else x = x->dir;
    };
    if(x != NodoNULL) return tree; // Nodo Ja Existe
    x = (UserTree*) malloc(sizeof(UserTree));
    aUser = amigo;  // nome do novo usuario
    x->aUser = aUser;
    x->esq = x->dir = NodoNULL;
    x->pai = p;
    x->red = 0;

    if( strncmp(p->aUser->name, amigo->name, NAME_SIZE) > 0)
        p->esq = x;
    else
        p->dir = x;

    // Nodo Foi Inserido mas pode ter modificado as regras então temos que verificar.
    return VerificaRN(tree, amigo->name);
};

// procura um usuário pelo nome
// retorna ponteiro para o usuário se achar, se não retorna NULL
User* Consulta(char name[], UserTree* tree)
{
    int cmp;

    if(tree == NULL || tree->aUser == NULL) return NULL;
    if( tree == NodoNULL ) return NULL;
    cmp = strncmp(name, tree->aUser->name, NAME_SIZE); // compara nome do usuário com palavra da busca
    if(cmp == 0) return tree->aUser;
    else if( cmp < 0 ) return Consulta( name, tree->esq );
    else if( cmp > 0 ) return Consulta( name, tree->dir );
    else return NULL;
};

// destrói árvore
void Destroi_Arv(UserTree* t)
{
 if(t->esq != NULL)
    Destroi_Arv(t->esq);
 if(t->dir != NULL)
    Destroi_Arv(t->dir);
 free(t);
};

// R-N, auxiliares
UserTree* VerificaRN(UserTree* t, char name[])
{
    UserTree* x = t;
    UserTree* p = x->pai;
    UserTree* v = p->pai;
    int cmp, cmpPX, cmpPV;

    cmp = strncmp(x->aUser->name, name, NAME_SIZE);
    while( cmp != 0 )  /* desce na árvore */
    {
        v = p; p = x;

        if( cmp > 0 ) {
            x = x->esq;
        } else {
             x = x->dir;
        }
        cmp = strncmp(x->aUser->name, name, NAME_SIZE);
    };
    // x contêm o novo e p o pai do novo.

    /* if(p->red == 0) - caso 1
     insere vermelho mas já tá então não precisa modificar.
    */
    // caso 2
    if(p->red)
    {
        if( v != NodoNULL) // pai não é raiz
        {
            cmp = strncmp(p->aUser->name, v->aUser->name, NAME_SIZE);
            if( cmp < 0) // p é filho a esquerda
            {
                // Caso 2.1
                if(v->dir->red) // tio é vermelho
                {
                    v->dir->red =0; // tio vira preto
                    if(p->red) {
                        p->red = 0; // troca a cor do pai
                    } else {
                        p->red = 1;
                    }
                    if(v->pai == NodoNULL) // avô é raiz
                    {
                        v->red = 0;
                    };
                } else {
                    cmpPX = strncmp(p->aUser->name, x->aUser->name, NAME_SIZE); // compara x com p
                    cmpPV = strncmp(p->aUser->name, v->aUser->name, NAME_SIZE); // compara v com p
                    if((cmpPX > 0) && (cmpPV < 0)) // Caso 2.2(A)
                    {
                        // rotacao a direita
                        RotacaoSimplesDir(v);

                        if(p->red) {
                            p->red = 0; // troca a cor do pai
                        } else {
                            p->red = 1;
                        }
                        if(v->red){
                            v->red = 0; // troca a cor do vo
                        } else {
                            v->red = 1;
                        }
                    } else {

                        if((cmpPX < 0) && (cmpPV > 0)) // Caso 2.2(B)
                        {
                            RotacaoSimplesEsq(v);
                            if(p->red){
                                p->red = 0; // troca a cor do pai
                            } else {
                                p->red = 1;
                            }
                            if(v->red) {
                                v->red = 0; // troca a cor do vo
                            } else {
                                v->red = 1;
                            }
                        } else {
                            if(cmpPV < 0)  // Caso 2.2(C)
                            {
                                RotacaoSimplesEsq(p);
                                RotacaoSimplesDir(v); // rotacao Dupla a direita
                                if(x->red) {
                                    x->red = 0; // troca a cor do novo
                                } else {
                                    x->red = 1;
                                }
                                if(v->red) {
                                    v->red = 0; // troca a cor do vo
                                } else {
                                    v->red = 1;
                                }
                            } else {        // Caso 2.2(D)

                                RotacaoSimplesDir(p);
                                RotacaoSimplesEsq(v); // rotacao Dupla a esquerda
                                if(x->red) {
                                    x->red = 0; // troca a cor do novo
                                } else {
                                    x->red = 1;
                                }
                                if(v->red) {
                                    v->red = 0; // troca a cor do vo
                                } else {
                                    v->red = 1;
                                }
                            };
                        };
                    };
                };
            } else {   // p é filho à direita

            // Caso 2.1
            if(v->esq->red) // tio é vermelho
            {
                v->esq->red =0; // tio vira preto
                if(p->red) {
                    p->red = 0; // troca a cor do pai
                } else {
                    p->red = 1;
                }
                if(v->pai == NodoNULL) // avô é raiz
                {
                    p->dir->red = 0;
                    p->red = 0;
                    v->red = 0;
                };
            } else {
                cmpPX = strncmp(p->aUser->name, x->aUser->name, NAME_SIZE); // compara x com p
                cmpPV = strncmp(p->aUser->name, v->aUser->name, NAME_SIZE); // compara v com p
                if((cmpPX > 0) && (cmpPV < 0)) // Caso 2.2(A)
                {
                  // rotacao a direita
                  RotacaoSimplesDir(v);

                  if(p->red) {
                      p->red = 0; // troca a cor do pai
                  } else {
                      p->red = 1;
                  }
                  if(v->red) {
                      v->red = 0; // troca a cor do vo
                  } else {
                      v->red = 1;
                  }
                } else
                  {
                    if((cmpPX < 0) && (cmpPV > 0)) // Caso 2.2(B)
                    {
                      RotacaoSimplesEsq(v); // --- aqui
                      if(p->red) {
                          p->red = 0; // troca a cor do pai
                      } else {
                          p->red = 1;
                      }
                      if(v->red) {
                          v->red = 0; // troca a cor do vo
                      } else {
                          v->red = 1;
                      }
                    } else
                      {
                        if(cmpPV < 0)  // Caso 2.2(C)
                        {
                          RotacaoSimplesEsq(p);
                          RotacaoSimplesDir(v); // rotacao Dupla a direita
                          if(x->red) {
                              x->red = 0; // troca a cor do novo
                          } else {
                              x->red = 1;
                          }
                          if(v->red) {
                              v->red = 0; // troca a cor do vo
                          } else {
                              v->red = 1;
                          }
                        } else        // Caso 2.2(D)
                          {
                            RotacaoSimplesDir(p);
                            RotacaoSimplesEsq(v); // rotacao Dupla a esquerda
                            if(x->red) {
                                x->red = 0; // troca a cor do novo
                            } else {
                                x->red = 1;
                            }
                            if(v->red) {
                                v->red = 0; // troca a cor do vo
                            } else {
                                v->red = 1;
                            }
                          };
                      };
                  };
              };
            };
        };
    };
    // testa se vô não acabou na raiz
    while(t->pai != NodoNULL) {
    t = t->pai;
    }

    t->red = 0;
    x->red = 1;

    return t;
}

/* rotaciona para a esquerda */
UserTree* RotacaoSimplesEsq(UserTree* t)
{
    UserTree* aux;

    aux = t->dir;
    aux->pai = t->pai;
    if(t->pai != NodoNULL) {        // testa se t não é raiz
        if(strncmp(t->pai->aUser->name, t->aUser->name, NAME_SIZE) < 0) {        // ajusta a ligação entre aux e o novo pai (antigo pai de t)
            t->pai->dir = aux;
        } else {
            t->pai->esq = aux;
        }
    }
    t->dir = aux->esq;
    t->pai = aux;
    aux->esq = t;

    return aux;   /* nova raiz */

}

/* rotaciona para a direita */
UserTree* RotacaoSimplesDir(UserTree* t)
{
    UserTree* aux;

    aux = t->esq;
    aux->pai = t->pai;
    if(t->pai != NodoNULL) {        // testa se t não é raiz
        if(strncmp(t->pai->aUser->name, t->aUser->name, NAME_SIZE) < 0) {        // ajusta a ligação entre aux e o novo pai (antigo pai de t)
            t->pai->dir = aux;
        } else {
            t->pai->esq = aux;
        }
    }
    aux->pai = t->pai;
    t->esq = aux->dir;
    t->pai = aux;
    aux->dir = t;

    return aux;  /* nova raiz */
}

void Desenha(UserTree* t, int nivel)
{
    int x;

    if (t !=NodoNULL && t->aUser != NULL)
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
UserTree* Maior(UserTree* t)
{
    while (t != NodoNULL) t = t->dir;
    return t->pai;
}

// percorre a árvore para a esquerda (direção do menor valor)
UserTree* Menor(UserTree* t)
{
    while (t != NodoNULL) t = t->esq;
    return t->pai;
}

/* =================================================================
            FIM DA ESTRUTURA DA BASE DE USUÁRIOS
                    (ÁRVORE RUBRO-NEGRA)
   =================================================================
         INÍCIO DA ESTRUTURA DO RANKING DE USUÁRIOS
                (LISTA DUPLAMENTE ENCADEADA)
   =================================================================*/

RankList* inicializa(void)
{
    return NULL;
};
/* imprime a lista duplamente encadeada já com saída para o arquqivo e com decremento de top */
int imprime(RankList* PtLista, int top, FILE *saida)
{
    int ret = 0;
    RankList* ptaux=PtLista;
    if (PtLista == NULL)
            ret = -1;           // ERRO, lista vazia
    else
    {
        do
        {
            fprintf(saida, "%s ", ptaux->pUser->name);
            printf("%s ", ptaux->pUser->name);
            ptaux = ptaux->prox;                // avança na lista
            top--;                              // decrementa top
        }
        while (ptaux != NULL && top != 0);
        fprintf(saida, "\n");
        printf("\n");
        ret = 1;
    }
    return ret;
}


RankList* insereOrd(RankList *PtLista, User *iuser)
{
    RankList *Pt, *PtAux, *user, *aux2;
    Pt = (RankList*) malloc(sizeof(RankList));	//aloca novo nodo
    Pt->pUser = iuser;		//coloca dados no novo nodo
    if ((PtLista) == NULL)		//lista vazia
    {
        PtLista = Pt;
        Pt->ant = NULL;
        Pt->prox = NULL;
        Pt->appearances = 1;
    }
    else  				// lista tem pelo menos um nodo
    {
        PtAux = PtLista;
        user = Consulta_LDE(PtAux, iuser->name);			 //auxiliar no início da lista
        if(user != NULL)
        {
            user->appearances++;    // incrementa contador de aparições
            Pt = user->ant;
            while(Pt != NULL && (Pt->appearances < user->appearances))  // testa se appearances é maior que o anteior
            {
            aux2 = user->prox;
            user->ant = Pt->ant;            // em todo este trecho
            user->prox = Pt;                // é feita a troca de posições
            if(user->ant == NULL)           // entre o anterior e o user
                PtLista = user;             // para refletir na reordenação
            else Pt->ant->prox = user;      // da lista
            if(aux2 != NULL)
                aux2->ant = Pt;
            Pt->ant = user;
            Pt->prox = aux2;
            Pt = user->ant;
            }
        }
        else
        {
            while (PtAux->prox != NULL) 	//PtAux avança até o fim e insere
                PtAux=PtAux->prox;
            PtAux->prox = Pt;
            Pt->ant = PtAux;		//Encadeia Pt com PtAux
            Pt->prox = NULL;
            Pt->appearances++;
        }
    }
    return PtLista;
}

RankList* destroi(RankList* ptLista)
{
    RankList *ptaux; //ponteiro auxiliar para percorrer a lista
    while (ptLista != NULL)
    {
        ptaux = ptLista;
        ptLista = ptLista->prox;
        free(ptaux);
    }
    free(ptLista);
    return NULL;
}

/* dada a string name, procura o usuário na lista. Se achá-lo, retorna seu ponteiro, se não, retorna NULL */
RankList* Consulta_LDE(RankList* ptLista, char name[])
{
    RankList *ptaux = ptLista;
    while ((strncmp(ptaux->pUser->name, name, 20) != 0 && ptaux->prox != NULL)) 	//Avança até serem iguais ou fim de lista
        ptaux = ptaux->prox;
    if(strncmp(ptaux->pUser->name, name, 20) != 0)     //testa se é fim de lista
        return NULL; // se sim, ret NULL (não achou)
    return ptaux; // else retorna ponteiro encontrado
};

