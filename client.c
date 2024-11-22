#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

 // порт, будет подключаться клиент
#define PORT 1024 
int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0}; 
    int guess;

    // проверка аргументов командной строки
    if (argc != 2) {
        printf("Использование: %s <Server IP>\n", argv[0]);
        return -1;
    }

    // создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Ошибка создания сокета\n");
        return -1;
    }

    // заполнение структуры с адресом сервера
    serv_addr.sin_family = AF_INET;         
    serv_addr.sin_port = htons(PORT);       

    // преобразование IP-адреса из текстового в бинарное
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("Неверный адрес/ Адрес не поддерживается\n");
        return -1;
    }

    // подключение к серверу
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Не удалось установить соединение\n");
        return -1;
    }

    // основной цикл игры
    while (1) {
        printf("Введите своё число в диапазоне от 1 до 255: ");
        scanf("%d", &guess);

        // отправка предположения на сервер
        sprintf(buffer, "%d", guess);
        send(sock, buffer, strlen(buffer), 0);

        // получение ответа от сервера с использованием функции recv
        memset(buffer, 0, sizeof(buffer));
        int valread = recv(sock, buffer, sizeof(buffer), 0);

        // проверка на ошибку получения данных
        if (valread <= 0) {
            printf("Ошибка при получении данных или соединение закрыто сервером\n");
            break;
        }

        printf("Сервер: %s\n", buffer);

        // проверка, угадано ли число
        if (strcmp(buffer, "Правильно!") == 0) {
            break;
        }
    }

    close(sock);  // закрытие сокета клиента
    return 0;
}
