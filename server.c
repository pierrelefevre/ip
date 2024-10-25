#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define RESPONSE_SIZE 64

void handle_client(int client_socket, struct sockaddr_in client_address) {
    char response[RESPONSE_SIZE];
    snprintf(response, RESPONSE_SIZE, "%s\n", inet_ntoa(client_address.sin_addr));

    char http_response[RESPONSE_SIZE + 64];
    snprintf(http_response, sizeof(http_response),
             "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: text/plain\r\n\r\n%s",
             strlen(response), response);

    send(client_socket, http_response, strlen(http_response), 0);
    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_len);

        if (client_socket >= 0) {
            handle_client(client_socket, client_address);
        }
    }

    close(server_socket);
    return 0;
}
