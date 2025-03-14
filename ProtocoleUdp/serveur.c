/*Assane Thiao 12/03/2025*/

#include "network.h"  /* Inclure le fichier d'en-tête personnalisé */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    SOCKET sock;  /* Déclaration du socket */
    struct sockaddr_in server_addr, client_addr;  /* Structures pour l'adresse du serveur et du client */
    char buffer[BUFFER_SIZE];  /* Buffer pour stocker les données reçues */
    int client_addr_len = sizeof(client_addr);  /* Taille de l'adresse du client */

    /* Initialisation du réseau */
    initNetwork();

    /* Création du socket */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        /* Erreur lors de la création du socket */
        printf("Erreur lors de la création du socket\n");
        cleanNetwork();  /* Nettoyage du réseau avant de quitter */
        return EXIT_FAILURE;
    }

    /* Configuration de l'adresse du serveur */
    memset(&server_addr, 0, sizeof(server_addr));  /* Initialisation de la structure server_addr à zéro */
    server_addr.sin_family = AF_INET;  /* Spécifie la famille d'adresses (IPv4) */
    server_addr.sin_port = htons(PORT);  /* Spécifie le port du serveur */
    server_addr.sin_addr.s_addr = INADDR_ANY;  /* Spécifie l'adresse IP du serveur (toutes les interfaces réseau) */

    /* Lier le socket à l'adresse du serveur */
    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        /* Erreur lors de la liaison du socket */
        printf("Erreur lors de la liaison du socket\n");
        closesocket(sock);  /* Fermeture du socket */
        cleanNetwork();  /* Nettoyage du réseau */
        return EXIT_FAILURE;
    }

    printf("Serveur en ecoute sur le port %d...\n", PORT);

    /* Boucle infinie pour rester en écoute */
    while (1) {
        /* Réception des données du client */
        int recv_len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if (recv_len == SOCKET_ERROR) {
            /* Gestion des erreurs lors de la réception des données */
            int error_code = WSAGetLastError();
            if (error_code == WSAEINTR) {
                /* Si l'appel est interrompu, simplement recommencer */
                continue;
            }
            /* Affichage de l'erreur */
            printf("Erreur lors de la reception du message: %d\n", error_code);
            break;  /* Sortir de la boucle si l'erreur est fatale */
        }

        buffer[recv_len] = '\0';  /* Ajout d'un caractère de fin de chaîne */
        printf("Message recu du client: %s\n", buffer);

        /* Répondre au client */
        const char* response = "Message recu avec succes!";
        int send_len = sendto(sock, response, strlen(response), 0, (struct sockaddr*)&client_addr, client_addr_len);
        if (send_len == SOCKET_ERROR) {
            /* Erreur lors de l'envoi du message */
            int error_code = WSAGetLastError();
            printf("Erreur lors de l'envoi du message: %d\n", error_code);
        }
        else {
            /* Affichage de la réponse envoyée */
            printf("Reponse envoyee au client: %s\n", response);
        }
    }

    /* Fermeture du socket */
    closesocket(sock);
    cleanNetwork();  /* Nettoyage du réseau avant de quitter */
    return 0;
}
