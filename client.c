#include "client.h"

int main(){
  int sd;
  struct sockaddr_in serv;
  sd = socket (AF_INET, SOCK_STREAM, 0);
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = INADDR_ANY;
  serv.sin_port = htons (2525);

  connect (sd, (const struct sockaddr *)&serv, sizeof(serv));

  struct Account account = login(sd);
  int kind = account.type;
  int id = account.userID;
  if(kind == 0){
    write(1,"\nEnter Valid Credentials!!\n\n",sizeof("\nEnter Valid Credentials!!\n\n"));
    exit(0);
  }
  while(1){
    if(task(sd,kind,id)){
      write(1,"\nTask completed Successfully!!\n\n",sizeof("\nTask completed Successfully!!\n"));
    }
    else
      write(1,"\nTask was not successful\nPlease try again\n\n",sizeof("\nTask was not successful\nPlease try again\n\n"));
  }
}

struct Account login(int socket){
  char password[20];
  int userid,mode;
  int ret;

  write(1,"\n1.Normal User\n2.Joint Account User\n3.Administrator\n\n",sizeof("\n1.Normal User\n2.Joint Account User\n3.Administrator\n\n"));
  write(1,"\nSelect how you want to login:",sizeof("\nSelect how you want to login:"));
  // read(0,&mode,sizeof(mode));
  scanf("%d",&mode);

  write(1,"\nUserId:",sizeof("\nUserId:"));
  // read(0,&userid,sizeof(userid));
  scanf("%d",&userid );
  write(1,"\nPassword:",sizeof("\nPassword:"));
  // read(0,password,sizeof(password));
  scanf("%s",password);

  write(socket,&mode,sizeof(mode));
  write(socket,&userid,sizeof(userid));
  write(socket,password,sizeof(password));

  read(socket,&ret,sizeof(ret));

  struct Account acc;
  acc.type = ret;
  acc.userID = userid;
  return acc;
}

bool task(int socket,int acc_type,int userid){
  write(socket,&acc_type,sizeof(acc_type));
  if(acc_type==3)
    return adminTask(socket,userid);
  else if(acc_type==1||acc_type==2)
    return userTask(socket,userid,acc_type);
  else
    return false;
}

bool adminTask(int socket,int userid){
  int task;
  write(1,"\n1.Add Account\n2.Delete Account\n3.Modify Account\n4.Get details of account\n5.Exit\n\n",sizeof("\n1.Add Account\n2.Delete Account\n3.Modify Account\n4.Get details of account\n5.Exit\n\n"));
  write(1,"\nChoose what you want to do:",sizeof("\nChoose what you want to do:"));
  scanf("%d",&task);
  write(socket,&task,sizeof(task));
  switch(task){
    case 1:
      return addAccount(socket);
      break;
    case 2:
      return deleteAccount(socket);
      break;
    case 3:
      return modifyAccount(socket);
      break;
    case 4:
      return searchAccount(socket);
      break;
    case 5:
      exit(0);
      break;
    default:
      write(1,"\nChoose vaid option\n\n",sizeof("\nChoose vaid option\n\n"));
      return false;
      break;
  }
}

bool userTask(int socket,int userid,int type){
  int task;
  write(1,"\n1.Deposit\n2.Withdraw\n3.Balance Enquiry\n4.Change Password\n5.Get details of account\n6.Exit\n\n",sizeof("\n1.Deposit\n2.Withdraw\n3.Balance Enquiry\n4.Change Password\n5.Get details of account\n6.Exit\n\n"));
  write(1,"\nChoose what you want to do:",sizeof("\nChoose what you want to do:"));
  scanf("%d",&task);
  write(socket,&task,sizeof(task));
  switch(task){
    case 1:
      return deposit(socket,userid,type);
      break;
    case 2:
      return withdraw(socket,userid,type);
      break;
    case 3:
      return balanceEnquiry(socket,userid,type);
      break;
    case 4:
      return changePassword(socket,userid,type);
      break;
    case 5:
      return getDetails(socket,userid,type);
      break;
    case 6:
      exit(0);
      break;
    default:
      write(1,"\nChoose vaid option\n\n",sizeof("\nChoose vaid option\n\n"));
      return false;
      break;
  }
}

bool addAccount(int socket){
  int type;
  write(1,"\n1.Normal account\n2.Joint account\n3.Admin\n\n",sizeof("\n1.Normal account\n2.Joint account\n3.Admin\n\n"));
  write(1,"\nEnter which type of account you want to add:",sizeof("\nEnter which type of account you want to add:"));
  scanf("%d",&type);
  write(socket,&type,sizeof(type));
  switch(type){
    case 1:
      return addNormalAccount(socket);
      break;
    case 2:
      return addJointAccount(socket);
      break;
    case 3:
      return addAdmin(socket);
      break;
    default:
      write(1,"\nSelect valid type of account\n\n",sizeof("\nSelect valid type of account\n\n"));
      return false;
      break;
  }
}

bool addNormalAccount(int socket){
  char name[30],password[20];
  float balance;
  int userid;
  long acc_no;
  int ret;
  write(1,"\nEnter the name of account holder:",sizeof("\nEnter the name of account holder:"));
  scanf("%s",name);
  write(1,"\nEnter the password for the account holder:",sizeof("\nEnter the password for the account holder:"));
  scanf("%s",password);
  write(1,"\nEnter balance:",sizeof("\nEnter balance:"));
  scanf("%f",&balance);

  write(socket,name,sizeof(name));
  write(socket,password,sizeof(password));
  write(socket,&balance,sizeof(balance));

  read(socket,&ret,sizeof(ret));
  read(socket,&userid,sizeof(userid));
  read(socket,&acc_no,sizeof(acc_no));

  if(!ret)
    return false;

  // write(1,"Your userid and account number are:\n",sizeof("Your userid and account number are:\n"));
  // write(1,&userid,sizeof(userid));
  // write(1,",",sizeof(","));
  // write(1,&acc_no,sizeof(acc_no));
  // write(1," respectively\n\n",sizeof(" respectively\n\n"));
  printf("\nYour user id:%d\nYour account number:%ld\n\n",userid,acc_no);
  return true;
}

bool addJointAccount(int socket){
  char name1[30],name2[30],password[20];
  float balance;
  int userid;
  long acc_no;
  int ret;
  write(1,"\nEnter the name of primary account holder:",sizeof("\nEnter the name of primary account holder:"));
  scanf("%s",name1);
  write(1,"\nEnter the name of secondary account holder:",sizeof("\nEnter the name of secondary account holder:"));
  scanf("%s",name2);
  write(1,"\nEnter the password:",sizeof("\nEnter the password:"));
  scanf("%s",password);
  write(1,"\nEnter balance:",sizeof("\nEnter balance:"));
  scanf("%f",&balance);

  write(socket,name1,sizeof(name1));
  write(socket,name2,sizeof(name2));
  write(socket,password,sizeof(password));
  write(socket,&balance,sizeof(balance));

  read(socket,&ret,sizeof(ret));
  read(socket,&userid,sizeof(userid));
  read(socket,&acc_no,sizeof(acc_no));

  if(!ret)
    return false;

  // write(1,"Your userid and account number are:",sizeof("Your userid and account number are:"));
  // write(1,&userid,sizeof(userid));
  // write(1,",",sizeof(","));
  // write(1,&acc_no,sizeof(acc_no));
  // write(1," respectively\n\n",sizeof(" respectively\n\n"));
  printf("\nYour user id:%d\nYour account number:%ld\n\n",userid,acc_no);
  return true;
}

bool addAdmin(int socket){
  char name[30],password[20];
  int userid;
  int ret;

  write(1,"\nEnter the name of account holder:",sizeof("\nEnter the name of account holder:"));
  scanf("%s",name);
  write(1,"\nEnter the password for the account holder:",sizeof("\nEnter the password for the account holder:"));
  scanf("%s",password);

  write(socket,name,sizeof(name));
  write(socket,password,sizeof(password));

  read(socket,&ret,sizeof(ret));
  read(socket,&userid,sizeof(userid));

  if(!ret)
    return false;
  // write(1,"Your userid is:",sizeof("Your userid is:"));
  // write(1,&userid,sizeof(userid));
  // write(1,"\n\n",sizeof("\n\n"));
  printf("\nYour user id:%d\n\n",userid);
  return true;
}

bool deleteAccount(int socket){
  int type;
  write(1,"\n1.Normal account\n2.Joint account\n3.Admin\n\n",sizeof("\n1.Normal account\n2.Joint account\n3.Admin\n\n"));
  write(1,"\nEnter which type of account you want to delete:",sizeof("\nEnter which type of account you want to delete:"));
  scanf("%d",&type);
  write(socket,&type,sizeof(type));
  if(type==1||type==2||type==3){
    int userid;
    int ret;
    write(1,"\nEnter the userid of account you want to  delete:",sizeof("\nEnter the userid of account you want to  delete:"));
    scanf("%d",&userid);

    write(socket,&userid,sizeof(userid));

    read(socket,&ret,sizeof(ret));

    if(!ret)
      return false;
    else
      return true;
  }
  else{
    write(1,"\nSelect valid type of account\n\n",sizeof("\nSelect valid type of account\n\n"));
    return false;
  }
}

bool modifyAccount(int socket){
  int type;
  write(1,"\n1.Normal account\n2.Joint account\n\n",sizeof("\n1.Normal account\n2.Joint account\n\n"));
  write(1,"\nEnter which type of account you want to modify:",sizeof("\nEnter which type of account you want to modify:"));
  scanf("%d",&type);
  write(socket,&type,sizeof(type));
  switch(type){
    case 1:
      return modifyNormalAccount(socket);
      break;
    case 2:
      return modifyJointAccount(socket);
      break;
    default:
      write(1,"\nSelect valid type of account\n\n",sizeof("\nSelect valid type of account\n\n"));
      return false;
      break;
  }
}

bool modifyNormalAccount(int socket){
  int userid;
  char name[30];
  float balance;
  int ret;
  write(1,"\nEnter the userid of account you want to  modify:",sizeof("\nEnter the userid of account you want to  modify:"));
  scanf("%d",&userid);
  write(1,"\nEnter new name:",sizeof("\nEnter new name:"));
  scanf("%s",name);
  write(1,"\nEnter balance to be updated:",sizeof("\nEnter balance to be updated:"));
  scanf("%f",&balance);

  write(socket,&userid,sizeof(userid));
  write(socket,name,sizeof(name));
  write(socket,&balance,sizeof(balance));

  read(socket,&ret,sizeof(ret));
  if(!ret)
    return false;
  return true;
}

bool modifyJointAccount(int socket){
  int userid;
  char name1[30],name2[30];
  float balance;
  int ret;
  write(1,"\nEnter the userid of account you want to  modify:",sizeof("\nEnter the userid of account you want to  modify:"));
  scanf("%d",&userid);
  write(1,"\nEnter new primary name:",sizeof("\nEnter new primary name:"));
  scanf("%s",name1);
  write(1,"\nEnter new secondary name:",sizeof("\nEnter new secondary name:"));
  scanf("%s",name2);
  write(1,"\nEnter balance to be updated:",sizeof("\nEnter balance to be updated:"));
  scanf("%f",&balance);

  write(socket,&userid,sizeof(userid));
  write(socket,name1,sizeof(name1));
  write(socket,name2,sizeof(name2));
  write(socket,&balance,sizeof(balance));

  read(socket,&ret,sizeof(ret));
  if(!ret)
    return false;
  return true;
}

bool searchAccount(int socket){
  int type;
  write(1,"\n1.Normal account\n2.Joint account\n\n",sizeof("\n1.Normal account\n2.Joint account\n\n"));
  write(1,"\nEnter which type of account you want to search:",sizeof("\nEnter which type of account you want to search:"));
  scanf("%d",&type);
  write(socket,&type,sizeof(type));
  switch(type){
    case 1:
      return searchNormalAccount(socket);
      break;
    case 2:
      return searchJointAccount(socket);
      break;
    default:
      write(1,"\nSelect valid type of account\n\n",sizeof("\nSelect valid type of account\n\n"));
      return false;
      break;
  }
}

bool searchNormalAccount(int socket){
  int userid;
  struct NormalUser user;
  write(1,"\nEnter the userid of account you want to search:",sizeof("\nEnter the userid of account you want to search:"));
  scanf("%d",&userid);
  write(socket,&userid,sizeof(userid));

  int ret = read(socket,&user,sizeof(struct NormalUser));
  if(!ret)
    return false;

  printf("\nName:%s\nAccount number:%ld\nStatus:%d\nBalance:%f\n\n",user.name,user.account_number,user.is_working,user.balance);
  return true;
}

bool searchJointAccount(int socket){
  int userid;
  struct JointAccountUser user;
  write(1,"\nEnter the userid of account you want to search:",sizeof("\nEnter the userid of account you want to search:"));
  scanf("%d",&userid);
  write(socket,&userid,sizeof(userid));

  int ret = read(socket,&user,sizeof(struct JointAccountUser));
  if(!ret)
    return false;

  printf("\nPrimary name:%s\nSecondary name:%s\nAccount number:%ld\nStatus:%d\nBalance:%f\n\n",user.primary_name,user.secondary_name,user.account_number,user.is_working,user.balance);
  return true;
}

bool deposit(int socket,int userid,int type){
  float amount;
  int ret;

  write(socket,&type,sizeof(type));
  write(socket,&userid,sizeof(userid));

  write(1,"\nEnter amount you want to deposit:",sizeof("\nEnter amount you want to deposit:"));
  scanf("%f",&amount);
  write(socket,&amount,sizeof(amount));

  read(socket,&ret,sizeof(ret));

  if(!ret)
    return false;
  return true;
}

bool withdraw(int socket,int userid,int type){
  float amount;
  int ret;

  write(socket,&type,sizeof(type));
  write(socket,&userid,sizeof(userid));

  write(1,"\nEnter amount you want to withdraw:",sizeof("\nEnter amount you want to withdraw:"));
  scanf("%f",&amount);
  write(socket,&amount,sizeof(amount));

  read(socket,&ret,sizeof(ret));

  if(!ret)
    return false;
  return true;
}

bool balanceEnquiry(int socket,int userid,int type){
  write(socket,&type,sizeof(type));

  switch (type) {
    case 1:
      return balanceEnquiryNormalAccount(socket,userid);
      break;
    case 2:
      return balanceEnquiryJointAccount(socket,userid);
      break;
    default:
      write(1,"\nEnter valid account\n\n",sizeof("\nEnter valid account\n\n"));
      return false;
      break;
  }
}

bool balanceEnquiryNormalAccount(int socket,int userid){
  struct NormalUser user;

  write(socket,&userid,sizeof(userid));

  int ret = read(socket,&user,sizeof(struct NormalUser));
  if(!ret)
    return false;

  printf("\nBalance in your account:%f\n\n",user.balance);
  return true;
}

bool balanceEnquiryJointAccount(int socket,int userid){
  struct JointAccountUser user;

  write(socket,&userid,sizeof(userid));

  int ret = read(socket,&user,sizeof(struct JointAccountUser));
  if(!ret)
    return false;

  printf("\nBalance in your account:%f\n\n",user.balance);
  return true;
}

bool changePassword(int socket,int userid,int type){
  int ret;
  char password[20];

  write(socket,&type,sizeof(type));
  write(socket,&userid,sizeof(userid));

  write(1,"\nEnter new password:",sizeof("\nEnter new password:"));
  scanf("%s",password);
  write(socket,password,sizeof(password));

  read(socket,&ret,sizeof(ret));

  if(!ret)
    return false;
  return true;
}

bool getDetails(int socket,int userid,int type){

  write(socket,&type,sizeof(type));

  switch (type) {
    case 1:
      return getDetailsNormalAccount(socket,userid);
      break;
    case 2:
      return getDetailsJointAccount(socket,userid);
      break;
    default:
      write(1,"\nEnter valid account\n\n",sizeof("\nEnter valid account\n\n"));
      return false;
      break;
  }
}

bool getDetailsNormalAccount(int socket,int userid){
  struct NormalUser user;

  write(socket,&userid,sizeof(userid));

  int ret = read(socket,&user,sizeof(struct NormalUser));
  if(!ret)
    return false;

  printf("\nName:%s\nAccount number:%ld\nBalance:%f\n\n",user.name,user.account_number,user.balance);
  return true;
}

bool getDetailsJointAccount(int socket,int userid){
  struct JointAccountUser user;

  write(socket,&userid,sizeof(userid));

  int ret = read(socket,&user,sizeof(struct JointAccountUser));
  if(!ret)
    return false;

  printf("\nPrimary name:%s\nSecondary name:%s\nAccount number:%ld\nBalance:%f\n\n",user.primary_name,user.secondary_name,user.account_number,user.balance);
  return true;
}
