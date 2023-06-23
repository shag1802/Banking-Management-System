#ifndef INCLUDEFILES_H
#define INCLUDEFILES_H

int maincontroller(int cli, int uid, long cookie);
int admincontroller(int cli, long cookie);
int viewaccountdetails(int cli);
int change_admin_pwd(int cli);
int change_acctype(int cli);
int delete_acc(int cli);
int add_acc(int cli);
int cli_exit(int cli, int uid);
int pwd_change(int cli, int uid);
int view_details(int cli, int uid);
int bal(int cli, int uid);
int deposit(int cli, int uid);
int withdraw(int cli, int uid);
long gencookie(int val);
int normallogin(int cli);
int adminlogin(int cli);
int loginHandler(int cli);
int connectsocket();
int connectCli(int socket_d) ;



////////////////////////////////////////////////////////////////
enum login_req{
    normalloginreq,
    adminloginreq,
    jointloginreq,
};

enum admin_req_type{
    admin_dummy_req,
    add_acc_req,
    del_acc_req,
    acc_type_req,
    chng_pwd_req,
    acc_det_req,
    admin_exit_req,
};
enum req_type{
    dummy_req,
    dep_req,
    withdrawl_req,    
    bal_req,
    pwd_chng_req,
    view_det_req,
    exit_req,
};

enum acc_type{
    singleAccount,
    jointAccount
};

enum transcation_type{
    Deposit,
    Withdrawal
};

typedef struct Cred{
    int id;
    char pwd[50];
}Cred;

typedef struct AdminCred{
    char uname[50];
    char pwd[50];
}AdminCred;

typedef struct Account{
    int id;
    int bal;
}Account;

typedef struct User{
    int id;
    char pwd[50];
    int acc_id;
    enum acc_type acc_type;
}User;


////////////////////////////////////////////////////////////////
User get_user(int uid);
int get_acc_id(int uid);
int opt_Deposit(int deposit, int uid);
int opt_withdraw(int withdraw, int uid);
int *apt_acc_details(int uid);
int opt_pwd_change(char *newpass, int uid);
int opt_bal(int uid);
int validatelogin(Cred *loginattm);
int validateadminlogin(AdminCred *loginattm);
int opt_del_acc(int account_no);
int opt_modify_acc_type(int account_no, int acc_type);
int opt_modify_admin_pwd(char *newpwd, char *uname);
int opt_add_acc(int acc_no);

////////////////////////////////////////
void exitcontroller(long cookie, int socket_d);
void acc_controller(long cookie, int socket_d);
void pwd_controller(long cookie, int socket_d);
void bal_controller(long cookie, int socket_d);
void deposit_controller(long cookie, int socket_d);
void withdraw_controller(long cookie, int socket_d);
int login_opt(long cookie, int socket_d);
int admin_login(long cookie, int socket_d, char *username);
void view_acc_details_controller(long cookie, int socket_d);
void Mod_admin_pwd(long cookie, int socket_d, char *username);
void Mod_acc_type(long cookie, int socket_d);
void del_acc_controller(long cookie, int socket_d);
void add_acc_controller(long cookie, int socket_d);
int logintoggle(int socket_d);
int jointlogin(int socket_d);
int normallogin(int socket_d);
int adminlogin(int socket_d);

#endif 



