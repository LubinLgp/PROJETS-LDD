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
    int sockfd; // Descripteur de socket
    struct sockaddr_in server_addr; // Structure d'adresse du serveur
    char buffer[BUFFER_SIZE]; // Tampon pour stocker les données

    // Vérification de la présence des arguments nécessaires (adresse IP serveur, message à envoyer)
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server_ip> <message>\n", argv[0]);
        return 1;
    }

    // Création du socket TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) {
        perror("Error opening socket"); // Affiche un message d'erreur (car si il y a une erreur, scoket renvoie -1)
        return 1;
    }

    // Remplissage de la structure d'adresse du serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Récupération de l'adresse IP du serveur à partir des arguments de la ligne de commande
    struct hostent *server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n"); // Affiche un message d'erreur
        return 1;
    }
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connexion au serveur
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server"); // Affiche un message d'erreur
        return 1;
    }

    // Préparation du message à envoyer
    if (strlen(argv[2]) >= BUFFER_SIZE) {
        fprintf(stderr, "Message too long\n"); // Affiche un message d'erreur
        return 1;
    }
    strcpy(buffer, argv[2]);

    // Envoi du message au serveur
    ssize_t sent_len = write(sockfd, buffer, strlen(buffer));
    if (sent_len < 0) {
        perror("Error sending message"); // Affiche un message d'erreur
        return 1;
    }

    printf("Message sent to the server: %s\n", buffer);

    // Réception de la réponse du serveur
    ssize_t recv_len = read(sockfd, buffer, BUFFER_SIZE - 1);
    if (recv_len < 0) {
        perror("Error receiving message"); // Affiche un message d'erreur
        return 1;
    }
    buffer[recv_len] = '\0';
    printf("Message received from server: %s\n", buffer);

    // Fermeture du socket
    close(sockfd);

    return 0;
}
