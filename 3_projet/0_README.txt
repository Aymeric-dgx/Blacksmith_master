Commande actuelle à executer dans le terminal

gcc -I src/include -L src/lib -o src/bin/0_main .\0_header.h .\0_SDL_personal_functions.c .\1_main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image ; ./src/bin/0_main.exe
--------------------------------------------------------------------------------------------------------------------------------


Structure des commande s'il y'a plusieurs fichier .c, plusieurs headers .h, etc :

	gcc -I src/include -L src/lib -o 0_main [fichiers. et headers.h] sqlite3.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf ; ./main.exe

	Exemple --> gcc -I src/include -L src/lib -o 0_main main.c player.c world.c sqlite3.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf ; ./main.exe


--------------------------------------------------------------------------------------------------------------------------------

gcc -I src/include -L src/lib -o src/bin/0_main .\0_header.h .\0_SDL_personal_functions.c .\1_main.c -lmingw32 -lSDL2m.\1_main.c
