# pragma warning (disable:4996)
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>

# define EN_ATTENTE "en attente"                        // Situation de l'absence en attente         
# define COURS_TRAITEMENT "en cours de traitement"		// Situation de l'absence en cours de traitement
# define NON_JUSTIFIE "non-justifiee"					// Situation de l'absence non-justifiée
# define JUSTIFIE "justifiee"							// Situation de l'absence justifiée
# define VALIDEE "validee"								// Situation de l'absence validee
# define NON_VALIDEE "non-validee"						// Situation de l'absence non-validée


enum {
	MAX_ETUDIANT = 100,				// Maximum d'étudiant 
	MAX_NOM = 31,					// Maximum de la longueur du nom
	MAX_COMMANDE = 12,				// Maximum de la longueur de la commande qui sera entrée
	MAX_JOURS = 80,					// Nombre de jour total 
	MAX_ABSENCES = 8000,			// Nombre maximum d'absence
	MAX_JUST = 51,					// Longueur maximum de la justification
	MAX_DELAI = 3,					// Delai maximum pour justifier l'absence
	MAX_ABR = 3,					// Longueur maximum des abréviations (am, pm, ok, ko) 
	MAX_SITUATION = 25,				// Longueur maximum de la situation du traitement de l'absence
	MAX_NONJUST = 4,				// Delai avant q'une absence soit defini com Non justifier
	MIN_JOUR = 1,					// Nombre minimum de jour
	MIN_ETUDIANT = 1				// Nombre minimum d'étudiant
};


/* Tableau représentant l'ensemble des étudiants */
typedef struct {
	int id_etudiant;				// Identifiant de l'étudiant
	char nom[MAX_NOM];				// Nom de l'étudiant
	int groupe;						// Groupe de l'étudiant
} Etudiant;							// Nom du tableau

/* Tableau représentant les justificatif */
typedef struct {
	int id_absence;					// Identifiant de l'absence
	int jour_justification;			// Jour de la justification
	char raison[MAX_JUST];			// La raison de l'absence
}Justificatif;						// Nom du tableau

/* Tableau représentant les absences */
typedef struct {
	int id_absence;					// Identifiant de l'absence
	int id_etudiant;				// Identifiant de l'étudiant
	int jour;						// Jour de l'absence
	char demi_journee[MAX_ABR];     // Demi journée de l'absence
	char situation[MAX_SITUATION];  // Situation du traitement de l'absence
	Justificatif justification;     // Appel du tableau Justificatif dans le tableau Absence 
}Absence;							// Nom du tableau


/* Cette fonction permet de permuter les étudiants dans les fonctions de trie. */
void permutationEtudiant(Etudiant* e1, Etudiant* e2) {
	Etudiant tmp = *e1;
	*e1 = *e2;
	*e2 = tmp;
}

/* Cette fonction permet de permuter les absences dans les fonctions de trie. */
void permutationAbsences(Absence* a1, Absence* a2) {
	Absence tmp = *a1;
	*a1 = *a2;
	*a2 = tmp;
}

/* Cette fonction trie les identifiants des étudiants par ordre croissant */
void tri_id_etudiant(Etudiant inscriptions[], const int nombre_inscriptions) {
	for (int i = 0; i < nombre_inscriptions - 1; ++i) {
		for (int j = 0; j < nombre_inscriptions - i - 1; ++j) {
			if (inscriptions[j].id_etudiant > inscriptions[j + 1].id_etudiant) {
				permutationEtudiant(&inscriptions[j], &inscriptions[j + 1]);
			}
		}
	}
}

/* Cette fonction trie les identifiants des absences par ordre croissant */
void tri_id_absence(Absence absences[], const int nombre_absences) {
	for (int i = 0; i < nombre_absences - 1; ++i) {
		for (int j = 0; j < nombre_absences - i - 1; ++j) {
			if (absences[j].id_absence > absences[j + 1].id_absence) {
				permutationAbsences(&absences[j], &absences[j + 1]);
			}
		}
	}
}

/* Cette fonction permet de supprimer les espaces en trop et les retours à la ligne */
void enleverEspaceTrop(char* str) {
	// Cette partie de la fonction enlève l'espace de trop 
	int index = 0;
	while (str[index] != '\0' && str[index] == ' ') {
		index++;
	}

	int i = 0;
	while (str[index] != '\0') {
		str[i] = str[index];
		i++;
		index++;
	}

	str[i] = '\0';

	// Cette partie de la fonction enlève le retour à la ligne
	if (strlen(str) > 0 && str[strlen(str) - 1] == '\n') {
		str[strlen(str) - 1] = '\0';
	}
}

/*---------------------------------------------------------------------------------------------------C1---------------------------------------------------------------------------------------------------------*/

/* Cette fonction vérifie si l'étudiant est déjà présent dans le tableau inscriptions */
int verification(const char nom[], const int groupe, const Etudiant inscriptions[], const int nombre_inscriptions) {
	for (int i = 0; i < nombre_inscriptions; ++i) {
		if (strcmp(inscriptions[i].nom, nom) == 0 && inscriptions[i].groupe == groupe) {
			return 1;
		}
	}
	return 0;
}

/* Cette fonction permet d'inscrire un nouvel étudiant dans le tableau inscriptions */
void inscrire_etudiant(const char nom[], const int groupe, Etudiant inscriptions[], int(*nombre_inscriptions)) {
	// Cette partie vérifie si le nombre d'inscriptions ne dépasse pas le nombre maximum d'étudiants
	if (*nombre_inscriptions >= MAX_ETUDIANT) {
		return;
	}
	assert(strlen(nom) < MAX_NOM);
	// Cette partie de la fonction inscrit l'étudiant
	if (verification(nom, groupe, inscriptions, (*nombre_inscriptions)) == 0) {
		Etudiant e;
		strcpy(e.nom, nom);
		e.groupe = groupe;
		e.id_etudiant = (*nombre_inscriptions) + 1;
		inscriptions[(*nombre_inscriptions)] = e;
		++(*nombre_inscriptions);

		printf("Inscription enregistree (%d)\n", (*nombre_inscriptions));
	}
	else {
		printf("Nom incorrect\n");
	}
}
/*-------------------------------------------------------------------------------------------------Fin C1--------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------C2---------------------------------------------------------------------------------------------------------*/

/* Cette fonction permet de rajouter une absence d'un étudiant dans le tableau absences */
void absence_etudiant(const int id_etudiant, const  int jour, const  char demi_journee[], const int nombre_inscriptions, Absence absences[], int(*nombre_absences)) {
	if (*nombre_absences >= MAX_ABSENCES) {
		return;
	}
	if (id_etudiant < 1 || id_etudiant > nombre_inscriptions) {
		printf("Identifiant incorrect\n");
		return;
	}
	if (jour < MIN_JOUR || jour > MAX_JOURS) {
		printf("Date incorrecte\n");
		return;
	}
	assert(strlen(demi_journee) < MAX_ABR);
	if (strcmp(demi_journee, "am") != 0 && strcmp(demi_journee, "pm") != 0) {
		printf("Demi-journee incorrecte\n");
		return;
	}
	for (int i = 0; i < *nombre_absences; ++i) {
		if (absences[i].id_etudiant == id_etudiant && absences[i].jour == jour && strcmp(absences[i].demi_journee, demi_journee) == 0) {
			printf("Absence deja connue\n");
			return;
		}
	}

	// Cette partie de la fonction inscrit l'absence de l'étudiant
	Absence a;
	a.id_absence = (*nombre_absences) + 1;
	a.id_etudiant = id_etudiant;
	a.jour = jour;
	strcpy(a.demi_journee, demi_journee);
	strcpy(a.situation, EN_ATTENTE);
	a.justification.jour_justification = -1;
	absences[(*nombre_absences)] = a;
	++(*nombre_absences);
	printf("Absence enregistree [%d]\n", (*nombre_absences));
}
/*-------------------------------------------------------------------------------------------------Fin C2--------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------C3-----------------------------------------------------------------------------------------------------------*/

/* Cette fonction trie les étudiants par ordre croissant en fonction du groupe puis par ordre alphabétique au sein d'un même groupe */
void tri_etudiant(Etudiant inscriptions[], int nombre_inscriptions) {
	// Cette partie de la fonction trie les étudiants par groupe
	for (int i = 0; i < nombre_inscriptions - 1; ++i) {
		for (int j = 0; j < nombre_inscriptions - i - 1; ++j) {
			if (inscriptions[j].groupe > inscriptions[j + 1].groupe) {
				permutationEtudiant(&inscriptions[j], &inscriptions[j + 1]);
			}
		}
	}
	// Cette partie de la fonction trie les noms par ordre alphabétique si les groupes des étudiants sont égaux
	for (int i = 0; i < nombre_inscriptions - 1; ++i) {
		for (int j = 0; j < nombre_inscriptions - i - 1; ++j) {
			if (inscriptions[j].groupe == inscriptions[j + 1].groupe &&
				strcmp(inscriptions[j].nom, inscriptions[j + 1].nom) > 0) {
				permutationEtudiant(&inscriptions[j], &inscriptions[j + 1]);
			}
		}
	}
}

/* Cette fonction permet de compter le nombre d'absence d'un étudiant */
int compteur_absence(const int id_etudiant, int jour, const Absence absences[], const int nombre_absences) {
	int absences_etudiants = 0;
	for (int i = 0; i < nombre_absences; ++i) {
		if (absences[i].id_etudiant == id_etudiant && absences[i].jour <= jour) {
			++absences_etudiants;
		}
	}
	return absences_etudiants;
}

/* Cette fonction permet d'afficher tous les informations des étudiants et son nombre d'absence associée jusqu'au jour donné en entrée*/
void etudiants(const int jour, Etudiant inscriptions[], const int nombre_inscriptions, const Absence absences[], const int nombre_absences) {
	if (nombre_inscriptions == 0) {
		printf("Aucun inscrit\n");
		return;
	}
	else if (jour < MIN_JOUR || jour > MAX_JOURS) {
		printf("Date incorrecte\n");
		return;
	}
	tri_etudiant(inscriptions, nombre_inscriptions);

	for (int i = 0; i < nombre_inscriptions; ++i) {
		printf("(%d) %-9s %3d %d\n", inscriptions[i].id_etudiant, inscriptions[i].nom, inscriptions[i].groupe, compteur_absence(inscriptions[i].id_etudiant, jour, absences, nombre_absences));
	}

	tri_id_etudiant(inscriptions, nombre_inscriptions);
}
/*-------------------------------------------------------------------------------------------------Fin C3---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------C4-----------------------------------------------------------------------------------------------------------*/

/* Cette fonction permet de rentrer une justification pour une absence*/
void justification(const int id_absence, const int jour_justification, const char raison[MAX_JUST], Absence absences[], const int nombre_absences, int(*nombre_justificatifs)) {
	if (id_absence < MIN_ETUDIANT || id_absence > nombre_absences) {
		printf("Identifiant incorrect\n");
		return;
	}
	if (jour_justification < MIN_JOUR || jour_justification > MAX_JOURS) {
		printf("Date incorrecte\n");
		return;
	}

	if (absences[id_absence - 1].id_absence == id_absence && absences[id_absence - 1].justification.jour_justification == jour_justification) {
		printf("Justificatif deja connu\n");
		return;
	}

	assert(strlen(raison) < MAX_JUST);

	// Cette partie de la fonction permet d'inscrire la justification de l'absence de l'étudiant
	Absence absence = absences[id_absence - 1];
	Justificatif j;
	j.id_absence = id_absence;
	j.jour_justification = jour_justification;
	strcpy(j.raison, raison);
	absences[id_absence - 1].justification = j;
	++(*nombre_justificatifs);

	// Cette partie de la fonction change la situation de l'absence
	if (jour_justification <= absence.jour + MAX_DELAI) {
		strcpy(absences[id_absence - 1].situation, COURS_TRAITEMENT);
	}
	else {
		strcpy(absences[id_absence - 1].situation, NON_JUSTIFIE);
	}
	printf("Justificatif enregistre\n");
}
/*-------------------------------------------------------------------------------------------------Fin C4---------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------C5----------------------------------------------------------------------------------------------------------*/

/* Cette fonction trie les absences en fonction de l'identifiant de l'étudiant par ordre croissant et pour un même étudiant, par ordre chronologique */
void tri_validations(Absence absences[], const int nombre_absences) {
	// Cette partie de la fonction trie l'identifiant de l'étudiant par ordre croissant
	for (int i = 0; i < nombre_absences - 1; ++i) {
		for (int j = 0; j < nombre_absences - i - 1; ++j) {
			if (absences[j].id_etudiant > absences[j + 1].id_etudiant) {
				permutationAbsences(&absences[j], &absences[j + 1]);
			}
			// Cette partie de la fonction trie les jours par ordre chronologique si les identifiants des étudiants sont égaux
			else if (absences[j].id_etudiant == absences[j + 1].id_etudiant &&
				absences[j].jour > absences[j + 1].jour) {
				permutationAbsences(&absences[j], &absences[j + 1]);
			}
			// Cette partie de la fonction trie les matinés (am, pm) par ordre chronologique si les identifiants des étudiant et les jours des absences sont égaux
			else if (absences[j].id_etudiant == absences[j + 1].id_etudiant &&
				absences[j].jour == absences[j + 1].jour &&
				strcmp(absences[j].demi_journee, absences[j + 1].demi_journee) > 0) {
				permutationAbsences(&absences[j], &absences[j + 1]);
			}
		}
	}
}

/* Cette fonction vérifie si une absence peux être validé ou pas */
int verif_valid(Absence absences[], int nombre_absences) {
	int cpt = 0;
	for (int i = 0; i < nombre_absences; i++) {
		if (strcmp(absences[i].situation, COURS_TRAITEMENT) == 0) {
			cpt++;
		}
	}

	if (cpt == 0)
		return 1;
	else
		return 0;
}

/* Cette fonction affiche les absences des étudiants avec sa justification associée seulement si elle sont en cours de traitement */
void validations(const int nombre_justificatifs, Absence absences[], const int nombre_absences, const Etudiant inscriptions[]) {
	if (nombre_absences == 0 || nombre_justificatifs == 0) {
		printf("Aucune validation en attente\n");
		return;
	}

	if (!verif_valid(absences, nombre_absences)) {
		tri_validations(absences, nombre_absences);

		for (int i = 0; i < nombre_absences; ++i) {
			Absence a = absences[i];
			int tmp_etu_id = a.id_etudiant - 1;
			Etudiant e = inscriptions[tmp_etu_id];
			if (strcmp(a.situation, COURS_TRAITEMENT) == 0) {
				printf("[%d] (%d) %s %d %d/%s (%s)\n", a.id_absence, e.id_etudiant, e.nom, e.groupe, a.jour, a.demi_journee, a.justification.raison);
			}
		}
		tri_id_absence(absences, nombre_absences);
	}
	else
		printf("Aucune validation en attente\n");
}
/*-------------------------------------------------------------------------------------------------Fin C5--------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------C6---------------------------------------------------------------------------------------------------------------------*/

/* Cette fonction permet de valider une absence ou non*/
void validation(Absence absences[], const int id_absence, const int nombre_absences, char val_ref[]) {
	if (id_absence < 1 || id_absence > nombre_absences || (strcmp(absences[id_absence - 1].situation, COURS_TRAITEMENT) != 0 && strcmp(absences[id_absence - 1].situation, VALIDEE) != 0)) {
		printf("Identifiant incorrect\n");
		return;
	}

	assert(strlen(val_ref) < MAX_ABR);
	if (strcmp(val_ref, "ok") != 0 && strcmp(val_ref, "ko") != 0) {
		printf("Code incorrect\n");
		return;
	}

	if (strcmp(absences[id_absence - 1].situation, VALIDEE) == 0) {
		printf("Validation deja connue\n");
		return;
	}

	else {
		if (strcmp(val_ref, "ok") == 0) {
			strcpy(absences[id_absence - 1].situation, VALIDEE);
		}
		else {
			strcpy(absences[id_absence - 1].situation, NON_VALIDEE);
		}
		printf("Validation enregistree\n");
	}
}
/*-------------------------------------------------------------------------------------------------Fin C6---------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------C7---------------------------------------------------------------------------------------------------------------------*/

/* Cette fonction permet d'afficher les informations d'un étudiant et ses différents absences en fonction de leur situation */
void etudiant(int id_etudiant, int jour, Etudiant inscriptions[], int nombre_inscriptions, Absence absences[], int nombre_absences) {
	if (id_etudiant < 1 || id_etudiant > nombre_inscriptions) {
		printf("Identifiant incorrect\n");
		return;
	}
	if (jour < MIN_JOUR) {
		printf("Date incorrecte\n");
		return;
	}
	int cpt_en_attente = 0;
	int cpt_traitement = 0;
	int cpt_validee = 0;
	int cpt_non_val = 0;

	printf("(%d) %s %d %d\n", inscriptions[id_etudiant - 1].id_etudiant, inscriptions[id_etudiant - 1].nom, inscriptions[id_etudiant - 1].groupe, compteur_absence(id_etudiant, jour, absences, nombre_absences));
	tri_validations(absences, nombre_absences);

	// Cette partie de la fonction affiche tous les absences en attente
	for (int i = 0; i < nombre_absences; ++i) {
		if (absences[i].jour > jour) {
			continue;
		}
		if (id_etudiant == absences[i].id_etudiant && (jour <= absences[i].jour + MAX_DELAI && (absences[i].justification.jour_justification > jour || !strcmp(absences[i].situation, EN_ATTENTE)))) {
			if (cpt_en_attente == 0) {
				printf("- En attente justificatif\n");
			}
			printf("  [%d] %d/%s\n", absences[i].id_absence, absences[i].jour, absences[i].demi_journee);
			++cpt_en_attente;
		}
	}

	// Cette partie de la fonction permet d'afficher les absences en attente de validation 
	for (int i = 0; i < nombre_absences; ++i) {
		if (absences[i].jour > jour) {
			continue;
		}
		if (id_etudiant == absences[i].id_etudiant && strcmp(absences[i].situation, COURS_TRAITEMENT) == 0 && jour >= absences[i].justification.jour_justification) {
			if (cpt_traitement == 0) {
				printf("- En attente validation\n");
			}
			printf("  [%d] %d/%s (%s)\n", absences[i].id_absence, absences[i].jour, absences[i].demi_journee, absences[i].justification.raison);
			++cpt_traitement;
		}
	}

	// Cette partie de la fonction permet d'afficher les absences justifiées
	for (int i = 0; i < nombre_absences; ++i) {
		if (absences[i].jour > jour) {
			continue;
		}
		if (id_etudiant == absences[i].id_etudiant && strcmp(absences[i].situation, VALIDEE) == 0 && jour >= absences[i].justification.jour_justification) {
			if (cpt_validee == 0) {
				printf("- Justifiees\n");
			}
			printf("  [%d] %d/%s (%s)\n", absences[i].id_absence, absences[i].jour, absences[i].demi_journee, absences[i].justification.raison);
			++cpt_validee;
		}
	}

	// Cette partie de la fonction permet d'afficher les absences non-justifiées
	for (int i = 0; i < nombre_absences; ++i) {
		if (absences[i].jour > jour) {
			continue;
		}
		if (id_etudiant == absences[i].id_etudiant && ((strcmp(absences[i].situation, NON_JUSTIFIE) == 0 && jour >= absences[i].justification.jour_justification) ||
			(strcmp(absences[i].situation, NON_VALIDEE) == 0 && jour >= absences[i].justification.jour_justification) ||
			(strcmp(absences[i].situation, EN_ATTENTE) == 0 && jour > absences[i].jour + MAX_DELAI))) {
			if (cpt_non_val == 0) {
				printf("- Non-justifiees\n");
			}
			printf("  [%d] %d/%s", absences[i].id_absence, absences[i].jour, absences[i].demi_journee);
			if (!(strcmp(absences[i].situation, EN_ATTENTE) == 0 && jour > absences[i].jour + MAX_DELAI)) {
				printf(" (%s)\n", absences[i].justification.raison);
			}
			else {
				printf("\n");
			}
			++cpt_non_val;
		}
	}

	tri_id_absence(absences, nombre_absences);
}

/*-------------------------------------------------------------------------------------------------Fin C7-----------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------C8------------------------------------------------------------------------------------------------------------------------------*/

/* Cette fonction permet d'afficher tous les étudiants qui ont plus de 5 absences non-justifiées */
void defaillants(const int jour, Etudiant inscriptions[], const int nombre_inscriptions, const Absence absences[], const int nombre_absences) {
	int cpt_etuDef = 0;
	if (jour < MIN_JOUR) {
		printf("Date incorrecte\n");
		return;
	}
	tri_etudiant(inscriptions, nombre_inscriptions);

	for (int i = 0; i < nombre_inscriptions; ++i) {
		int cpt_nbrAbsenceEtu = 0;
		int cpt_absNonJu = 0;
		for (int j = 0; j < nombre_absences; ++j) {
			if (inscriptions[i].id_etudiant == absences[j].id_etudiant && absences[j].jour <= jour) {
				++cpt_nbrAbsenceEtu;
				if (inscriptions[i].id_etudiant == absences[j].id_etudiant && (!(absences[j].justification.jour_justification <= jour && (strcmp(absences[j].situation, VALIDEE) == 0 ||
					strcmp(absences[j].situation, COURS_TRAITEMENT) == 0) || absences[j].justification.jour_justification > jour || strcmp(absences[j].situation, EN_ATTENTE) == 0 && jour <= absences[j].jour + MAX_DELAI))) {
					++cpt_absNonJu;
				}
			}
		}
		if (cpt_absNonJu > MAX_NONJUST) {
			printf("(%d) %s %d %d\n", inscriptions[i].id_etudiant, inscriptions[i].nom, inscriptions[i].groupe, compteur_absence(inscriptions[i].id_etudiant, jour, absences, nombre_absences));
			++cpt_etuDef;
		}

	}
	if (cpt_etuDef == 0) {
		printf("Aucun defaillant\n");
	}
	tri_id_etudiant(inscriptions, nombre_inscriptions);
}
/*-------------------------------------------------------------------------------------------------Fin C8--------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------MAIN----------------------------------------------------------------------------------------------------------*/
int main() {
	char commande[MAX_COMMANDE];
	char nom[MAX_NOM];
	int groupe, id_etudiant, jour, id_absence;
	char demi_journee[MAX_ABR], val_ref[MAX_ABR];
	char raison[MAX_JUST];

	Etudiant inscriptions[MAX_ETUDIANT];
	Absence absences[MAX_ABSENCES];

	int nombre_inscriptions = 0;
	int nombre_absences = 0;
	int nombre_justificatifs = 0;


	do {
		scanf("%s", commande);

		if (strcmp(commande, "inscription") == 0) {																		// Appel de la commande "inscription"
			scanf("%s %d", nom, &groupe);
			inscrire_etudiant(nom, groupe, inscriptions, &nombre_inscriptions);											// Appel de la fonction 
		}
		else if (strcmp(commande, "absence") == 0) {																	// Appel de la commande "absence"
			scanf("%d %d %s", &id_etudiant, &jour, demi_journee);
			absence_etudiant(id_etudiant, jour, demi_journee, nombre_inscriptions, absences, &nombre_absences);			// Appel de la fonction 
		}
		else if (strcmp(commande, "etudiants") == 0) {																	// Appel de la commande "etudiants"
			scanf("%d", &jour);
			etudiants(jour, inscriptions, nombre_inscriptions, absences, nombre_absences);								// Appel de la fonction 
		}
		else if (strcmp(commande, "justificatif") == 0) {																// Appel de la commande "justificatif"
			scanf("%d %d", &id_absence, &jour);
			fgets(raison, MAX_JUST, stdin);																				// Utilisation de fgets pour permettre d'avoir les espaces dans la saisie de la raison
			enleverEspaceTrop(raison);																					// Appel de la fonction qui permet de retirer l'espace et le retour à la ligne en trop de fgets
			justification(id_absence, jour, raison, absences, nombre_absences, &nombre_justificatifs);					// Appel de la fonction qui inscrit la justification
		}
		else if (strcmp(commande, "validations") == 0) {																// Appel de la commande "validations"
			validations(nombre_justificatifs, absences, nombre_absences, inscriptions);									// Appel de la fonction 
		}
		else if (strcmp(commande, "validation") == 0) {																	// Appel de la commande "validation"
			scanf("%d %s", &id_absence, val_ref);
			validation(absences, id_absence, nombre_absences, val_ref);													// Appel de la fonction 
		}
		else if (strcmp(commande, "etudiant") == 0) {																	// Appel de la commande "etudiant"
			scanf("%d %d", &id_etudiant, &jour);
			etudiant(id_etudiant, jour, inscriptions, nombre_inscriptions, absences, nombre_absences);					// Appel de la fonction
		}
		else if (strcmp(commande, "defaillants") == 0) {																// Appel de la commande "defaillants"
			scanf("%d", &jour);
			defaillants(jour, inscriptions, nombre_inscriptions, absences, nombre_absences);							// Appel de la fonction 
		}
	} while (strcmp(commande, "exit") != 0);																			// Sortie / Fin du programme
}
/*-------------------------------------------------------------------------------------------------FIN-------------------------------------------------------------------------------------------------------------*/