#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR_LEN 100

typedef struct Node
{
    char artist[MAX_STR_LEN];
    char song[MAX_STR_LEN];
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct
{
    Node *head;
    Node *current;
} CircularDoublyLinkedList;

CircularDoublyLinkedList *createPlaylist();
Node *createNode(const char *artist, const char *song);
void insertNode(CircularDoublyLinkedList *list, const char *artist, const char *song);
void removeNode(CircularDoublyLinkedList *list, const char *song);
void displayPlaylist(CircularDoublyLinkedList *list);
void displayPlaylistSorted(CircularDoublyLinkedList *list);
Node *searchSong(CircularDoublyLinkedList *list, const char *song);
void nextSong(CircularDoublyLinkedList *list);
void prevSong(CircularDoublyLinkedList *list);
void saveToFile(CircularDoublyLinkedList *list, const char *filename);
void loadFromFile(CircularDoublyLinkedList *list, const char *filename);
void freeList(CircularDoublyLinkedList *list);
void sortPlaylist(Node *head);
void swapNodes(Node *a, Node *b);
void trimWhiteSpace(char *str);

int main()
{
    CircularDoublyLinkedList *playlist = createPlaylist();
    loadFromFile(playlist, "musicas.txt");

    int choice;
    char artist[MAX_STR_LEN], song[MAX_STR_LEN];

    do
    {
        printf("\n╔═══════════════════════════════════════════════════════════╗\n");
        printf("║                           Menu                            ║\n");
        printf("╠═══════════════════════════════════════════════════════════╣\n");
        printf("║ 1. Exibir playlist pela ordem de cadastro                 ║\n");
        printf("║ 2. Exibir playlist ordenada pelo nome das músicas         ║\n");
        printf("║ 3. Inserir nova música                                    ║\n");
        printf("║ 4. Remover uma música                                     ║\n");
        printf("║ 5. Buscar uma música                                      ║\n");
        printf("║ 6. Avançar para próxima música                            ║\n");
        printf("║ 7. Retornar à música anterior                             ║\n");
        printf("║ 8. Sair                                                   ║\n");
        printf("╚═══════════════════════════════════════════════════════════╝\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            displayPlaylist(playlist);
            break;
        case 2:
            displayPlaylistSorted(playlist);
            break;
        case 3:
            printf("\n╔══════════════════════════════════════════════╗\n");
            printf("║                Inserir Nova Música             ║\n");
            printf("╠══════════════════════════════════════════════╣\n");
            printf("║                                                ║\n");
            printf("║    🎵 Nome do Artista:                         ║\n");
            printf("║                                                ║\n");
            printf("║    🎵 Nome da Música:                          ║\n");
            printf("║                                                ║\n");
            printf("╚══════════════════════════════════════════════╝\n");
            printf("👉 ");
            fgets(artist, MAX_STR_LEN, stdin);
            artist[strcspn(artist, "\n")] = '\0';
            printf("👉 ");
            fgets(song, MAX_STR_LEN, stdin);
            song[strcspn(song, "\n")] = '\0';
            insertNode(playlist, artist, song);
            saveToFile(playlist, "musicas.txt");
            break;
        case 4:
            printf("\n╔════════════════════════════════════════════╗\n");
            printf("║            Remover uma Música                ║\n");
            printf("╠════════════════════════════════════════════╣\n");
            printf("║                                              ║\n");
            printf("║    🎵 Nome da Música a remover:              ║\n");
            printf("║                                              ║\n");
            printf("╚════════════════════════════════════════════╝\n");
            printf("👉 ");
            fgets(song, MAX_STR_LEN, stdin);
            song[strcspn(song, "\n")] = '\0';
            removeNode(playlist, song);
            saveToFile(playlist, "musicas.txt");

            break;
        case 5:
            printf("\n╔════════════════════════════════════════════╗\n");
            printf("║             Buscar uma Música                ║\n");
            printf("╠════════════════════════════════════════════╣\n");
            printf("║                                              ║\n");
            printf("║    🎵 Nome da Música a buscar:               ║\n");
            printf("║                                              ║\n");
            printf("╚════════════════════════════════════════════╝\n");
            printf("👉 ");
            fgets(song, MAX_STR_LEN, stdin);
            song[strcspn(song, "\n")] = '\0';
            Node *found = searchSong(playlist, song);
            if (found)
            {
                printf("\n🎵 Música encontrada: %s - %s\n", found->artist, found->song);
            }
            else
            {
                printf("\n❌ Música não encontrada.\n");
            }
            break;
        case 6:
            nextSong(playlist);
            printf("\n╔════════════════════════════════════════════╗\n");
            if (playlist->current != NULL)
            {
                printf("║             Próxima Música                   ║\n");
                printf("╠════════════════════════════════════════════╣\n");
                printf("║    🎵 Música atual: %s - %s\n", playlist->current->artist, playlist->current->song);
            }
            else
            {
                printf("║              Playlist Vazia                  ║\n");
                printf("╠════════════════════════════════════════════╣\n");
                printf("║          A playlist está vazia.             ║\n");
            }
            printf("╚════════════════════════════════════════════╝\n");

            break;
        case 7:
            prevSong(playlist);
            printf("\n╔════════════════════════════════════════════╗\n");
            if (playlist->current != NULL)
            {
                printf("║             Música Anterior                  ║\n");
                printf("╠════════════════════════════════════════════╣\n");
                printf("║    🎵 Música atual: %s - %s\n", playlist->current->artist, playlist->current->song);
            }
            else
            {
                printf("║              Playlist Vazia                  ║\n");
                printf("╠════════════════════════════════════════════╣\n");
                printf("║          A playlist está vazia.             ║\n");
            }
            printf("╚════════════════════════════════════════════╝\n");

            break;
        case 8:
            printf("\n╔════════════════════════════════════════════╗\n");
            printf("║            👋 Saindo...                    ║\n");
            printf("╚════════════════════════════════════════════╝\n");
            break;
        default:
            printf("\n❌ Opção inválida. Tente novamente.\n");
        }
    } while (choice != 8);

    freeList(playlist);
    return 0;
}

CircularDoublyLinkedList *createPlaylist()
{
    CircularDoublyLinkedList *list = (CircularDoublyLinkedList *)malloc(sizeof(CircularDoublyLinkedList));
    list->head = NULL;
    list->current = NULL;
    return list;
}

Node *createNode(const char *artist, const char *song)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    strncpy(newNode->artist, artist, MAX_STR_LEN);
    strncpy(newNode->song, song, MAX_STR_LEN);
    newNode->next = newNode->prev = newNode;
    return newNode;
}

void insertNode(CircularDoublyLinkedList *list, const char *artist, const char *song)
{
    Node *newNode = createNode(artist, song);
    if (list->head == NULL)
    {
        list->head = newNode;
        list->current = newNode;
    }
    else
    {
        Node *tail = list->head->prev;
        tail->next = newNode;
        newNode->prev = tail;
        newNode->next = list->head;
        list->head->prev = newNode;
    }
}

void removeNode(CircularDoublyLinkedList *list, const char *song)
{
    if (list->head == NULL)
        return;

    Node *current = list->head;
    do
    {
        if (strcmp(current->song, song) == 0)
        {
            if (current->next == current)
            {
                free(current);
                list->head = list->current = NULL;
                return;
            }
            if (current == list->head)
            {
                list->head = current->next;
            }
            Node *prevNode = current->prev;
            Node *nextNode = current->next;
            prevNode->next = nextNode;
            nextNode->prev = prevNode;
            if (list->current == current)
            {
                list->current = nextNode;
            }
            free(current);
            return;
        }
        current = current->next;
    } while (current != list->head);
}

void displayPlaylist(CircularDoublyLinkedList *list)
{
    if (list->head == NULL)
    {
        printf("\n╔═══════════════════════════════════════════╗\n");
        printf("║            Playlist Atual                  ║\n");
        printf("╠════════════════════════════════════════════╣\n");
        printf("║               Playlist Vazia               ║\n");
        printf("╚════════════════════════════════════════════╝\n");
        return;
    }

    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║            Playlist Atual                  ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║           🎵 Artista | Música              ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    Node *current = list->head;
    int index = 1;
    do
    {
        printf("║     %2d. %-18s | %-20s     \n", index++, current->artist, current->song);
        current = current->next;
    } while (current != list->head);

    printf("╚════════════════════════════════════════════╝\n");
}
void displayPlaylistSorted(CircularDoublyLinkedList *list)
{
    if (list->head == NULL)
    {
        printf("\n╔════════════════════════════════════════════╗\n");
        printf("║            Playlist Ordenada               ║\n");
        printf("╠═══════════════════════════════════════════╣\n");
        printf("║               Playlist Vazia               ║\n");
        printf("╚════════════════════════════════════════════╝\n");
        return;
    }

    CircularDoublyLinkedList *sortedList = createPlaylist();
    Node *current = list->head;
    do
    {
        insertNode(sortedList, current->artist, current->song);
        current = current->next;
    } while (current != list->head);
    sortPlaylist(sortedList->head);

    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║            Playlist Ordenada               ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║       🎵 Artista  |   Música               ║\n");
    printf("╠════════════════════════════════════════════╣\n");

    current = sortedList->head;
    int index = 1;
    do
    {
        printf("║  %2d. %-18s | %-20s  \n", index++, current->artist, current->song);
        current = current->next;
    } while (current != sortedList->head);

    printf("╚════════════════════════════════════════════╝\n");
    freeList(sortedList);
}

void nextSong(CircularDoublyLinkedList *list)
{
    if (list->current != NULL)
    {
        list->current = list->current->next;
    }
}

void prevSong(CircularDoublyLinkedList *list)
{
    if (list->current != NULL)
    {
        list->current = list->current->prev;
    }
}

void saveToFile(CircularDoublyLinkedList *list, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    if (list->head != NULL)
    {
        Node *current = list->head;
        do
        {
            fprintf(file, "%s;%s\n", current->artist, current->song);
            current = current->next;
        } while (current != list->head);
    }

    fclose(file);
}

void trimWhiteSpace(char *str)
{
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1]))
    {
        str[len - 1] = '\0';
        len--;
    }
}

void loadFromFile(CircularDoublyLinkedList *list, const char *filename)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_STR_LEN * 2];
    if (!file)
    {
        perror("❌ Erro ao abrir o arquivo");
        return;
    }

    printf("\n╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                            Carregando Músicas do Arquivo                   ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════╣\n");

    printf("║                  ✔️ Arquivo aberto com sucesso: %s\n", filename);
    printf("╠════════════════════════════════════════════════════════════════════════════╣\n");

    printf("║                                Músicas Carregadas                          ║\n");

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n' || line[0] == '\0')
        {
            continue;
        }
        line[strcspn(line, "\n")] = '\0';
        char *artist = strtok(line, ";");
        char *song = strtok(NULL, ";");
        if (artist && song)
        {
            trimWhiteSpace(artist);
            trimWhiteSpace(song);
            insertNode(list, artist, song);
            printf("║  🎵 Música carregada: %s - %s\n", artist, song);
        }
    }

    fclose(file);

    printf("╚════════════════════════════════════════════╝\n");
    printf("🎉 Carregamento concluído.\n");
}

Node *searchSong(CircularDoublyLinkedList *list, const char *song)
{
    if (list->head == NULL)
        return NULL;

    Node *current = list->head;
    do
    {
        if (strcmp(current->song, song) == 0)
        {
            printf("\n🎵 Música encontrada: %s - %s\n", current->artist, current->song);
            return current;
        }
        current = current->next;
    } while (current != list->head);

    printf("\n❌ Música não encontrada.\n");
    return NULL;
}

void freeList(CircularDoublyLinkedList *list)
{
    if (list->head == NULL)
    {
        free(list);
        return;
    }

    Node *current = list->head;
    do
    {
        Node *next = current->next;
        free(current);
        current = next;
    } while (current != list->head);

    free(list);
}

void sortPlaylist(Node *head)
{
    int swapped;
    Node *ptr1;
    Node *lptr = NULL;

    if (head == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr && ptr1->next != head)
        {
            if (strcmp(ptr1->song, ptr1->next->song) > 0)
            {
                swapNodes(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void swapNodes(Node *a, Node *b)
{
    char tempArtist[MAX_STR_LEN], tempSong[MAX_STR_LEN];
    strncpy(tempArtist, a->artist, MAX_STR_LEN);
    strncpy(tempSong, a->song, MAX_STR_LEN);
    strncpy(a->artist, b->artist, MAX_STR_LEN);
    strncpy(a->song, b->song, MAX_STR_LEN);
    strncpy(b->artist, tempArtist, MAX_STR_LEN);
    strncpy(b->song, tempSong, MAX_STR_LEN);
}
