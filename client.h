#include "users.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

struct Account login(int socket);
bool task(int socket,int acc_type,int userid);
bool adminTask(int socket,int userid);
bool userTask(int socket,int userid,int type);
bool addAccount(int socket);
bool addNormalAccount(int socket);
bool addJointAccount(int socket);
bool addAdmin(int socket);
bool deleteAccount(int socket);
bool modifyAccount(int socket);
bool modifyNormalAccount(int socket);
bool modifyJointAccount(int socket);
bool searchAccount(int socket);
bool searchNormalAccount(int socket);
bool searchJointAccount(int socket);
bool deposit(int socket,int userid,int type);
bool withdraw(int socket,int userid,int type);
bool balanceEnquiry(int socket,int userid,int type);
bool balanceEnquiryNormalAccount(int socket,int userid);
bool balanceEnquiryJointAccount(int socket,int userid);
bool changePassword(int socket,int userid,int type);
bool getDetails(int socket,int userid,int type);
bool getDetailsNormalAccount(int socket,int userid);
bool getDetailsJointAccount(int socket,int userid);
