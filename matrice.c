#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"
#include <math.h>


int* nbdonnee(FILE* fichier)
{
   if (fichier!=NULL)
    { 
	
	int*t;
	t=(int*)malloc(2*sizeof(int));
	//on compte le nb de colonnes
	
	char carac,c,u;
	int nbcolonne=0;

	//on va au début du fichier
	fseek(fichier,0,0);

	do{
		carac = fgetc(fichier);
		if(carac == ' ')
			nbcolonne = nbcolonne + 1;	
	}while((carac != '\n')&&(carac!= EOF));

	t[1]=nbcolonne;
	
	// nombre de lignes
	int nbl=0;
	fseek(fichier,0,SEEK_SET);
/*
	char nom[100];
	while(fgets(nom,100,fichier)!=NULL)
	{
		fgets(nom,100,fichier);nbl++;
		printf("\n%s",nom);
	}
*/
	u=fgetc(fichier);
	c=u;
	while (u!=EOF)
	{
		if (u=='\n')
		{
			nbl++;
		}
		c=u;
		u=fgetc(fichier);
	}
	if ((c!=EOF)&&(c!='\n'))
	{
		nbl++ ; // on est sur une ligne qui n'est pas terminée par /n
	}
	t[0]=nbl;
		
	return t;
   }
   else
   {
	int* t;
	t=(int*)malloc(2*sizeof(int));
	t[0]=-1;
	t[1]= -1;
	return t;
   }
}	


void lecture(FILE* fichier , elemtab** ini,personne** pers)
{
	if (fichier != NULL)
	{
		float u ;// char c;char z[30];
		personne * first; personne* other;
		elemtab* t; elemtab* tif;
		t = (elemtab*) malloc(sizeof(elemtab));
		*ini = t;
		tif= t;
		int* mes;
		mes = nbdonnee(fichier);
		int nbl = mes[0];
		int nbc = mes[1];
		last * l ; last* lif; int i ; int j;
		l=(last*)malloc(sizeof(last));
		lif=l;

		// creation de la ligne des labels: on a déjà une case au début
		for (j=1; j<nbc; j++)
		{
			tif->next = (elemtab*)malloc(sizeof(elemtab));
			lif->next= (last*)malloc(sizeof(last));
			lif = lif->next;
			tif = tif->next;
			tif->next=NULL;
			lif->next = NULL;
		} // fin de la ligne des labels
		
		//free(tif); free(lif); // on supprime la case en trop
		
		tif = t ; lif=l; fseek(fichier,0,0);// on s'apprete pour la ligne de donnees

		for (i=0;i<nbl;i++)
		{
			if(i==0) // la premiere ligne
			{
			
				first=(personne*)malloc(sizeof(personne));other=first;*pers=first;
				fscanf(fichier,"%s", other->nom ); // on lit le nom
				printf(" %10s\t",other->nom);
				for (j=0;j<nbc;j++)
				{
					fscanf(fichier,"%f",&u);
					lif->tab=(cellule*)malloc(sizeof(cellule));
					lif->tab->val=u;
					tif->tab= lif->tab;
					printf(" %1.3f\t",lif->tab->val);
					lif=lif->next;
					tif=tif->next;
				}
				printf("\n");
				lif=l;
				tif=t;
				
			}
			
			else
			{
				other->next = (personne*)malloc(sizeof(personne));
				other=other->next;
				fscanf(fichier,"%s",other->nom);
				printf(" %10s\t",other->nom);
				for(j=0;j<nbc;j++)
				{
					fscanf(fichier,"%f",&u);
					lif->tab->succ=(cellule*)malloc(sizeof(cellule));
					lif->tab=lif->tab->succ;
					lif->tab->val = u ;
					printf(" %1.3f\t",lif->tab->val);
					lif = lif->next;
				}
				printf("\n");
				lif=l;
			}
		}
		
		

	}
	
}


matrix* newmat(elemtab*T)
{
//on ecrit un programme indépendant du fichier car il sera utilisé en cas de modification de données

	matrix* mat;
	mat= (matrix*) malloc(sizeof(matrix));
	int nl,nc,j,i;
	elemtab* tif;
	nc=0;
	nl=0;
	cellule* cel;
	tif = T;
	
	
	// nombre de lignes
	if (T->tab!=NULL)	// il y a au moins une donnée
	{
		cel = T->tab;
		do 
		{
			nl= nl+1;
			cel = cel->succ;
		}while (cel!=NULL);
	}

	//nombre de colonnes
	
	if (nl!=0)		// s'il y a une donnée
	{

		while (tif!=NULL)
		{
			nc++;
			tif=tif->next;
		}
		
	}

	tif= T;
	
	mat->nl=nl;
	mat->nc=nc;
	mat->m = (float*)malloc(nl*nc*sizeof(float));
	for (j=0;j<nc;j++)
	{
		cel=tif->tab;

		for (i=0;i<nl;i++)
		{
			mat->m[i*nc+j] = cel->val;
			cel = cel->succ;
		}
		tif=tif->next ;// on va  à la colonne suivante
	}
return mat;
}	

void affmatrix(matrix* matrice)
{
	int i,j;
	for (i=0;i<matrice->nl;i++)
	{
		for (j=0;j<matrice->nc;j++)
		{
			printf("%2f\t",matrice->m[i*matrice->nc+j]) ;

		}
		printf("\n");
	}
	printf("\n");
}
	
void moyenne(matrix* matrice,float *moy)//calcul des moyennes de chaque colonne stocké dans un tableau
{
	int i,j;
	//a la sortie de cette double boucle moy[i] est égale à la somme des valeur de la colonne i
	for(i=0;i<matrice->nc;i++)
	{
		
		for(j=0;j<matrice->nl;j++)
		{
			moy[i]=moy[i]+matrice->m[i+j*matrice->nc];
		}
	}
	//on divise la valeur précédente par le nombre de valeur d'une colonne et on obtient la moyenne de cette colonne 
	for(i=0;i<matrice->nc;i++)
	{
		moy[i]=moy[i]/matrice->nl;
	}
}


void ecart_type(matrix * matrice,float *moy, float *ecart)
{
	int i,j;
	float calc;
	for(i=0;i< matrice->nc;i++)//pour chaque colonne
	{
		//on calcul l'écart type
		calc=0;
		for(j=0;j<matrice->nl;j++)
		{
			calc = calc + (matrice->m[j*matrice->nc+i] - moy[i]) * (matrice->m[j*matrice->nc+i] - moy[i]);
		}
		ecart[i] = sqrt((calc/matrice->nl));// ici calc/matrice->nl = variance
	}
}


void centre_reduit(matrix* matrice)
{
	float* moy,*ecart;
	int i,j;

	moy = (float*)calloc(matrice->nc,sizeof(float));
	ecart = (float*)calloc(matrice->nc,sizeof(float));

	moyenne(matrice,moy);
	ecart_type(matrice,moy,ecart);

	for(i=0;i<matrice->nc;i++)
	{
		for(j=0;j<matrice->nl;j++)
		{
			matrice->m[j*matrice->nc+i] = (matrice->m[j*matrice->nc+i] - moy[i])/ecart[i];
		}
	}
}

matrix* multi(matrix* mat,matrix* p)
{
	int i,j,k;
	matrix* prod;
	prod = (matrix*)malloc(sizeof(matrix));
	if (mat->nc==p->nl)
	{
		prod->nc=p->nc;
		prod->nl=mat->nl;
		prod->m=(float*)malloc((prod->nc*prod->nl)*sizeof(float));
		for (i=0 ; i<mat->nl;i++)
		{
			for ( j = 0; j < p->nc; j++)
			{
				prod->m[i*prod->nc+j]=0;
				for ( k = 0; k < mat->nc; k++)
				{
					prod->m[i*prod->nc+j]=prod->m[i*prod->nc+j]+(mat->m[i*mat->nc+k]*p->m[k*p->nc+j]);
				}
			}
		}
		return prod;
	}
	else
	{
		i=0;
	}
}

matrix* multiscalair(matrix* matrice, float x)
{
	int i;
	matrix* prod;
	prod = (matrix*)malloc(sizeof(matrix));
	prod->nc=matrice->nc;
	prod->nl=matrice->nl;
	prod->m=(float*)malloc((prod->nc*prod->nl)*sizeof(float));
	for(i=0;i<matrice->nc*matrice->nl;i++)
	{
		prod->m[i]= x * matrice->m[i];
	}
	return prod;
}

matrix* transposee(matrix* matrice)
{
	int i,j;
	matrix* tmat;
	tmat = (matrix*)malloc(sizeof(matrix));
	tmat->nc=matrice->nl;
	tmat->nl=matrice->nc;
	tmat->m=(float*)malloc(matrice->nl*matrice->nc*sizeof(float));
	for(i=0;i<matrice->nc;i++)
	{
		for(j=0;j<matrice->nl;j++)
		{
			tmat->m[i*matrice->nl+j] = matrice->m[j*matrice->nc +i];
		}
	}
	return tmat;
}


matrix* correlation(matrix* matrice)
{
	matrix* cor;//matrice de correlation de matrice

	cor=(multiscalair(multi(transposee(matrice),matrice),(1/(float)matrice->nl)));

	return cor;
}

matrix* jacobi(matrix* corr)
{
	int n,i,j,k,l;
	n = corr->nl;
	float* A,*B,*D;
	//float* desalloc;
	matrix *retour;
	retour = (matrix *)malloc(2*sizeof(matrix));
	
	A=(float*)malloc(n*n*sizeof(float));
	B=(float*)malloc(n*sizeof(float));
	D=(float*)malloc(n*sizeof(float));
	
	matrix P,Q;
	P.nl=n;
	P.nc=n;
	Q.nc=n;
	Q.nl=n;
	
	//on recopie la matrice initiale 
	for(i=0;i<n*n;i++)
	{
		A[i]=corr->m[i];		
	}
	//on initialise la matrice de vecteurs propre à l'identité : matrice  0 puis 1 sur les diagonales
	P.m=(float*)calloc(n*n,sizeof(float));
	Q.m=(float*)calloc(n*n,sizeof(float));
	for(i=0;i<n*n;i++)
	{
		P.m[i]=0;		
	}

	for(i=0;i<n;i++)
	{
		P.m[i*n+i]=1;
	}

	int me=1;
	float eps,t,c,s;
	
	while( me == 1)//me==0 si en parcourant toute la matrice on a rien modifié
	{
		me=0; //on n'a pas envie de modifié
		for(i=0;i<n;i++)
		{
			for(j=i+1;j<n;j++)
			{
				if((A[i*n+j] > 0.000000000000001)||((-A[i*n+j]) > 0.000000000000001))
				{
					me=1; // on modifie
					eps= (A[j*n+j]-A[i*n+i])/(2*A[i*n+j]);
					if(eps < 0)
					{
						t= - eps - sqrt(1+(eps*eps));
					}
					else{
						t= -eps + sqrt(1+(eps*eps));
					}
					c=1/(sqrt(1+(t*t)));
					s=t/(sqrt(1+(t*t)));
					for(k=0;k<n;k++)
					{
						B[k]=A[i*n+k];
						D[k]=A[k*n+j];
					}
					for(k=0;k<n;k++)
					{
						A[i*n+k]=c*B[k] - s*D[k];
						A[k*n+j]=s*B[k] + c*D[k];
						//on recopie ensuite pour symétrie
						A[k*n+i] = A[i*n+k];
						A[j*n+k] = A[k*n+j];
					}
					//on traite l'élément diagonal puis l'élément i,j
					A[j*n+j] = D[j] + t*B[j];
					A[i*n+i] = B[i] - t*D[i];
					A[i*n+j] = 0;
					A[j*n+i] = 0; // par symétrie
					
					//on construit la matrice Q due à la transformation
					for (l=0;l<n*n;l++)
					{
						Q.m[l]=0;
					}

					for(l=0;l<n;l++)
					{
						Q.m[l*n+l]=1;
					}
					Q.m[i*n+i] = c;
					Q.m[j*n+j] = c;
					Q.m[i*n+j] = s;
					Q.m[j*n+i] = -s;
					
					//desalloc = P.m;
						
					P=*multi(&P,&Q);
					//free(desalloc);
					
				}
			}
		}
	}
	retour[0].nl = n;
	retour[0].nc = n;
	retour[1].nl = n;
	retour[1].nc = n;
	retour[0].m = A;//matrice diagonale
	retour[1].m = P.m;//vecteurs propres
	return retour;
}

int compare(void const* u , void const* v)
{
	float gv,gu;
	gv=((valp*)v)->val;
	gu = ((valp*)u)->val;
	if ((gv-gu)>0)
	{
		return(1);
	}
	if ((gv-gu)<0)
	{
		return(-1);
	}		
	else
	{
		return(0);
	}
		// tri décroissant
}	

void rangeons(matrix* diag , matrix* p )
	{
		int n = p->nl;
		int i,j;
		valp l[n];
		float* pro; // on y mettra la nouvelle matrice de vecteurs propres
		pro = (float*)malloc((n*n)*sizeof(float));
		for (i=0;i<n;i++)
		{
			l[i].val = diag->m[i*n+i]; // on recopie les valeurs propres et leurs positions 
			l[i].pos = i;
			
		}
		printf("\n");
		qsort(l,n,sizeof ( *l),compare);
		
		for (j=0;j<n;j++)
		{
			for (i=0;i<n;i++)
			{
				pro[i*n+j]= p->m[i*n+l[j].pos]; // on met dans la colonne j , le vecteur propre de la j ème valeur propre du tableau ordonné
			}
			diag->m[j*n+j] = l[j].val; // on met la j ième valeur propre à la position j de la diagonale
		}

		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				p->m[i*n+j]=pro[i*n+j]; // on met la matrice initiale de vecteurs propres à jour
			}
		}

	}
int comp(char* name, char* nom)
{
	int i;i=0;
	while((name[i]!='\0')&&(name[i]==nom[i]))
	{
		i++;
	}
	if(name[i]==nom[i])
	{
		return 0;
	}
	return -1;
}

void supligne(elemtab** T, personne** pers,char* name)
{
	elemtab* tif; personne* p; personne* after; int k,j;
	cellule* h; cellule* g;
	tif = *T; p=*pers; k=1;
	while((p!=NULL)&&(comp(p->nom,name)!=0))
	{
		k++;
		p= p->next;

	}

	if (p!=NULL) // la ligne à supprimer
	{
		p = *pers;
		//suppression du nom
		
		for (j=1;j<k-1;j++)
		{
			p=p->next;
		}
		if (k==1) // c'est la première valeur qu'on supprime
		{
			*pers = p->next; // on modifie le pointeur sur la première valeur( on mod le contenu du pointeur sur pointeur)
		}
		else
		{
			after = p->next;
			p->next = after->next;
			free(after);
		}
		
		int cpt=0;
		// suppression de la ligne
		
		while (tif!=NULL)	// tant qu'on n'a pas dépassé la dernière colonne:
		{	
			cpt++ ; // on compte le nb de colonnes
			h=tif->tab;

			for (j=1;j<k-1;j++)
			{
				h=h->succ;
			} // on décale jusqu'à la case précédent celle qu'on supprime
			if (k==1)
			{
				tif->tab = h->succ;
				tif = tif->next;
			}
			else
			{
				g = h->succ;
				h->succ = g->succ;
				free(g);
			}
			tif=tif->next;
			
		}
				
		
	}
}


void supcol(elemtab** T,int num)
{

	elemtab* tif; int cpt, nbc; 
	elemtab* tmp ; 
	cellule* truf;
	cellule* taf;
	tif = *T;
	cpt= 1 ;
	nbc=1;

	// nombre de colonnes
	while ((tif ->next!= NULL))
		{	
			tif = tif->next;
			nbc++;			
		}
	tif = *T;

	if (num == 1)
	{
		*T=tif->next;
	}
	else
	{
		// on se place sur la bonne colonne
		while ((tif ->next!= NULL)&&(cpt!=num))
		{	
			tmp = tif;
			tif = tif->next;
			cpt++;			
		}
	}
	
	if ((num<=nbc)&&(num >=0))
	{

		truf=tif->tab;
		taf = tif->tab;
		tmp -> next = tif->next;	// on décalle vers le suivant du suivant
		tif->tab = NULL;
		free (tif); // on désalloue la première case
	

		while(taf !=NULL)
		{
			taf= taf->succ;
			free(truf);
			truf=taf;	// on stoque la case suivante , on déshaloue la case actuelle et on passe à la suivante on le fait de haut en bas.
		}
	
	}
}

void ajoutcolonne(elemtab**T)

{
	
	elemtab* tif;
	int nl=0;
	cellule* cel;
	tif = *T; 
	float p;
	
	
	// nombre de lignes
	if (tif->tab!=NULL)	// il y a au moins une donnée
	{
		cel = tif->tab;
		do 
		{
			nl++;
			cel = cel->succ;
		}while (cel!=NULL);
	}
	
	
	printf(" \nle tableau à traiter contient %d lignes (s)\n entrez les données du nouveau critère relatives à chaque individu \n",nl);
	
	tif = (elemtab*)malloc(sizeof(elemtab));
	tif->next = *T; 
	*T = tif ;
	if (nl>0)
	{
		nl--;
		tif->tab= (cellule*)malloc(sizeof(cellule));
		cel=tif->tab;
		scanf("%f",&p);
		cel->val=p;		
		
		while (nl !=0)
		{
			cel->succ = (cellule*)malloc(sizeof(cellule));	
			cel=cel->succ;
			scanf("%f",&p);
			cel->val = p;
			nl--;
		}
		cel->succ=NULL;	
	}	
	

}

void ajoutligne(elemtab**T,personne**pers)
{
	if (*T !=NULL )

	{

		elemtab* tif;
		tif = *T;
		cellule*cel;
		int nc=0;
		

		
		
		if (tif->tab!= NULL)		// s'il y a une donnée
		{
						//nombre de colonnes
			while (tif!=NULL)
			{
				nc++;
				tif=tif->next;
			}

			tif = *T;
			printf(" \nentrez le nom du nouvel individu  ");
			personne* u;
			u = (personne*)malloc(sizeof(personne)) ;
			u->next = *pers;
			*pers = u;
			scanf("%s",u->nom);
			printf(" \nle tableau à traiter contient %d colonne(s)\n entrez les données du nouvel individu \n",nc);

						// ajout en début de liste
			while(nc!=0)
			{
				cel = (cellule*)malloc(sizeof(cellule));
				cel->succ = tif->tab;
				tif->tab=cel;
				float u;
				scanf("%f",&u);
				cel->val = u;
				tif = tif->next;
				nc--;
			
			}
			
		}
	}	
}

void G_plot(matrix F)
{
	int i=0;

	FILE *fp,*fq;
	fp = fopen("gnu.gnu","w");
	if (fp == NULL)
	{
		printf("Impossible d'ouvrir le fichier gnu.gnu");
		exit(0);
	}
	fprintf(fp,"set terminal png\n");
	fprintf(fp,"set output 'graph.png'\n");
	fprintf(fp," set zeroaxis\n"
		//"set  autoscale\n"
		"set xtic auto\n"
		"set ytic auto\n");
	fq = fopen("data.dat","w");
	if (fq == NULL)
	{
		printf("Impossible d'ouvrir le fichier data.dat");
		exit(0);
	}
	for(i=0;i<F.nl;i++)
	{
		fprintf(fq, "%f %f\n", F.m[i*F.nc],F.m[i*F.nc+1]);
	}

	fprintf(fp, "plot 'data.dat' with points\n");

	fclose(fq);
	fclose(fp);

}

void traitement(elemtab *T)
{
	FILE * fichbin;
	fichbin = fopen("sav.bin" , "ab");
	if (fichbin == NULL)
	{
		printf("Impossible d'ouvrir le fichier sav.bin");
		exit(0);
	}
	matrix* matrice;
	matrix* cor;
	matrix* mat_ini;
	mat_ini = newmat(T);
	matrice = newmat(T);
	affmatrix(matrice);
	fwrite( matrice->m , sizeof(matrice->m[0]) , sizeof(matrice->m)/sizeof(matrice->m[0]) , fichbin);

	printf("Matrice centré réduite \n");
	centre_reduit(matrice);
	affmatrix(matrice);
	fwrite( matrice->m , sizeof(matrice->m[0]) , sizeof(matrice->m)/sizeof(matrice->m[0]) , fichbin);

	printf("Matrice de Correlation \n");
	cor=correlation(matrice);
	affmatrix(cor);
	fwrite( cor->m , sizeof(cor->m[0]) , sizeof(cor->m)/sizeof(cor->m[0]) , fichbin);

	matrix * diag;
	diag=jacobi(cor); // dans le contenu de diag

	rangeons(diag,(diag+1));
	printf("Matrice diagonale \n");
	affmatrix(diag);
	fwrite( diag->m , sizeof(diag->m[0]) , sizeof(diag->m)/sizeof(diag->m[0]) , fichbin);

	printf("\nvecteurs propres\n");
	affmatrix((diag+1));
	fwrite( (diag+1)->m , sizeof((diag+1)->m[0]) , sizeof((diag+1)->m)/sizeof((diag+1)->m[0]) , fichbin);

	matrix *mat_gnu = multi(mat_ini,(diag+1));
	affmatrix(mat_gnu);

	G_plot(*mat_gnu);
	fclose(fichbin);

}


