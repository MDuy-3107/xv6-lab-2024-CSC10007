#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    char buf[2048], ch;
    char *p = buf;
    char *args[MAXARG];
    int count = 0;
    int blanks = 0;
    int offset = 0;

    if(argc <= 1) {
        fprintf(2, "Usage: xargs <command> [...]\n");
        exit(1);
    }

    //Doc cac tham so
    for(int i = 1; i < argc; i++) {
        args[count++] = argv[i];
    }

    while(read(0, &ch, 1) > 0) {
        if(ch == ' ' || ch == '\t') {
            blanks++;
            continue;
        }
        if(blanks) { //Neu gap khoang trang
            buf[offset++] = 0; //Ket thuc chuoi
            args[count++] = p; //Luu dia chi cua chuoi
            p = buf + offset; //Chuan bi cho chuoi tiep theo
            blanks = 0; //Dat lai bien blanks
        }
        if(ch != '\n')
            buf[offset++] = ch; //Luu ky tu vao buf
        else {
            args[count++] = p; //Luu dia chi cua chuoi
            p = buf + offset; //Chuan bi cho chuoi tiep theo
            if(fork() == 0)
                exit(exec(args[0], args)); //Thuc thi lenh
            wait(0); //Cho tien trinh con ket thuc
            count = argc - 1; //Dat lai so luong tham so
        }
    }

    exit(0);
}