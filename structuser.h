// base de dados geral

/* informações do usuário */
struct user {
  char nome[50];
  struct friendList *like, *unlinke;
}
  ;
typedef struct user User;

/* base de usuários: árvore Rubro-Negra */

struct userTree {
  User *aUser;         // chave da árvore: aUser->nome
  int red;
  struct userTree *esq;
  struct userTree *dir;
  struct userTree *pai;
}; 

typedef struct userTree userBase;


/* lista de amigos de um usuário */
struct friendList {
  User *pUser;
  int appearances;
  struct friendList *next;
};

typedef struct friendList FriendList;

/* declaração do nodo NULL */
static userBase* NodoNULL = NULL;

/* funções das estruturas definidas */

// R-N
userBase* Insere(userBase* user, char key[]);
int Consulta(int x, userBase* user);
userBase* Remove(userBase* user, char key[]);
void Destroi(userBase* user);

// R-N, auxiliares

userBase* VerificaRN(userBase* user, char key[]);
userBase* RotacaoSimplesEsq(userBase* user);
userBase* RotacaoSimplesDir(userBase* user);
void Desenha(userBase* user, int nivel);
userBase* Maior(userBase* user);
userBase* Menor(userBase* user);

// LSE

FriendList* cria_lista(void);
FriendList* insere_ord(FriendList* flist, User user);
void imprime(FriendList* flist);
FriendList* remover(FriendList* flist, char x[]);
FriendList* destroi(FriendList* flist);
