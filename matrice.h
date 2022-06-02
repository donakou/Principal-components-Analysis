#ifndef __MATRICE_H__
#define __MATRICE_H__

typedef struct cellule{
	float val;
	struct cellule * succ;
}cellule;



typedef struct elemtab {
	char * label;
	struct elemtab* next;
	struct cellule *tab;

}elemtab;

typedef struct last {
	cellule* tab;
	struct last* next;
}last;

typedef struct personne {
	char nom[30];
	struct personne* next;
}personne;
typedef struct matrix {
	int nl,nc;
	float* m;
}matrix;

typedef struct valp{float val;
	int pos;} valp;


int* nbdonnee(FILE* fichier);
void lecture(FILE* fichier, elemtab** ini, personne** pers);
matrix* newmat(elemtab *);
void moyenne(matrix*,float*);
void ecart_type(matrix*,float*,float*);
void centre_reduit(matrix*);
matrix* multi(matrix*,matrix*);
matrix* transposee(matrix*);
void affmatrix(matrix*);
matrix* correlation(matrix*);
matrix* jacobi(matrix*);
int compare(void const* u , void const* v);// il faut use des void ou void const 
void rangeons(matrix* , matrix*);
void supligne(elemtab** T, personne** pers,char* name);
void supcol(elemtab** T,int num);
int comp(char* name, char* nom);
void ajoutcolonne(elemtab**T);
void ajoutligne(elemtab**T,personne**pers);
void G_plot(matrix);
void traitement(elemtab*);
#endif
