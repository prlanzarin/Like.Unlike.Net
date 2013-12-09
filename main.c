#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structuser.h"
#include "post.h"
#define POST_SIZE 80

/* Variáveis globais */
PostList* _pPostList;
UserTree* _userTree = NULL;


// le comando no arquivo e seleciona a operação
void getOperation(FILE *arq, FILE *saida);

// insere amigo
int insertFriend(char nome1[], char nome2[], int tipo);

// salva o post de um usuário
int post(PostList* pList, char usr_name[NAME_SIZE], char text[POST_SIZE], int visi);

/* mostra posts de um usuário e de seus amigos retorna 0 se usuário não existir
top = quantidade de mensagens mostradas (-1 para todas)
tipo = (1 para like, 2 - unlike, 0 - todas)*/
int showPanel(PostList* pList, char usr_name[NAME_SIZE], int tipo, int top, FILE *output);

// imprime o nome dos usuários na ordem dada (1 - alfabetica, 2 - alfabetica inversa)
// top é o numero de usuários exibidos (-1 para todos)
int getUsersOrdered(UserTree *tree, int ord, int top, FILE *output);

/* exibe amigos da lista like (tipo = 1), unlike (tipo =2). Top = 0 -> todos, top = n -> imprime n usuários */
void showFriends(char nome1[], int tipo, int top, FILE *saida);

/* caminhamento central à esquerda com decremento de top */
int scanAlphabetical(UserTree* t, FILE *saida, int top);

int main()
{
    FILE *input, *saida;
    char filename[NAME_SIZE];
    int opened = 0;
    while(opened == 0)
    {
        printf("Insira o nome do arquivo de entrada: ");
        scanf("%s", filename);
        if((input = fopen(filename, "r")) != NULL)
            opened = 1;
    }
    opened = 0;
    while(opened == 0)
    {
        printf("Insira o nome do arquivo de saída: ");
        scanf("%s", filename);
        if((saida = fopen(filename, "w+")) != NULL)
            opened = 1;
    }

    printf("Resultado do arquivo de entrada: \n");
    getOperation(input, saida);
    fclose(input);
    fclose(saida);
    return 0;
}

void getOperation(FILE *arq, FILE *saida)
{
    User* user;
    char nome1[NAME_SIZE], nome2[NAME_SIZE], post[POST_SIZE];
    int i, j, ret;
    while(!feof(arq))
    {
        switch(fgetc(arq))
        {
        case 'i':
        {

            fscanf(arq, "%s", nome1);
            user = Consulta(nome1, _userTree);
            if(user == NULL)
            {
                _userTree = Insere(_userTree, nome1);
                fprintf(saida, "i usuario cadastrado com sucesso\n");
                printf("i usuario cadastrado com sucesso\n");
            }
            else
            {
                fprintf(saida, "i ERRO usuario ja cadastrado\n");
                printf("i ERRO usuario ja cadastrado\n");
            }
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
            ret = insertFriend(nome1, nome2, i);
            if(ret == 1)
            {
                if(i == 1)
                {
                    fprintf(saida, "a amigo inserido com sucesso\n");
                    printf("a amigo inserido com sucesso\n");
                }
                else
                {
                    fprintf(saida, "a rival inserido com sucesso\n");
                    printf("a rival inserido com sucesso\n");
                }
            }
            else
            {
                fprintf(saida, "a ERRO usuario nao cadastrado\n");
                printf("a ERRO usuario nao cadastrado\n");
            }
        }
        break;

        case 'm':
        {
            fscanf(arq, "%s %d %d", nome1, &i, &j);
            showFriends(nome1, i, j, saida);
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
};


/* Insere o amigo nome2 no usuário nome1 de acordo com o tipo (1 - LIKE/ 2- Unlike)
    Retorna 0 se não foi inserido (nome1 ou nome2 não está na rede ou nome2 já está
    na lista like ou unlike de nome1 */
int insertFriend(char nome1[], char nome2[], int tipo)
{
    User *user, *amigo;
    int inserted = 0;
    if((user = Consulta(nome1, _userTree)) != NULL && (amigo = Consulta(nome2, _userTree)) != NULL) // verifica se existem
    {
        if(Consulta(nome2, user->like) == NULL && Consulta(nome2, user->unlike) == NULL) // verifica se já não é amigo
        {
            if(tipo == 1) // lista like
            {
                user->like = InserePonteiro(user->like, amigo);
                inserted = 1;
            }
            else if(tipo == 2) //lista unlike
            {
                user->like = InserePonteiro(user->unlike, amigo);
                inserted = 1;
            }
        }
    }
    return inserted;
};


/* insere post na lista de posts dada uma pList inicializada */
int post(PostList* pList, char usr_name[NAME_SIZE], char text[POST_SIZE], int visi)
{
    if (Consulta(usr_name, _userTree) != NULL)         // testa se usuário existe
    {
        pList = plInsert(pList, usr_name, text, visi);
        return 1;
    }
    else
    {
        return 0;       // retorna 0 se usuário não existir
    }
};

/* mostra posts de um usuário e de seus amigos retorna 0 se usuário não existir
top = quantidade de mensagens mostradas (-1 para todas)
tipo = (1 para like, 2 - unlike, 0 - todas)*/
int showPanel(PostList* pList, char usr_name[NAME_SIZE], int tipo, int top, FILE *output)
{
    fprintf(output, "p\n");
    User* user, *userAux;
    PostList* aux;
    int cmp;
    user = Consulta(usr_name, _userTree);
    if (user != NULL)         // testa se usuário existe
    {
        for(aux = pList; aux != NULL; aux = aux->next)      // percorre lista de posts
        {
            if(top != 0)
            {
                cmp = strncmp(usr_name, aux->post.usrName, NAME_SIZE);    // testa se que postou é o usuário selecionado
                if(cmp == 0)
                {
                    fprintf(output, "%s\n", aux->post.msg);             // se sim, mostra a mensagem
                    top--;          // decrementa contador
                }
                else
                {
                    if(tipo == 0 || tipo == 1)
                    {
                        userAux = Consulta(aux->post.usrName, user->like);
                        if(userAux != NULL)     // se não, testa se está na lista like
                        {
                            fprintf(output, "%s\n", aux->post.msg);
                            top--;          // decrementa contador

                        }
                    }
                    else if (tipo == 0 || tipo == 2)
                    {
                        userAux = Consulta(aux->post.usrName, user->unlike);
                        if (userAux != NULL)   // se não, testa se está na lista unlike
                        {
                            fprintf(output, "%s\n", aux->post.msg);
                            top--;          // decrementa contador

                        }
                    }
                }

            }
            else
            {
                return 1;
            }
        }
        return 1;
    }
    else
    {
        return 0;       // retorna 0 se usuário não existir
    }
}

/* imprime os top primeiros usuários, top = -1 para imprimir todos */
<<<<<<< HEAD
int getUsersOrdered(UserTree* tree, int ord, int top, FILE *output) {
    fprintf(output, "n\n");
    if(top == 0 || tree == NULL || tree == NodoNULL || tree->aUser == NULL) {
        return top;
    } else {
        if(ord == 2) {
            top = getUsersOrdered(tree->dir, ord, top, output);
            if(top != 0) {
                fprintf(output, "%s\n", tree->aUser->name);
                top--;
                top = getUsersOrdered(tree->esq, ord, top, output);
            }
        } else if(ord == 1) {
            top = getUsersOrdered(tree->esq, ord, top, output);
            if(top != 0) {
                fprintf(output, "%s\n", tree->aUser->name);
                top--;
                top = getUsersOrdered(tree->dir, ord, top, output);
            }
        }
    }
    return top;
}

/* exibe amigos da lista like (tipo = 1), unlike (tipo =2). Top = 0 -> todos, top = n -> imprime n usuários */
void showFriends(char nome1[], int tipo, int top, FILE *saida)
{
    User *user;
    if(top == 0)
        top = -1;  // top set -1 -> exibir todos os usuários da lista like ou unlike
    if((user = Consulta(nome1, _userTree)) != NULL) // usuário na base
    {
        if(tipo == 1) // lista like
        {
            if(user->like != NULL && user->like != NodoNULL) // like não é arv. vazia
            {
                fprintf(saida,"m ");
                printf("m ");
                scanAlphabetical(user->like, saida, top); // imprime usuários na ordem alfabética
                fprintf(saida, "\n");
                printf("\n");
            }
            else
            {
                fprintf(saida, "m ERRO nenhum amigo cadastrado\n"); // like é vazia
                printf("m ERRO nenhum amigo cadastrado\n");
            }
        }
        else if(tipo == 2) // lista unlike
        {
            if(user->unlike != NULL && user->unlike != NodoNULL) // unlike não é uma arv. vazia
            {
                fprintf(saida,"m ");
                printf("m ");
                scanAlphabetical(user->like, saida, top); // imprime usuários no arquivo em ordem alfabética
                fprintf(saida, "\n");
                printf("\n");
            }
            else
            {
                fprintf(saida, "m ERRO nenhum amigo cadastrado\n"); // unlike vazia
                printf("m ERRO nenhum amigo cadastrado\n");
            }
        }
        else
        {
            fprintf(saida, "m ERRO tipo invalido\n"); // tipo != 1 ou 2
            printf("m ERRO tipo invalido\n");
        }
    }
    else
    {
        fprintf(saida, "m ERRO usuario nao cadastrado\n"); // usuário não está na base
        printf("m ERRO usuario nao cadastrado\n");
    }
};

/* caminhamento central à esquerda com decremento de top */
int scanAlphabetical(UserTree* t, FILE *saida, int top)
{
    if(t != NULL && t != NodoNULL && t->aUser != NULL) // ainda não é NULL
    {
        if(top == -1) // exibe todos
        {
            top = scanAlphabetical(t->esq, saida, top); //varre esquerda
            fprintf(saida, "%s ", t->aUser->name); // imprime raiz no arquivo
            printf("%s ", t->aUser->name);
            top = scanAlphabetical(t->dir, saida, top); // varre direita
        }
        else if(top != 0) // while cont < top
        {
            top = scanAlphabetical(t->esq, saida, top); // esquerda
            fprintf(saida, "%s ", t->aUser->name); // imprime no arquivo
            printf("%s ", t->aUser->name);
            top--; // decrementa número de usuários a imprimir
            top = scanAlphabetical(t->dir, saida, top); // varre direita
        }
    }
    return top;
}
