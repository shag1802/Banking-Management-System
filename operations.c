#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "include/IncludeFiles.h"

extern int errno;

User get_user(int uid){
    User valid; 
    int seekamt = uid -1; 
    int fd = open("data/Users.dat", O_RDONLY,0744);
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_start = seekamt*sizeof(User);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(User);
    lock.l_pid = getpid();
    

    int ret = fcntl(fd, F_SETLK, &lock);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "write lock failed %s\n",strerror(err));
        return valid;
    }
    lseek(fd,seekamt*sizeof(valid),SEEK_SET);
    read(fd, &valid, sizeof(valid));
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return valid;
}

int get_acc_id(int uid){
    User user = get_user(uid);
    return user.acc_id;
}


int opt_Deposit(int deposit, int uid){
    int acc_id = get_acc_id(uid);
    Account upd;
    int seekamt = acc_id -1; 
    int fd = open("data/Accounts.dat", O_RDWR);
    if(fd == -1) {
      printf("Unable to open the file\n");
      return -1;
    }
    static struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_start = seekamt*sizeof(upd);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(upd);
    lock.l_pid = getpid();

    int ret = fcntl(fd, F_SETLKW, &lock);
    if (ret == -1) {
      printf("Attempt to set write lock failed\n");
      return -1;
    }
    
    lseek(fd,seekamt*sizeof(upd),SEEK_SET);
    read(fd, &upd, sizeof(upd));
    printf("Initial balance was : %d\n", upd.bal);

    upd.bal += deposit;
    lseek(fd, seekamt*(sizeof(upd)), SEEK_SET);
    write(fd, &upd, sizeof(upd));
    
    lseek(fd, seekamt*(sizeof(upd)), SEEK_SET);
    read(fd, &upd, sizeof(upd));
    printf("Final balance is : %d\n",upd.bal);
    
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &lock);
    close(fd);
    return 1;
}


int opt_withdraw(int withdraw, int uid){
    int acc_id = get_acc_id(uid);
    Account upd;
    int seekamt = acc_id -1; 
    int fd = open("data/Accounts.dat", O_RDWR);
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_start = seekamt*sizeof(upd);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(upd);
    lock.l_pid = getpid();

    int ret = fcntl(fd, F_SETLKW, &lock);
    if (ret == -1) {
      printf("Attempt to set write lock failed\n");
      return -1;
    }
    
    lseek(fd,seekamt*sizeof(upd),SEEK_SET);
    read(fd, &upd, sizeof(upd));
    printf("Current balance : %d\n", upd.bal);
    printf("Withdrawl amount: %d\n", withdraw);
    if (upd.bal < withdraw){
        printf("Amount of withdrawl is greater than current balance \n");
        return -1;
    }
    upd.bal -= withdraw;
    lseek(fd, seekamt*(sizeof(upd)), SEEK_SET);
    write(fd, &upd, sizeof(upd));
 
    lseek(fd, seekamt*(sizeof(upd)), SEEK_SET);
    read(fd, &upd, sizeof(upd));
    printf("final balance is : %d\n",upd.bal);
    
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &lock);
    close(fd);
    return 1;
}
int *apt_acc_details(int uid){
    static int acc_details[4] = {0,0,0,0}; 
    User user = get_user(uid);
    int account_balance = opt_bal(uid);
    printf("UserID: %d\n", uid);
    printf("AccountNo: %d\n", user.acc_id);
    printf("accountType: %d\n", user.acc_type);
    printf("acc bal: %d\n", account_balance);
    acc_details[0] = uid;
    acc_details[1] = user.acc_id;
    acc_details[2] = user.acc_type;
    acc_details[3] = account_balance;
    return acc_details;
}
int opt_pwd_change(char *newpwd, int uid){
    User valid; 
    int seekamt = uid -1; 
    int fd = open("data/Users   dat", O_RDWR);
    if(fd == -1) {
      printf("Unable to open the file\n");
      return -1;
    }
    static struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_start = seekamt*sizeof(valid);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(valid);
    lock.l_pid = getpid();

    int ret = fcntl(fd, F_SETLKW, &lock);
    if (ret == -1) {
        printf("Attempt to set write lock failed\n");
        return -1;
    }
    lseek(fd,seekamt*sizeof(valid),SEEK_SET);
    read(fd, &valid, sizeof(valid));
    printf("Old password: %s\n", valid.pwd);
    
    strcpy(valid.pwd, newpwd);
    lseek(fd,seekamt*sizeof(valid),SEEK_SET);
    write(fd, &valid, sizeof(valid));
    
    
    lseek(fd,seekamt*sizeof(valid),SEEK_SET);
    read(fd, &valid, sizeof(valid));
    printf("New password: %s\n", valid.pwd);
    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
    close(fd);
    return 1;
}

int opt_bal(int uid){
    int acc_id = get_acc_id(uid);
    Account upd;
    int seekamt = acc_id -1; 
    int fd = open("data/Accounts.dat", O_RDWR);
    if(fd == -1) {
      printf("Unable to open the file\n");
      return -1;
    }
    static struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_start = seekamt*sizeof(upd);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(upd);
    lock.l_pid = getpid();

    int ret = fcntl(fd, F_SETLKW, &lock);
    if (ret == -1) {
      printf("Attempt to set write lock failed\n");
      return -1;
    }
    
    lseek(fd,seekamt*sizeof(upd),SEEK_SET);
    read(fd, &upd, sizeof(upd));
    printf("balance is : %d\n", upd.bal);
    
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &lock);
    close(fd);
    return upd.bal;
}
int validatelogin(Cred *loginatt) {
    int uid = loginatt->id; 
    User valid = get_user(uid); 
    int res;
    if (valid.id == loginatt->id){
        if (strcmp(valid.pwd, loginatt->pwd) == 0){
            res = 1;
        }
        else{
            res = -1;
        }
    }
    else{
        res = -1;
    }
    return res;
} 


int validateadminlogin(AdminCred *loginatt) {
    printf("inside the validateAdminLogin function\n"); 
    AdminCred valid; 
    int seekamt = 0; 
    int fd = open("data/Admins.dat", O_RDONLY,0744);
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock;
    
    lock.l_type = F_RDLCK;
    lock.l_start =0; 
    lock.l_whence = SEEK_SET;
    lock.l_len =0; 
    lock.l_pid = getpid();

    int ret = fcntl(fd, F_SETLK, &lock);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock failed %s\n",strerror(err));
        return -1;
    }
    int res = -1;
    lseek(fd,0,SEEK_SET); 
    printf("Username entered : %s\n", loginatt->uname);
    printf("Password entered: %s\n", loginatt->pwd);
    while(read(fd, &valid, sizeof(valid))){
        if (strcmp(valid.uname, loginatt->uname) == 0){
            if (strcmp(valid.pwd, loginatt->pwd) == 0){
                return 1;
            }
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return res;
} 

int opt_del_acc(int acc_no){
    Account acc;
    Account del = {
        .id = -1,
        .bal = -1
    };
    int fd = open("data/Accounts.dat", O_RDWR,0744);
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock;
    
    lock.l_type = F_WRLCK;
    lock.l_start =0; 
    lock.l_whence = SEEK_SET;
    lock.l_len =0; 
    lock.l_pid = getpid();
    

    int ret = fcntl(fd, F_SETLK, &lock);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock failed %s\n",strerror(err));
        return -1;
    }
    int res = -1;
    lseek(fd,0,SEEK_SET);
    while(read(fd, &acc, sizeof(acc))){
        if (acc.id == acc_no){
            lseek(fd,-sizeof(acc), SEEK_CUR);
            write(fd, &del, sizeof(del));
            res = 0; 
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    
    User user;
    fd = open("data/Users.dat", O_RDWR,0744);
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock1;
    
    lock1.l_type = F_WRLCK;
    lock1.l_start =0; 
    lock1.l_whence = SEEK_SET;
    lock1.l_len =0; 
    lock1.l_pid = getpid();
    

    ret = fcntl(fd, F_SETLK, &lock1);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock failed %s\n",strerror(err));
        return -1;
    }
    
    lseek(fd,0,SEEK_SET);
    while(read(fd, &user, sizeof(user))){
        if (user.acc_id == acc_no){
            user.acc_id = -1;
            lseek(fd,-sizeof(user), SEEK_CUR);
            write(fd, &user, sizeof(user));
            res = 1; 
        }
    }
    lock1.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock1);
    close(fd);
    return res;

}

int opt_modify_acc_type(int acc_no, int acc_type){
    User user;
    acc_type = acc_type -1;
    printf("modify account type section\n");
    int fd = open("data/Users.dat", O_RDWR,0744);
    int res;
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock1;
    
    lock1.l_type = F_WRLCK;
    lock1.l_start =0; 
    lock1.l_whence = SEEK_SET;
    lock1.l_len =0; 
    lock1.l_pid = getpid();
    

    int ret = fcntl(fd, F_SETLK, &lock1);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock failed %s\n",strerror(err));
        return -1;
    }
    
    lseek(fd,0,SEEK_SET);
    while(read(fd, &user, sizeof(user))){
        if (user.acc_id == acc_no){
            user.acc_type = acc_type;
            lseek(fd,-sizeof(user), SEEK_CUR);
            write(fd, &user, sizeof(user));
            res = 1; 
            break;
        }
    }
    if (res != 1){
        res = -1;
    }
    lock1.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock1);
    close(fd);
    return res;
}

int opt_modify_admin_pwd(char *newpwd, char *uname){
    AdminCred valid; 
    printf("modify admin password section\n");
    int fd = open("data/Admins.dat", O_RDWR,0744);
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock;
    
    lock.l_type = F_WRLCK;
    lock.l_start =0; 
    lock.l_whence = SEEK_SET;
    lock.l_len =0; 
    lock.l_pid = getpid();
    

    int ret = fcntl(fd, F_SETLK, &lock);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock failed %s\n",strerror(err));
        return -1;
    }
    lseek(fd,0,SEEK_SET); 
    while(read(fd, &valid, sizeof(valid))){
        if (strcmp(valid.uname, uname) == 0){
            strcpy(valid.pwd, newpwd);
            lseek(fd,-sizeof(valid), SEEK_CUR);
            write(fd, &valid, sizeof(valid));
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            close(fd);
            return 1;
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return -1;

}

int opt_add_acc(int acc_no){  
    Account upd = {
        .id = acc_no,
        .bal = 0,
    };
    int fd = open("data/Accounts.dat", O_RDWR | O_APPEND);
    if(fd == -1) {
      printf("Unable to open the file\n");
      return -1; 
    }
    static struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();

    int ret = fcntl(fd, F_SETLKW, &lock);
    if (ret == -1) {
      printf("Attempt to set write lock failed\n");
      return -1;
    }
    
    write(fd, &upd, sizeof(upd));
    
    
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &lock);
    close(fd);
    printf("acc created\n");
    User newUser;
    fd = open("data/Users.dat", O_RDWR,0744);
    if(fd == -1) {
      printf("Unable to open the file\n");
      return -1;
    }
    static struct flock lock1;

    lock1.l_type = F_WRLCK;
    lock1.l_start = 0;
    lock1.l_whence = SEEK_SET;
    lock1.l_len =0; 
    lock1.l_pid = getpid();
    

    ret = fcntl(fd, F_SETLK, &lock1);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock1 failed %s\n",strerror(err));
        return -1;
    }
    
    int max_user_id;
    while(read(fd, &newUser, sizeof(newUser))){
        if (newUser.id > max_user_id)
            max_user_id = newUser.id;
    }
    
    printf("max_user_id: %d\n", max_user_id); 
    newUser.id = max_user_id+1;
    newUser.acc_id =acc_no; 
    newUser.acc_type = singleAccount;
    char pass[50] = "dummypwd123";
    strcpy(newUser.pwd, pass);
    
    ret = write(fd, &newUser, sizeof(newUser)); 
    if (ret < 0){
        printf("New user can't be written error\n");
        return -1;
    }
    printf("User is written in databse, but still in critical section\n"); 
    lock1.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock1);
    close(fd);
    printf("New user created succesfully and assigned new account\n");
    return 1;
}

