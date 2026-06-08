#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <time.h>

#define SYS_set_tickets 549
#define ITERATIONS 1000000000ULL  // 確保運算量足夠大（約數秒）

void do_cpu_bound_work(const char* name) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // CPU 密集型迴圈
    volatile unsigned long long i;
    for (i = 0; i < ITERATIONS; i++) {
        // 純粹消耗 CPU 時間
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("進程 [%s] (PID: %d) 完成運算，耗時: %.2f 秒\n", name, getpid(), elapsed);
}

int main() {
    printf("=========================================\n");
    printf(" 開始執行彩票排程基準測試 (Lottery Benchmark) \n");
    printf("=========================================\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork 失敗");
        return 1;
    } else if (pid == 0) {
        // 子進程：分配較多彩票 (300 張)
        if (syscall(SYS_set_tickets, 300) < 0) {
            perror("子進程設定彩票失敗");
        }
        
        // 稍微等待親進程，確保兩者幾乎同時開始競爭 CPU
        usleep(100000); 
        do_cpu_bound_work("子進程 - 300張彩票");
        exit(0);
    } else {
        // 親進程：分配預設/較少彩票 (100 張)
        if (syscall(SYS_set_tickets, 100) < 0) {
            perror("親進程設定彩票失敗");
        }

        usleep(100000);
        do_cpu_bound_work("親進程 - 100張彩票");

        // 等待子進程結束
        wait(NULL);
    }

    printf("基準測試結束。\n");
    return 0;
}