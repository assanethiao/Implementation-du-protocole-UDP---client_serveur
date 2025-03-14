/*Assane Thiao 12/03/2025*/

#include "network.h"  /* Inclure le fichier d'en-tête personnalisé */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"  /* L'adresse IP du serveur (127.0.0.1 pour localhost) */
#define BUFFER_SIZE 1024

int main() {
    SOCKET sock;  /* Déclaration du socket */
    struct sockaddr_in server_addr;  /* Déclaration de la structure d'adresse du serveur */
    char buffer[BUFFER_SIZE];  /* Buffer pour stocker les données reçues */

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
    server_addr.sin_port = htons(SERVER_PORT);  /* Spécifie le port du serveur */
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);  /* Spécifie l'adresse IP du serveur */

    /* Envoi du message au serveur */
    const char* message = "Hello from client";
    int send_len = sendto(sock, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (send_len == SOCKET_ERROR) {
        /* Erreur lors de l'envoi du message */
        printf("Erreur lors de l'envoi du message: %d\n", WSAGetLastError());
        closesocket(sock);  /* Fermeture du socket */
        cleanNetwork();  /* Nettoyage du réseau */
        return EXIT_FAILURE;
    }

    printf("Message envoye au serveur: %s\n", message);  /* Affichage du message envoyé au serveur */

    /* Réception de la réponse du serveur */
    int recv_len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
    if (recv_len == SOCKET_ERROR) {
        /* Erreur lors de la réception du message */
        printf("Erreur lors de la réception du message: %d\n", WSAGetLastError());
        closesocket(sock);  /* Fermeture du socket */
        cleanNetwork();  /* Nettoyage du réseau */
        return EXIT_FAILURE;
    }

    buffer[recv_len] = '\0';  /* Ajouter un caractère de fin de chaîne */
    printf("Reponse du serveur: %s\n", buffer);  /* Affichage de la réponse du serveur */

    /* Fermeture du socket */
    closesocket(sock);
    cleanNetwork();  /* Nettoyage du réseau avant de quitter */
    return 0;
}
