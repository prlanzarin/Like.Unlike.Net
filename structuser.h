#include <stdio.h>
#include <stdlib.h>

#define NAME_SIZE 50
// base de dados geral

/* informações do usuário */
struct user {
  char name[NAME_SIZE];
  struct userTree *like, *unlike;
};

typedef struct user User;

/* base de usuários: árvore Rubro-Negra */

struct userTree {
  User *aUser;         // chave da árvore: aUser->nome
  int red;              // cor do nó
  struct userTree *esq;
  struct userTree *dir;
  struct userTree *pai;
};
typedef struct userTree UserTree;


/* lista de amigos de um usuário */
struct rankList {
  User *pUser;              // ponteiro para usuário
  int appearances;         // quantas vezes aparece
  struct rankList *prox;  // próximo elemento
  struct rankList *ant;        // elemento anterior
};
typedef struct rankList RankList;

/* declaração do nodo NULL */
static UserTree* NodoNULL = NULL;

/* funções das estruturas definidas */

// R-N
UserTree* Insere(UserTree* tree, char name[]);
UserTree* InserePonteiro(UserTree* tree, User *amigo);
User* Consulta(char name[], UserTree* tree);
UserTree* Remove(UserTree* tree, char name[]);
void Destroi(UserTree* tree);

// R-N, auxiliares
UserTree* VerificaRN(UserTree* tree, char name[]);
UserTree* RotacaoSimplesEsq(UserTree* tree);
UserTree* RotacaoSimplesDir(UserTree* tree);
void Desenha(UserTree* tree, int nivel);
UserTree* Maior(UserTree* tree);
UserTree* Menor(UserTree* tree);

// LDE para ranqueamento
RankList* inicializa(void);
int imprime(RankList* PtLista, int top, FILE *saida);
RankList* insereOrd(RankList *PtLista, User *iuser);
RankList* destroi(RankList* ptLista);
RankList* Consulta_LDE(RankList* ptLista, char name[]);

