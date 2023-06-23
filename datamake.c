#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "include/IncludeFiles.h"


int main(){
    int fd;
    fd = open("data/Accounts.dat", O_CREAT | O_RDWR, 0777);
    if (fd == -1){
        printf("file not found: %s\n");
        exit(1);
    }
    lseek(fd, 0, SEEK_SET);

    Account acc1 = {
        .id = 1,
        .bal = 20000,
    };

    write(fd, &acc1, sizeof(acc1));

    Account acc2 = {
        .id = 2,
        .bal = 40000,
    };

    write(fd, &acc2, sizeof(acc2));
    
    Account acc3 = {
        .id = 3,
        .bal = 60000,
    };

    write(fd, &acc3, sizeof(acc3));
    
    Account acc4 = {
        .id = 4,
        .bal = 80000,
    };

    write(fd, &acc4, sizeof(acc4));
    close(fd);
    ////////////////////////////////////////////////////////////////
    fd = open("data/Admins.dat", O_CREAT | O_RDWR, 0777);
    if (fd == -1){
        printf("file not found: %s\n");
        exit(1);
    }
    lseek(fd, 0, SEEK_SET);

    AdminCred a1 = {
        .uname = "a1",
        .pwd = "1",
    };

    write(fd, &a1, sizeof(a1));

    AdminCred a2 = {
        .uname = "a2",
        .pwd = "2",
    };
    write(fd, &a2, sizeof(a2));
    close(fd);
    //////////////////////////////////////////////////////////////////
    fd = open("data/Users.dat", O_CREAT | O_RDWR, 0777);
    if (fd == -1){
        printf("file not found: %s\n");
        exit(1);
    }
    lseek(fd, 0, SEEK_SET);

    User u1 = {
        .id = 1,
        .pwd = "1",
        .acc_type = singleAccount,
        .acc_id = 1,
    };

    write(fd, &u1, sizeof(u1));

    User u2 = {
        .id = 2,
        .pwd = "2",
        .acc_type = singleAccount,
        .acc_id = 2,
    };

    write(fd, &u2, sizeof(u2));
    
    User u3 = {
        .id = 3,
        .pwd = "3",
        .acc_type = jointAccount,
        .acc_id = 3,
    };

    write(fd, &u3, sizeof(u3));
    
    User u4 = {
        .id = 4,
        .pwd = "4",
        .acc_type = jointAccount,
        .acc_id = 4,
    };

    write(fd, &u4, sizeof(u4));
    close(fd);
    return 0;

}