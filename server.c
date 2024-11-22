#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 1024      
#define MAX_CONN 5     

// Функция для логов сообщений от клиента
void log_message(const char* client_ip, const char* message) {
    printf("%s: %s\n", client_ip, message);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int number_to_guess, guess;
    char buffer[1024] = {0};   
    char client_ip[INET_ADDRSTRLEN];  

    // создание сокета
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");   
        exit(EXIT_FAILURE);
    }

    // привязка сокета к порту
    // используем IPv4
    address.sin_family = AF_INET; 
    // принимаем подключения с ЛЮБОГО IP
    address.sin_addr.s_addr = INADDR_ANY;
    // привязка к заданному порту
    address.sin_port = htons(PORT);                

    // привязка сокета к указанному адресу и порту
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");   // ошибка привязки
        exit(EXIT_FAILURE);
    }

    // прослушивание подключений
    if (listen(server_fd, MAX_CONN) < 0) {
        perror("listen");   
        exit(EXIT_FAILURE);
    }
    
    printf("Сервер запущен по порту %d\n", PORT);

    // основной цикл сервера
    while (1) {
        printf("Ожидание соединения...\n");
        // ожидание подключения клиента
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // получение IP-адреса клиента в строке
        inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
        log_message(client_ip, "Подключенный клиент");

        // генерация случайного числа
        srand(time(NULL));
        number_to_guess = rand() % 255 + 1;

        while (1) {
            // очистка буфера приемом
            memset(buffer, 0, sizeof(buffer));  
            int valread = recv(new_socket, buffer, sizeof(buffer), 0);
            if (valread <= 0) {
                log_message(client_ip, "Клиент отключен");
                close(new_socket);
                break;
            }

            guess = atoi(buffer);  // преобразование строки в число
            printf("Полученное предположение: %d from %s\n", guess, client_ip);

            // проверка введенного числа
            if (guess == number_to_guess) {
                send(new_socket, "Правильно!", strlen("Правильно!"), 0); 
                log_message(client_ip, "Клиент угадал правильно");
                break;
            } else if (guess < number_to_guess) {
                const char* response = "Загаданное число больше";
                send(new_socket, response, strlen(response), 0);  // загаданное число больше
                log_message(client_ip, response); // Логируем сообщение
            } else {
                const char* response = "Загаданное число меньше";
                send(new_socket, response, strlen(response), 0);  // загаданное число меньше
                log_message(client_ip, response); // Логируем сообщение
            }
        }
         // закрытие соединения с клиентом
        close(new_socket); 
    }
     // закрытие сокета сервера
    close(server_fd); 
    return 0;
}
