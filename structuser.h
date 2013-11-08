// estrutura do usuário

struct user {

    char nome[50];
    struct friendList *like, *unlinke;

};
typedef struct user User;

struct friendList {
    User *pUser;
    int appearances;
    struct friendList *next;
};
typedef struct friendList FriendList;

