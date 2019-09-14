#include "users.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

void login(int socket);
void normalLogin(int socket);
void jointAccountLogin(int socket);
void adminLogin(int socket);
void task(int socket);
void adminTask(int socket);
void userTask(int socket);
void addAccount(int socket);
void addNormalAccount(int socket);
void addJointAccount(int socket);
void addAdmin(int socket);
void deleteAccount(int socket);
void deleteNormalAccount(int socket);
void deleteJointAccount(int socket);
void deleteAdmin(int socket);
void modifyAccount(int socket);
void modifyNormalAccount(int socket);
void modifyJointAccount(int socket);
void searchAccount(int socket);
void searchNormalAccount(int socket);
void searchJointAccount(int socket);
void deposit(int socket);
void depositNormalAccount(int socket);
void depositJointAccount(int socket);
void withdraw(int socket);
void withdrawNormalAccount(int socket);
void withdrawJointAccount(int socket);
void changePassword(int socket);
void changePasswordNormalAccount(int socket);
void changePasswordJointAccount(int socket);
