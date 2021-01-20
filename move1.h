//
// Created by Thib on 14/12/2020.
//

#ifndef T2R_TEST_MOVE_H
#define T2R_TEST_MOVE_H

#include "TicketToRideAPI1.h"
#include "game1.h"

/* ask for a move */
void askMove(t_move* move, int* firstmove);

/* plays the move given as a parameter (send to the server)
 * returns the return code */
t_return_code playOurMove(t_move* move, t_color* lastCard);

/* tell if we need to replay */
int needReplay(t_move* move, t_color lastCard);

//Algorithme à choosemove2 sont les dernières versions utilisées
void algorithme(int src,int nbCities, int dest, int G[nbCities][nbCities],int D[nbCities],int Prec[nbCities]);

int distanceMini2(int nbCities,int D[nbCities],int Visite[nbCities]);

void displayway2(int src,int dest,int nbCities,int Prec[nbCities],t_track Tracks[nbCities]);

void trackstotake2(int nbCities,t_track Tracks[nbCities],int dest,int src,int nbTracks,t_track AllTracks[nbTracks]);

int choosemove2(t_player* You,int nbCities, t_track Tracks[nbCities],t_move *move,int G[nbCities][nbCities],t_color faceUp[5]);

int distanceMini(int nbCities, int* Visite,int D[nbCities]);
void algoDijkstra(int depart,int nbCities,int G[nbCities][nbCities],int D[nbCities],int Prec[nbCities]);
void displayway(int src, int dest,int* Prec,int* Villes);

void fillArray(int nbCities, int G[nbCities][nbCities],int* arraytracks,t_track* Alltracks);

void trackstotake(int Villes[15], t_track* AllTracks,t_track* Tracks,int dest,int src,int nbTracks);

void initplayer(t_player* You,t_color* cards);

int choosemove(t_player* You,t_track* Tracks,t_move* move,int nbCities,int G[nbCities][nbCities],t_color faceUp[5]);
#endif //T2R_TEST_MOVE_H