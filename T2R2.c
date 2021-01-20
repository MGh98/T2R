#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "game1.h"
#include "TicketToRideAPI1.h"
#include "move1.h"

int main() {
	char gameName[20];		/* game name */
	int nbCities, nbTracks;	/* number of cities and tracks */
	int* arrayTracks;		/* array of int for the tracks */
	t_color faceUp[5];		/* face up cards */
	t_color ourCards[4];	/* our cards */
	int player;				/* player == 0 -> we play,  ==1 -> the opponent plays */
	t_return_code retCode;
	int replay = 0;			/* boolean, tells if the player replays */
	t_move move;			/* current move */
	t_color lastMove = NONE;	/* last Card taken we need to replay else NONE */

	int firstplay=0; // Boolean to tell if we played our first turn
	int choose=0; //Boolean to tell if we have choosed our move yet

	t_objective obj[10];//Objectives we take


	t_player You;//our player


	/* connect to server */
	connectToServer("li1417-56.members.linode.com", 5678, "Champ");

	/* wait for a game and get the map */
	waitForT2RGame("TRAINING PLAY_RANDOM timeout=10000", gameName, &nbCities, &nbTracks);
	arrayTracks = malloc(5*nbTracks*sizeof(int));
	player = getMap(arrayTracks, faceUp, ourCards);

	int G[nbCities][nbCities];//array of tracks between cities for the function algoDjisktra
	int* pG=(int*)memset(G,0,nbCities*nbCities*sizeof(int));


	int D[nbCities];//array for algoDjisktra
	int* pD=memset(D,0,nbCities*sizeof(int));

	int Prec[nbCities];//array for algoDjisktra
	int* pPrec=memset(Prec,0,nbCities*sizeof(int));

	int Villes[15];//cities to visit before destination
	int* pVilles=memset(Villes,0,15*sizeof(int));

	t_track AllTracks[nbTracks];//array with all the game tracks (all the infos of the tracks)

	fillArray(nbCities,G,arrayTracks,AllTracks);//fill array G for algoDjisktra and Alltracks of the game
	initplayer(&You,ourCards);//Initialization of the informations of our player

	t_track Tracks[15]; //List of the tracks to take to complete the objective
	You.nbObjectives=0;

	int u=1;
	/* game loop */
	do{
		/* display map */
		if (!replay)
			printMap();

		/* the opponent plays ??*/
		if (player == 1){

			retCode = getMove(&move, &replay);
			if(move.type==3)//we update the faceup cards if the opponent choose the move Drawcard
			{
				for(int i=0;i<5;i++)
						{
							faceUp[i]=move.drawCard.faceUp[i];
						}
			}
			if(move.type==1)//we update the routes if the opponent choose the move claimRoute
			{
				for(int i=0;i<nbTracks;i++)
				{
					if(AllTracks[i].city1==move.claimRoute.city1 && AllTracks[i].city2==move.claimRoute.city2)
					{
						AllTracks[i].taken=1;

					}
				}

			}
			// update game
		}
		else {
			/* we play */



			if(You.nbObjectives!=0)//if the player has objectives we run the algorithm
			{
					
					//we use nbObjectives, (we decrease the nbObjectives when one is taken)

					algorithme(obj[You.nbObjectives-1].city1,nbCities,obj[You.nbObjectives-1].city2,G,D,Prec);//Djisktra's Algorithm
					printf("Fait\n");
					
					//We store the tracks we need to fulfill the objectives
					displayway2(obj[You.nbObjectives-1].city1,obj[You.nbObjectives-1].city2,nbCities,Prec,Tracks);

					//we retrieve the infos about the tracks (length, color, taken)
					trackstotake2(nbCities,Tracks,obj[You.nbObjectives-1].city2,obj[You.nbObjectives-1].city1,nbTracks,AllTracks);

					//we use the infos about the tracks to choose between drawCard, drawBlindcard or claimRoute
					move.type=choosemove2(&You,nbCities,Tracks,&move,G,faceUp);
			}

					    	
					    							    		
					    		


			if(move.type==1 && choose==0)//Claimroute
			{
				//tentative d'implémentation uniquement du choosemove
				/*for(int i=0;i<5;i++)
						{
							if(faceUp[i]==AllTracks[u].color1 && AllTracks[u].taken==0)
							{
								move.type=3;
								move.drawCard.card=faceUp[i];
								i=5;
							}
							if(AllTracks[u].taken==1)
							{
								u++;
							}
							else{
								move.type=2;
								
							}
						}
						
						if(You.cards[AllTracks[u].color1]==AllTracks[u].length && AllTracks[u].taken==0)
					    			{
					    				move.type=1;
					    				move.claimRoute.color=AllTracks[u].color1;
					    				You.cards[AllTracks[u].color1]-=AllTracks[u].length;
					    				move.claimRoute.city1=AllTracks[u].city1;
					    				move.claimRoute.city2=AllTracks[u].city2;
					    				move.claimRoute.nbLocomotives=0;
					    				You.nbWagons=You.nbWagons-AllTracks[u].length;
					    				You.nbCards=You.nbCards-AllTracks[u].length;

					    				AllTracks[u].taken=1;
					    				

					    			}
					    			else if (You.cards[AllTracks[u].color2]==AllTracks[u].length && AllTracks[u].taken==0)
					    			{
					    				move.type=1;
					    				move.claimRoute.color=AllTracks[u].color2;
					    				You.cards[AllTracks[u].color2]-=AllTracks[u].length;
					    				move.claimRoute.city1=AllTracks[u].city1;
					    				move.claimRoute.city2=AllTracks[u].city2;
					    				move.claimRoute.nbLocomotives=0;
					    				You.nbCards=You.nbCards-AllTracks[u].length;
					    				You.nbWagons=You.nbWagons-AllTracks[u].length;
					    				AllTracks[u].taken=1;
					    			}
					    			else if(You.cards[MULTICOLOR]==AllTracks[u].length+1 && AllTracks[u].taken==0)
					    			{
					    				move.type=1;
					    				move.claimRoute.color=MULTICOLOR;
					    				You.cards[MULTICOLOR]-=AllTracks[u].length;
					    				move.claimRoute.city1=AllTracks[u].city1;
					    				move.claimRoute.city2=AllTracks[u].city2;
					    				move.claimRoute.nbLocomotives=0;
					    				You.nbCards=You.nbCards-AllTracks[u].length;
					    				You.nbWagons=You.nbWagons-AllTracks[u].length;
					    				AllTracks[u].taken=1;
					    			}*/
						

				choose=1;
			}
			if(move.type==2 && choose==0)//Draw Blindcard
			{
				You.cards[move.drawBlindCard.card]++; //we update our cards deck
				You.nbCards++;

				/*for(int i=0;i<5;i++)
						{
							
							if(faceUp[i]==AllTracks[u].color1 && AllTracks[u].taken==0)
							{
								move.type=3;
								move.drawCard.card=faceUp[i];
								i=5;
							}
							if(AllTracks[u].taken==1)
							{
								u++;
							}
							else{
								move.type=2;
								
							}
						}*/
						
						/*if(You.cards[AllTracks[u].color1]==AllTracks[u].length && AllTracks[u].taken==0)
					    			{
					    				move.type=1;
					    				move.claimRoute.color=AllTracks[u].color1;
					    				You.cards[AllTracks[u].color1]-=AllTracks[u].length;
					    				move.claimRoute.city1=AllTracks[u].city1;
					    				move.claimRoute.city2=AllTracks[u].city2;
					    				move.claimRoute.nbLocomotives=0;
					    				You.nbCards=You.nbCards-AllTracks[u].length;
					    				You.nbWagons=You.nbWagons-AllTracks[u].length;
					    				AllTracks[u].taken=1;

					    			}
					    			else if (You.cards[AllTracks[u].color2]==AllTracks[u].length && AllTracks[u].taken==0)
					    			{
					    				move.type=1;
					    				move.claimRoute.color=AllTracks[u].color2;
					    				You.cards[AllTracks[u].color2]-=AllTracks[u].length;
					    				move.claimRoute.city1=AllTracks[u].city1;
					    				move.claimRoute.city2=AllTracks[u].city2;
					    				move.claimRoute.nbLocomotives=0;
					    				You.nbWagons=You.nbWagons-AllTracks[u].length;
					    				You.nbCards=You.nbCards-AllTracks[u].length;
					    				AllTracks[u].taken=1;
					    			}
					    			else if(You.cards[MULTICOLOR]==AllTracks[u].length+1 && AllTracks[u].taken==0)
					    			{
					    				move.type=1;
					    				move.claimRoute.color=MULTICOLOR;
					    				move.claimRoute.city1=AllTracks[u].city1;
					    				move.claimRoute.city2=AllTracks[u].city2;
					    				move.claimRoute.nbLocomotives=0;
					    				You.nbWagons=You.nbWagons-AllTracks[u].length;
					    				You.nbCards=You.nbCards-AllTracks[u].length;
					    				You.cards[MULTICOLOR]-=AllTracks[u].length;
					    				AllTracks[u].taken=1;
					    			}*/
						

				choose=1;

			}
			if(move.type==3 && choose==0)//Drawcard
					{
						You.nbCards++;
						You.cards[move.drawCard.card]++;

						for(int i=0;i<5;i++)
						{
							faceUp[i]=move.drawCard.faceUp[i];//we update the actual faceUp cards
							/*if(faceUp[i]==AllTracks[u].color1 && AllTracks[u].taken==0)
							{
								move.type=3;
								move.drawCard.card=faceUp[i];
								i=5;
							}
							if(AllTracks[u].taken==1)
							{
								u++;
							}
							else{
								move.type=2;
								
							}*/
						}
						
						/*if(You.cards[AllTracks[u].color1]==AllTracks[u].length && AllTracks[u].taken==0)
					    			{
					    				move.type=1;
					    				move.claimRoute.color=AllTracks[u].color1;
					    				move.claimRoute.city1=AllTracks[u].city1;
					    				move.claimRoute.city2=AllTracks[u].city2;
					    				move.claimRoute.nbLocomotives=0;
					    				You.nbWagons=You.nbWagons-AllTracks[u].length;
					    				You.cards[AllTracks[u].color1]-=AllTracks[u].length;
					    				AllTracks[u].taken=1;

					    			}
					    			else if (You.cards[AllTracks[u].color2]==AllTracks[u].length && AllTracks[u].taken==0)
					    			{
					    				move.type=1;
					    				move.claimRoute.color=AllTracks[u].color2;
					    				move.claimRoute.city1=AllTracks[u].city1;
					    				move.claimRoute.city2=AllTracks[u].city2;
					    				move.claimRoute.nbLocomotives=0;
					    				You.nbWagons=You.nbWagons-AllTracks[u].length;
					    				You.cards[AllTracks[u].color2]-=AllTracks[u].length;
					    				AllTracks[u].taken=1;
					    			}
					    			else if(You.cards[MULTICOLOR]==AllTracks[u].length+1 && AllTracks[u].taken==0)
					    			{
					    				move.type=1;
					    				move.claimRoute.color=MULTICOLOR;
					    				move.claimRoute.city1=AllTracks[u].city1;
					    				move.claimRoute.city2=AllTracks[u].city2;
					    				move.claimRoute.nbLocomotives=AllTracks[u].length;
					    				You.nbWagons=You.nbWagons-AllTracks[u].length;
					    				You.cards[MULTICOLOR]-=AllTracks[u].length;
					    				AllTracks[u].taken=1;
					    			}*/
						

						choose=1;
					}
			if(move.type ==4 && choose==0)//Drawobjectives, we take all the objectives and we "choose objectives"
					{
						
						move.chooseObjectives.chosen[0]=1;
				        move.chooseObjectives.chosen[1]=1; 
				        move.chooseObjectives.chosen[2]=1;
				        move.type = 5;
				        choose=1;

					}
			if(move.type == 5 && choose ==0)//ChooseObjectives
			{

						for(int i=0;i<3;i++) //we retrieve the objectives
						{

						obj[i].city1=move.drawObjectives.objectives[i].city1;
						obj[i].city2=move.drawObjectives.objectives[i].city2;
						obj[i].score=move.drawObjectives.objectives[i].score;

						}
						/*for(int i=0;i<5;i++)
						{
							if(faceUp[i]==AllTracks[u].color1 && AllTracks[u].taken==0)
							{
								move.type=3;
								move.drawCard.card=faceUp[i];
							}
							if(AllTracks[u].taken==1)
							{
								u++;
							}
							else{
								move.type=2;
								
							}
						}*/
						move.type=2;

						You.nbObjectives=3;
						choose=1;
			}

			askMove(&move,&firstplay);
			replay = needReplay(&move, lastMove);
			retCode = playOurMove(&move, &lastMove);
			
			
			choose=0;

			// update game
		}
		/* change player */
		if (retCode == NORMAL_MOVE && !replay)
			player = !player;

	} while (retCode == NORMAL_MOVE);

	/* check who wins */
	if ((retCode == WINNING_MOVE && player == 0) || (retCode == LOOSING_MOVE && player == 1))
		printf("We are the best !\n");
	else
		printf("The opponent was lucky\n");


	closeConnection();
	return 0;

}