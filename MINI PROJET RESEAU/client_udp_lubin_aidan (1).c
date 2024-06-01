#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define PORT 9600
#define BUFFER_SIZE 1024

//LONGUEPEE Lubin
//BAROUK Aidan

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE]; //variable qui va contenir le message (qui devra faire au plus 1024 caractères)

    // Vérifie si les arguments requis sont présents (adresse IP serveur, message à envoyer)
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server_ip> <message>\n", argv[0]);
        return 1;
    }

    // Crée un socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); //la fonction socket crée un point de communication entre des machines. La fonction renvoie un entier, -1 s'il y a eu une erreur.
    if (sockfd < 0) {
        perror("Erreur lors de l'ouverture du socket");
        return 1;
    }

    // Remplit la structure d'adresse du serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Obtient l'adresse IP du serveur depuis les arguments de la ligne de commande
    struct hostent *server = gethostbyname(argv[1]); //argv correspond au tableau des arguments, et le champ 1 correspond à l'adresse ip
    if (server == NULL) {
        fprintf(stderr, "Error, serveur ne pointe vers rien\n");
        return 1;
    }
	// Copie l'adresse IP obtenue depuis la résolution DNS vers la structure d'adresse du serveur
	memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);


    // Prépare le message à envoyer au serveur
    if (strlen(argv[2]) >= BUFFER_SIZE) {
        fprintf(stderr, "Message trop long\n");
        return 1;
    }
    strcpy(buffer, argv[2]);

    // Envoie le message au serveur via le socket UDP
    ssize_t sent_len = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (sent_len < 0) {
        perror("Erreur lors de l'envoi du message");
        return 1;
    }

    printf("Message envoyé au serveur: %s\n", buffer);

    // Ferme le socket
    close(sockfd);

    return 0;
}
