#define NAME_SIZE 50
// base de dados geral

/* informações do usuário */
struct user {
  char name[NAME_SIZE];
  struct UserTree *like, *unlike;
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
struct friendList {
  User *pUser;              // ponteiro para usuário
  int appearances;         // quantas vezes aparece
  struct friendList *next;  // próximo elemento
};

typedef struct friendList FriendList;

/* declaração do nodo NULL */
static UserTree* NodoNULL = NULL;

/* funções das estruturas definidas */

// R-N
UserTree* Insere(UserTree* tree, char name[]);
int Consulta(char name[], UserTree* tree);
UserTree* Remove(UserTree* tree, char name[]);
void Destroi(UserTree* tree);

// R-N, auxiliares

UserTree* VerificaRN(UserTree* tree, char name[]);
UserTree* RotacaoSimplesEsq(UserTree* tree);
UserTree* RotacaoSimplesDir(UserTree* tree);
void Desenha(UserTree* tree, int nivel);
UserTree* Maior(UserTree* tree);
UserTree* Menor(UserTree* tree);

// LSE

FriendList* cria_lista(void);
FriendList* insere_ord(FriendList* flist, User user);
void imprime(FriendList* flist);
FriendList* remover(FriendList* flist, char x[]);
FriendList* destroi(FriendList* flist);
