#include <stdio.h>
#include <stdlib.h>
#include "structuser.h"
#include <string.h>

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
// retorna ponteiro para o usuário se achar, se não retorna NULL
User* Consulta(char name[], UserTree* tree){
    int cmp;

    if(tree == NULL) return NULL;            
    if( tree == NodoNULL ) return NULL;
    cmp = strncmp(name, tree->aUser->name, NAME_SIZE);     // compara nome do usuário com palavra da busca
    if(cmp == 0) return tree->aUser;
    else if( cmp < 0 ) return Consulta( name, tree->esq );
        else if( cmp > 0 ) return Consulta( name, tree->dir );
            else return NULL;
};

UserTree* Remove(UserTree* tree, char name[])
{
  UserTree* x = tree;
  UserTree* y;
  UserTree* p = x->pai;
  UserTree* v = p->pai;

  strcpy(NodoNULL->aUser->name, name);
  while(strncmp(x->aUser->name, name, NAME_SIZE) != 0)  /* desce na árvore */
  {
   v = p; p = x;
   if(strncmp(name,x->aUser->name, NAME_SIZE) > 0) x = x->esq;
   else x = x->dir;
  };
  if(x == NodoNULL) return tree;
  if(x->red) // nodo é vermelho
  {
    if((x->esq == NodoNULL) && (x->dir == NodoNULL)) // nodo folha
    {
      if(strncmp(x->aUser->name, p->aUser->name, NAME_SIZE) < 0) p->esq = NodoNULL;
      else p->dir = NodoNULL;
      free(x);
      return tree;
    };
    if(strncmp(x->aUser->name,p->aUser->name, NAME_SIZE) < 0)
    {
     y = Menor(tree->dir);
     p->esq = y;
     y->esq = x->esq;
     y->dir = x->dir;
     free(x);
    } else
      {
        y = Maior(tree->esq);
        p->dir = y;
        y->dir = x->dir;
        y->esq = x->esq;
        free(x);
      };
  }
  else // nodo é preto
  {
    if(strncmp(x->aUser->name,p->aUser->name, NAME_SIZE) < 0) // filho a esquerda
    {
      if((p->dir->red == 0) && ((x->esq->red == 0) && (x->dir->red == 0))) // irmão preto e dois filhos pretos
      {
        if(p->red) p->red = 0; // troca a cor do pai
        else p->red = 1;
        p->dir->red = 1; // troca a cor do irmao
        y = Menor(tree->dir);
        p->esq = y;
        y->esq = x->esq;
        y->dir = x->dir;
        free(x);
      } else if ((p->dir->red == 0) && ((x->esq->red != 0) || (x->dir->red != 0)))
        {
         y = Menor(tree->dir);
         p->esq = y;
         y->esq = x->esq;
         y->dir = x->dir;
         RotacaoSimplesDir(p);
         if(x->red) x->red = 0; // troca a cor do nodo
         else x->red = 1;
         free(x);
        }else // irmão vermelho
          {
            if(p->dir->red)
            {
               y = Menor(tree->dir);
               p->esq = y;
               y->esq = x->esq;
               y->dir = x->dir;
               RotacaoSimplesDir(p);
               free(x);
            };
          };
    } else // filho a direita
      {
      if((p->esq->red == 0) && ((x->esq->red == 0) && (x->dir->red == 0))) // irmão preto e dois filhos pretos
      {
        if(p->red) p->red = 0; // troca a cor do pai
        else p->red = 1;
        p->dir->red = 1; // troca a cor do irmao
        y = Maior(tree->esq);
        p->dir = y;
        y->dir = x->dir;
        y->esq = x->esq;
        free(x);
      } else if ((p->dir->red == 0) && ((x->esq->red != 0) || (x->dir->red != 0)))
        {
         y = Maior(tree->esq);
        p->dir = y;
        y->dir = x->dir;
        y->esq = x->esq;
         RotacaoSimplesEsq(p);
         if(x->red) x->red = 0; // troca a cor do nodo
         else x->red = 1;
         free(x);
        } else // irmão vermelho
          {
            if(p->esq->red)
            {
               y = Maior(tree->esq);
               p->dir = y;
               y->dir = x->dir;
               y->esq = x->esq;
               RotacaoSimplesEsq(p);
               free(x);
            };
          };
    };
  };
  VerificaRN(tree,name);
  return tree;
}

void Destroi(UserTree* tree) {

}

// R-N, auxiliares
UserTree* VerificaRN(UserTree* t, char name[])
{
  UserTree* x = t;
  UserTree* p = x->pai;
  UserTree* v = p->pai;
  while(strncmp(x->aUser->name, name, NAME_SIZE) != 0)  /* desce na árvore */
  {
    v = p;
    p = x;
    if(strncmp(x->aUser->name,name, NAME_SIZE) > 0)
        x = x->esq;
    else x = x->dir;
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
      if(strncmp(p->aUser->name, v->aUser->name, NAME_SIZE) < 0) // p é filho a esquerda
      {
        // Caso 2.1
        if(v->dir->red) // tio é vermelho
        {
          v->dir->red =0; // tio vira preto
          if(p->red) p->red = 0; // troca a cor do pai
          else p->red = 1;
          if(v->pai == NodoNULL) // avô é raiz
          {
            p->dir->red = 0;
            p->red = 0;
            v->red = 0;
          };
        } else
          {
            if(strncmp(x->aUser->name, p->aUser->name, NAME_SIZE) < 0 && strncmp(p->aUser->name, v->aUser->name, NAME_SIZE) < 0) // Caso 2.2(A)
            {
              // rotacao a direita
              RotacaoSimplesDir(v);
              if(p->red) p->red = 0; // troca a cor do pai
              else p->red = 1;
              if(v->red) v->red = 0; // troca a cor do vo
              else v->red = 1;
            } else
              {
                if(strncmp(x->aUser->name, p->aUser->name, NAME_SIZE) > 0 && strncmp(p->aUser->name, v->aUser->name, NAME_SIZE) > 0) // Caso 2.2(B)
                {
                  RotacaoSimplesEsq(v);
                  if(p->red) p->red = 0; // troca a cor do pai
                  else p->red = 1;
                  if(v->red) v->red = 0; // troca a cor do vo
                  else v->red = 1;
                } else
                  {
                    if(strncmp(p->aUser->name, v->aUser->name, NAME_SIZE) < 0)  // Caso 2.2(C)
                    {
                      RotacaoSimplesEsq(p);
                      RotacaoSimplesDir(v); // rotacao Dupla a direita
                      if(x->red) x->red = 0; // troca a cor do novo
                      else x->red = 1;
                      if(v->red) v->red = 0; // troca a cor do vo
                      else v->red = 1;
                    } else        // Caso 2.2(D)
                      {
                        RotacaoSimplesDir(p);
                        RotacaoSimplesEsq(v); // rotacao Dupla a esquerda
                        if(x->red) x->red = 0; // troca a cor do novo
                        else x->red = 1;
                        if(v->red) v->red = 0; // troca a cor do vo
                        else v->red = 1;
                      };
                  };
              };
          };
      } else
        {
        // Caso 2.1
        if(v->esq->red) // tio é vermelho
        {
          v->esq->red =0; // tio vira preto
          if(p->red) p->red = 0; // troca a cor do pai
          else p->red = 1;
          if(v->pai == NodoNULL) // avô é raiz
          {
            p->dir->red = 0;
            p->red = 0;
            v->red = 0;
          };
        } else
          {
            if(strncmp(x->aUser->name, p->aUser->name, NAME_SIZE) < 0 && strncmp(p->aUser->name, v->aUser->name, NAME_SIZE) < 0) // Caso 2.2(A)
            {
              // rotacao a direita
              RotacaoSimplesDir(v);
              if(p->red) p->red = 0; // troca a cor do pai
              else p->red = 1;
              if(v->red) v->red = 0; // troca a cor do vo
              else v->red = 1;
            } else
              {
                if(strncmp(x->aUser->name, p->aUser->name, NAME_SIZE) > 0 && strncmp(p->aUser->name, v->aUser->name, NAME_SIZE) > 0) // Caso 2.2(B)
                {
                  RotacaoSimplesEsq(v); // --- aqui
                  if(p->red) p->red = 0; // troca a cor do pai
                  else p->red = 1;
                  if(v->red) v->red = 0; // troca a cor do vo
                  else v->red = 1;
                } else
                  {
                    if(strncmp(p->aUser->name, v->aUser->name, NAME_SIZE) < 0)  // Caso 2.2(C)
                    {
                      RotacaoSimplesEsq(p);
                      RotacaoSimplesDir(v); // rotacao Dupla a direita
                      if(x->red) x->red = 0; // troca a cor do novo
                      else x->red = 1;
                      if(v->red) v->red = 0; // troca a cor do vo
                      else v->red = 1;
                    } else        // Caso 2.2(D)
                      {
                        RotacaoSimplesDir(p);
                        RotacaoSimplesEsq(v); // rotacao Dupla a esquerda
                        if(x->red) x->red = 0; // troca a cor do novo
                        else x->red = 1;
                        if(v->red) v->red = 0; // troca a cor do vo
                        else v->red = 1;
                      };
                  };
              };
          };
      };
    };
  };
  t->red = 0;
  x->red = 1;
  return t;
}

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

