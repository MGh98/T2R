PROGRAMS= clientAPI.o 

Aventuriersdurail: move.o clientAPI.o TicketToRideAPI1.o T2R2.o 
	gcc -o Aventuriersdurail T2R2.o clientAPI.o TicketToRideAPI1.o move.o
	chmod u+rwx Aventuriersdurail


clientAPI.o: clientAPI.c clientAPI.h
	gcc -Wall -Wuninitialized -c clientAPI.c -o clientAPI.o

TicketToRideAPI1.o: TicketToRideAPI1.c TicketToRideAPI1.h clientAPI.h game1.h
	gcc -Wall -Wuninitialized -c TicketToRideAPI1.c -o TicketToRideAPI1.o


move.o: move1.c move1.h TicketToRideAPI1.h 
	gcc -Wall -Wuninitialized -c move1.c  -o move.o

T2R2.o: T2R2.c move1.h TicketToRideAPI1.h
	gcc -Wall -Wuninitialized -c T2R2.c -o T2R2.o






clean:
	rm $(PROGRAMS)