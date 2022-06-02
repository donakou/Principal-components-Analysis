#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"
#include <math.h>





int main()
{
	elemtab *T;
	int n;
	char c;
	char* nom;
	nom=(char*)malloc(20*sizeof(char));
	char* sup;
	char* defaut;
	defaut="donnee.txt";
	FILE* fichier;
	printf("Voulez-vous utilisé le fichier par défaut ?(y or n)\n");
	scanf("%s",&c);
	if(c == 'y')
	{
		fichier=fopen(defaut,"r");
	}
	else
	{
		printf("Entrez le nom d'un fichier de données :\n");
		scanf("%s",nom);
		fichier=fopen(nom,"r");
	}
	
	if (fichier == NULL)
	{
		printf("Impossible d'ouvrir le fichier ");
		exit(0);
	}
	personne* pers; // permet d'avor la liste des personnes
	int* t;
	t=nbdonnee(fichier);
	printf("nb de colonnes est %d  et celui de ligne est %d\n",t[1],t[0]);
	
	n=t[1]; // on recueille le nombre de colonnes
	lecture(fichier,&T,&pers);
	printf("Lecture s'est bien terminée \n");

	traitement(T);
	


	printf("Voulez-vous supprimer une ligne ?(tapez y pour Oui et n pour Non) \n");

	scanf("%s",&c);
	if(c=='y')
	{
		printf("Suppression de ligne :\n");
		printf("entrez le nom de la personne à supprimer de la liste  ");
		sup = (char*)malloc(15*sizeof(char));
		scanf("%s",sup);
	
		supligne(&T,&pers,sup);
		printf("traitement sans %s :\n",sup);
		traitement(T);
	}


	printf("Voulez-vous supprimer une colonne ?(tapez y pour Oui et n pour Non) \n");
	scanf("%s",&c);
	if(c=='y')
	{
		printf("Supression de colonne :\n");
		printf("entrez le numéro de la colonne à supprimer de la liste  ");
		int num;
		scanf("%i",&num);
		supcol(&T,num);
		printf("traitement sans la colonne n°%d :\n",num);
		traitement(T);
	}

	printf("Voulez-vous ajouter une colonne ?(tapez y pour Oui et n pour Non) \n");
	scanf("%s",&c);
	if(c=='y')
	{
		// ajout de données
		printf("Ajout de colonne :\n");
		ajoutcolonne(&T);
		printf("traitement avec la nouvelle colonne :\n");
		traitement(T);
	}

	printf("Voulez-vous ajouter une ligne ?(tapez y pour Oui et n pour Non) \n");
	scanf("%s",&c);
	if(c=='y')
	{
		// ajout ligne
		printf("Ajout de ligne :\n");
		ajoutligne(&T,&pers);

		printf("traitement avec la nouvelle ligne :\n");
		traitement(T);
	}
	
	fclose(fichier);
	
	return 0;
}



