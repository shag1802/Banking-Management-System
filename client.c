#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<errno.h>
#include<arpa/inet.h>
#include "include/IncludeFiles.h"

#define PORT 4444
extern int errno;

void exitcontroller(long cookie, int socket_d){
    int opt = 6;
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &opt, sizeof(opt));
    close(socket_d);
    printf("Disconnected from server\n");
    exit(0);
}

void acc_controller(long cookie, int socket_d){
    int opt = 5;
    int acc_details[4] = {0,0,0,0}; 
    
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &opt, sizeof(opt)); 
    
    read(socket_d, acc_details, sizeof(acc_details)); 
    printf("User id: %d\n", acc_details[0]);
    printf("Account Number: %d\n", acc_details[1]);
    printf("Account Type: %d\n", acc_details[2]);
    printf("Account balance: %d\n", acc_details[3]);
}

void pwd_controller(long cookie, int socket_d){
    int opt = 4;
    char pwd[50];
    int servResp;
    printf("Please enter new password: ");
    scanf("%s", pwd);
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &opt, sizeof(opt));
    write(socket_d, pwd, sizeof(pwd));


    read(socket_d, &servResp, sizeof(servResp));
    if (servResp > 0){
        printf("Password Changed \n");
    }
    else{
        printf("Error: unable to change password\n");
    }
}

void bal_controller(long cookie, int socket_d){
    int servResp;
    int opt = 3;
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &opt, sizeof(opt));

    read(socket_d, &servResp, sizeof(servResp));
    if (servResp > 0){
        printf("current balance is: %d\n", servResp);
    }
    else{
        printf("Error : unable to display balance\n");
    }
}
void deposit_controller(long cookie, int socket_d){
    int dep;
    int servResp;
    int opt = 1;
    printf("Input amount to be deposited: ");
    scanf("%d", &dep);
    
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &opt, sizeof(opt));
    write(socket_d, &dep, sizeof(dep));

    read(socket_d, &servResp, sizeof(servResp));
    if (servResp == 1){
        printf("Money deposited successfully\n");
    }
    else{
        printf("Error : couldn't deposit money\n");
    }
}

void withdraw_controller(long cookie, int socket_d){
    int withdraw;
    int servResp;
    int opt = 2;
    printf("Please enter the amount you would like to withdraw:\n");
    scanf("%d", &withdraw);
    
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &opt, sizeof(opt));
    write(socket_d, &withdraw, sizeof(withdraw));

    read(socket_d, &servResp, sizeof(servResp));
    if (servResp == 1){
        printf("Money withdrawn successfully\n");
    }
    else{
        printf("Error:  couldn't withdrawn money\n");
    }
}

int login_opt(long cookie, int socket_d){
    write(1, "1. Deposit\n", sizeof("1. Deposit\n")); 
    write(1, "2. Withdraw\n", sizeof("1. Withdraw\n")); 
    write(1, "3. Balance Enquiry\n", sizeof("3. Balance Enquiry\n")); 
    write(1, "4. Password Change\n", sizeof("4. Password Change\n")); 
    write(1, "5. Account Details\n", sizeof("5. Account Details\n")); 
    write(1, "6. Exit\n", sizeof("6. Exit\n")); 
    int res;
    scanf("%d", &res);
    switch(res){
        case 1:
            deposit_controller(cookie, socket_d);break;
        case 2:
            withdraw_controller(cookie, socket_d);break;
        case 3:
            bal_controller(cookie, socket_d);break;
        case 4:
            pwd_controller(cookie, socket_d);break;
        case 5:
            acc_controller(cookie, socket_d);break;
        case 6:
            exitcontroller(cookie, socket_d);break;
    }
    login_opt(cookie, socket_d);
}

int admin_login(long cookie, int socket_d, char *username){
    write(1, "1. Add new Account\n", sizeof("1. Add new Account\n")); 
    write(1, "2. Delete Account\n", sizeof("1. Delete Account\n")); 
    write(1, "3. Change Account Type\n", sizeof("3. Change Account Type\n")); 
    write(1, "4. Change password of admin\n", sizeof("4. Change password of admin\n")); 
    write(1, "5. Account Details\n", sizeof("5. Account Details\n")); 
    write(1, "6. Exit\n", sizeof("6. Exit\n")); 
    int res;
    scanf("%d", &res);
    printf("recorded res: %d\n", res);
    switch(res){
        case 1:
            add_acc_controller(cookie, socket_d);break;
        case 2:
            del_acc_controller(cookie, socket_d);break;
        case 3:
            Mod_acc_type(cookie, socket_d);break;
        case 4:
            Mod_admin_pwd(cookie, socket_d, username);break;
        case 5:
            view_acc_details_controller(cookie, socket_d);break;
        case 6:
            exitcontroller(cookie, socket_d);break;
    }
    admin_login(cookie, socket_d, username);
}

void view_acc_details_controller(long cookie, int socket_d){
    int uid;
    int opt = 5;
    printf("Input user ID : ");
    scanf("%d", &uid);
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &opt, sizeof(opt));
    write(socket_d, &uid, sizeof(uid));
    printf("written everything\n"); 
    int acc_details[4] = {0,0,0,0}; 
    
    read(socket_d, acc_details, sizeof(acc_details)); 
    printf("UserID: %d\n", acc_details[0]);
    printf("AccountNo: %d\n", acc_details[1]);
    printf("accountType: %d\n", acc_details[2]);
    printf("account balance: %d\n", acc_details[3]);

}


void Mod_admin_pwd(long cookie, int socket_d, char *username){
    char new_pwd[50];
    int opt = 4;
    printf("Input new admin account password: ");
    scanf("%s", new_pwd);
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &opt, sizeof(opt));
    write(socket_d, new_pwd, sizeof(new_pwd));
    write(socket_d, username, sizeof(username));
    int ret;
    read(socket_d, &ret, sizeof(ret));
    if (ret == 1){
        printf("admin password changed successfully\n");
    }
    else{
        printf("Error: password change3 failed \n");
    }
}
void Mod_acc_type(long cookie, int socket_d){
    int acc_no;
    int acc_type;
    int opt = 3;
    printf("Input the acc_no you want to modify: ");
    scanf("%d", &acc_no);
    printf("New Account type:\n1. Single Account\n2. Joint Account\n");
    scanf("%d", &acc_type);
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &opt, sizeof(opt));
    write(socket_d, &acc_no, sizeof(acc_no));
    write(socket_d, &acc_type, sizeof(acc_type));
    int ret;
    read(socket_d, &ret, sizeof(ret));
    if (ret == 1){
        printf("account no:%d's type successfully set\n", acc_no);
    }
    else{
        printf("Error : failed to change account type\n");
    }

}
void del_acc_controller(long cookie, int socket_d){
    int acc_no;
    int opt = 2;
    printf("Input the account number you want to delete: ");
    scanf("%d", &acc_no);
    
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &opt, sizeof(opt));
    write(socket_d, &acc_no, sizeof(acc_no));

    int ret;
    read(socket_d, &ret, sizeof(ret));
    if (ret == 1){
        printf("account no: %d successfully delelted\n", acc_no);
    }
    else{
        printf("delete account failed\n");
    }
}

void add_acc_controller(long cookie, int socket_d){
    int acc_no;
    int acc_type;
    int opt = 1;
    printf("Input the account number you want to add: ");
    scanf("%d", &acc_no);
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &opt, sizeof(opt));
    write(socket_d, &acc_no, sizeof(acc_no));
    int ret;
    read(socket_d, &ret, sizeof(ret));
    if (ret == 1){
        printf("account no: %d successfully added\n", acc_no);
    }
    else{
        printf("Error : Coudn't add account\n");
    }

    opt = 3;
    printf("New Account type:\n1. Single Account\n2. Joint Account\n");
    scanf("%d", &acc_type);
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &opt, sizeof(opt));
    write(socket_d, &acc_no, sizeof(acc_no));
    write(socket_d, &acc_type, sizeof(acc_type));
    ret;
    read(socket_d, &ret, sizeof(ret));
    if (ret == 1){
        printf("account no:%d's type successfully set\n", acc_no);
    }
    else{
        printf("Error : failed to change account type\n");
    }
}

int logintoggle(int socket_d){
    write(1, "0. RegularLogin\n", sizeof("1. RegularLogin\n")); 
    write(1, "1. AdminLogin\n", sizeof("1. AdminLogin\n"));
    write(1, "2. JointLogin\n", sizeof("2. JointLogin\n")); 
    int res;
    scanf("%d", &res);
    switch(res){
        case 0:
            normallogin(socket_d);
            break;
        case 1:
            adminlogin(socket_d);
            break;
        case 2:
            jointlogin(socket_d);
            break;
    }

}
int jointlogin(int socket_d){
    enum login_req type = jointloginreq;
    int temp = write(socket_d, &type, sizeof(type));
    Cred *req = malloc(sizeof(Cred));
    int userID;
    char password[50];
    printf("Joint Account Login.......\n");
    printf("UserID: ");
    scanf("%d", &req->id);
    printf("Password: ");
    scanf("%s", password);
    strcpy(req->pwd, password);
    int ret = write(socket_d, req, sizeof(Cred));
    if (ret < 0){
        int err = errno;	
        fprintf(stderr, "unable to write to server: %s\n",strerror(err));
        return -1;
    }
    long cookie;
    read(socket_d, &cookie, sizeof(cookie));
    if (cookie == -1)
        logintoggle(socket_d);
    else
        login_opt(cookie, socket_d); 
        return 1;
}
int normallogin(int socket_d){
    enum login_req type = normalloginreq;
    int temp = write(socket_d, &type, sizeof(type));
    
    Cred *req = malloc(sizeof(Cred));
    int userID;
    char password[50];
    printf("Normal Login ....\n");
    printf("UserID: ");
    scanf("%d", &req->id);
    printf("Password: ");
    scanf("%s", password);
    strcpy(req->pwd, password);
    int ret = write(socket_d, req, sizeof(Cred));
    if (ret < 0){
        int err = errno;	
        fprintf(stderr, "unable to write to server: %s\n",strerror(err));
        return -1;
    }
    long cookie;
     
    read(socket_d, &cookie, sizeof(cookie));
    
    if (cookie == -1)
        logintoggle(socket_d);
    else
        login_opt(cookie, socket_d); 
        return 1;
}

int adminlogin(int socket_d){
    enum login_req type = adminloginreq;
    int temp = write(socket_d, &type, sizeof(type));
    AdminCred *req = malloc(sizeof(AdminCred));
    char username[50];
    char password[50];
    printf("Admin Login ....\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
    strcpy(req->pwd, password);
    strcpy(req->uname, username);
    int ret = write(socket_d, req, sizeof(AdminCred));
    if (ret < 0){
        int err = errno;	
        fprintf(stderr, "unable to write to server: %s\n",strerror(err));
        return -1;
    }
    long cookie;
    read(socket_d, &cookie, sizeof(cookie));
    if (cookie == -1)
        logintoggle(socket_d);
    else
        admin_login(cookie, socket_d, username); 
        return 1;
}

int main(){
    struct sockaddr_in server;
    int socket_d;
    int cnct; // for connection
    char buff[5];
    socket_d = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    cnct = connect(socket_d, (struct sockaddr *)&server, sizeof(server));
    if (cnct< 0){
        int err = errno;	
        fprintf(stderr, "server is unable to connect: %s\n",strerror(err));
        return -1;
    }
    long cookie = logintoggle(socket_d); 
    printf("returned to main\n");
    return 0;
}
