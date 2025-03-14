/*Assane Thiao 12/03/2025*/

#include "network.h"  /* Inclure le fichier d'en-t�te personnalis� */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    SOCKET sock;  /* D�claration du socket */
    struct sockaddr_in server_addr, client_addr;  /* Structures pour l'adresse du serveur et du client */
    char buffer[BUFFER_SIZE];  /* Buffer pour stocker les donn�es re�ues */
    int client_addr_len = sizeof(client_addr);  /* Taille de l'adresse du client */

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
    server_addr.sin_port = htons(PORT);  /* Sp�cifie le port du serveur */
    server_addr.sin_addr.s_addr = INADDR_ANY;  /* Sp�cifie l'adresse IP du serveur (toutes les interfaces r�seau) */

    /* Lier le socket � l'adresse du serveur */
    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        /* Erreur lors de la liaison du socket */
        printf("Erreur lors de la liaison du socket\n");
        closesocket(sock);  /* Fermeture du socket */
        cleanNetwork();  /* Nettoyage du r�seau */
        return EXIT_FAILURE;
    }

    printf("Serveur en ecoute sur le port %d...\n", PORT);

    /* Boucle infinie pour rester en �coute */
    while (1) {
        /* R�ception des donn�es du client */
        int recv_len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if (recv_len == SOCKET_ERROR) {
            /* Gestion des erreurs lors de la r�ception des donn�es */
            int error_code = WSAGetLastError();
            if (error_code == WSAEINTR) {
                /* Si l'appel est interrompu, simplement recommencer */
                continue;
            }
            /* Affichage de l'erreur */
            printf("Erreur lors de la reception du message: %d\n", error_code);
            break;  /* Sortir de la boucle si l'erreur est fatale */
        }

        buffer[recv_len] = '\0';  /* Ajout d'un caract�re de fin de cha�ne */
        printf("Message recu du client: %s\n", buffer);

        /* R�pondre au client */
        const char* response = "Message recu avec succes!";
        int send_len = sendto(sock, response, strlen(response), 0, (struct sockaddr*)&client_addr, client_addr_len);
        if (send_len == SOCKET_ERROR) {
            /* Erreur lors de l'envoi du message */
            int error_code = WSAGetLastError();
            printf("Erreur lors de l'envoi du message: %d\n", error_code);
        }
        else {
            /* Affichage de la r�ponse envoy�e */
            printf("Reponse envoyee au client: %s\n", response);
        }
    }

    /* Fermeture du socket */
    closesocket(sock);
    cleanNetwork();  /* Nettoyage du r�seau avant de quitter */
    return 0;
}
