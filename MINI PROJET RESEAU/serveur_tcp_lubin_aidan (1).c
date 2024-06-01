#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 9600
#define BUFFER_SIZE 1024
//LONGUEPEE Lubin
//BAROUK Aidan
int main() {
    int sockfd, newsockfd; // Descripteurs de socket pour le socket d'écoute et le nouveau socket accepté
    struct sockaddr_in server_addr, client_addr; // Structures d'adresse pour le serveur et le client
    socklen_t client_len; // Taille de la structure d'adresse client
    char buffer[BUFFER_SIZE]; // Tampon pour stocker les données

    // Création du socket TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket"); // Affiche un message d'erreur
        return 1;
    }

    // Remplissage de la structure d'adresse du serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accepte les connexions sur n'importe quelle interface

    // Liaison du socket au port spécifié
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding"); // Affiche un message d'erreur
        return 1;
    }

    // Écoute des connexions entrantes
    listen(sockfd, 5);

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        client_len = sizeof(client_addr);

        // Accepte une connexion entrante
        newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (newsockfd < 0) {
            perror("Error accepting connection"); // Affiche un message d'erreur
            return 1;
        }

        // Réception du message du client
        ssize_t recv_len = read(newsockfd, buffer, BUFFER_SIZE - 1);
        if (recv_len < 0) {
            perror("Error receiving message"); // Affiche un message d'erreur
            return 1;
        }
        buffer[recv_len] = '\0';
        printf("Message received from client: %s\n", buffer);

        // Envoi d'une réponse au client
        const char *response = "Message received by server";
        ssize_t sent_len = write(newsockfd, response, strlen(response));
        if (sent_len < 0) {
            perror("Error sending response"); // Affiche un message d'erreur
            return 1;
        }

        // Fermeture de la connexion acceptée
        close(newsockfd);
    }

    // Fermeture du socket d'écoute
    close(sockfd);

    return 0;
}
