#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "sqlite3.h"

/* 
Fonction "callback" (càd f° que l'on va passer en paramètre à une autre f°), afin de manipuler la sortie du SELECT et le présenter
grâce au printf dans le terminal. La donction sera appelé pour chaque ligne que renvoie le SELECT
*/
/*
NB : ici on ne fait que l'afficher, mais si néscessaire, on peut les stocker dans le backup_table pour pouvoir réutiliser les
données "en dehors" du sqlite3_exec. Pour cela, il faut :
    -Dans le sqlite3_exec, faire (exemple) --> rc = sqlite3_exec(db, sql_request, ma_fonction, data_table, &err) avec
        - ma_fonction la fonction de "manipulation" de sortie du SELECT (par exemple le read_select_request ci dessous)
        - data_table qui est un void* dans lequelles on va pouvoir stocker les valeurs qui nous intéressent (ATTENTION : le 
            stockage des données est à faire "mannuellement" dans la fonction ma_fonction)
    - Et la fonction a passer en paramètre, comme par exemple read_select_request, avec :
        - void* backup_table : le tableau pour stocker les données et le réutiliser en dehors du sqlite3_exec
        - int nb_columns : le nombre de collones de la table de données
        - char** column_values : valeur de chaque collone sur cette ligne (ex : column_values[2] = 500 --> nb de gold)
        - char** column_names : le nom de chaque (ex : column_names[2] = "gold")
*/
int read_select_request(void* backup_table, int nb_columns, char** column_values, char** column_names) {
    for (int i=0 ; i<nb_columns ; i++) {
        printf("%s = %s     ", column_names[i], column_values[i]);
    }
    printf("\n");
    return 0;
}

// Exemple de manipulation de SDLite (sqlite3)
// NB : argc et argv sont non obligatoire, juste plus simple de les garder pour pas à avoir à modifier l'appel de SDL présent de base
int main(int argc, char* argv[]) {
    
    //Ouverture d'une base de données (nb : la créer automatiquement si elle n'existe pas)
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);
    if (rc != SQLITE_OK) {
        printf("Erreur ouverture base: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    printf("Base ouverte avec succes\n");


    //Création de la table "player"
    const char* sql_create =
        "CREATE TABLE IF NOT EXISTS player ("
        "id_player INTEGER PRIMARY KEY AUTOINCREMENT,"
        "gold INT);";
    char* err = 0;
    rc = sqlite3_exec(db, sql_create, 0, 0, &err);
    if (rc != SQLITE_OK) printf("Erreur creation table : %s\n", err);
    else printf("Table creee ou deja existante\n");


    //Insertion d'une donnée
    const char* sql_insert = "INSERT INTO player(gold) VALUES (500)";
    rc = sqlite3_exec(db, sql_insert, 0, 0, &err);
    if (rc != SQLITE_OK) printf("Erreur insertion : %s\n", err);
    else printf("Donnees inserer !\n");


    //Lecture d'une donnée
    const char* sql_select = "SELECT * FROM player;";
    rc = sqlite3_exec(db, sql_select, read_select_request, 0, &err);
    if(rc != SQLITE_OK) printf("Prolemes dans la lecture des donnees avec SELECT --> %s\n", err);
    else printf("Lecture des donnees OK\n");


    //Mise à jour de données
    const char* sql_update = "UPDATE player SET gold = gold + 50;";
    rc = sqlite3_exec(db, sql_update, 0, 0, &err);
    if(rc != SQLITE_OK) printf("Prolemes dans la maj des donnees --> %s\n", err);
    else printf("Maj des donnees OK\n");


    //Suppression de données
    const char* sql_delete = "DELETE FROM player;";
    rc = sqlite3_exec(db, sql_delete, 0, 0, &err);
    if(rc != SQLITE_OK) printf("Prolemes dans la suppression des donnees --> %s\n", err);
    else printf("suppression des donnees OK\n");

    //Fermeture de la base de données
    sqlite3_close(db);
    return 0;

}
