#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structuser.h"
#include "post.h"
#define POST_SIZE 80

/* Variáveis globais */
PostList* _pPostList;
UserTree* _userTree = NULL;
RankList* _gRank = NULL;
RankList* _lRank = NULL;
RankList* _ulRank = NULL;

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

/*ranking geral de usuários presentes nas listas like e unlike*/
void topRanking(int tipo, int top, FILE *saida);

/* dá o ranking circular do usuário name analisando seus amigos e/ou rivais e as listas de like e/ou unlike deles */
void friendRanking(char name[], int tipo, int top, FILE *saida);

/* caminhamento central à esquerda com decremento de top, mas gera lista a partir da árvore*/
RankList* scanRanking(RankList* fRank , UserTree* t);

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
    //Destroi_Arv(_userTree);
    destroi_posts(_pPostList);
    destroi(_gRank);
    destroi(_lRank);
    destroi(_ulRank);
    return 0;
}

void getOperation(FILE *arq, FILE *saida)
{
    User* user;
    char nome1[NAME_SIZE], nome2[NAME_SIZE], post[1000];
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
            fprintf(saida, "e ");
            if(j == 0)
            {
                ret = getUsersOrdered(_userTree, i, -1, saida);
            }
            else
            {
                ret = getUsersOrdered(_userTree, i, j, saida);
            }
            if(ret == -1 || ret == j)
            {
                fprintf(saida, "ERRO nenhum usuario cadastrado");
            }
            fprintf(saida, "\n");
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
            if(Consulta(nome1, _userTree) != NULL)
            {
                _pPostList = plInsert(_pPostList, nome1, post, i);
                fprintf(saida, "t post inserido com sucesso\n");
            }
            else
            {
                fprintf(saida, "t ERRO usuario nao cadastrado\n");
            }
            printf("Você quis postar a mensagem - %s - emitida pelo usuário %s com a opção de visibilidade %d (0 - todos, 1 - Like, 2 - UnLike) \n", post, nome1, i);
        }
        break;

        case 'p':
        {
            fscanf(arq, "%s %d %d", nome1, &i, &j);
            if(j == 0)
            {
                ret = showPanel(_pPostList, nome1, i, -1, saida);
            }
            else
            {
                ret = showPanel(_pPostList, nome1, i, j, saida);
            }
            if(ret == 0)
            {
                fprintf(saida, "ERRO usuario nao cadastrado\n");
            }
            printf("Você quis exibir o painel de mensagens do usuário %s, usando o parâmetro %d, (0 - Usuário, Like e UnLike), (1 - Usuário e Like), (2 - Usuário e UnLike) dos %d primeiros usuários (0 - todos) \n", nome1, i, j);
        }
        break;

        case 'c':
        {
            fscanf(arq, "%s %d %d", nome1, &i, &j);
            // chamada da função friendRanking
            friendRanking(nome1, i, j, saida);
            printf("Você quis exibir o ranking de popularidade do círculo do usuário %s na lista %d (0 - Like e Unlike, 1 - Like, 2 - UnLike). Serão exibidos os %d primeiros usuários (0 - Todos) \n", nome1, i, j);
        }
        break;

        case 'r':
        {
            fscanf(arq, "%d %d", &i, &j);
            topRanking(i, j, saida);
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
    user = Consulta(nome1, _userTree);
    amigo = Consulta(nome2, _userTree);
    if(user != NULL && amigo != NULL) // verifica se existem
    {
        if(Consulta(nome2, user->like) == NULL && Consulta(nome2, user->unlike) == NULL) // verifica se já não é amigo
        {
            if(tipo == 1) // lista like
            {
                _lRank = insereOrd(_lRank, amigo);              //insere amigo na lista like (ranking)
                user->like = InserePonteiro(user->like, amigo);
                inserted = 1;
            }
            else if(tipo == 2) //lista unlike
            {
                _ulRank = insereOrd(_ulRank, amigo);            //insere rival na lista unlike (ranking)
                user->unlike = InserePonteiro(user->unlike, amigo);
                inserted = 1;
            }
            _gRank = insereOrd(_gRank, amigo);                  //ranking geral
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
                    fprintf(output, "\"%s\" %s\n", aux->post.msg, aux->post.usrName);             // se sim, mostra a mensagem
                    top--;          // decrementa contador
                }
                else
                {
                    if(tipo == 0 || tipo == 1)
                    {
                        userAux = Consulta(aux->post.usrName, user->like);
                        if(userAux != NULL)     // se não, testa se está na lista like
                        {
                            fprintf(output, "\"%s\" %s\n", aux->post.msg, aux->post.usrName);
                            top--;          // decrementa contador

                        }
                    }
                    else if (tipo == 0 || tipo == 2)
                    {
                        userAux = Consulta(aux->post.usrName, user->unlike);
                        if (userAux != NULL)   // se não, testa se está na lista unlike
                        {
                            fprintf(output, "\"%s\" %s\n", aux->post.msg, aux->post.usrName);
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
int getUsersOrdered(UserTree* tree, int ord, int top, FILE *output)
{

    if(top == 0 || tree == NULL || tree == NodoNULL || tree->aUser == NULL)     //testa se chegou em um nó folha ou no último nó que deveria ser exibido
    {
        return top;
    }
    else
    {
        if(ord == 2)    // cetral-direita (ordem alfabetica inversa
        {
            top = getUsersOrdered(tree->dir, ord, top, output);
            if(top != 0)
            {
                fprintf(output, "%s ", tree->aUser->name);
                top--;
                top = getUsersOrdered(tree->esq, ord, top, output);
            }
        }
        else if(ord == 1)       // central-esquerda(ordem alfabetica
        {
            top = getUsersOrdered(tree->esq, ord, top, output);
            if(top != 0)
            {
                fprintf(output, "%s ", tree->aUser->name);
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
                getUsersOrdered(user->like, 1, top, saida); // imprime usuários na ordem alfabética
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
                getUsersOrdered(user->unlike, 1, top, saida); // imprime usuários no arquivo em ordem alfabética
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

/* dá o ranking geral dos usuários da rede */
void topRanking(int tipo, int top, FILE *saida)
{
    int ret;
    fprintf(saida, "r ");
    printf("r ");
    if(tipo == 1) // like
    {
        ret = imprime(_lRank, top, saida); // imprime a lista já ordenada e atualizada _lRank (likes)
        if(ret == -1)
        {
            fprintf(saida, "ERRO nenhum usuario citado\n");  // tratamento de erro
            printf("ERRO nenhum usuario citado\n");
        }
    }
    else if(tipo == 2) // unlike
    {
        ret = imprime(_ulRank, top, saida); // imprime a lista já ordenada e atualizada _ulRank (unlike)
        if(ret == -1)
        {
            fprintf(saida, "ERRO nenhum usuario citado\n"); // tratamento de erro
            printf("ERRO nenhum usuario citado\n");
        }
    }
    else
    {
        ret = imprime(_gRank, top, saida); // imprime a lista já ordenada e atualizada _gRank (geral)
        if(ret == -1)
        {
            fprintf(saida, "ERRO nenhum usuario citado\n"); // tratamento de erro
            printf("ERRO nenhum usuario citado\n");
        }
    }
}

/* dá o ranking circular do usuário name analisando seus amigos e/ou rivais e as listas de like e/ou unlike deles */
void friendRanking(char name[], int tipo, int top, FILE *saida)
{
    RankList* fRank = inicializa(), *aux = inicializa();
    User *user;
    int status = 1;
    fRank = inicializa();
    user = Consulta(name, _userTree); //verifica se usuário está na base
    fprintf(saida, "c ");
    printf("c ");
    if(user != NULL) // se está
    {
        if(tipo == 1) // se tipo é like
        {
            if(user->like != NULL) // se list alike não está vazia
            {
                fRank = scanRanking(fRank, user->like); // forma uma lista de amigos de user
                aux = scanRanking(aux, user->like); // copia lista de amigos de user
                if(fRank != NULL) // se lista não é vazia
                {
                    while(aux->prox != NULL && aux != NULL)
                    {
                        fRank = scanRanking(fRank, aux->pUser->like); // cria lista, adiciona e reordena em fRank com os amigos de aux atual
                        aux = aux->prox; // avança aux para o próximo amigo de user
                    }
                    fRank = scanRanking(fRank, aux->pUser->like); // último amigo de user
                    imprime(fRank, top, saida); // imprime ranking circular like no arquivo
                }
                else status = -1; // se não, código de erro
            }
            else status = -1; // érro
        }
        else if(tipo == 2) // tipo like
        {
            if(user->unlike != NULL) // unlike not NULL
            {
                fRank = scanRanking(fRank, user->unlike); // cria lista de rivais de user
                aux = scanRanking(aux, user->unlike); // copia lista de rivais
                if(fRank != NULL)
                {
                    while(aux->prox != NULL)
                    {
                        fRank = scanRanking(fRank, aux->pUser->unlike); // cria lista de rivais dos rivais de user, adiciona em fRank e ordena
                        aux = aux->prox; // avança na lista de rivais de user
                    }
                    fRank = scanRanking(fRank, aux->pUser->unlike); // último rival
                    imprime(fRank, top, saida); // imprime no arquivo
                }
                else status = -1; // erro
            }
            else status = -1; // erro
        }
        else if(tipo == 0) // tipo gerla
        {
            if(user->unlike != NULL && user->like != NULL)
            {
                fRank = scanRanking(fRank, user->like); // cria lista de amigos
                fRank = scanRanking(fRank, user->unlike);// e rivais de user
                aux = scanRanking(aux, user->like); // copia a lista
                aux = scanRanking(aux, user->unlike); // acima
                if(fRank != NULL)
                {
                    while(aux->prox != NULL)
                    {
                        fRank = scanRanking(fRank, aux->pUser->like); // gera, insere e reordena fRank com os amigos e rivais
                        fRank = scanRanking(fRank, aux->pUser->unlike); // do amigo e rival da lista de user
                        aux = aux->prox; // avança na lista de amigos e rivais de user
                    }
                    fRank = scanRanking(fRank, aux->pUser->like); // último amigo ou rival de user
                    fRank = scanRanking(fRank, aux->pUser->unlike); // último amigo ou rival de user
                    imprime(fRank, top, saida); // imprime no arquivo
                }
                else status = -1; //erro
            }
            else status = -1; // erro
        }
    }
    else status = 0;
    if(status == 0)
    {
        fprintf(saida, "ERRO usuario nao cadastrado\n");
        printf("ERRO usuario nao cadastrado\n");
    }
    else if(status == -1)
    {
        fprintf(saida, "ERRO lista vazia\n");
        printf("ERRO lista vazia\n");
    }
    fRank = destroi(fRank); //destroi lista

}

/* caminhamento central à esquerda com decremento de top */
RankList* scanRanking(RankList* fRank , UserTree* t)
{
    if(t != NULL && t != NodoNULL && t->aUser != NULL) // ainda não é NULL
    {
        fRank = scanRanking(fRank, t->esq); //varre esquerda
        fRank = insereOrd(fRank, t->aUser);
        fRank = scanRanking(fRank, t->dir); // varre direita
    }
    return fRank;
}

