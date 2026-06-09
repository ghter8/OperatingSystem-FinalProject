#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

// 定義你剛剛在核心設定的系統呼叫號碼
#define SYS_mycall 454

int main() {
    int test_val = 12;
    printf("Sending %d to System Call %d...\n", test_val, SYS_mycall);
    
    // 使用 syscall() 函式，傳入號碼與參數
    long result = syscall(SYS_mycall, test_val);
    
    printf("Result received from kernel: %ld\n", result);
    return 0;
}