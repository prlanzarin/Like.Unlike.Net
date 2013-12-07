#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structuser.h"
#include "post.h"

/* Variáveis globais */
PostList* _pPostList;
UserTree* _userTree = NULL;


// le comando no arquivo e seleciona a operação
void getOperation(FILE *arq);

// salva o post de um usuário
int post(PostList* pList, char usr_name[NAME_SIZE], char text[POST_SIZE], int visi);

// imprime mensagens de um usuário, de seus amigos e de seus inimigos
int showPanel(PostList* pList, char usr_name[NAME_SIZE]);

int main()
{
    FILE *input;

    printf("Resultado do arquivo de entrada: \n");
    getOperation(input);

    return 0;
}

void getOperation(FILE *arq)
{
  char nome1[50], nome2[50], post[1000];
  int i, j;
  if((arq = fopen("teste.txt", "r")) != NULL)
    {
      while(!feof(arq))
	{
    	  switch(fgetc(arq))
	    {
	    case 'i':
	      {
		fscanf(arq, "%s", nome1);
		//chamada de função insertUser
		printf("Você quis inserir o usuário: %s\n", nome1);
	      }
	      break;

	    case 'e':
	      {
		fscanf(arq, "%d %d", &i, &j);
		//chamada de função showUsers
		printf("Você quis exibir os usuários da rede social na ordem %d e os %d primeiros usuários (0 = todos) \n", i, j);
	      }
	      break;

	    case 'a':
	      {
		fscanf(arq, "%s %s %d", nome1, nome2, &i);
		//chamada da função insertFriend
		printf("Você quis adicionar um amigo ao usuário %s. O amigo era %s. Você quis adicionar ele na lista %d (1 = Like, 2 = UnLike)\n", nome1, nome2, i);
	      }
	      break;

	    case 'm':
	      {
		fscanf(arq, "%s %d %d", nome1, &i, &j);
		//chamada da função showFriends
		printf("Você quis exibir a lista de amigos do usuário %s. A lista a ser exibida é %d (1 - Like, 2 - UnLike). Você quis exibir %d usuários (0- todos) \n", nome1, i, j);
	      }
	      break;

	    case 't':
	      {
		fgets(post, 1000, arq);
		strcpy(nome1, strtok(post, " "));
		strcpy(post, strtok(NULL, "\""));
		i = atoi(strtok(NULL, " "));
		//chamada da função postMessage
		printf("Você quis postar a mensagem - %s - emitida pelo usuário %s com a opção de visibilidade %d (0 - todos, 1 - Like, 2 - UnLike) \n", post, nome1, i);
	      }
	      break;

	    case 'p':
	      {
		fscanf(arq, "%s %d %d", nome1, &i, &j);
		// chamada da função showPanel
		printf("Você quis exibir o painel de mensagens do usuário %s, usando o parâmetro %d, (0 - Usuário, Like e UnLike), (1 - Usuário e Like), (2 - Usuário e UnLike) dos %d primeiros usuários (0 - todos) \n", nome1, i, j);
	      }
	      break;

	    case 'c':
	      {
		fscanf(arq, "%s %d %d", nome1, &i, &j);
		// chamada da função friendRanking
		printf("Você quis exibir o ranking de popularidade do círculo do usuário %s na lista %d (0 - Like e Unlike, 1 - Like, 2 - UnLike). Serão exibidos os %d primeiros usuários (0 - Todos) \n", nome1, i, j);
	      }
	      break;

	    case 'r':
	      {
		fscanf(arq, "%d %d", &i, &j);
		// chamada da função topRanking
		printf("Você quis exibir o ranking geral da rede com o tipo %d (indefinido). Serão exibidos os %d primeiros usuários (0 - Todos)\n", i, j);
	      }
	      break;
	    }
	}
    }
  else printf("Erro na abertura do arquivo! \n");
  fclose(arq);
};


/* insere post na lista de posts dada uma pList inicializada */
int post(PostList* pList, char usr_name[NAME_SIZE], char text[POST_SIZE], int visi) {
    if (Consulta(usr_name, _userTree) != NULL) {       // testa se usuário existe
        pList = plInsert(pList, usr_name, text, visi);
        return 1;
    } else {
        return 0;       // retorna 0 se usuário não existir
    }
}

/* mostra posts de um usuário e de seus amigos retorna 0 se usuário não existir*/
int showPanel(PostList* pList, char usr_name[NAME_SIZE]) {
    User* user, *userAux;
    PostList* aux;
    int cmp;
    user = Consulta(usr_name, _userTree);
    if (user != NULL) {       // testa se usuário existe
        for(aux = pList; aux != NULL; aux = aux->next) {    // percorre lista de posts
            cmp = strncmp(usr_name, aux->post.usrName, NAME_SIZE);    // testa se que postou é o usuário selecionado
            if(cmp == 0) {
                printf("%s\n", aux->post.msg);             // se sim, mostra a mensagem
            } else {
                userAux = Consulta(aux->post.usrName, user->like);
                if(userAux != NULL) {   // se não, testa se está na lista like
                    printf("%s\n", aux->post.msg);
                } else {
                    userAux = Consulta(aux->post.usrName, user->unlike);
                    if (userAux != NULL) { // se não, testa se está na lista unlike
                        printf("%s\n", aux->post.msg);
                    }
                }
            }
        }
        return 1;
    } else {
        return 0;       // retorna 0 se usuário não existir
    }
}
