#define _CRT_SECURE_NO_WARNINGS
#include "casino.h"

int main() {
    loadPlayers();
    loadGames();

    int choice;
    do {
        printf("\n=== Casino Game Manager ===\n");
        printf("1. Dodaj igraca\n");
        printf("2. Prikazi sve igrace\n");
        printf("3. Azuriraj saldo igraca\n");
        printf("4. Obrisi igraca\n");
        printf("5. Dodaj igru\n");
        printf("6. Prikazi sve igre\n");
        printf("7. Azuriraj igru\n");
        printf("8. Obrisi igru\n");
        printf("9. Sortiraj igrace po saldu\n");
        printf("10. Pretrazi igraca po ID-u (binarna pretraga)\n");
        printf("11. Ispis igraca rekurzijom\n");
        printf("12. Sortiraj igre (bubble sort)\n");
        printf("13. Preimenuj datoteku\n");
        printf("14. Obrisi datoteku\n");
        printf("16. Sortiraj igrace funkcijskim pokazivacem\n");
        printf("0. Izlaz\n");
        printf("Odabir: ");
        (void)scanf("%d", &choice);
        (void)getchar();

        switch (choice) {
        case 1:
            addPlayer();
            break;
        case 2:
            listPlayers();
            break;
        case 3:
            updatePlayer();
            break;
        case 4:
            deletePlayer();
            break;
        case 5:
            addGame();
            break;
        case 6:
            listGames();
            break;
        case 7:
            updateGame();
            break;
        case 8:
            deleteGame();
            break;
        case 9:
            sortPlayers();
            break;
        case 10: {
            int id;
            printf("Unesite ID igraca za pretragu: ");
            (void)scanf("%d", &id);
            Player* p = searchPlayerBinary(id);
            if (p)
                printf("Pronadjen: %s %s - Saldo: %.2lf\n", p->firstName, p->lastName, p->balance);
            else
                printf("Igrac nije pronadjen.\n");
            break;
        }
        case 11:
            recursivePrintPlayers(0);
            break;
        case 12:
            sortGames();
            break;
        case 13: {
            char oldName[100], newName[100];
            printf("Unesite staro ime datoteke: ");
            (void)scanf("%99s", oldName);
            printf("Unesite novo ime datoteke: ");
            (void)scanf("%99s", newName);
            renameFile(oldName, newName);
            break;
        }
        case 14: {
            char filename[100];
            printf("Unesite ime datoteke za brisanje: ");
            (void)scanf("%99s", filename);
            deleteFile(filename);
            break;
        }
        case 16:
            sortPlayersWithFunctionPointer(compareByBalance);
            break;
        case 0:
            break;
        default:
            printf("Nevazeci odabir.\n");
        }
    } while (choice != 0);

    freeMemory();
    return 0;
}
