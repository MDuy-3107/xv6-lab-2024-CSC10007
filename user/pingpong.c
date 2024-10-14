#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int parent[2], child[2];  
  char *c;  // Biến để lưu ký tự được truyền qua pipe

  if (pipe(parent) < 0 || pipe(child) < 0) {
    fprintf(2, "pipe creation failed!\n");
    exit(1);
}
  if(fork()==0){
    close(parent[1]);
    close(child[0]);
    if(read(parent[0], &c, 1)!=1){
      fprintf(2,"read failed!\n");
      exit(1);
    }

    printf("%d: received ping\n", getpid());

    if(write(child[1],c,1) != 1){
      fprintf(2,"write failed!\n");
      exit(1);
    }
    exit(0);
  }else{
    c = ".";
    close(parent[0]);
    close(child[1]);
    if(write(parent[1],c,1)!=1){
      fprintf(2,"write failed!\n");
      exit(1);
    }
    if(read(child[0],&c,1) != 1){
      fprintf(2,"read failed!\n");
      exit(1);
    }
   printf("%d: received pong\n", getpid());
   exit(0);
  }
}
