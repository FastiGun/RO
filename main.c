/*
 *  TP pour un problème de Sac à dos
 *  Le but de ce TP est de comprendre l'algorithme greedy et de l'implémenter
 *  Il s'agira ensuite de permettre à travers votre programme de tester de l'influence de l'ordre
 *  de traitement des variables sur l'efficacité de la méthode.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>


//////////////////////////////////////// STRUCTURES ET VARIABLES //////////////////////////////////////////
// Structure de donnée associée à un objet : celui-ci est représenté par :
typedef struct CELLULE {
	int ID;        // identifiant de l'objet
	int poids;       // poids de l'objet
	int score;    // score associé à l'objet
	struct CELLULE* suiv;
} cel;



// Variables globales
cel* LIST;   	     // La liste des objets
cel* LISTord;	     // copie de la liste afin d'effectuer les différents tris
int* X;		     // les variables de décision. X[i]=1 si
int nbObj, poidsMax; // le nombre d'objets et le poids maximum du sac-à-dos



////////////////////////// CREATION INSTANCE à partir du FICHIER  //////////////////////////////
// Construction de la liste d'objets L (des variables libres) a partir du fichier "name" qui représente une instance du problème
// appel inputInstance(nomFichier)
void inputInstance(char* name) {
	int id, v, s;
	int i;
	cel* objet;

	FILE* fp;

	// Ouverture fichier
	if ((fp = fopen(name, "r")) == NULL) {
		printf("\n nerreur ouverture\n");
		exit(0);
	}

	//Affichage du nom de l'instance, du nombre d'objets et du poids maximum du sac-à-dos
	printf("\nProbleme : %s", name);
	fscanf(fp, "%d\t%d\n", &nbObj, &poidsMax);
	printf("\nNombre d'objets : %d", nbObj);
	printf("\npoids du sac  : %d\n", poidsMax);

	// Liste des objets telle que L[i] correspond à l'objet n°i
	LIST = (cel*)malloc((nbObj + 1) * sizeof(cel));
	if (LIST == NULL) exit(0);
	// Liste des objets telle que L[i] correspond à l'objet n°i
	LISTord = (cel*)malloc((nbObj + 1) * sizeof(cel));
	if (LISTord == NULL) exit(0);

	// Un tableau tel que X[i]=1 si l'objet n°i est dans le sac à dos.
	X = (int*)malloc((nbObj + 1) * sizeof(int));
	if (X == NULL) exit(0);


	// Construction des listes LIST, LISTord
	for (i = 1; i <= nbObj; i++) {
		fscanf(fp, "%d\t%d\t%d\n", &id, &v, &s);
		LIST[i].ID = LISTord[i].ID = id;
		LIST[i].poids = LISTord[i].poids = v;
		LIST[i].score = LISTord[i].score = s;
	}

	fclose(fp);
}


//////////////////////////////////// AFFICHAGE LISTES et SOLUTIONS //////////////////////////////

void affichListObj(cel* L) {
	int i;
	printf("\nListe :");
	for (i = 1; i <= nbObj; i++) {
		printf("%d[p=%d;s=%d]-", L[i].ID, L[i].poids, L[i].score);
	}
	printf("\n");
}

void affichSolution(cel* L, int* X) {
	int i, ptotal = 0, stotal = 0;

	printf("\nSolution :");
	for (i = 1; i <= nbObj; i++) {
		if (X[i] == 1) {
			printf("%d[p=%d;s=%d]-", L[i].ID, L[i].poids, L[i].score);
			ptotal += L[i].poids;
			stotal += L[i].score;
		}
	}
	printf("\nPoids = %d, Score=%d \n", ptotal, stotal);
}

//////////////////////////////////// TRIS DES LISTES //////////////////////////////
// échange des valeurs L[x] et L[y]
void exchange(cel* L, int x, int y) {
	int IDx, Px, Sx;
	IDx = L[x].ID;
	Px = L[x].poids;
	Sx = L[x].score;
	L[x].ID = L[y].ID;
	L[x].poids = L[y].poids;
	L[x].score = L[y].score;
	L[y].ID = IDx;
	L[y].poids = Px;
	L[y].score = Sx;
}


// tri n°1 : tri des objets dans L suivant un ordre croissant des poids
// appel : triP1(L) tri la liste L suivant cet ordre
void triP1(cel* L) {

	int OK, i, j;
	i = 1;
	do {
		OK = 1;
		for (j = 1; j <= nbObj - i; j++)
			if (L[j].poids > L[j + 1].poids) {
				exchange(L, j, j + 1);
				OK = 0;
			}
		i++;
	} while (OK == 0);
}

// tri n°2 : tri des objets dans L suivant un ordre décroissant des poids
// appel : triP2(L) tri la liste L suivant cet ordre
void triP2(cel* L) {

	int OK, i, j;
	i = 1;
	do {
		OK = 1;
		for (j = 1; j <= nbObj - i; j++)
			if (L[j].poids < L[j + 1].poids) {
				exchange(L, j, j + 1);
				OK = 0;
			}
		i++;
	} while (!OK);
}
// tri n°3 : tri des objets dans L suivant un ordre croissant des scores
// appel : L=triS1(L) tri la liste L suivant cet ordre
void triS1(cel* L) {

	int OK, i, j;
	i = 1;
	do {
		OK = 1;
		for (j = 1; j <= nbObj - i; j++)
			if (L[j].score > L[j + 1].score) {
				exchange(L, j, j + 1);
				OK = 0;
			}
		i++;
	} while (!OK);
}

// tri n°4 : tri des objets dans L suivant un ordre décroissant des scores
// appel : triS2(L) tri la liste L suivant cet ordre
void triS2(cel* L) {

	int OK, i, j;
	i = 1;
	do {
		OK = 1;
		for (j = 1; j <= nbObj - i; j++)
			if (L[j].score < L[j + 1].score) {
				exchange(L, j, j + 1);
				OK = 0;
			}
		i++;
	} while (!OK);
}

//////////////////////////////////    ALGORITHME GLOUTON     ////////////////////////////////////
// Vous devez intervenir ici !!!
//
void algorithmeGlouton(cel* L) {
	int P_total = 0;
	int score_total = 0;

	for (int i = 1; i <= nbObj; i++)
	{
		if (L[i].poids + P_total <= poidsMax)
		{
			X[L[i].ID] = 1;
			score_total += L[i].score;
			P_total += L[i].poids;
		}
		else
		{
			X[L[i].ID] = 0;
		}
	}
}

//////////////////////////////////// FONCTION PRINCIPALE //////////////////////////////////////

// Fonction principale : l'argument argv[1] est un fichier texte contenant une instance du problème.
// Ces instances vous sont transmises avec ce programme

int main(int argc, char* argv[]) {

	//inputInstance(argv[1]);
	inputInstance("inst35obj.txt"); //argument du programme : nom du fichier de l'instance

	//printf("\nListe initiale");
	//affichListObj(LIST);

	// exemple d'appel à l'algorithme glouton après tri n°1
	triP1(LISTord);
	affichListObj(LISTord);
	algorithmeGlouton(LISTord);
	affichSolution(LIST, X);

	// exemple d'appel à l'algorithme glouton après tri n°2
	triP2(LISTord);
	affichListObj(LISTord);
	algorithmeGlouton(LISTord);
	affichSolution(LIST, X);

	// exemple d'appel à l'algorithme glouton après tri n°3
	triS1(LISTord);
	affichListObj(LISTord);
	algorithmeGlouton(LISTord);
	affichSolution(LIST, X);

	// exemple d'appel à l'algorithme glouton après tri n°4
	triS2(LISTord);
	affichListObj(LISTord);
	algorithmeGlouton(LISTord);
	affichSolution(LIST, X);

	return 0;
}