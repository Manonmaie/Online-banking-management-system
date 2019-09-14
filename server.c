#include "server.h"

int main(){
  int nsd,sd;
  struct sockaddr_in serv, cli;
  sd = socket (AF_INET, SOCK_STREAM, 0);
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = INADDR_ANY;
  serv.sin_port = htons (2525);
  bind (sd, (const struct sockaddr *)&serv, sizeof (serv));
  listen (sd, 5);
  while (1) {
    int size = sizeof(cli);
    nsd = accept (sd,(struct sockaddr*)&cli, &size);
    if (!fork()) {
      close(sd);
      login(nsd);
      while(1)
        task(nsd);
      exit(0);
    }
    else
      close(nsd);
  }
  return 0;
}

void login(int socket){

  int mode;

  read(socket,&mode,sizeof(mode));

  int fd;
  if(mode == 1){
    normalLogin(socket);
  }
  else if(mode == 2){
    jointAccountLogin(socket);
  }
  else if(mode == 3){
    adminLogin(socket);
  }
  else{
    int mode = 0;
    write(socket,&mode,sizeof(mode));
  }
  return;
}

void normalLogin(int socket){
  char password[20];
  int userid,size;
  char orgi_password[20];
  struct NormalUser user;

  read(socket,&userid,sizeof(userid));
  read(socket,password,sizeof(password));

  int fd = open("NormalUser",O_RDONLY);
  size = sizeof(struct NormalUser);

  struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
	lseek(fd,(userid-1)*size,SEEK_SET);
	read(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  if(strcmp(password,user.password)==0 && user.is_working==1){
    int mode = 1;
    write(socket,&mode,sizeof(mode));
  }
  else{
    int mode = 0;
    write(socket,&mode,sizeof(mode));
  }
}

void jointAccountLogin(int socket){
  char password[20];
  int userid,size;
  char orgi_password[20];
  struct JointAccountUser user;

  read(socket,&userid,sizeof(userid));
  read(socket,password,sizeof(password));

  int fd = open("JointAccountUser",O_RDONLY);
  size = sizeof(struct JointAccountUser);

  struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
	lseek(fd,(userid-1)*size,SEEK_SET);
	read(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  if(strcmp(password,user.password)==0 && user.is_working==1){
    int mode = 2;
    write(socket,&mode,sizeof(mode));
  }
  else{
    int mode = 0;
    write(socket,&mode,sizeof(mode));
  }
}

void adminLogin(int socket){
  char password[20];
  int userid,size;
  char orgi_password[20];
  struct Administrator user;

  read(socket,&userid,sizeof(userid));
  read(socket,password,sizeof(password));

  int fd = open("Administrator",O_RDONLY);
  size = sizeof(struct Administrator);

  struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
	lseek(fd,(userid-1)*size,SEEK_SET);
	read(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  if(strcmp(password,user.password)==0 && user.is_working==1){
    int mode = 3;
    write(socket,&mode,sizeof(mode));
  }
  else{
    int mode = 0;
    write(socket,&mode,sizeof(mode));
  }
}

void task(int socket){
  int acc_type;
  read(socket,&acc_type,sizeof(acc_type));
  if(acc_type==3)
    adminTask(socket);
  else if(acc_type==1||acc_type==2)
    userTask(socket);
  return;
}

void adminTask(int socket){
  int task;
  read(socket,&task,sizeof(task));
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
      return;
      break;
  }
}

void userTask(int socket){
  int task;
  read(socket,&task,sizeof(task));
  switch(task){
    case 1:
      return deposit(socket);
      break;
    case 2:
      return withdraw(socket);
      break;
    case 3:
      return searchAccount(socket);
      break;
    case 4:
      return changePassword(socket);
      break;
    case 5:
      return searchAccount(socket);
      break;
    case 6:
      exit(0);
      break;
    default:
      return;
      break;
  }
}

void addAccount(int socket){
  int type;
  read(socket,&type,sizeof(type));
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
      return;
  }
}

void addNormalAccount(int socket){
  struct NormalUser user;
  int prev_uid,prev_acc_no;

  user.is_working = 1;

  int fd = open("NormalUser",O_RDWR);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
  read(socket,user.name,sizeof(user.name));
  read(socket,user.password,sizeof(user.password));
  read(socket,&user.balance,sizeof(user.balance));
	lseek(fd,-1*sizeof(struct NormalUser),SEEK_END);
  read(fd,&prev_uid,sizeof(int));
  lseek(fd,sizeof(user.password),SEEK_CUR);
	read(fd,&prev_acc_no,sizeof(long));

  user.userID = prev_uid+1;
  user.account_number = prev_acc_no+1;

  lseek(fd,0,SEEK_END);
  int ret = write(fd,&user,sizeof(struct NormalUser));

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
  write(socket,&user.userID,sizeof(user.userID));
  write(socket,&user.account_number,sizeof(user.account_number));
}

void addJointAccount(int socket){
  struct JointAccountUser user;
  int prev_uid,prev_acc_no;

  user.is_working = 1;

  int fd = open("JointAccountUser",O_RDWR);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
  read(socket,user.primary_name,sizeof(user.primary_name));
  read(socket,user.secondary_name,sizeof(user.secondary_name));
  read(socket,user.password,sizeof(user.password));
  read(socket,&user.balance,sizeof(user.balance));
	lseek(fd,-1*sizeof(struct JointAccountUser),SEEK_END);
  read(fd,&prev_uid,sizeof(int));
  lseek(fd,sizeof(user.password),SEEK_CUR);
	read(fd,&prev_acc_no,sizeof(long));

  user.userID = prev_uid+1;
  user.account_number = prev_acc_no+1;

  lseek(fd,0,SEEK_END);
  int ret = write(fd,&user,sizeof(struct JointAccountUser));

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
  write(socket,&user.userID,sizeof(user.userID));
  write(socket,&user.account_number,sizeof(user.account_number));
}

void addAdmin(int socket){
  struct Administrator user;
  int prev_uid;

  user.is_working = 1;

  int fd = open("Administrator",O_RDWR);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
  read(socket,user.name,sizeof(user.name));
  read(socket,user.password,sizeof(user.password));
	lseek(fd,-1*sizeof(struct Administrator),SEEK_END);
  read(fd,&prev_uid,sizeof(int));

  user.userID = prev_uid+1;

  lseek(fd,0,SEEK_END);
  int ret = write(fd,&user,sizeof(struct Administrator));

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
  write(socket,&user.userID,sizeof(user.userID));
}

void deleteAccount(int socket){
  int type;
  read(socket,&type,sizeof(type));
  switch(type){
    case 1:
      return deleteNormalAccount(socket);
      break;
    case 2:
      return deleteJointAccount(socket);
      break;
    case 3:
      return deleteAdmin(socket);
      break;
    default:
      return;
  }
}

void deleteNormalAccount(int socket){
  int userid;
  read(socket,&userid,sizeof(userid));
  struct NormalUser user;

  int fd = open("NormalUser",O_RDWR);
  int size = sizeof(struct NormalUser);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
	lseek(fd,(userid-1)*size,SEEK_SET);
  read(fd,&user,size);
  user.is_working = 0;
  lseek(fd,(userid-1)*size,SEEK_SET);
	int ret = write(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
}

void deleteJointAccount(int socket){
  int userid;
  read(socket,&userid,sizeof(userid));
  struct JointAccountUser user;

  int fd = open("JointAccountUser",O_RDWR);
  int size = sizeof(struct JointAccountUser);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
	lseek(fd,(userid-1)*size,SEEK_SET);
  read(fd,&user,size);
  user.is_working = 0;
  lseek(fd,(userid-1)*size,SEEK_SET);
	int ret = write(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
}

void deleteAdmin(int socket){
  int userid;
  read(socket,&userid,sizeof(userid));
  struct Administrator user;

  int fd = open("Administrator",O_RDWR);
  int size = sizeof(struct Administrator);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
	lseek(fd,(userid-1)*size,SEEK_SET);
  read(fd,&user,size);
  user.is_working = 0;
  lseek(fd,(userid-1)*size,SEEK_SET);
	int ret = write(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
}

void modifyAccount(int socket){
  int type;
  read(socket,&type,sizeof(type));
  switch(type){
    case 1:
      return modifyNormalAccount(socket);
      break;
    case 2:
      return modifyJointAccount(socket);
      break;
    default:
      return;
  }
}

void modifyNormalAccount(int socket){
  int userid;
  char name[30];
  float balance;

  read(socket,&userid,sizeof(userid));
  read(socket,name,sizeof(name));
  read(socket,&balance,sizeof(balance));

  struct NormalUser user;

  int fd = open("NormalUser",O_RDWR);
  int size = sizeof(struct NormalUser);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
	lseek(fd,(userid-1)*size,SEEK_SET);
  read(fd,&user,size);
  if(user.is_working==0){
    int ret = 0;
    write(socket,&ret,sizeof(ret));
    return;
  }
  strcpy(user.name,name);
  user.balance = balance;
  lseek(fd,(userid-1)*size,SEEK_SET);
	int ret = write(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
}

void modifyJointAccount(int socket){
  int userid;
  char name1[30],name2[30];
  float balance;

  read(socket,&userid,sizeof(userid));
  read(socket,name1,sizeof(name1));
  read(socket,name2,sizeof(name2));
  read(socket,&balance,sizeof(balance));

  struct JointAccountUser user;

  int fd = open("JointAccountUser",O_RDWR);
  int size = sizeof(struct JointAccountUser);

  struct flock lock;
  lock.l_type=F_WRLCK;
  lock.l_whence=SEEK_SET;
  lock.l_start=(userid-1)*size;
  lock.l_len=size;
  lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
  lseek(fd,(userid-1)*size,SEEK_SET);
  read(fd,&user,size);
  if(user.is_working==0){
    int ret = 0;
    write(socket,&ret,sizeof(ret));
    return;
  }
  strcpy(user.primary_name,name1);
  strcpy(user.secondary_name,name2);
  user.balance = balance;
  lseek(fd,(userid-1)*size,SEEK_SET);
  int ret = write(fd,&user,size);

  lock.l_type=F_UNLCK;
  fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
}

void searchAccount(int socket){
  int type;
  read(socket,&type,sizeof(type));
  switch(type){
    case 1:
      return searchNormalAccount(socket);
      break;
    case 2:
      return searchJointAccount(socket);
      break;
    default:
      return;
  }
}

void searchNormalAccount(int socket){
  int userid;
  read(socket,&userid,sizeof(userid));
  struct NormalUser user;

  int fd = open("NormalUser",O_RDONLY);
  int size = sizeof(struct NormalUser);

  struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
	lseek(fd,(userid-1)*size,SEEK_SET);
	read(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&user,size);
}

void searchJointAccount(int socket){
  int userid;
  read(socket,&userid,sizeof(userid));
  struct JointAccountUser user;

  int fd = open("JointAccountUser",O_RDONLY);
  int size = sizeof(struct JointAccountUser);

  struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
	lseek(fd,(userid-1)*size,SEEK_SET);
	read(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&user,size);
}

void deposit(int socket){
  int type;
  read(socket,&type,sizeof(type));
  switch(type){
    case 1:
      return depositNormalAccount(socket);
      break;
    case 2:
      return depositJointAccount(socket);
      break;
    default:
      return;
  }
}

void depositNormalAccount(int socket){
  int userid;
  read(socket,&userid,sizeof(userid));
  float amount;

  struct NormalUser user;

  int fd = open("NormalUser",O_RDWR);
  int size = sizeof(struct NormalUser);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
  read(socket,&amount,sizeof(amount));
	lseek(fd,(userid-1)*size,SEEK_SET);
  read(fd,&user,size);
  if(user.is_working==0){
    int ret = 0;
    write(socket,&ret,sizeof(ret));
    return;
  }
  user.balance += amount;
  lseek(fd,(userid-1)*size,SEEK_SET);
	int ret = write(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
}

void depositJointAccount(int socket){
  int userid;
  read(socket,&userid,sizeof(userid));
  float amount;

  struct JointAccountUser user;

  int fd = open("JointAccountUser",O_RDWR);
  int size = sizeof(struct JointAccountUser);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
  read(socket,&amount,sizeof(amount));
	lseek(fd,(userid-1)*size,SEEK_SET);
  read(fd,&user,size);
  if(user.is_working==0){
    int ret = 0;
    write(socket,&ret,sizeof(ret));
    return;
  }
  user.balance += amount;
  lseek(fd,(userid-1)*size,SEEK_SET);
	int ret = write(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
}

void withdraw(int socket){
  int type;
  read(socket,&type,sizeof(type));
  switch(type){
    case 1:
      return withdrawNormalAccount(socket);
      break;
    case 2:
      return withdrawJointAccount(socket);
      break;
    default:
      return;
  }
}

void withdrawNormalAccount(int socket){
  int userid;
  read(socket,&userid,sizeof(userid));
  float amount;

  struct NormalUser user;

  int fd = open("NormalUser",O_RDWR);
  int size = sizeof(struct NormalUser);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
  read(socket,&amount,sizeof(amount));
	lseek(fd,(userid-1)*size,SEEK_SET);
  read(fd,&user,size);
  if(user.is_working==0||user.balance<amount){
    int ret = 0;
    write(socket,&ret,sizeof(ret));
    return;
  }
  user.balance -= amount;
  lseek(fd,(userid-1)*size,SEEK_SET);
	int ret = write(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
}

void withdrawJointAccount(int socket){
  int userid;
  read(socket,&userid,sizeof(userid));
  float amount;

  struct JointAccountUser user;

  int fd = open("JointAccountUser",O_RDWR);
  int size = sizeof(struct JointAccountUser);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
  read(socket,&amount,sizeof(amount));
	lseek(fd,(userid-1)*size,SEEK_SET);
  read(fd,&user,size);
  if(user.is_working==0||user.balance<amount){
    int ret = 0;
    write(socket,&ret,sizeof(ret));
    return;
  }
  user.balance -= amount;
  lseek(fd,(userid-1)*size,SEEK_SET);
	int ret = write(fd,&user,size);

  sleep(10);
  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
}

void changePassword(int socket){
  int type;
  read(socket,&type,sizeof(type));
  switch(type){
    case 1:
      return changePasswordNormalAccount(socket);
      break;
    case 2:
      return changePasswordJointAccount(socket);
      break;
    default:
      return;
  }
}

void changePasswordNormalAccount(int socket){
  int userid;
  char new_password[20];

  read(socket,&userid,sizeof(userid));

  struct NormalUser user;

  int fd = open("NormalUser",O_RDWR);
  int size = sizeof(struct NormalUser);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
  read(socket,new_password,sizeof(new_password));
	lseek(fd,(userid-1)*size,SEEK_SET);
  read(fd,&user,size);
  if(user.is_working==0){
    int ret = 0;
    write(socket,&ret,sizeof(ret));
    return;
  }
  strcpy(user.password,new_password);
  lseek(fd,(userid-1)*size,SEEK_SET);
	int ret = write(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
}

void changePasswordJointAccount(int socket){
  int userid;
  char new_password[20];

  read(socket,&userid,sizeof(userid));

  struct JointAccountUser user;

  int fd = open("JointAccountUser",O_RDWR);
  int size = sizeof(struct JointAccountUser);

  struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(userid-1)*size;
	lock.l_len=size;
	lock.l_pid=getpid();

  fcntl(fd,F_SETLKW,&lock);
  read(socket,new_password,sizeof(new_password));
	lseek(fd,(userid-1)*size,SEEK_SET);
  read(fd,&user,size);
  if(user.is_working==0){
    int ret = 0;
    write(socket,&ret,sizeof(ret));
    return;
  }
  strcpy(user.password,new_password);
  lseek(fd,(userid-1)*size,SEEK_SET);
	int ret = write(fd,&user,size);

  lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
  close(fd);

  write(socket,&ret,sizeof(ret));
}
