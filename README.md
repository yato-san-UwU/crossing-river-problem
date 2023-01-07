# crossing-river-problem
crossing-river-problem avec sémaphore et mémoire partagée en C pour un système linux

explication du programme :

deux programme hacker et window qui traverse la rivier
le problemme ici et d'attendre l'arriver de 4 processus sous ce shema : 
[2H et 2W] ou [4H ou 4W] 
si 1H et 3W ou inversement (3H et 1W) le bateau ne demare pas.

les fichier bibi.c et bibi.h represente une bibliothèque pour tout le programme
pour lancer utiliser la bibilotheque lors de l'execution : exp "gcc -o main main.c bibi.c"

sinon consulter le manuelle linux pou les differente fct (shmat,shmget,semop...)
