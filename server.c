#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include "include/IncludeFiles.h"

#define PORT 4444
extern int errno;

int maincontroller(int cli,int uid, long cookie){
    printf("Inside maincontroller\n");
    enum req_type type;
    long cookie_recv;
    read(cli, &cookie_recv, sizeof(cookie_recv));
    read(cli, &type, sizeof(type));
    if (cookie_recv == cookie){ 
        printf("matched\n");
        printf("type recieved is: %d\n", type);
        switch(type){
            case dep_req:
                deposit(cli, uid);
                break;
            case withdrawl_req:
                withdraw(cli, uid);
                break;
            case bal_req:
                bal(cli, uid);
                break;
            case pwd_chng_req:
                pwd_change(cli, uid);
                break;
            case view_det_req:
                view_details(cli, uid);
                break;
            case exit_req:
                cli_exit(cli, uid);
                break;
        } 
    }
    maincontroller(cli, uid, cookie);

}

int admincontroller(int cli, long cookie){
    printf("admincontroller section\n");
    enum admin_req_type type;
    long cookie_recv;
    read(cli, &cookie_recv, sizeof(cookie_recv));
    read(cli, &type, sizeof(type));
    if (cookie_recv == cookie){ 
        printf("matched\n");
        printf("type recieved is: %d\n", type);
        switch(type){
            case add_acc_req:
                add_acc(cli);
                break;
            case del_acc_req:
                delete_acc(cli);
                break;
            case acc_type_req:
                change_acctype(cli);
                break;
            case chng_pwd_req:
                change_admin_pwd(cli);
                break;
            case acc_det_req:
                viewaccountdetails(cli);
                break;
            case admin_exit_req:
                cli_exit(cli, -1);
                break;
            default:
                printf("Unexpexted value \n");
                break;
        } 
    }
    admincontroller(cli, cookie);
}

int viewaccountdetails(int cli){
    int uid;
    read(cli, &uid, sizeof(uid));
    view_details(cli, uid);
    return 1;

}
int change_admin_pwd(int cli){
    char pwd[50];
    char username[50];
    printf("Modify admin pwd function section\n");
    read(cli, pwd, sizeof(pwd));
    read(cli, username, sizeof(username));
    printf("pwd recv: %s\n", pwd);
    printf("username recv: %s\n", username);
    int ret = opt_modify_admin_pwd(pwd, username);
    write(cli, &ret, sizeof(ret)); 
    if (ret == 1){
        printf("successfully changed the admin pwd\n");
    }
    else{
        printf("unable to change the pwd\n");
    }
    return ret;
}

int change_acctype(int cli){
    int accno;
    int acctype; 
    printf("Modify account type section\n");
    read(cli, &accno, sizeof(accno));
    read(cli, &acctype, sizeof(acctype)) ;
    printf("accno read : %d\n", accno);
    printf("acctype read : %d\n", acctype);
    int ret = opt_modify_acc_type(accno, acctype);
    write(cli, &ret, sizeof(ret)); 
    if (ret == 1){
        printf("successfully changed\ accounttype\n");
    }
    else
        printf("couldn't change account type\n");
    return ret;
}

int delete_acc(int cli){
    int accno;
    read(cli, &accno, sizeof(accno));
    int ret = opt_del_acc(accno);
    write(cli, &ret, sizeof(ret));
    if (ret == 1){
        printf("successfully deleted the account\n");
    }
    else{
        printf("couldn't delete the account\n");
    }
    return ret;
}

int add_acc(int cli){
    printf("add_acc section\n");
    int accno;

   read(cli, &accno, sizeof(accno));
   printf("readed account no :%d\n", accno);
   int ret = opt_add_acc    (accno);
   write(cli, &ret, sizeof(ret));
   return ret;
}

int cli_exit(int cli,int uid){
    close(cli);
    if (cli == -1)
        printf("connection ended with admin\n");
    else
        printf("connection with client is closed now\n");
    exit(0);
}

int pwd_change(int cli, int uid){
    char pwd[50];
    int ret;
    read(cli, pwd, sizeof(pwd));
    ret = opt_pwd_change(pwd, uid);
    write(cli, &ret, sizeof(ret));
    return ret;
}

int view_details(int cli, int uid){
    printf("view_details section\n");
    int *details = malloc(sizeof(int)*4);
    details = apt_acc_details(uid); 
    // 4 integers should be written to cli
    write(cli, details, sizeof(int)*4);
    
    return 1;
} 

int bal(int cli, int uid){
    int bal = opt_bal(uid);
    write(cli, &bal, sizeof(bal));
    if (bal > 0)
        return 1;
    else
        return -1;
}


int deposit(int cli, int uid){
    int n ;
    read(cli, &n, sizeof(n));
    int ret = opt_Deposit(n, uid);
    write(cli, &ret, sizeof(ret));
    return ret;
}

int withdraw(int cli, int uid){
    int w;
    printf("withdraw section\n");
    read(cli, &w, sizeof(w));
    int ret = opt_withdraw(w, uid);
    write(cli, &ret, sizeof(ret));
    return ret;

}

long gencookie(int val){
    if (val == 1){
        return random();
    }
    else{
        return -1;
    }
}

int normallogin(int cli){
    Cred *loginatt = malloc(sizeof(Cred));
    printf("wait for login credentials...\n");
    int bytes = read(cli, loginatt, sizeof(Cred));
    int val = validatelogin(loginatt);
    long cookie = gencookie(val);
    int normallogin(int cli);
    write(cli, &cookie, sizeof(cookie));  
    if (val == 1){
        int uid = loginatt->id;
        maincontroller(cli, uid,  cookie);
    } 
    else{
        loginHandler(cli);
    }
    return 0;
} 

int adminlogin(int cli){
    AdminCred *loginatt = malloc(sizeof(AdminCred));
    printf("waiting for admin login cred\n");
    int bytes = read(cli, loginatt, sizeof(AdminCred));
    printf("bytes read %d\n", bytes);
    int val = validateadminlogin(loginatt);
    long cookie = gencookie(val);


    write(cli, &cookie, sizeof(cookie));  
    if (val == 1){
        admincontroller(cli, cookie);
    } 
    else{
        loginHandler(cli);
    }
    return 0;

}


int loginHandler(int cli){
    enum login_req type;
    printf("Login Handler section\n");
    int ret; 
    ret = read(cli, &type, sizeof(type));
    printf("bytes read : %d\n", ret);
    if (ret < 0){
        printf("first query is not doing anything\n");
        return -1;
    }
    switch(type){
        case normalloginreq:
            normallogin(cli);break;
        case adminloginreq:
            adminlogin(cli);break;
    } 
    return 0;
}

int connectsocket(){
    int socket_d;
    struct sockaddr_in serveraddr, cliaddr;
    int opt = 1;
    socket_d = socket(AF_INET, SOCK_STREAM, 0);// creating socket in namespace
    if (socket_d == 0){
        int err = errno;	
        fprintf(stderr, "failure in server end socket creation: %s\n", strerror(err));
        return -1;
    }
    setsockopt(socket_d, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);
    if(bind(socket_d, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
        int err = errno;	
        fprintf(stderr, "failure in server end binding: %s\n", strerror(err));
        return -1;
    }
    return socket_d;
}

int connectCli(int socket_d)  {
    // maximum 10 users at once
    struct sockaddr_in cliaddr;
	socklen_t addr_size = sizeof(cliaddr);
    listen(socket_d, 10);
    int childpid;


    while(1){
        int cli = accept(socket_d, (struct sockaddr *)&cliaddr,&addr_size);
        if (cli < 0){
            int err = errno;	
            fprintf(stderr, "connection failed: %s\n", strerror(err));
            return -1;
        }
        printf("connection accepted from %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
        if ((childpid=fork()) == 0){
            close(socket_d);
            loginHandler(cli);    
            return 1;
        }
        else{
            close(cli);
        }
        
    }
return 0;
}


int main(){
    srandom(42);   
    int socket_d = connectsocket();
    connectCli(socket_d);
    return 0;
}

