/*Assane Thiao 12/03/2025*/

#include "network.h"  /* Inclure le fichier d'en-t�te personnalis� */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"  /* L'adresse IP du serveur (127.0.0.1 pour localhost) */
#define BUFFER_SIZE 1024

int main() {
    SOCKET sock;  /* D�claration du socket */
    struct sockaddr_in server_addr;  /* D�claration de la structure d'adresse du serveur */
    char buffer[BUFFER_SIZE];  /* Buffer pour stocker les donn�es re�ues */

    /* Initialisation du r�seau */
    initNetwork();

    /* Cr�ation du socket */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        /* Erreur lors de la cr�ation du socket */
        printf("Erreur lors de la cr�ation du socket\n");
        cleanNetwork();  /* Nettoyage du r�seau avant de quitter */
        return EXIT_FAILURE;
    }

    /* Configuration de l'adresse du serveur */
    memset(&server_addr, 0, sizeof(server_addr));  /* Initialisation de la structure server_addr � z�ro */
    server_addr.sin_family = AF_INET;  /* Sp�cifie la famille d'adresses (IPv4) */
    server_addr.sin_port = htons(SERVER_PORT);  /* Sp�cifie le port du serveur */
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);  /* Sp�cifie l'adresse IP du serveur */

    /* Envoi du message au serveur */
    const char* message = "Hello from client";
    int send_len = sendto(sock, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (send_len == SOCKET_ERROR) {
        /* Erreur lors de l'envoi du message */
        printf("Erreur lors de l'envoi du message: %d\n", WSAGetLastError());
        closesocket(sock);  /* Fermeture du socket */
        cleanNetwork();  /* Nettoyage du r�seau */
        return EXIT_FAILURE;
    }

    printf("Message envoye au serveur: %s\n", message);  /* Affichage du message envoy� au serveur */

    /* R�ception de la r�ponse du serveur */
    int recv_len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
    if (recv_len == SOCKET_ERROR) {
        /* Erreur lors de la r�ception du message */
        printf("Erreur lors de la r�ception du message: %d\n", WSAGetLastError());
        closesocket(sock);  /* Fermeture du socket */
        cleanNetwork();  /* Nettoyage du r�seau */
        return EXIT_FAILURE;
    }

    buffer[recv_len] = '\0';  /* Ajouter un caract�re de fin de cha�ne */
    printf("Reponse du serveur: %s\n", buffer);  /* Affichage de la r�ponse du serveur */

    /* Fermeture du socket */
    closesocket(sock);
    cleanNetwork();  /* Nettoyage du r�seau avant de quitter */
    return 0;
}
