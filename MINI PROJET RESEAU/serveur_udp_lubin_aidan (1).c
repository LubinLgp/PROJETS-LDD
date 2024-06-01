#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 9600
#define BUFFER_SIZE 20

//LONGUEPEE Lubin
//BAROUK Aidan

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];

    // la fonction crée une connexion udp
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Erreur de connexion du socket");
        return 1;
    }

    // Remplit la sturcture des adresses du serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // Définit le port du serveur
    server_addr.sin_addr.s_addr = INADDR_ANY; // Écoute sur toutes les interfaces


    // On lie le socket au port client
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("erreur de liage");
        return 1;
    }

    printf("Le serveur est connecté sur le port %d...\n", PORT);

    while (1) {
        client_len = sizeof(client_addr);

        // La fonction permet de recevoir les informations/données du client
        ssize_t recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
        if (recv_len < 0) {
            perror("Erreur dans la recepetion des données");
            break;
        }

        buffer[recv_len] = '\0'; // Ajoute un terminateur nul aux données reçues
        printf("Message recu de client : %s\n", buffer);
    }

    // la fonction close permet de fermer le socket 
    close(sockfd);

    return 0;
}
