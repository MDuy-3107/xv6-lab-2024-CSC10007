#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int left_pipe) __attribute__((noreturn));

void sieve(int left_pipe) {
    int prime;
    int n;

    // Đọc số nguyên tố đầu tiên từ pipe
    if (read(left_pipe, &prime, sizeof(prime)) == 0) {
        close(left_pipe);
        exit(0);
    }

    printf("prime %d\n", prime); // In ra số nguyên tố

    int p[2];
    if (pipe(p) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    if (fork() == 0) {
        // Process con
        close(p[1]);  // Đóng đầu ghi của pipe mới
        close(left_pipe);  // Đóng đầu đọc của pipe cũ (không cần nữa)
        sieve(p[0]);  // Gọi lại hàm sieve với pipe mới
    } else {
        // Process cha
        close(p[0]);  // Đóng đầu đọc của pipe mới
        while (read(left_pipe, &n, sizeof(n)) > 0) {
            if (n % prime != 0) {
                write(p[1], &n, sizeof(n));  // Ghi số không chia hết vào pipe mới
            }
        }
        close(left_pipe);  // Đóng pipe cũ
        close(p[1]);  // Đóng pipe mới
        wait(0);  // Chờ tiến trình con
        exit(0);  // Kết thúc process
    }
}

int main(void) {
    int p[2];
    if (pipe(p) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    if (fork() == 0) {
        // Process con
        close(p[1]);  // Đóng đầu ghi của pipe
        sieve(p[0]);  // Bắt đầu sàng
    } else {
        // Process cha
        close(p[0]);  // Đóng đầu đọc của pipe
        for (int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(i));  // Ghi các số vào pipe
        }
        close(p[1]);  // Đóng đầu ghi
        wait(0);  // Chờ process con hoàn thành
        exit(0);  // Thoát chương trình
    }
}
