#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
int choice, curr_user;

struct Singleuser
{
    int uid;
    char uname[40];
    char pwd[20];
    int accountNo;
    float balance;
    char status[20];
};

struct Jointuser
{
    int uid;
    char uname1[40];
    char uname2[40];
    char pwd[20];
    int accountNo;
    float balance;
    char status[20];
};

struct Admin
{
    int id;
    char uname[40];
    char pwd[20];
};

void Menu(int temp);
void Jointuserlogin(int temp);
void Adminlogin(int temp);

void Singleuserlogin(int temp)
{
    bool flag;
    struct Singleuser u;
    printf("User ID: ");
    scanf("%d", &u.uid);
    printf("Enter password: ");
    scanf("%s", u.pwd);
    write(temp, &choice, sizeof(int));
    write(temp, &u, sizeof(u));
    read(temp, &flag, sizeof(flag));

    if (flag)
        printf("Login successful\n");

    else
    {
        int choice;
        printf("Invalid Login\n");
        printf("Press the corresponding number\n");
        printf("1. Single user Login\n");
        printf("2. Joint User Login\n");
        printf("3. Admin Login\n");
        scanf("%d", &choice);
        if (choice == 1)
            Singleuserlogin(temp);

        else if (choice == 2)
            Jointuserlogin(temp);

        else if (choice == 3)
            Adminlogin(temp);

        else
            printf("Invalid Choice\n");
    }
}

void Jointuserlogin(int temp)
{
    bool flag;
    struct Jointuser u;
    printf("Joint User ID: ");
    scanf("%d", &u.uid);
    printf("Enter password: ");
    scanf("%s", u.pwd);
    write(temp, &choice, sizeof(int));
    write(temp, &u, sizeof(u));
    read(temp, &flag, sizeof(flag));
    if (flag)
        printf("Login succesful\n");
    else
    {
        int choice;
        printf("Invalid Login\n");
        printf("Press the corresponding number\n");
        printf("1. Single user Login\n");
        printf("2. Joint User Login\n");
        printf("3. Admin Login\n");
        scanf("%d", &choice);
        if (choice == 1)
            Singleuserlogin(temp);

        else if (choice == 2)
            Jointuserlogin(temp);

        else if (choice == 3)
            Adminlogin(temp);

        else
            printf("Invalid Choice\n");
    }
}

void Adminlogin(int temp)
{
    bool flag;
    struct Admin u;
    printf("Admin ID: ");
    scanf("%d", &u.id);
    curr_user = u.id;
    printf("Enter password: ");
    scanf("%s", u.pwd);
    write(temp, &choice, sizeof(int));
    write(temp, &u, sizeof(u));
    read(temp, &flag, sizeof(flag));

    if (flag)
        printf("Successfully Logged in\n");
    else
    {
        int choice;
        printf("Invalid Login\n");
        printf("Press the corresponding number\n");
        printf("1. Single user Login\n");
        printf("2. Joint User Login\n");
        printf("3. Admin Login\n");
        scanf("%d", &choice);
        if (choice == 1)
            Singleuserlogin(temp);

        else if (choice == 2)
            Jointuserlogin(temp);

        else if (choice == 3)
            Adminlogin(temp);

        else
            printf("Invalid Choice\n");
    }
}

void Deposit(int temp)
{
    float amount;
    int select = 1;
    bool flag;

    printf("Please enter the amount to be deposited: ");
    scanf("%f", &amount);
    write(temp, &select, sizeof(int));
    write(temp, &amount, sizeof(float));
    read(temp, &flag, sizeof(flag));

    if (flag)
        printf("Amount Successfully Deposited\n");
    else
        printf("Money couldn't be deposited\n");
    Menu(temp);
}

void JointDeposit(int temp)
{
    float amount;
    int select = 1;
    bool flag;
    int key, semid;
    key = ftok(".", 'a');
    struct sembuf buf = {0, -1, 0 | SEM_UNDO};
    semid = semget(key, 1, 0);
    semop(semid, &buf, 1);
    printf("Please enter the amount to be deposited: ");
    scanf("%f", &amount);
    write(temp, &select, sizeof(int));
    write(temp, &amount, sizeof(float));
    read(temp, &flag, sizeof(flag));
    if (flag)
        printf("Amount Successfully Deposited\n");
    else
        printf("Money couldn't be deposited\n");

    buf.sem_op = 1;
    semop(semid, &buf, 1);
    Menu(temp);
}

void Withdraw(int temp)
{
    float amount;
    int select = 2;
    bool flag;
    printf("Please enter the amount to be withdrawn: ");
    scanf("%f", &amount);
    write(temp, &select, sizeof(int));
    write(temp, &amount, sizeof(float));
    read(temp, &flag, sizeof(flag));

    if (flag)
        printf("Amount Successfully Withdrawn\n");
    else
        printf("Money couldn't be withdrawn\n");
    Menu(temp);
}

void Jointwithdraw(int temp)
{
    float amount;
    int select = 2;
    bool flag;
    int key, semid;
    key = ftok(".", 'a');
    struct sembuf buf = {0, -1, 0 | SEM_UNDO};
    semid = semget(key, 1, 0);
    semop(semid, &buf, 1);
    printf("Please enter the amount to be withdrawn: ");
    scanf("%f", &amount);
    write(temp, &select, sizeof(int));
    write(temp, &amount, sizeof(float));
    read(temp, &flag, sizeof(flag));
    if (flag)
        printf("Amount Successfully Withdrawn\n");
    else
        printf("Money couldn't be withdrawn\n");
    buf.sem_op = 1;
    semop(semid, &buf, 1);
    Menu(temp);
}

void Enquirebalance(int temp)
{
    float amount;
    int select = 3;
    write(temp, &select, sizeof(int));
    read(temp, &amount, sizeof(amount));
    printf("Available Balance: %0.2f\n", amount);
    Menu(temp);
}

void Changepwd(int temp)
{
    int select = 4;
    char new_pwd[20];
    bool flag;
    printf("Enter the new password[maximum 20 characters]: ");
    scanf("%s", new_pwd);
    write(temp, &select, sizeof(int));
    write(temp, new_pwd, sizeof(new_pwd));
    read(temp, &flag, sizeof(flag));
    if (flag)
        printf("Successfully Changed password\n");
    else
        printf("Couldn't change the password\n");
    Menu(temp);
}

void Accountdetails(int temp)
{
    int select = 5;
    write(temp, &select, sizeof(int));
    if (choice == 1)
    {
        struct Singleuser u;
        read(temp, &u, sizeof(struct Singleuser));

        printf("User ID : %d\n", u.uid);
        printf("Name : %s\n", u.uname);
        printf("Account Number : %d\n", u.accountNo);
        printf("Balance : Rs.%0.2f\n", u.balance);
        printf("Status : %s\n\n", u.status);
    }
    else if (choice == 2)
    {
        struct Jointuser u;
        read(temp, &u, sizeof(struct Jointuser));
        printf("User ID : %d\n", u.uid);
        printf("Main Holder's Name : %s\n", u.uname1);
        printf("Second Account Holder's Name: %s\n", u.uname2);
        printf("Account Number : %d\n", u.accountNo);
        printf("Balance : Rs.%0.2f\n", u.balance);
        printf("Status : %s\n\n", u.status);
    }
    Menu(temp);
}

void NewAccount(int temp)
{
    int select = 1;
    int AccountType;
    bool flag;
    write(temp, &select, sizeof(int));
    printf("1. Normal Account\n");
    printf("2. Joint Account\n");
    printf("Enter the Account Type");
    scanf("%d", &AccountType);
    write(temp, &AccountType, sizeof(int));

    if (AccountType == 1)
    {
        struct Singleuser newuser;
        printf("Name of the account holder : ");
        scanf(" %[^\n]", newuser.uname);
        printf("Password(max 20 characters) : ");
        scanf("%s", newuser.pwd);
        printf("Basic Deposit : Rs.");
        scanf("%f", &newuser.balance);
        write(temp, &newuser, sizeof(struct Singleuser));
    }

    if (AccountType == 2)
    {
        struct Jointuser newuser;
        printf("Name of the primary account holder : ");
        scanf(" %[^\n]", newuser.uname1);
        printf("Name of the secondary account holder : ");
        scanf(" %[^\n]", newuser.uname2);
        printf("Password(max 20 characters) : ");
        scanf("%s", newuser.pwd);
        printf("Basic Deposit : Rs.");
        scanf("%f", &newuser.balance);
        write(temp, &newuser, sizeof(struct Jointuser));
    }
    read(temp, &flag, sizeof(flag)); // from the server
    if (flag)
    {
        printf("Succesfully added the account\n");
    }
    else
    {
        printf("Couldn't add the new account");
    }
    Menu(temp);
}

void Delete(int temp)
{
    int select = 2;
    int AccountType, uid;
    bool flag;
    write(temp, &select, sizeof(int));
    printf("1. Normal Account\n");
    printf("2. Joint Account\n");
    printf("Enter the Account Type");
    scanf("%d", &AccountType);
    write(temp, &AccountType, sizeof(int));
    printf("Enter User ID to be deleted: ");
    scanf("%d", &uid);
    write(temp, &uid, sizeof(int));
    read(temp, &flag, sizeof(flag));
    if (flag)
    {
        printf("Succesfully Deleted the account\n");
    }
    else
    {
        printf("Couldn't delete the account");
    }
    Menu(temp);
}

void Modify(int temp)
{
    int select = 3;
    int AccountType;
    bool flag;
    write(temp, &select, sizeof(int));
    printf("1. Normal Account\n");
    printf("2. Joint Account\n");
    printf("Enter the Account Type");
    scanf("%d", &AccountType);
    write(temp, &AccountType, sizeof(int));

    if (AccountType == 1)
    {
        struct Singleuser u;
        write(1, "User ID : ", sizeof("User ID : "));
        scanf("%d", &u.uid);
        write(1, "Account Number : ", sizeof("Account Number : "));
        scanf("%d", &u.accountNo);
        write(1, "New Name of the account holder : ", sizeof("New Name of the account holder : "));
        scanf(" %[^\n]", u.uname);
        write(1, "New pwd(max 10 characters) : ", sizeof("New pwd(max 10 characters) : "));
        scanf("%s", u.pwd);
        write(1, "New Balance : ", sizeof("New Balance : "));
        scanf("%f", &u.balance);
        write(temp, &u, sizeof(struct Singleuser));
    }

    if (AccountType == 2)
    {
        struct Jointuser u;
        write(1, "User ID : ", sizeof("User ID : "));
        scanf("%d", &u.uid);
        write(1, "Account Number : ", sizeof("Account Number : "));
        scanf("%d", &u.accountNo);
        write(1, "New Name of the primary account holder : ", sizeof("New Name of the primary account holder : "));
        scanf(" %[^\n]", u.uname1);
        write(1, "New Name of the other account holder : ", sizeof("New Name of the other account holder : "));
        scanf(" %[^\n]", u.uname2);
        write(1, "New password(max 20 characters) : ", sizeof("New password(max 20 characters) : "));
        scanf("%s", u.pwd);
        write(1, "New Balance : ", sizeof("New Balance : "));
        scanf("%f", &u.balance);
        write(temp, &u, sizeof(struct Jointuser));
    }

    read(temp, &flag, sizeof(flag)); // from the server

    if (!flag)
    {
        write(1, "Error modifying\n", sizeof("Error modifying\n"));
    }
    else
    {
        write(1, "Succesfully modified\n", sizeof("Succesfully modified"));
    }
    Menu(temp);
    return;
}

void FindAccount(int temp)
{
    int select = 4;
    int AccountType, len;
    bool flag;

    write(temp, &select, sizeof(int));
    printf("1. Normal Account\n");
    printf("2. Joint Account\n");
    printf("Enter the Account Type");
    scanf("%d", &AccountType);
    write(temp, &AccountType, sizeof(int));

    if (AccountType == 1)
    {
        struct Singleuser u;
        int uid;
        printf("User ID : ");
        scanf("%d", &uid);
        write(temp, &uid, sizeof(int));
        len = read(temp, &u, sizeof(struct Singleuser));
        if (len == 0)
        {
            printf("Error in userid");
        }
        else
        {
            printf("User ID : %d\n", u.uid);
            printf("Name : %s\n", u.uname);
            printf("Account Number : %d\n", u.accountNo);
            printf("Balance : Rs.%0.2f\n", u.balance);
            printf("Status : %s\n\n", u.status);
        }
    }

    if (AccountType == 2)
    {
        struct Jointuser u;
        int uid;
        printf("User ID : ");
        scanf("%d", &uid);
        write(temp, &uid, sizeof(int));

        len = read(temp, &u, sizeof(struct Jointuser));
        if (len == 0)
        {
            printf("Please check the user id");
        }
        else
        {
            printf("User ID : %d\n", u.uid);
            printf("Main Account Holder's Name : %s\n", u.uname1);
            printf("Other Account Holder's Name : %s\n", u.uname2);
            printf("Account Number : %d\n", u.accountNo);
            printf("Balance : Rs.%0.2f\n", u.balance);
            printf("Status : %s\n\n", u.status);
        }
    }
    Menu(temp);
}

void Menu(int temp)
{
    int option;

    if (choice == 1)
    {
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Enquire Balance\n");
        printf("4. Change pwd\n");
        printf("5. View Details\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        if (option == 1)
            Deposit(temp);
        

        else if (option == 2)
            Withdraw(temp);
        

        else if (option == 3)
            Enquirebalance(temp);
        

        else if (option == 4)
            Changepwd(temp);
        

        else if (option == 5)
            Accountdetails(temp);
        

        else if (option == 6)
            write(temp, &option, sizeof(int));
            
        else
            Menu(temp);
    }

    else if (choice == 2)
    {
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Enquire Balance\n");
        printf("4. Change pwd\n");
        printf("5. View Details\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        if (option == 1)
            JointDeposit(temp);
        

        else if (option == 2)
            Jointwithdraw(temp);
        

        else if (option == 3)
            Enquirebalance(temp);
        

        else if (option == 4)
            Changepwd(temp);
        

        else if (option == 5)
            Accountdetails(temp);
        

        else if (option == 6)
            write(temp, &option, sizeof(int));
        else
            Menu(temp);
    }

    else if (choice == 3)
    {
        printf("1. Add User\n");
        printf("2. Delete user\n");
        printf("3. Modify user\n");
        printf("4. Find user\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        if (option == 1)
            NewAccount(temp);
        

        else if (option == 2)
            Delete(temp);
        

        else if (option == 3)
            Modify(temp);
        

        else if (option == 4)
            FindAccount(temp);
        

        else if (option == 5)
        {
            write(temp, &choice, sizeof(int));
            exit(0);
        }

        else
        {
            printf("Invalid Choice!!\n");
            Menu(temp);
        }
    }
}

int main()
{
    struct sockaddr_in server;
    int temp, msgLength;
    char buff[50];
    char flag;
    temp = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server.sin_port = htons(5710);
    connect(temp, (struct sockaddr *)&server, sizeof(server));
    while (1)
    {
        
        printf("1. Normal user Login\n");
        printf("2. Joint User Login\n");
        printf("3. Admin Login\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1)
        {
            Singleuserlogin(temp);
            break;
        }

        else if (choice == 2)
        {
            Jointuserlogin(temp);
            break;
        }

        else if (choice == 3)
        {
            Adminlogin(temp);
            break;
        }
        else
        {
            printf("Invalid\n");
        }
    }

    Menu(temp);

    close(temp);

    return 0;
}
