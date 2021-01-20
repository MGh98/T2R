//
// Created by Thib on 14/12/2020.
//

#include "move1.h"
#include "game.h"
#include <limits.h>


/* ask for a move */
void askMove(t_move* move, int* firstmove){
	/* ask for the type */
	printf("Choose a move\n");
	printf("1. claim a route\n");
	printf("2. draw a blind card\n");
	printf("3. draw a card\n");
	printf("4. draw objectives\n");
	printf("5. choose objectives\n");
	if(*firstmove == 0)
	{
		move->type = 4;
		*firstmove=1;

	}
	//scanf("%d", &move->type);
	printf("move is %d\n",move->type );

	/* ask for details */
	if (move->type == CLAIM_ROUTE) {
		printf("Give city1, city2, color and nb of locomotives: ");
		//scanf("%d %d %d %d", &move->claimRoute.city1, &move->claimRoute.city2, &move->claimRoute.color,
		//	  &move->claimRoute.nbLocomotives);
	}
	else if (move->type == DRAW_CARD) {
		printf("Give the color: ");
		//scanf("%d", &move->drawCard.card);
	}
	else if (move->type == CHOOSE_OBJECTIVES){
		printf("For each objective, give 0 or 1:");
		//scanf("%d %d %d", &move->chooseObjectives.chosen[0], &move->chooseObjectives.chosen[1], &move->chooseObjectives.chosen[2]);
	}
	

}




/* plays the move given as a parameter (send to the server)
 * returns the return code */
t_return_code playOurMove(t_move* move, t_color* lastCard){
	t_return_code ret;

	switch (move->type) {
		case CLAIM_ROUTE:
			ret = claimRoute(move->claimRoute.city1, move->claimRoute.city2, move->claimRoute.color, move->claimRoute.nbLocomotives);
			*lastCard = NONE;
			break;
		case DRAW_CARD:
			ret = drawCard(move->drawCard.card, move->drawCard.faceUp);
			*lastCard = (*lastCard==NONE && move->drawCard.card!= MULTICOLOR) ? move->drawCard.card : NONE;
			break;
		case DRAW_BLIND_CARD:
			ret = drawBlindCard(&move->drawBlindCard.card);
			*lastCard = (*lastCard==NONE) ? move->drawBlindCard.card : NONE;
			break;
		case DRAW_OBJECTIVES:
			ret = drawObjectives(move->drawObjectives.objectives);
			for(int i=0; i<3; i++){
				printf("%d. %d (", i, move->drawObjectives.objectives[i].city1);
				printCity(move->drawObjectives.objectives[i].city1);
				printf(") -> (");
				printCity(move->drawObjectives.objectives[i].city2);
				printf(") %d (%d pts)\n", move->drawObjectives.objectives[i].city2, move->drawObjectives.objectives[i].score);
			}
			*lastCard = NONE;
			break;
		case CHOOSE_OBJECTIVES:
			ret = chooseObjectives(move->chooseObjectives.chosen);
			*lastCard = NONE;
			break;
	}

	return ret;
}


/* tell if we need to replay */
int needReplay(t_move* move, t_color lastCard){
	int replay = 0;

	if (move->type == DRAW_OBJECTIVES)
		replay = 1;
	else if (move->type == DRAW_BLIND_CARD && lastCard == NONE)
		replay = 1;
	else if (move->type == DRAW_CARD && lastCard == NONE && move->drawCard.card != MULTICOLOR)
		replay = 1;

	return replay;
}

int distanceMini(int nbCities, int Visite[nbCities],int D[nbCities])
{
	int min, indice_min;
	min = INT_MAX;

	for(int i=0;i<nbCities;i++)
	{
		if (Visite[i]==0 && D[i]<min)
		{
			min=D[i];
			indice_min=i;
		}
	}
	return indice_min;
}

//Actuelle fonction utilisée
void algorithme(int src,int nbCities, int dest, int G[nbCities][nbCities],int D[nbCities],int Prec[nbCities])
{
	int u=src;
	int j=0;
	int Visite[nbCities];
	for(int i=0;i<nbCities;i++)
	{
		D[i]=INT_MAX;
		Visite[i]=0;
	}
	D[src]=0;
	//Visite[src]=1;
	while(u!=dest)
	{
		
		u=distanceMini2(nbCities,D,Visite);
		Visite[u]=1;
		printf("u=%d\n", u);
		for (int i = 0; i < nbCities; i++)
		{
			if(Visite[i]==0 && G[u][i]!=INT_MAX && D[u]+G[u][i]<D[i])
			{
				
				D[i]=D[u]+G[u][i];
				Prec[i]=u;
			}
		}
		
	}
}

//Actuelle fonction utilisée
int distanceMini2(int nbCities,int D[nbCities],int Visite[nbCities])
{
	int min,indice_min;
	min=INT_MAX;
	for (int i = 0; i < nbCities; i++)
	{
		if(Visite[i]==0 && D[i]<min)
		{
			min=D[i];
			indice_min=i;
		}
	}
	return indice_min;
}

void algoDijkstra(int depart,int nbCities,int G[nbCities][nbCities],int D[nbCities],int Prec[nbCities])
{
	int Visite[nbCities];
	int u;
	
	for(int i=0;i<nbCities;i++)
	{
		D[i]= INT_MAX;
		Visite[i]=0;
	}
	D[depart] = 0;
	for(int i=0;i<nbCities;i++)
	{
		u=distanceMini(nbCities,Visite,D);
		
		Visite[u]=1;
		for(int j=0;j<nbCities;j++)
		{
			if ((Visite[j]==0) && (G[u][j]!=INT_MAX) && ((D[u]+G[u][j])<D[j]))
			{
				printf("BOUH\n");
				D[j]=D[u]+G[u][j];
				Prec[j]=u;
			}
		}
	}
}

void displayway(int src, int dest,int* Prec,int* Villes)
{
	int v;
	int i=0;
	v= dest;
	while (*Villes++!=src)
	{
		*Villes++ =Prec[v];
		
	}
}


void displayway2(int src,int dest,int nbCities,int Prec[nbCities],t_track Tracks[nbCities])
{
	int v=dest;
	int i=0;
	while(v!=src)
	{
		Tracks[i].city1=v;
		v=Prec[v];
		//Tracks[i].city1=v;
		Tracks[i].city2=Prec[v];

	}
}


void fillArray(int nbCities, int G[nbCities][nbCities], int* arraytracks, t_track* AllTracks)
{
	int i=0;
	int v=0;
	while(arraytracks[i])
		{
			G[arraytracks[i]][arraytracks[i+1]]=arraytracks[i+2];
			AllTracks[v].city1=arraytracks[i];
			
			AllTracks[v].city2=arraytracks[i+1];
			
			AllTracks[v].length=arraytracks[i+2];
			
			AllTracks[v].color1=arraytracks[i+3];
		
			AllTracks[v].color2=arraytracks[i+4];
			AllTracks[v].taken=0;
			v++;
			
			i=i+5;
		}
	for(int j=0;j<nbCities;j++)
	{
		for(int u=0;u<nbCities;u++)
		{
			if(G[j][u]==0)
			{
				G[j][u]=INT_MAX;
				
			}
		}
	}
}

//Actuelle fonction
void trackstotake2(int nbCities,t_track Tracks[nbCities],int dest,int src,int nbTracks,t_track AllTracks[nbTracks])
{
	for(int j=0;j<nbTracks;j++)
	{
		for(int i=0;i<nbCities;i++)
		{
			if(AllTracks[j].city1==Tracks[i].city1 && AllTracks[j].city2==Tracks[i].city2)
			{
				
				
				Tracks[i].length=AllTracks[j].length;
				Tracks[i].color1=AllTracks[j].color1;
				Tracks[i].color2=AllTracks[j].color2;
				Tracks[i].taken=AllTracks[j].taken;
				
			}
			
		

		}
	}
}
//obsolete
void trackstotake(int Villes[15], t_track* AllTracks,t_track* Tracks,int dest,int src,int nbTracks)
{
	int i=0;
	for(int j=0;j<nbTracks;j++)
		{
			
			if(AllTracks[j].city1==dest && AllTracks[j].city2==Villes[i])
			{
				printf("Ok2\n");
				Tracks[i].city1=dest;
				Tracks[i].city2=Villes[i];
				Tracks[i].length=AllTracks[j].length;
				Tracks[i].color1=AllTracks[j].color1;
				Tracks[i].color2=AllTracks[j].color2;
				Tracks[i].taken=AllTracks[j].taken;
				break;
			}
			
		}

	while(Villes[i]!=src)

	{
		
		for (int j = 0; j < nbTracks; j++)
	
		{
			

			if(AllTracks[j].city1==Villes[i] && AllTracks[j].city2==Villes[i+1])
			{
				printf("OK5\n");
				Tracks[i].city1=Villes[i];
				Tracks[i].city2=Villes[i+1];
				Tracks[i].length=AllTracks->length;
				Tracks[i].color1=AllTracks->color1;
				Tracks[i].color2=AllTracks->color2;
				Tracks[i].taken=AllTracks->taken;
				i++;
			}
		
		}
		
	}
}

void initplayer(t_player* You,t_color* colorcards)
{
	You->nbWagons=45;
	You->nbCards=4;
	You->nbObjectives=0;
	for(int i=0;i<4;i++)
	{
		You->cards[colorcards[i]]+=1;
	}
}

//actuelle fonction
int choosemove2(t_player* You,int nbCities, t_track Tracks[nbCities],t_move *move,int G[nbCities][nbCities],t_color faceUp[5])
{

	int i=0;
	int p=0;
	int u=0;
	for(int i=0;i<5;i++)
						{
							
							if(faceUp[i]==Tracks[u].color1 && Tracks[u].taken==0)
							{
								move->type=3;
								move->drawCard.card=faceUp[i];
								i=5;
							}
							if(Tracks[u].taken==1)
							{
								u++;
							}
							else if(faceUp[i]!=Tracks[u].color1 && Tracks[u].taken==0){
								p++;
							
							
								
							}
						}
						if(p>4){
							move->type=2;
						}
						
						if(You->cards[Tracks[u].color1]==Tracks[u].length && Tracks[u].taken==0)
					    			{
					    				move->type=1;
					    				if(Tracks[u].color1==9)
					    				{
					    					for (int i = 0; i <9 ; ++i)
					    					{
					    						if(You->cards[i]==Tracks[u].length)
					    							move->claimRoute.color=i;
					    					}
					    				}
					    				else{
					    					move->claimRoute.color=Tracks[u].color1;

					    				}
					    				
					    				
					    				You->cards[Tracks[u].color1]-=Tracks[u].length;
					    				move->claimRoute.city1=Tracks[u].city1;
					    				move->claimRoute.city2=Tracks[u].city2;
					    				move->claimRoute.nbLocomotives=0;
					    				You->nbCards=You->nbCards-Tracks[u].length;
					    				You->nbWagons=You->nbWagons-Tracks[u].length;
					    				Tracks[u].taken=1;

					    			}
					    			else if (You->cards[Tracks[u].color2]==Tracks[u].length && Tracks[u].taken==0)
					    			{
					    				move->type=1;
					    				move->claimRoute.color=Tracks[u].color2;
					    				You->cards[Tracks[u].color2]-=Tracks[u].length;
					    				move->claimRoute.city1=Tracks[u].city1;
					    				move->claimRoute.city2=Tracks[u].city2;
					    				move->claimRoute.nbLocomotives=0;
					    				You->nbWagons=You->nbWagons-Tracks[u].length;
					    				You->nbCards=You->nbCards-Tracks[u].length;
					    				Tracks[u].taken=1;
					    			}
					    			else if(You->cards[MULTICOLOR]==Tracks[u].length+1 && Tracks[u].taken==0)
					    			{
					    				move->type=1;
					    				move->claimRoute.color=MULTICOLOR;
					    				move->claimRoute.city1=Tracks[u].city1;
					    				move->claimRoute.city2=Tracks[u].city2;
					    				move->claimRoute.nbLocomotives=Tracks[u].length;
					    				You->nbWagons=You->nbWagons-Tracks[u].length;
					    				You->nbCards=You->nbCards-Tracks[u].length;
					    				You->cards[MULTICOLOR]-=Tracks[u].length;
					    				Tracks[u].taken=1;
					    			}
	return move->type;

}

int findlong(int nbTracks,t_track AllTracks[nbTracks])// to find the longest tracks
{
	int u=0;
	for (int i = 0; i < nbTracks; ++i)
	{
		if(AllTracks[i].length==6 && AllTracks[i].taken==0)
		{
			u=i;

		}
	}
	return u;
}





					    				
//fonction obsolète
int choosemove(t_player* You,t_track* Tracks,t_move* move,int nbCities,int G[nbCities][nbCities],t_color faceUp[5])
{
	if((You->cards[Tracks->color1]==Tracks->length || You->cards[Tracks->color2]==Tracks->length || You->cards[MULTICOLOR]==Tracks->length) && Tracks->taken==0)
					    		{
					    			printf("DECISIONCLAIMROUTE\n");

					    			move->type=1;
					    			move->claimRoute.city1=Tracks->city1;
					    			move->claimRoute.city2=Tracks->city2;
					    			move->claimRoute.nbLocomotives=Tracks->length;
					    			You->nbWagons=You->nbWagons-Tracks->length;
					    			Tracks->taken=1;
					    			G[Tracks->city1][Tracks->city2]=0;
					    			if(You->cards[Tracks->color1]==Tracks->length)
					    			{
					    				move->claimRoute.color=Tracks->color1;
					    				You->cards[Tracks->color1]-=Tracks->length;

					    			}
					    			else if (You->cards[Tracks->color2]==Tracks->length)
					    			{
					    				move->claimRoute.color=Tracks->color2;
					    				You->cards[Tracks->color2]-=Tracks->length;
					    			}
					    			else
					    			{
					    				move->claimRoute.color=MULTICOLOR;
					    				You->cards[MULTICOLOR]-=Tracks->length;
					    			}
					    		
					    			


					    		}
					    		else
					    		{
					    			printf("DECISIONDRAWCARD\n");
					    			for(int j=0;j<5;j++)
					    			{
					    				if(faceUp[j]==Tracks->color1)
					    				{
					    					move->type=3;
					    					move->drawCard.card=Tracks->color1;
					    					j=5;
					    					


					    				}
					    				if(faceUp[j]==Tracks->color2)
					    				{
					    					printf("movetype3\n");
					    					move->type=3;
					    					move->drawCard.card=Tracks->color2;
					    					j=5;
					    					
					    				}
					    				else{
					    					printf("movetype2\n");
					    					move->type=2;
					    					j=5;
					    					
					    				}
					    			}
					    		}
					    		return move->type;
					    	
	}

					    