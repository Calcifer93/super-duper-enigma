#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define OK (0)
#define GAGNE (1)
#define EGALITE (2)


void affiche_grille(void);
void calcule_position(int, struct position*);
unsigned nb_jetons_ligne(struct position*, int, int, char);
unsigned calcule_nb_jetons_total(struct position*, char);
bool grille_complete(void);
void initialise_grille(void);
bool position_valide(struct position*);
int statut_jeu(struct position* pos, char);
unsigned xmax(unsigned, unsigned);
int lireNombre(void);
static char grille[7][6];
struct position
{
	int colonne;
	int ligne;
};

void affiche_grille(void)
{
	int col, lgn;
	for (lgn = 0; lgn < 6; ++lgn) {
		printf("|");
		for (col = 0; col < 7; ++col)
			printf(" %c |", grille[col][lgn]);
		printf("\n");

		for (col = 1; col <= 7; ++col)
			printf("____");

		printf("\n");
	}


	for (col = 1; col <= 7; ++col)
		printf("  %d ", col);

	printf("\n");
}

void initialise_grille() {

	unsigned col, lgn;

	for (col = 0; col < 7; ++col)
		for (lgn = 0; lgn < 6; ++lgn)
			grille[col][lgn] = ' ';
}

void calcule_position(int coup, struct position* pos) {
	int lgn;
	pos->colonne = coup;
	for (lgn = 6 - 1; lgn >= 0; --lgn)
		if (grille[pos->colonne][lgn] == ' ') {
			pos->ligne = lgn;
			break;
		}
}
static unsigned nb_jetons_ligne(struct position* pos,
	int dx, int dy, char jeton) {

	struct position temp;
	unsigned nb = 1; //car on compte le jeton sur place

	temp.colonne = pos->colonne + dx;
	temp.ligne = pos->ligne + dy;

	while (position_valide(&temp)) {
		if (grille[temp.colonne][temp.ligne] == jeton)
			++nb;
		else
			break;
		temp.colonne += dx;
		temp.ligne += dy;
	}
	return nb;
}

unsigned calcule_nb_jetons_total(struct position* pos, char jeton) {
	unsigned max;
	max = nb_jetons_ligne(pos, 0, 1, jeton);//pas de y=-1 car on compte du bas vers le haut depuis le debut de la partie.
	max = xmax(max, nb_jetons_ligne(pos, 1, 0, jeton) +
		nb_jetons_ligne(pos, -1, 0, jeton) - 1);// "-1" car on compte 2x le jeton sur place lors des 2 appel à la fonction, il faut donc enlever le jeton de trop.
	max = xmax(max, nb_jetons_ligne(pos, 1, 1, jeton) +
		nb_jetons_ligne(pos, -1, -1, jeton) - 1);
	max = xmax(max, nb_jetons_ligne(pos, 1, -1, jeton) +
		nb_jetons_ligne(pos, -1, 1, jeton) - 1);

	return max;
}

bool grille_complete() {
	unsigned int col, lgn;
	for (col = 0; col < 7; ++col)
		for (lgn = 0; lgn < 6; ++lgn)
			if (grille[col][lgn] == ' ')
				return false;
	return true;
}

unsigned xmax(unsigned a, unsigned b) {
	return (a > b) ? a : b;
}
bool position_valide(struct position* pos) {
	bool ret = true;
	if (pos->colonne >= 7 || pos->colonne < 0)
		ret = false;
	else if (pos->ligne >= 6 || pos->ligne < 0)
		ret = false;
	return ret;
}
int statut_jeu(struct position* pos, char jeton) {

	if (grille_complete())
		return EGALITE;
	else if (calcule_nb_jetons_total(pos, jeton) >= 4)
		return GAGNE;

	return OK;
}

int lireNombre(void) {
	int n;   /* nombre saisi */
	int c;   /* caractère lu */
	int ok;   /* la saisie est-elle valide ? */

	printf("Tapez un nombre entre 1 et 7 compris: ");

	do {
		ok = 1;

		/* saisie */
		if (!scanf_s("%d", &n))
			ok = 0;

		/* vérification de la fin de la saisie
		   et vidage du flux par la même occasion */
		while ((c = getchar()) != '\n' && c != EOF) {
			if (c != ' ' && c != '\t' && c != '\f' && c != '\r')   /* si ce n'est pas une "caractère blanc" */
				ok = 0;
		}

		/* si invalide, on redemande */
		if (!ok)
			printf("Veuillez saisir un nombre uniquement.\nRecommencez : ");

	} while (!ok);   /* on recommence si la saisie n'est pas valide */

	return n;
}

int main() {
	int statut;
	char jeton = 'O';
	initialise_grille();
	affiche_grille();
	bool action = true;

	while (action) {
		struct position pos;
		int coup;
		for (;;) {
			if (jeton == 'O')
				printf("Joueur 1 : \n");
			else
				printf("joueur 2 : \n");
			coup = lireNombre();
			if ((coup >= 1) && (coup <= 7))
				break;
		}
		pos.colonne = coup;
		calcule_position(pos.colonne - 1, &pos);
		grille[pos.colonne][pos.ligne] = jeton;
		affiche_grille();
		statut = statut_jeu(&pos, jeton);
		if (statut != OK)
			break;
		if (jeton == 'O')
			jeton = 'X';
		else
			jeton = 'O';
		action = true;
	}

	if (statut == GAGNE)
		printf("Victoire du joueur %i\n", (jeton == 'O') ? 1 : 2);
	else if (statut == EGALITE)
		printf("Egalite \n");
	return 0;
}