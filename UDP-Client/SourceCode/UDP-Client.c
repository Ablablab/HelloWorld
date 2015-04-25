/* daytime_clientUDP.c - code for example client program that uses UDP
 Ultima revisione: 14 gennaio 2008 */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conf_Client.h"

#define MAXLINE     1024

int main(int argc, char *argv[]) {

	int client, byte_ricevuti;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;

	/*apro una socket*/
	if ((client = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("errore in socket");
		exit(1);
	}


	memset((void *) &servaddr, 0, sizeof(servaddr)); /* azzera servaddr */
	servaddr.sin_family = AF_INET; /* assegna il tipo di indirizzo */
	servaddr.sin_port = htons(SERVER_PORT); /* assegna la porta del server */
	/* assegna l'indirizzo del server prendendolo dalla riga di comando. L'indirizzo � una stringa da convertire in intero secondo network byte order. */
	if (inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) <= 0) {
		/* inet_pton (p=presentation) vale anche per indirizzi IPv6 */
		fprintf(stderr, "errore in inet_pton per %s", argv[1]);
		exit(EXIT_FAILURE);
	}

	/* Invia al server il pacchetto di richiesta*/
	if (sendto(client, NULL, 0, 0, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		perror("errore in sendto");
		exit(EXIT_FAILURE);
	}

	while(1){
		/* Legge dal socket il pacchetto di risposta */
		byte_ricevuti = recvfrom(client, recvline, MAXLINE, 0, NULL, NULL);
		if (byte_ricevuti < 0) {
			perror("errore in recvfrom");
			exit(EXIT_FAILURE);
		}

		if (byte_ricevuti > 0) {
			recvline[byte_ricevuti] = 0; /* aggiunge il carattere di terminazione */
			if (fputs(recvline, stdout) == EOF) { /* stampa recvline sullo stdout */
				fprintf(stderr, "errore in fputs");
				exit(EXIT_FAILURE);
			}
		}
	}
	exit(EXIT_SUCCESS);
}
