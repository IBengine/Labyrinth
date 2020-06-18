#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#ifndef boolean
#define boolean int
#endif

enum choix_type_labyrinthe {fichier_test,generer_aleatoirement,generer_aleatoirement_difficultees_parcours};
enum direction {nord,est,sud,ouest};
enum type_elements_affichage {mur=-1,zero=48};

void lecture_fichier_text_donnee_tableau(int tableau_donnees[2],char crd[100]);
void lecture_fichier_text_labyrinthe(int dimension_m_ligne_ordonnee,int dimension_n_colonne_abscisse,int labyrinthe[dimension_m_ligne_ordonnee][dimension_n_colonne_abscisse],char crd[100]);
void Parcours_simple_labyrinthe_informations_deplacement(int dimension_m_ligne_ordonnee,int dimension_n_colonne_abscisse,int labyrinthe[dimension_m_ligne_ordonnee][dimension_n_colonne_abscisse],char cwd[100]);
void fonction_deplacement_robot_gauche_plus_affichage(int dimension_m_ligne_ordonnee,int dimension_n_colonne_abscisse,int labyrinthe[dimension_m_ligne_ordonnee][dimension_n_colonne_abscisse],int coordonnee_robot_ligne,int coordonnee_robot_colonne,int deplacement_t_moins_1,int coordonnee_sortie_colonne,int coordonnee_sortie_ligne,char cwd[100],boolean affichage_rapport_w_fichier);
void saisie_donnee(int tableau_donnees[3]);
void generation_aleatoire_labyrinthe(int tableau_donnees[3],int labyrinthe[tableau_donnees[0]][tableau_donnees[1]]);
void ecriture_fichier_text_donnee_tableau_plus_labyrinthe(int tableau_donnees[3],int labyrinthe[tableau_donnees[0]][tableau_donnees[1]],char cwd[100]);
void saisie_donnee_difficultees_parcours(int tableau_donnees[4]);
void generation_aleatoire_labyrinthe_difficultees_parcours(int tableau_donnees[4],int labyrinthe[tableau_donnees[0]][tableau_donnees[1]]);

void main()
{
    boolean validite_labyrinthe=FALSE; boolean saisie_validite_labyrinthe=FALSE;
    int type_labyrinthe=4; int dimension_m_ligne_ordonnee; int dimension_n_colonne_abscisse;
    char crd[100]; char cwd[100];
    getcwd(crd, sizeof(crd));
    memcpy(cwd, crd, strlen(crd)+1);
    strcat(crd,"\\Labyrinthe_r_v0-1.txt");
    strcat(cwd,"\\Labyrinthe_w_v0-1.txt");
    while (1)
    {
        while (type_labyrinthe>2||type_labyrinthe<0)
        {
            printf("Quel type de labyrinthe souhaitez vous avoir ?\n");
            printf("______________________________________________\n");
            printf("tapez 0 pour lire le fichier.txt de test\n");
            printf("tapez 1 pour generer aleatoirement un labyrinthe\n");
            printf("tapez 2 pour generer aleatoirement un labyrinthe avec des difficultes de parcours\n");
            scanf("%d",&type_labyrinthe);
        }
            if (type_labyrinthe==fichier_test)
            {
                int tableau_donnees[2];
                lecture_fichier_text_donnee_tableau(tableau_donnees,crd);
                dimension_m_ligne_ordonnee = tableau_donnees[0];
                dimension_n_colonne_abscisse = tableau_donnees[1];
                int labyrinthe[dimension_m_ligne_ordonnee][dimension_n_colonne_abscisse];
                lecture_fichier_text_labyrinthe(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,crd);
                Parcours_simple_labyrinthe_informations_deplacement(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,crd);
                return 0;
            }
            if (type_labyrinthe==generer_aleatoirement)
            {
                int tableau_donnees[3];
                saisie_donnee(tableau_donnees);
                int labyrinthe[tableau_donnees[0]][tableau_donnees[1]];
                while(validite_labyrinthe==0)
                {
                    generation_aleatoire_labyrinthe(tableau_donnees,labyrinthe);
                    do {
                        printf("Le labyrinthe est-il resoluble, tapez 1 pour oui et tapez 0 pour non\n");
                        scanf("%d",&validite_labyrinthe);
                    } while (validite_labyrinthe != 0 && validite_labyrinthe != 1);
                }
                ecriture_fichier_text_donnee_tableau_plus_labyrinthe(tableau_donnees,labyrinthe,cwd);
                dimension_m_ligne_ordonnee = tableau_donnees[0];
                dimension_n_colonne_abscisse = tableau_donnees[1];
                Parcours_simple_labyrinthe_informations_deplacement(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,cwd);
                return 0;
            }
            if (type_labyrinthe==generer_aleatoirement_difficultees_parcours)
            {
                int tableau_donnees[4];
                saisie_donnee_difficultees_parcours(tableau_donnees);
                int labyrinthe[tableau_donnees[0]][tableau_donnees[1]];
                while(validite_labyrinthe==FALSE)
                {
                    generation_aleatoire_labyrinthe_difficultees_parcours(tableau_donnees,labyrinthe);
                    printf("Le labyrinthe est-il resoluble, tapez 1 pour oui et tapez 0 pour non\n");
                    scanf("%d",&validite_labyrinthe);
                }
                ecriture_fichier_text_donnee_tableau_plus_labyrinthe(tableau_donnees,labyrinthe,cwd);
                dimension_m_ligne_ordonnee = tableau_donnees[0];
                dimension_n_colonne_abscisse = tableau_donnees[1];
                Parcours_simple_labyrinthe_informations_deplacement(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,cwd);
                return 0;
            }
    }
}

void lecture_fichier_text_donnee_tableau(int tableau_donnees[2],char crd[100])
{
	int dimension_m_ligne_ordonnee;
	int dimension_n_colonne_abscisse;
	FILE* fichier = NULL;
	fichier = fopen(crd, "r");
	if (fichier != NULL)
	{
		printf("Possible de lire le fichier (1) %s\n",crd);
		fscanf(fichier, "%d/n", &dimension_m_ligne_ordonnee);
		fscanf(fichier, "%d/n", &dimension_n_colonne_abscisse);
		tableau_donnees[0]=dimension_m_ligne_ordonnee;
		tableau_donnees[1]=dimension_n_colonne_abscisse;
	}
	else
	{
		printf("ERREUR Impossible de lire le fichier (1) %s\n",crd);
		scanf("%d",&dimension_m_ligne_ordonnee);
		return 0;
	}
	fclose(fichier);
	return 0;
}

void lecture_fichier_text_labyrinthe(int dimension_m_ligne_ordonnee,int dimension_n_colonne_abscisse,int labyrinthe[dimension_m_ligne_ordonnee][dimension_n_colonne_abscisse],char crd[100])
{
	int compteur_ligne;
	int compteur_colonne;
	char caractere_lu_type_car;
	int caractere_lu_type_int;
	FILE* fichier = NULL;
	fichier = fopen(crd, "r");
	if (fichier != NULL)
	{
		printf("Possible de lire le fichier (2) %s\n",crd);
		caractere_lu_type_car = fgetc(fichier);
		caractere_lu_type_car = fgetc(fichier);
		for (compteur_ligne = 0; compteur_ligne <= dimension_m_ligne_ordonnee-1; compteur_ligne++)
		{
			caractere_lu_type_car = fgetc(fichier);
			for (compteur_colonne = 0; compteur_colonne <= dimension_n_colonne_abscisse*2-1; compteur_colonne++)
			{
				caractere_lu_type_car = fgetc(fichier);
				if (caractere_lu_type_car==' ')
				{
					printf(" ");
				}
				else
				{
					if (caractere_lu_type_car=='*')
					{
						printf("%c",caractere_lu_type_car);
						labyrinthe[compteur_ligne][compteur_colonne/2]=-1;
					}
					else
					{
						caractere_lu_type_int=caractere_lu_type_car;
						if (caractere_lu_type_int==zero)
						{
							printf("0");
							labyrinthe[compteur_ligne][compteur_colonne/2]=0;
						}
					}
				}
			}
			printf("\n");
		}
	}
	else
	{
		printf("ERREUR Impossible de lire le fichier (2) %s\n",crd);
		scanf("%d",&dimension_m_ligne_ordonnee);
		return 0;
	}
	fclose(fichier);
	return 0;
}

void Parcours_simple_labyrinthe_informations_deplacement(int dimension_m_ligne_ordonnee,int dimension_n_colonne_abscisse,int labyrinthe[dimension_m_ligne_ordonnee][dimension_n_colonne_abscisse],char cwd[100])
{
	int coordonnee_sortie_ligne=0;int coordonnee_entree_ligne=0;int coordonnee_sortie_colonne=dimension_n_colonne_abscisse-1;
	int coordonnee_entree_colonne=0;int coordonnee_robot_colonne;int coordonnee_robot_ligne;int deplacement_t_moins_1=est;
	boolean affichage_rapport_w_fichier = TRUE;
	/*printf("billy\n");*/
	do {
		coordonnee_sortie_ligne=coordonnee_sortie_ligne+1;
	} while(labyrinthe[coordonnee_sortie_ligne][coordonnee_sortie_colonne]==-1);
	do {
		coordonnee_entree_ligne=coordonnee_entree_ligne+1;
	} while(labyrinthe[coordonnee_entree_ligne][coordonnee_entree_colonne]==-1);
	coordonnee_robot_colonne=coordonnee_entree_colonne;
	coordonnee_robot_ligne=coordonnee_entree_ligne;
	fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne,coordonnee_robot_colonne,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
	return 0;
}

void fonction_deplacement_robot_gauche_plus_affichage(int dimension_m_ligne_ordonnee,int dimension_n_colonne_abscisse,int labyrinthe[dimension_m_ligne_ordonnee][dimension_n_colonne_abscisse],int coordonnee_robot_ligne,int coordonnee_robot_colonne,int deplacement_t_moins_1,int coordonnee_sortie_colonne,int coordonnee_sortie_ligne,char cwd[100],boolean affichage_rapport_w_fichier)
{
  /*
  deux rotations_du robot possible:
    - RO = rotation ouest ou rotation trigonometrique/antihoraire
    - RE = rotation est ou rotation horaire
  */
	int deplacement_a_effectuer;
	FILE* fichier = NULL;
  /*printf("billy2\n");*/
	fichier = fopen(cwd,"r+");
  fseek(fichier, 0, SEEK_END);
  /*printf("billy1_%d\n",SEEK_END);*/
	if (fichier != NULL)
	{
		if (affichage_rapport_w_fichier == TRUE)
		{
		printf("Possible d'ecrire sur le fichier (4) %s\n",cwd);
		affichage_rapport_w_fichier = FALSE;
		}
		if ((coordonnee_robot_colonne!=coordonnee_sortie_colonne)||(coordonnee_robot_ligne!=coordonnee_sortie_ligne))
		{
			switch (deplacement_t_moins_1)
			{
				case nord:
				if (labyrinthe[coordonnee_robot_ligne][coordonnee_robot_colonne-1]!=-1)
				{
					deplacement_a_effectuer=ouest;
					printf("RO  ");
                    fseek(fichier, 0, SEEK_CUR);
                    printf("billy3_(%d)\n",SEEK_CUR);
                    fputs("RO  ",fichier);
                    fseek(fichier, 0, SEEK_CUR);
                    printf("billy4_(%d)\n",SEEK_CUR);
					printf("O  ");					fputs("O  ",fichier);
					deplacement_t_moins_1=deplacement_a_effectuer;
					fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne,coordonnee_robot_colonne-1,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
				}
				else
				{
					if (labyrinthe[coordonnee_robot_ligne-1][coordonnee_robot_colonne]!=-1)
					{
						deplacement_a_effectuer=nord;
						printf("N  ");						fputs("N  ",fichier);
						deplacement_t_moins_1=deplacement_a_effectuer;
						fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne-1,coordonnee_robot_colonne,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
					}
					else
					{
						if (labyrinthe[coordonnee_robot_ligne][coordonnee_robot_colonne+1]!=-1)
						{
							deplacement_a_effectuer=est;
							printf("RE  ");							fputs("RE  ",fichier);
							printf("E  ");							fputs("E  ",fichier);
							deplacement_t_moins_1=deplacement_a_effectuer;
							fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne,coordonnee_robot_colonne+1,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
						}
						else
						{
							if (labyrinthe[coordonnee_robot_ligne+1][coordonnee_robot_colonne]!=-1)
							{
								deplacement_a_effectuer=sud;
								printf("RE  ");								fputs("RE  ",fichier);
								printf("RE  ");								fputs("RE  ",fichier);
								printf("S  ");								fputs("S  ",fichier);
								deplacement_t_moins_1=deplacement_a_effectuer;
								fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne+1,coordonnee_robot_colonne,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
							}
						}
					}
				}
				break;
				case ouest:
				if (labyrinthe[coordonnee_robot_ligne+1][coordonnee_robot_colonne]!=-1)
				{
					deplacement_a_effectuer=sud;
					printf("RO  ");					fputs("RO  ",fichier);
					printf("S  ");					fputs("S  ",fichier);
					deplacement_t_moins_1=deplacement_a_effectuer;
					fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne+1,coordonnee_robot_colonne,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
				}
				else
				{
					if (labyrinthe[coordonnee_robot_ligne][coordonnee_robot_colonne-1]!=-1)
					{
						deplacement_a_effectuer=ouest;
						printf("O  ");						fputs("O  ",fichier);
						deplacement_t_moins_1=deplacement_a_effectuer;
						fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne,coordonnee_robot_colonne-1,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
					}
					else
					{
						if (labyrinthe[coordonnee_robot_ligne-1][coordonnee_robot_colonne]!=-1)
						{
							deplacement_a_effectuer=nord;
							printf("RE  ");							fputs("RE  ",fichier);
							printf("N  ");							fputs("N  ",fichier);
							deplacement_t_moins_1=deplacement_a_effectuer;
							fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne-1,coordonnee_robot_colonne,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
						}
						else
						{
							if (labyrinthe[coordonnee_robot_ligne][coordonnee_robot_colonne+1]!=-1)
							{
								deplacement_a_effectuer=est;
								printf("RE  ");								fputs("RE  ",fichier);
								printf("RE  ");								fputs("RE  ",fichier);
								printf("E  ");								fputs("E  ",fichier);
								deplacement_t_moins_1=deplacement_a_effectuer;
								fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne,coordonnee_robot_colonne+1,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
							}
						}
					}
				}
				break;
				case sud:
				if (labyrinthe[coordonnee_robot_ligne][coordonnee_robot_colonne+1]!=-1)
				{
					deplacement_a_effectuer=est;
					printf("RO  ");					fputs("RO  ",fichier);
					printf("E  ");					fputs("E  ",fichier);
					deplacement_t_moins_1=deplacement_a_effectuer;
					fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne,coordonnee_robot_colonne+1,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
				}
				else
				{
					if (labyrinthe[coordonnee_robot_ligne+1][coordonnee_robot_colonne]!=-1)
					{
						deplacement_a_effectuer=sud;
						printf("S  ");						fputs("S  ",fichier);
						deplacement_t_moins_1=deplacement_a_effectuer;
						fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne+1,coordonnee_robot_colonne,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
					}
					else
					{
						if (labyrinthe[coordonnee_robot_ligne][coordonnee_robot_colonne-1]!=-1)
						{
							deplacement_a_effectuer=ouest;
							printf("RE  ");							fputs("RE  ",fichier);
							printf("O  ");							fputs("O  ",fichier);
							deplacement_t_moins_1=deplacement_a_effectuer;
							fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne,coordonnee_robot_colonne-1,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
						}
						else
						{
							if (labyrinthe[coordonnee_robot_ligne-1][coordonnee_robot_colonne]!=-1)
							{
								deplacement_a_effectuer=nord;
								printf("RE  ");								fputs("RE  ",fichier);
								printf("RE  ");								fputs("RE  ",fichier);
								printf("N  ");								fputs("N  ",fichier);
								deplacement_t_moins_1=deplacement_a_effectuer;
								fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne-1,coordonnee_robot_colonne,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
							}
						}
					}
				}
				break;
				case est:
				if (labyrinthe[coordonnee_robot_ligne-1][coordonnee_robot_colonne]!=-1)
				{
					deplacement_a_effectuer=nord;
					printf("RO  ");					fputs("RO  ",fichier);
					printf("N  ");					fputs("N  ",fichier);
					deplacement_t_moins_1=deplacement_a_effectuer;
					fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne-1,coordonnee_robot_colonne,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
				}
				else
				{
					if (labyrinthe[coordonnee_robot_ligne][coordonnee_robot_colonne+1]!=-1)
					{
						deplacement_a_effectuer=est;
						printf("E  ");						fputs("E  ",fichier);
						deplacement_t_moins_1=deplacement_a_effectuer;
						fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne,coordonnee_robot_colonne+1,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
					}
					else
					{
						if (labyrinthe[coordonnee_robot_ligne+1][coordonnee_robot_colonne]!=-1)
						{
							deplacement_a_effectuer=sud;
							printf("RE  ");							fputs("RE  ",fichier);
							printf("S  ");							fputs("S  ",fichier);
							deplacement_t_moins_1=deplacement_a_effectuer;
							fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne+1,coordonnee_robot_colonne,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
						}
						else
						{
							if (labyrinthe[coordonnee_robot_ligne][coordonnee_robot_colonne-1]!=-1)
							{
								deplacement_a_effectuer=ouest;
								printf("RE  ");								fputs("RE  ",fichier);
								printf("RE  ");								fputs("RE  ",fichier);
								printf("O  ");								fputs("O  ",fichier);
								deplacement_t_moins_1=deplacement_a_effectuer;
								fonction_deplacement_robot_gauche_plus_affichage(dimension_m_ligne_ordonnee,dimension_n_colonne_abscisse,labyrinthe,coordonnee_robot_ligne,coordonnee_robot_colonne-1,deplacement_t_moins_1,coordonnee_sortie_colonne,coordonnee_sortie_ligne,cwd,affichage_rapport_w_fichier);
							}
						}
					}
				}
				break;
			}
		}
		fclose(fichier);
		return 0;
	}
	else
	{
		printf("ERREUR Impossible d'ecrire sur le fichier (4) %s\n",cwd);
		scanf("%d",&coordonnee_sortie_ligne);
		return 0;
	}
}

void saisie_donnee(int tableau_donnees[3])
{
    int dimension_m_ligne_ordonnee=0;
    int dimension_n_colonne_abscisse=0;
    int ratio_espace_nombre_cases_total=0;
    while (dimension_n_colonne_abscisse<=4)
    {
        printf("Entrer le nombre de colonnes superieur ou egale a 5\n");
        scanf("%d",&dimension_n_colonne_abscisse);
    }
    while (dimension_m_ligne_ordonnee<=4)
    {
        printf("Entrer le nombre de lignes superieur ou egale a 5\n");
        scanf("%d",&dimension_m_ligne_ordonnee);
    }
    while (ratio_espace_nombre_cases_total<40||ratio_espace_nombre_cases_total>100)
    {
        printf("Entrer le pourcentage de case sans mur entre 40 et 100\n");
        scanf("%d",&ratio_espace_nombre_cases_total);
    }
    tableau_donnees[0]=dimension_m_ligne_ordonnee;
    tableau_donnees[1]=dimension_n_colonne_abscisse;
    tableau_donnees[2]=ratio_espace_nombre_cases_total;
    return 0;
}

void generation_aleatoire_labyrinthe(int tableau_donnees[3],int labyrinthe[tableau_donnees[0]][tableau_donnees[1]])
{
	srand(time(NULL));
	int compteur;
	int compteur_ligne;
	int compteur_colonne;
	int coordonnee_sortie_ligne=(rand()%(tableau_donnees[0]-2))+1;
	int coordonnee_entree_ligne=(rand()%(tableau_donnees[0]-2))+1;
	int coordonnee_sortie_colonne=tableau_donnees[1]-1;
	int coordonnee_entree_colonne=0;
	int nombre_espace=((tableau_donnees[0]-2)*(tableau_donnees[1]-2)*(tableau_donnees[2]))/100;
	int coordonnee_m_ligne_ordonnee,coordonnee_n_colonne_abscisse;
	for (coordonnee_m_ligne_ordonnee=0; coordonnee_m_ligne_ordonnee<=tableau_donnees[0];coordonnee_m_ligne_ordonnee++)
	{
		for (coordonnee_n_colonne_abscisse=0; coordonnee_n_colonne_abscisse<=tableau_donnees[1];coordonnee_n_colonne_abscisse++)
		{
			labyrinthe[coordonnee_m_ligne_ordonnee][coordonnee_n_colonne_abscisse]=mur;
		}
	}
	labyrinthe[coordonnee_sortie_ligne][coordonnee_sortie_colonne]=0;
	labyrinthe[coordonnee_entree_ligne][coordonnee_entree_colonne]=0;
	for (compteur=0; compteur<=nombre_espace; compteur++)
	{
		do
		{
			coordonnee_m_ligne_ordonnee=(rand()%(tableau_donnees[0]-2))+1;
			coordonnee_n_colonne_abscisse=(rand()%(tableau_donnees[1]-2))+1;
		}while (labyrinthe[coordonnee_m_ligne_ordonnee][coordonnee_n_colonne_abscisse]>=0);
		labyrinthe[coordonnee_m_ligne_ordonnee][coordonnee_n_colonne_abscisse]=0;
	}
	for (compteur_ligne = 0; compteur_ligne <= tableau_donnees[0]-1; compteur_ligne++)
	{
		for (compteur_colonne = 0; compteur_colonne <= tableau_donnees[1]-1; compteur_colonne++)
		{
			if (labyrinthe[compteur_ligne][compteur_colonne]==0)
			{
				printf(" %d ",labyrinthe[compteur_ligne][compteur_colonne]);
			}
			if (labyrinthe[compteur_ligne][compteur_colonne]==-1)
			{
				printf("%d ",labyrinthe[compteur_ligne][compteur_colonne]);
			}
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

void ecriture_fichier_text_donnee_tableau_plus_labyrinthe(int tableau_donnees[3],int labyrinthe[tableau_donnees[0]][tableau_donnees[1]],char cwd[100])
{
	int compteur_ligne;
	int compteur_colonne;
	int compteur;
	FILE* fichier = NULL;
	fichier = fopen(cwd,"r+");
	if (fichier != NULL)
	{
		printf("Possible d'ecrire sur le fichier (3) %s\n",cwd);
		fprintf(fichier,"%d\n",tableau_donnees[0]);
		fprintf(fichier,"%d\n",tableau_donnees[1]);
		for (compteur_ligne = 0; compteur_ligne <= tableau_donnees[0]-1; compteur_ligne++)
		{
			for (compteur_colonne = 0; compteur_colonne <= tableau_donnees[1]-1; compteur_colonne++)
			{
				if (labyrinthe[compteur_ligne][compteur_colonne]==-1)
				{
					fprintf(fichier," *");
				}
				else
				{
					if (labyrinthe[compteur_ligne][compteur_colonne]==0)
					{
						fprintf(fichier," 0");
					}
					if (labyrinthe[compteur_ligne][compteur_colonne]==1)
					{
						fprintf(fichier," 1");
					}
					if (labyrinthe[compteur_ligne][compteur_colonne]==2)
					{
						fprintf(fichier," 2");
					}
					if (labyrinthe[compteur_ligne][compteur_colonne]==3)
					{
						fprintf(fichier," 3");
					}
					if (labyrinthe[compteur_ligne][compteur_colonne]==4)
					{
						fprintf(fichier," 4");
					}
					if (labyrinthe[compteur_ligne][compteur_colonne]==5)
					{
						fprintf(fichier," 5");
					}
					if (labyrinthe[compteur_ligne][compteur_colonne]==6)
					{
						fprintf(fichier," 6");
					}
					if (labyrinthe[compteur_ligne][compteur_colonne]==7)
					{
						fprintf(fichier," 7");
					}
					if (labyrinthe[compteur_ligne][compteur_colonne]==8)
					{
						fprintf(fichier," 8");
					}
					if (labyrinthe[compteur_ligne][compteur_colonne]==9)
					{
						fprintf(fichier," 9");
					}
					if (labyrinthe[compteur_ligne][compteur_colonne]==10)
					{
						fprintf(fichier," 10");
					}
				}
				fprintf(fichier," ");
			}
			fprintf(fichier,"\n");
		}
	}
	else
	{
		printf("ERREUR Impossible d'ecrire sur le fichier (3) %s\n",cwd);
		scanf("%d",&tableau_donnees[0]);
		return 0;
	}
  /*printf("billy0_%d\n",SEEK_END);*/
	fclose(fichier);
	return 0;
}

void saisie_donnee_difficultees_parcours(int tableau_donnees[4])
{
	int dimension_m_ligne_ordonnee=0;
	int dimension_n_colonne_abscisse=0;
	int ratio_espace_nombre_cases_total=0;
	int niveau_difficulte_max=0;
	while (dimension_n_colonne_abscisse<=4)
	{
		printf("Entrer le nombre de colonnes superieur ou egale a 5\n");
		scanf("%d",&dimension_n_colonne_abscisse);
	}
	while (dimension_m_ligne_ordonnee<=4)
	{
		printf("Entrer le nombre de lignes superieur ou egale a 5\n");
		scanf("%d",&dimension_m_ligne_ordonnee);
	}
	while (ratio_espace_nombre_cases_total<40||ratio_espace_nombre_cases_total>100)
	{
		printf("Entrer le pourcentage de case sans mur entre 40 et 100\n");
		scanf("%d",&ratio_espace_nombre_cases_total);
	}
	while (niveau_difficulte_max>10||niveau_difficulte_max<1)
	{
		printf("Entrer le niveau de difficultee maximum de 1 a 10\n");
		scanf("%d",&niveau_difficulte_max);
	}
	tableau_donnees[0]=dimension_m_ligne_ordonnee;
	tableau_donnees[1]=dimension_n_colonne_abscisse;
	tableau_donnees[2]=ratio_espace_nombre_cases_total;
	tableau_donnees[3]=niveau_difficulte_max;
	return 0;
}


void generation_aleatoire_labyrinthe_difficultees_parcours(int tableau_donnees[4],int labyrinthe[tableau_donnees[0]][tableau_donnees[1]])
{
	srand(time(NULL));
	int compteur;
	int compteur_ligne;
	int compteur_colonne;
	int coordonnee_sortie_ligne=(rand()%(tableau_donnees[0]-2))+1;
	int coordonnee_entree_ligne=(rand()%(tableau_donnees[0]-2))+1;
	int coordonnee_sortie_colonne=tableau_donnees[1]-1;
	int coordonnee_entree_colonne=0;
	int nombre_espace=((tableau_donnees[0]-2)*(tableau_donnees[1]-2)*(tableau_donnees[2]))/100;
	int coordonnee_m_ligne_ordonnee,coordonnee_n_colonne_abscisse;
	for (coordonnee_m_ligne_ordonnee=0; coordonnee_m_ligne_ordonnee<=tableau_donnees[0];coordonnee_m_ligne_ordonnee++)
	{
		for (coordonnee_n_colonne_abscisse=0; coordonnee_n_colonne_abscisse<=tableau_donnees[1];coordonnee_n_colonne_abscisse++)
		{
			labyrinthe[coordonnee_m_ligne_ordonnee][coordonnee_n_colonne_abscisse]=mur;
		}
	}
	labyrinthe[coordonnee_sortie_ligne][coordonnee_sortie_colonne]=rand()%(tableau_donnees[3]+1);
	labyrinthe[coordonnee_entree_ligne][coordonnee_entree_colonne]=rand()%(tableau_donnees[3]+1);
	for (compteur=0; compteur<=nombre_espace; compteur++)
	{
		do{
			coordonnee_m_ligne_ordonnee=(rand()%(tableau_donnees[0]-2))+1;
			coordonnee_n_colonne_abscisse=(rand()%(tableau_donnees[1]-2))+1;
		}while (labyrinthe[coordonnee_m_ligne_ordonnee][coordonnee_n_colonne_abscisse]>=0);
		labyrinthe[coordonnee_m_ligne_ordonnee][coordonnee_n_colonne_abscisse]=rand()%(tableau_donnees[3]+1);
	}
	for (compteur_ligne = 0; compteur_ligne <= tableau_donnees[0]-1; compteur_ligne++)
	{
		for (compteur_colonne = 0; compteur_colonne <= tableau_donnees[1]-1; compteur_colonne++)
		{
			if (labyrinthe[compteur_ligne][compteur_colonne]==-1||labyrinthe[compteur_ligne][compteur_colonne]>=10)
			{
				printf("%d ",labyrinthe[compteur_ligne][compteur_colonne]);
			}
			else
				{
					printf(" %d ",labyrinthe[compteur_ligne][compteur_colonne]);
				}
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}
