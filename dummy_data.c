#include "users.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int fd;
  struct NormalUser nu;
  struct JointAccountUser jau;
  struct Administrator a;
	fd=creat("NormalUser",O_CREAT|0755);
  for(int i=0;i<1;i++){
    // read(0,&(nu.userID),sizeof(nu.userID));
    // read(0,nu.password,sizeof(nu.password));
    // read(0,&(nu.account_number),sizeof(nu.account_number));
    // read(0,nu.name,sizeof(nu.name));
    // read(0,&(nu.balance),sizeof(nu.balance));
		scanf("%d",&nu.userID);
    scanf("%s",nu.password );
		scanf("%ld",&nu.account_number);
		// scanf("%d",&nu.is_working);
		nu.is_working = 1;
		scanf("%f",&nu.balance);
		scanf("%s",nu.name );
  	write(fd,&nu,sizeof(nu));
  }
	close(fd);
  fd=creat("JointAccountUser",O_CREAT|0755);
  for(int i=0;i<1;i++){
    // read(0,&(jau.userID),sizeof(jau.userID));
    // read(0,jau.password,sizeof(jau.password));
    // read(0,&jau.account_number,sizeof(jau.account_number));
    // read(0,jau.primary_name,sizeof(jau.primary_name));
    // read(0,jau.secondary_name,sizeof(jau.secondary_name));
    // read(0,&jau.balance,sizeof(jau.balance));
		scanf("%d",&jau.userID);
    scanf("%s",jau.password);
		scanf("%ld",&jau.account_number);
		jau.is_working = 1;
		scanf("%f",&jau.balance);
		scanf("%s",jau.primary_name);
		scanf("%s",jau.secondary_name);
  	write(fd,&jau,sizeof(jau));
  }
	close(fd);
  fd=creat("Administrator",O_CREAT|0755);
  for(int i=0;i<1;i++){
    // read(0,&(a.userID),sizeof(a.userID));
    // read(0,a.password,sizeof(a.password));
    // read(0,a.name,sizeof(a.name));
		scanf("%d",&a.userID);
    scanf("%s",a.password);
		a.is_working = 1;
		scanf("%s",a.name);
  	write(fd,&a,sizeof(a));
  }
	close(fd);
}
