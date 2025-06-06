#define _CRT_SECURE_NO_WARNINGS
#include "casino.h"
#include <errno.h>
#include <string.h>

Player* players = NULL;
Game* games = NULL;
int playerCount = 0;
int gameCount = 0;
int nextPlayerID = 1;
int nextGameID = 1;

void freeMemory() {
    free(players);
    free(games);
    players = NULL;
    games = NULL;
}


void addPlayer() {
    Player* tmp = realloc(players, (playerCount + 1) * sizeof(Player));
    if (!tmp) {
        perror("Neuspjesno alociranje memorije za igrace");
        exit(EXIT_FAILURE);
    }
    players = tmp;

    Player* p = &players[playerCount];
    p->playerID = nextPlayerID++;
    printf("Unesite ime igraca: ");
    (void)scanf("%49s", p->firstName);
    printf("Unesite prezime igraca: ");
    (void)scanf("%49s", p->lastName);
    printf("Unesite saldo: ");
    (void)scanf("%lf", &p->balance);

    playerCount++;
    savePlayers();
    printf("Igrac dodan: %s %s (ID: %d)\n", p->firstName, p->lastName, p->playerID);
}

void listPlayers() {
    printf("\nLista svih igrača:\n");
    for (int i = 0; i < playerCount; i++) {
        printf("%d. %s %s - Saldo: %.2lf\n",
            players[i].playerID,
            players[i].firstName,
            players[i].lastName,
            players[i].balance);
    }
}

void updatePlayer() {
    int id;
    printf("Unesite ID igraca za azuriranje: ");
    (void)scanf("%d", &id);

    for (int i = 0; i < playerCount; i++) {
        if (players[i].playerID == id) {
            printf("Unesite novi saldo: ");
            (void)scanf("%lf", &players[i].balance);
            savePlayers();
            printf("Igrac azuriran.\n");
            return;
        }
    }
    printf("Igrac s tim ID-om nije pronaden.\n");
}

void deletePlayer() {
    int id;
    printf("Unesite ID igraca za brisanje: ");
    (void)scanf("%d", &id);

    for (int i = 0; i < playerCount; i++) {
        if (players[i].playerID == id) {
            for (int j = i; j < playerCount - 1; j++) {
                players[j] = players[j + 1];
            }
            playerCount--;
            savePlayers();
            printf("Igrac obrisan.\n");
            return;
        }
    }
    printf("Igrac s tim ID-om nije pronaden.\n");
}

void savePlayers() {
    FILE* file = fopen("players.dat", "wb");
    if (!file) {
        perror("Greska pri otvaranju datoteke za zapis igraca");
        return;
    }
    fwrite(&playerCount, sizeof(int), 1, file);
    fwrite(players, sizeof(Player), playerCount, file);
    fclose(file);
}

void loadPlayers() {
    FILE* file = fopen("players.dat", "rb");
    if (!file) return;
    fread(&playerCount, sizeof(int), 1, file);
    players = malloc(playerCount * sizeof(Player));
    if (!players && playerCount > 0) {
        perror("Neuspjesno ucitavanje memorije za igrace");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fread(players, sizeof(Player), playerCount, file);
    nextPlayerID = playerCount > 0 ? players[playerCount - 1].playerID + 1 : 1;
    fclose(file);
}

int compareByBalance(const void* a, const void* b) {
    double balanceA = ((Player*)a)->balance;
    double balanceB = ((Player*)b)->balance;
    return (balanceA > balanceB) - (balanceA < balanceB);
}

void sortPlayers() {
    qsort(players, playerCount, sizeof(Player), compareByBalance);
    printf("Igraci sortirani po saldu.\n");
    listPlayers();
}

Player* searchPlayerBinary(int id) {
    int low = 0, high = playerCount - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (players[mid].playerID == id)
            return &players[mid];
        else if (players[mid].playerID < id)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return NULL;
}

void recursivePrintPlayers(int index) {
    if (index >= playerCount)
        return;
    printf("%d. %s %s - Saldo: %.2lf\n",
        players[index].playerID,
        players[index].firstName,
        players[index].lastName,
        players[index].balance);
    recursivePrintPlayers(index + 1);
}

Player* findPlayerByID(int id) {
    return searchPlayerBinary(id);
}


void addGame() {
    Game* tmp = realloc(games, (gameCount + 1) * sizeof(Game));
    if (!tmp) {
        perror("Neuspjesno alociranje memorije za igre");
        exit(EXIT_FAILURE);
    }
    games = tmp;

    Game* g = &games[gameCount];
    g->gameID = nextGameID++;
    printf("Unesite ime igre: ");
    (void)scanf("%49s", g->gameName);
    printf("Unesite minimalnu okladu: ");
    (void)scanf("%lf", &g->minBet);
    printf("Unesite maksimalnu okladu: ");
    (void)scanf("%lf", &g->maxBet);

    gameCount++;
    saveGames();
    printf("Igra dodana: %s (ID: %d)\n", g->gameName, g->gameID);
}

void listGames() {
    printf("\nLista svih igara:\n");
    for (int i = 0; i < gameCount; i++) {
        printf("%d. %s - Min: %.2lf, Max: %.2lf\n",
            games[i].gameID,
            games[i].gameName,
            games[i].minBet,
            games[i].maxBet);
    }
}

void updateGame() {
    int id;
    printf("Unesite ID igre za azuriranje: ");
    (void)scanf("%d", &id);
    for (int i = 0; i < gameCount; i++) {
        if (games[i].gameID == id) {
            printf("Unesite novo ime igre (trenutno: %s): ", games[i].gameName);
            (void)scanf("%49s", games[i].gameName);
            printf("Unesite novi minimalni ulog (trenutno: %.2lf): ", games[i].minBet);
            (void)scanf("%lf", &games[i].minBet);
            printf("Unesite novi maksimalni ulog (trenutno: %.2lf): ", games[i].maxBet);
            (void)scanf("%lf", &games[i].maxBet);
            saveGames();
            printf("Igra azurirana.\n");
            return;
        }
    }
    printf("Igra s tim ID-om nije pronadena.\n");
}

void deleteGame() {
    int id;
    printf("Unesite ID igre za brisanje: ");
    (void)scanf("%d", &id);
    for (int i = 0; i < gameCount; i++) {
        if (games[i].gameID == id) {
            for (int j = i; j < gameCount - 1; j++) {
                games[j] = games[j + 1];
            }
            gameCount--;
            saveGames();
            printf("Igra obrisana.\n");
            return;
        }
    }
    printf("Igra s tim ID-om nije pronadena.\n");
}

void saveGames() {
    FILE* file = fopen("games.dat", "wb");
    if (!file) {
        perror("Greska pri zapisu igara");
        return;
    }
    fwrite(&gameCount, sizeof(int), 1, file);
    fwrite(games, sizeof(Game), gameCount, file);
    fclose(file);
}

void loadGames() {
    FILE* file = fopen("games.dat", "rb");
    if (!file) return;
    fread(&gameCount, sizeof(int), 1, file);
    games = malloc(gameCount * sizeof(Game));
    if (!games && gameCount > 0) {
        perror("Neuspjesno ucitavanje memorije za igre");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fread(games, sizeof(Game), gameCount, file);
    nextGameID = gameCount > 0 ? games[gameCount - 1].gameID + 1 : 1;
    fclose(file);
}

void sortGames() {
    int choice;
    printf("Sortirati igre po:\n1. Naziv\n2. Minimalni ulog\n3. Maksimalni ulog\nOdabir: ");
    (void)scanf("%d", &choice);

    for (int i = 0; i < gameCount - 1; i++) {
        for (int j = 0; j < gameCount - i - 1; j++) {
            int swap = 0;
            if ((choice == 1 && strcmp(games[j].gameName, games[j + 1].gameName) > 0) ||
                (choice == 2 && games[j].minBet > games[j + 1].minBet) ||
                (choice == 3 && games[j].maxBet > games[j + 1].maxBet)) {
                swap = 1;
            }
            if (swap) {
                Game temp = games[j];
                games[j] = games[j + 1];
                games[j + 1] = temp;
            }
        }
    }

    printf("Igre sortirane.\n");
    listGames();
}


void renameFile(const char* oldName, const char* newName) {
    if (rename(oldName, newName) != 0) {
        perror("Greska pri preimenovanju datoteke");
    }
    else {
        printf("Datoteka je preimenovana.\n");
    }
}

void deleteFile(const char* filename) {
    if (remove(filename) != 0) {
        perror("Greska pri brisanju datoteke");
    }
    else {
        printf("Datoteka obrisana.\n");
    }
}



void sortPlayersWithFunctionPointer(CompareFunc cmp) {
    qsort(players, playerCount, sizeof(Player), cmp);
    printf("Igraci sortirani (funkcijskim pokazivacem):\n");
    listPlayers();
}
