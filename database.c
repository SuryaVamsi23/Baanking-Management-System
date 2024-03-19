#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

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

int Singleuser()
{
    int fd = open("SingleUsers.dat", O_RDONLY, 0744);
    struct Singleuser u;
    lseek(fd,-sizeof(u), SEEK_END);
    read(fd, &u, sizeof(struct Singleuser));
    close(fd);
    return u.uid + 1;
}

int Jointuser()
{
    int fd = open("JointUsers.dat", O_RDONLY, SEEK_END);
    struct Jointuser u;
    lseek(fd,-sizeof(u), SEEK_END);
    read(fd, &u, sizeof(u));
    close(fd);
    return u.uid + 1;
}

int Admin()
{
    int fd = open("Admins.dat", O_RDONLY, SEEK_END);
    struct Admin u;
    lseek(fd,-sizeof(u), SEEK_END);
    read(fd, &u, sizeof(u));
    close(fd);
    return u.id + 1;
}

int main()
{
    int fd = open("Admins.dat", O_RDWR|O_CREAT, 0744);
    int Continue = 0;
    struct Admin admin;
    printf("Adding Admins\n");
    printf("Please enter the username of Admin: ");
    scanf("%s", admin.uname);
    printf("Please enter the password[max. 20 characters]: ");
    scanf("%s", admin.pwd);
    admin.id=1;
    printf("Your Admin ID is: %d\n", admin.id);
    write(fd, &admin, sizeof(admin));
    printf("Do you want to Continue?\n");
    printf("Enter 0 to quit and 1 to Continue.\n");
    scanf("%d", &Continue);
    while(Continue)
    {
        printf("Adding Admins\n");
        printf("Please enter the username of Admin: ");
        scanf("%s", admin.uname);
        printf("Please enter the password[max. 20 characters]: ");
        scanf("%s", admin.pwd);
        admin.id=Admin();
        printf("Your Admin ID is: %d\n", admin.id);
        write(fd, &admin, sizeof(admin));
        printf("Do you want to Continue?\n");
        printf("Enter 0 to quit and 1 to Continue.\n");
        scanf("%d", &Continue);
    }
    close(fd);
    printf("Adding Users\n");
    fd = open("SingleUsers.dat", O_RDWR|O_CREAT, 0744);
    Continue = 0;
    struct Singleuser u;
    printf("Please enter the username: ");
    scanf("%s", u.uname);
    printf("Please enter the password[max. 20 characters]: ");
    scanf("%s", u.pwd);
    u.uid=1;
    u.balance=1000;
    u.accountNo=u.uid;
    printf("Your User ID is: %d\n", u.uid);
    printf("Your Account no is: %d\n", u.accountNo);
	strcpy(u.status,"ACTIVE");
    write(fd, &u, sizeof(u));
    printf("Do you want to Continue?\n");
    printf("Enter 0 to quit and 1 to Continue.\n");
    scanf("%d", &Continue);
    while(Continue)
    {
        printf("Adding Users\n");
        printf("Please enter the username: ");
        scanf("%s", u.uname);
        printf("Please enter the password[max. 20 characters]: ");
        scanf("%s", u.pwd);
        u.uid=Singleuser();
        u.balance=1000;
        u.accountNo=u.uid;
        printf("Your User ID is: %d\n", u.uid);
        printf("Your Account no is: %d\n", u.accountNo);
	    strcpy(u.status,"ACTIVE");
        write(fd, &u, sizeof(u));
        printf("Do you want to Continue?\n");
        printf("Enter 0 to quit and 1 to Continue.\n");
        scanf("%d", &Continue);
    }
    close(fd);
    printf("Adding Joint users\n");
    fd = open("JointUsers.dat", O_RDWR|O_CREAT, 0744);
    Continue = 0;
    struct Jointuser User;
    printf("Please enter the main username: ");
    scanf("%s", User.uname1);
    printf("Please enter the second username: ");
    scanf("%s", User.uname2);
    printf("Please enter the password[max. 20 characters]: ");
    scanf("%s", User.pwd);
    User.uid=1;
    User.balance=1000;
    User.accountNo=User.uid;
    printf("Your User ID is: %d\n", User.uid);
    printf("Your Account no is: %d\n", User.accountNo);
	strcpy(User.status,"ACTIVE");
    write(fd, &User, sizeof(User));
    printf("Do you want to Continue?\n");
    printf("Enter 0 to quit and 1 to Continue.\n");
    scanf("%d", &Continue);
    while(Continue)
    {
        printf("Adding Joint users\n");
        printf("Please enter the main username: ");
        scanf("%s", User.uname1);
        printf("Please enter the second username: ");
        scanf("%s", User.uname2);
        printf("Please enter the password[max. 20 characters]: ");
        scanf("%s", User.pwd);
        User.uid=Jointuser();
        User.balance=1000;
        User.accountNo=User.uid;
        printf("Your User ID is: %d\n", u.uid);
        printf("Your Account no is: %d\n", u.accountNo);
	    strcpy(User.status,"ACTIVE");
        write(fd, &User, sizeof(User));
        printf("Do you want to Continue?\n");
        printf("Enter 0 to quit and 1 to Continue.\n");
        scanf("%d", &Continue);
    }
    close(fd);
    return 0;
}