struct NormalUser{
  int userID;
  char password[20];
  long account_number;
  int is_working;//if the account is still there or not
  float balance;
  char name[30];
};

struct JointAccountUser{
  int userID;
  char password[20];
  long account_number;
  int is_working;//if the account is still there or not
  float balance;
  char primary_name[30];
  char secondary_name[30];
};

struct Administrator{
  int userID;
  char password[20];
  int is_working;
  char name[30];
};

struct Account{
  int type;//1.NormalUser 2.JointAccountUser 3.Administrator
  int userID;
};
