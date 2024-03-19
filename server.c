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

typedef struct Singleuser
{
    int uid;
    char uname[40];
    char pwd[20];
    int accountNo;
    float balance;
    char status[20];
} Singleuser;

typedef struct Jointuser
{
    int uid;
    char uname1[40];
    char uname2[40];
    char pwd[20];
    int accountNo;
    float balance;
    char status[20];
} Jointuser;

typedef struct Admin
{
    int admin_id;
    char admin_username[40];
    char pwd[20];
} Admin;

Singleuser getSingleuser(int uid)
{
    Singleuser currentUser;
    int fd = open("SingleUsers.dat", O_RDONLY, 0744);
    struct flock flock;

    flock.l_type = F_RDLCK;
    flock.l_whence = SEEK_SET;
    flock.l_start = (uid - 1) * sizeof(currentUser);
    flock.l_len = sizeof(currentUser);
    flock.l_pid = getpid();

    fcntl(fd, F_SETLKW, &flock);

    lseek(fd, (uid - 1) * sizeof(currentUser), SEEK_SET);
    read(fd, &currentUser, sizeof(currentUser));

    flock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flock);

    close(fd);
    return currentUser;
}

Jointuser getJointuser(int uid)
{
    Jointuser currentUser;
    int fd = open("JointUsers.dat", O_RDONLY, 0744);
    struct flock flock;

    flock.l_type = F_RDLCK;
    flock.l_whence = SEEK_SET;
    flock.l_start = (uid - 1) * sizeof(currentUser);
    flock.l_len = sizeof(currentUser);
    flock.l_pid = getpid();

    fcntl(fd, F_SETLKW, &flock);

    lseek(fd, (uid - 1) * sizeof(currentUser), SEEK_SET);
    read(fd, &currentUser, sizeof(currentUser));

    flock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flock);

    close(fd);
    return currentUser;
}

Admin getAdmin(int uid)
{
    Admin currentUser;
    int fd = open("Admins.dat", O_RDONLY, 0744);
    struct flock flock;

    flock.l_type = F_RDLCK;
    flock.l_whence = SEEK_SET;
    flock.l_start = (uid - 1) * sizeof(currentUser);
    flock.l_len = sizeof(currentUser);
    flock.l_pid = getpid();
    fcntl(fd, F_SETLKW, &flock);
    lseek(fd, (uid - 1) * sizeof(currentUser), SEEK_SET);
    read(fd, &currentUser, sizeof(currentUser));
    flock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flock);
    close(fd);
    return currentUser;
}

bool CheckSingleuser(Singleuser currentUser)
{
    Singleuser temp;
    bool result;
    int fd = open("SingleUsers.dat", O_RDONLY, 0744);
    struct flock flock;

    flock.l_type = F_RDLCK;
    flock.l_whence = SEEK_SET;
    flock.l_start = (currentUser.uid - 1) * sizeof(currentUser);
    flock.l_len = sizeof(currentUser);
    flock.l_pid = getpid();
    fcntl(fd, F_SETLKW, &flock);

    lseek(fd, (currentUser.uid - 1) * sizeof(temp), SEEK_SET);
    read(fd, &temp, sizeof(temp));
    //printf("%s %s",temp.pwd,currentUser.pwd);
    if (temp.uid == currentUser.uid && !strcmp(temp.pwd, currentUser.pwd) && !strcmp(temp.status, "ACTIVE"))
        result = true;
    else
        result = false;
    flock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flock);

    close(fd);
    return result;
}

bool Checkjointuser(Jointuser currentUser)
{
    Jointuser temp;
    bool result;
    int fd = open("JointUsers.dat", O_RDONLY, 0744);
    struct flock flock;

    flock.l_type = F_RDLCK;
    flock.l_whence = SEEK_SET;
    flock.l_start = (currentUser.uid - 1) * sizeof(currentUser);
    flock.l_len = sizeof(currentUser);
    flock.l_pid = getpid();
    fcntl(fd, F_SETLKW, &flock);
    lseek(fd, (currentUser.uid - 1) * sizeof(temp), SEEK_SET);
    read(fd, &temp, sizeof(temp));
    if (temp.uid == currentUser.uid && !strcmp(temp.pwd, currentUser.pwd) && !strcmp(temp.status, "ACTIVE"))
        result = true;
    else
        result = false;

    flock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flock);

    close(fd);
    return result;
}

bool Checkadmin(struct Admin currentUser)
{
    struct Admin temp;
    bool result;
    int fd = open("Admins.dat", O_RDONLY, 0744);
    struct flock flock;
    flock.l_type = F_RDLCK;
    flock.l_whence = SEEK_SET;
    flock.l_start = (currentUser.admin_id - 1) * sizeof(currentUser);
    flock.l_len = sizeof(Admin);
    flock.l_pid = getpid();
    fcntl(fd, F_SETLKW, &flock);

    lseek(fd, (currentUser.admin_id - 1) * sizeof(Admin), SEEK_SET);
    read(fd, &temp, sizeof(Admin));

    printf("%d %d %s %s", temp.admin_id, currentUser.admin_id, temp.pwd, currentUser.pwd);
    if (temp.admin_id == currentUser.admin_id && !strcmp(temp.pwd, currentUser.pwd))
        result = true;
    else
        result = false;
    flock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flock);
    close(fd);
    return result;
}

bool Deposit(int Accounttype, int uid, float amount)
{
    if (Accounttype == 1)
    {
        int fd = open("SingleUsers.dat", O_RDWR, 0744);
        bool result;
        struct flock flock;
        flock.l_type = F_WRLCK;
        flock.l_whence = SEEK_SET;
        flock.l_start = (uid - 1) * sizeof(Singleuser);
        flock.l_len = sizeof(Singleuser);
        flock.l_pid = getpid();
        fcntl(fd, F_SETLKW, &flock);
        Singleuser currUser;
        lseek(fd, (uid - 1) * sizeof(Singleuser), SEEK_SET);
        read(fd, &currUser, sizeof(Singleuser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            currUser.balance += amount;
            lseek(fd, sizeof(Singleuser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(Singleuser));
            result = true;
        }
        else
            result = false;

        flock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &flock);

        close(fd);
        return result;
    }

    else if (Accounttype == 2)
    {
        int fd = open("JointUsers.dat", O_RDWR, 0744);
        bool result;
        struct flock flock;
        flock.l_type = F_WRLCK;
        flock.l_whence = SEEK_SET;
        flock.l_start = (uid - 1) * sizeof(Jointuser);
        flock.l_len = sizeof(Jointuser);
        flock.l_pid = getpid();
        fcntl(fd, F_SETLKW, &flock);
        printf("Amount: %f", amount);
        Jointuser currUser;
        lseek(fd, (uid - 1) * sizeof(Jointuser), SEEK_SET);
        read(fd, &currUser, sizeof(Jointuser));
        printf("%s\n", currUser.status);

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            currUser.balance += amount;
            printf("%f", currUser.balance);
            lseek(fd, sizeof(Jointuser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(Jointuser));
            result = true;
        }
        else
            result = false;

        flock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &flock);

        close(fd);
        printf("%d", result);
        return result;
    }
    return false;
}

bool Withdraw(int Accounttype, int uid, float amount)
{
    if (Accounttype == 1)
    {
        int fd = open("SingleUsers.dat", O_RDWR, 0744);
        bool result;
        struct flock flock;
        flock.l_type = F_WRLCK;
        flock.l_whence = SEEK_SET;
        flock.l_start = (uid - 1) * sizeof(Singleuser);
        flock.l_len = sizeof(Singleuser);
        flock.l_pid = getpid();
        fcntl(fd, F_SETLKW, &flock);
        Singleuser currUser;
        lseek(fd, (uid - 1) * sizeof(Singleuser), SEEK_SET);
        read(fd, &currUser, sizeof(Singleuser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            currUser.balance -= amount;
            lseek(fd, sizeof(Singleuser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(Singleuser));
            result = true;
        }
        else
            result = false;

        flock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &flock);

        close(fd);
        return result;
    }

    else if (Accounttype == 2)
    {
        int fd = open("JointUsers.dat", O_RDWR, 0744);
        bool result;
        struct flock flock;
        flock.l_type = F_WRLCK;
        flock.l_whence = SEEK_SET;
        flock.l_start = (uid - 1) * sizeof(Jointuser);
        flock.l_len = sizeof(Jointuser);
        flock.l_pid = getpid();
        fcntl(fd, F_SETLKW, &flock);

        Jointuser currUser;
        lseek(fd, (uid - 1) * sizeof(Jointuser), SEEK_SET);
        read(fd, &currUser, sizeof(Jointuser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            currUser.balance -= amount;
            lseek(fd, sizeof(Jointuser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(Jointuser));
            result = true;
        }
        else
            result = false;

        flock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &flock);

        close(fd);
        return result;
    }
    return false;
}

float Balance(int Accounttype, int uid)
{
    float result;
    if (Accounttype == 1)
    {
        int fd = open("SingleUsers.dat", O_RDWR, 0744);
        struct flock flock;
        flock.l_type = F_RDLCK;
        flock.l_whence = SEEK_SET;
        flock.l_start = (uid - 1) * sizeof(Singleuser);
        flock.l_len = sizeof(Singleuser);
        flock.l_pid = getpid();
        fcntl(fd, F_SETLKW, &flock);
        Singleuser currUser;
        lseek(fd, (uid - 1) * sizeof(Singleuser), SEEK_SET);
        read(fd, &currUser, sizeof(Singleuser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            result = currUser.balance;
        }
        else
            result = 0;

        flock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &flock);

        close(fd);
        return result;
    }

    else if (Accounttype == 2)
    {
        int fd = open("JointUsers.dat", O_RDWR, 0744);
        struct flock flock;
        flock.l_type = F_RDLCK;
        flock.l_whence = SEEK_SET;
        flock.l_start = (uid - 1) * sizeof(Jointuser);
        flock.l_len = sizeof(Jointuser);
        flock.l_pid = getpid();
        fcntl(fd, F_SETLKW, &flock);
        Jointuser currUser;
        lseek(fd, (uid - 1) * sizeof(Jointuser), SEEK_SET);
        read(fd, &currUser, sizeof(Jointuser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            result = currUser.balance;
        }
        else
            result = 0;

        flock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &flock);

        close(fd);
        return result;
    }
    return 0;
}

bool Changepwd(int Accounttype, int uid, char new_password[20])
{
    printf("Printing passworrd %s",new_password);
    if (Accounttype == 1)
    {
        int fd = open("SingleUsers.dat", O_RDWR, 0744);
        bool result;
        struct flock flock;
        flock.l_type = F_WRLCK;
        flock.l_whence = SEEK_SET;
        flock.l_start = (uid - 1) * sizeof(Singleuser);
        flock.l_len = sizeof(Singleuser);
        flock.l_pid = getpid();
        fcntl(fd, F_SETLKW, &flock);
        Singleuser currUser;
        lseek(fd, (uid - 1) * sizeof(Singleuser), SEEK_SET);
        read(fd, &currUser, sizeof(Singleuser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            strcpy(currUser.pwd, new_password);
            lseek(fd, sizeof(Singleuser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(Singleuser));
            result = true;
        }
        else
            result = false;

        flock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &flock);

        close(fd);
        return result;
    }

    else if (Accounttype == 2)
    {
        int fd = open("JointUsers.dat", O_RDWR, 0744);
        bool result;
        struct flock flock;
        flock.l_type = F_WRLCK;
        flock.l_whence = SEEK_SET;
        flock.l_start = (uid - 1) * sizeof(Jointuser);
        flock.l_len = sizeof(Jointuser);
        flock.l_pid = getpid();
        fcntl(fd, F_SETLKW, &flock);
        Jointuser currUser;
        lseek(fd, (uid - 1) * sizeof(Jointuser), SEEK_SET);
        read(fd, &currUser, sizeof(Jointuser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            strcpy(currUser.pwd, new_password);
            lseek(fd, sizeof(Jointuser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(Jointuser));
            result = true;
        }
        else
            result = false;

        flock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &flock);

        close(fd);
        return result;
    }
    return false;
}

bool AddSingleuser(Singleuser record)
{
    int fd = open("SingleUsers.dat", O_RDWR);
    bool result;
    struct flock flock;
    flock.l_type = F_WRLCK;
    flock.l_whence = SEEK_END;
    flock.l_start = (-1) * sizeof(Singleuser);
    flock.l_len = sizeof(Singleuser);
    flock.l_pid = getpid();
    fcntl(fd, F_SETLKW, &flock);

    Singleuser User;
    lseek(fd, (-1) * sizeof(User), SEEK_END);
    read(fd, &User, sizeof(User));
    record.uid = User.uid + 1;
    record.accountNo = User.accountNo + 1;
    strcpy(record.status, "ACTIVE");

    int j = write(fd, &record, sizeof(Singleuser));
    if (j != 0)
        result = true;
    else
        result = false;

    flock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flock);

    close(fd);
    return result;
}

bool AddJointuser(Jointuser record)
{
    int fd = open("JointUsers.dat", O_RDWR);
    bool result;
    struct flock flock;
    flock.l_type = F_WRLCK;
    flock.l_whence = SEEK_END;
    flock.l_start = (-1) * sizeof(Jointuser);
    flock.l_len = sizeof(Jointuser);
    flock.l_pid = getpid();
    fcntl(fd, F_SETLKW, &flock);
    Jointuser User;
    lseek(fd, (-1) * sizeof(User), SEEK_END);
    read(fd, &User, sizeof(User));
    record.uid = User.uid + 1;
    record.accountNo = User.accountNo + 1;
    strcpy(record.status, "ACTIVE");
    int j = write(fd, &record, sizeof(Jointuser));
    if (j != 0)
        result = true;
    else
        result = false;
    flock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flock);
    close(fd);
    return result;
}

bool Deleteuser(int uid)
{
    int fd = open("SingleUsers.dat", O_RDWR);
    bool result;
    struct flock flock;
    flock.l_type = F_WRLCK;
    flock.l_whence = SEEK_END;
    flock.l_start = (uid - 1) * sizeof(Singleuser);
    flock.l_len = sizeof(Singleuser);
    flock.l_pid = getpid();
    fcntl(fd, F_SETLKW, &flock);
    getchar();
    Singleuser User;
    lseek(fd, (uid - 1) * sizeof(User), SEEK_END);
    read(fd, &User, sizeof(User));
    printf("%s\n", User.status);
    if (!strcmp(User.status, "ACTIVE"))
    {
        strcpy(User.status, "CLOSED");
        User.balance = 0;
        lseek(fd, (-1) * sizeof(Singleuser), SEEK_CUR);
        int j = write(fd, &User, sizeof(Singleuser));
        printf("%d", j);
        if (j != 0)
            result = true;
        else
            result = false;
    }

    flock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flock);
    close(fd);
    return result;
}

bool DeleteJointuser(int uid)
{
    int fd = open("JointUsers.dat", O_RDWR);
    bool result;
    struct flock flock;
    flock.l_type = F_WRLCK;
    flock.l_whence = SEEK_END;
    flock.l_start = (uid - 1) * sizeof(Jointuser);
    flock.l_len = sizeof(Jointuser);
    flock.l_pid = getpid();
    fcntl(fd, F_SETLKW, &flock);
    Jointuser User;
    lseek(fd, (uid - 1) * sizeof(User), SEEK_END);
    read(fd, &User, sizeof(User));

    if (!strcmp(User.status, "ACTIVE"))
    {
        strcpy(User.status, "CLOSED");
        User.balance = 0;
        lseek(fd, (-1) * sizeof(Jointuser), SEEK_CUR);
        int j = write(fd, &User, sizeof(Jointuser));
        if (j != 0)
            result = true;
        else
            result = false;
    }
    flock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flock);
    close(fd);
    return result;
}

bool ModifySingleuser(Singleuser modUser)
{
    int fd = open("SingleUsers.dat", O_RDWR, 0744);
    bool result = false;
    struct flock flock;
    flock.l_type = F_WRLCK;
    flock.l_whence = SEEK_SET;
    flock.l_start = (modUser.uid - 1) * sizeof(Singleuser);
    flock.l_len = sizeof(Singleuser);
    flock.l_pid = getpid();
    fcntl(fd, F_SETLKW, &flock);
    Singleuser currUser;
    lseek(fd, (modUser.uid - 1) * sizeof(Singleuser), SEEK_SET);
    read(fd, &currUser, sizeof(Singleuser));

    if (!strcmp(currUser.status, "ACTIVE") && (modUser.accountNo == currUser.accountNo))
    {
        strcpy(modUser.status, "ACTIVE");
        lseek(fd, (-1) * sizeof(Singleuser), SEEK_CUR);
        int j = write(fd, &modUser, sizeof(Singleuser));
        if (j != 0)
            result = true;
        else
            result = false;
    }

    flock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flock);
    close(fd);
    return result;
}

bool ModifyJointuser(Jointuser modUser)
{
    int fd = open("JointUsers.dat", O_RDWR, 0744);
    bool result = false;
    struct flock flock;
    flock.l_type = F_WRLCK;
    flock.l_whence = SEEK_SET;
    flock.l_start = (modUser.uid - 1) * sizeof(Jointuser);
    flock.l_len = sizeof(Jointuser);
    flock.l_pid = getpid();
    fcntl(fd, F_SETLKW, &flock);
    Singleuser currUser;
    lseek(fd, (modUser.uid - 1) * sizeof(Jointuser), SEEK_SET);
    read(fd, &currUser, sizeof(Jointuser));
    if (!strcmp(currUser.status, "ACTIVE") && (modUser.accountNo == currUser.accountNo))
    {
        strcpy(modUser.status, "ACTIVE");
        lseek(fd, (-1) * sizeof(Jointuser), SEEK_CUR);
        int j = write(fd, &modUser, sizeof(Jointuser));
        if (j != 0)
            result = true;
        else
            result = false;
    }
    flock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &flock);
    close(fd);
    return result;
}

void Server1(int nsd)
{
    int msg, select, type, choice, Accounttype, Currentuser;
    bool result;
    while (1)
    {
        read(nsd, &choice, sizeof(choice));
        printf("Choice: %d\n", choice);

        if (choice == 1)
        {
            struct Singleuser currentUser;
            Accounttype = 1;
            msg = read(nsd, &currentUser, sizeof(currentUser));
            printf("User ID: %d\n", currentUser.uid);
            printf("Password: %s\n", currentUser.pwd);
            result = CheckSingleuser(currentUser);
            write(nsd, &result, sizeof(result));
        }
        else if (choice == 2)
        {
            struct Jointuser currentUser;
            Accounttype = 2;
            msg = read(nsd, &currentUser, sizeof(currentUser));
            printf("User ID: %d\n", currentUser.uid);
            printf("Password: %s\n", currentUser.pwd);
            result = Checkjointuser(currentUser);
            write(nsd, &result, sizeof(result));
        }
        else if (choice == 3)
        {
            struct Admin currentUser2;
            Accounttype = 3;
            msg = read(nsd, &currentUser2, sizeof(currentUser2));
            printf("User ID: %d\n", currentUser2.admin_id);
            printf("Password: %s\n", currentUser2.pwd);
            result = Checkadmin(currentUser2);
            write(nsd, &result, sizeof(result));
        }
        else
        {
            result = false;
            write(nsd, &result, sizeof(result));
        }
        if (result)
            break;
    }

    while (1)
    {
        read(nsd, &select, sizeof(int));
        if (choice == 1 || choice == 2)
        {
            printf("Entering here\n");
            if (select == 1)
            {
                float amount;
                read(nsd, &amount, sizeof(float));
                result = Deposit(Accounttype, Currentuser, amount);
                write(nsd, &result, sizeof(result));
            }
            else if (select == 2)
            {
                float amount;
                read(nsd, &amount, sizeof(float));
                result = Withdraw(Accounttype, Currentuser, amount);
                write(nsd, &result, sizeof(result));
            }
            else if (select == 3)
            {
                float amount;
                amount = Balance(Accounttype, Currentuser);
                write(nsd, &amount, sizeof(float));
            }
            else if (select == 4)
            {
                char pwd[20];
                read(nsd, &pwd, sizeof(pwd));
                printf("Recieved password %s",pwd);
                result = Changepwd(type, Currentuser, pwd);
                write(nsd, &result, sizeof(result));
            }
            else if (select == 5)
            {
                if (choice == 1)
                {
                    Singleuser user1 = getSingleuser(Currentuser);
                    write(nsd, &user1, sizeof(Singleuser));
                }
                else if (choice == 2)
                {
                    Jointuser user2 = getJointuser(Currentuser);
                    write(nsd, &user2, sizeof(Jointuser));
                }
            }
            else if (select == 6)
                break;
        }
        else if (choice == 3)
        {
            read(nsd, &type, sizeof(int));
            if (select == 1)
            {
                if (type == 1)
                {
                    struct Singleuser Newuser;
                    read(nsd, &Newuser, sizeof(Singleuser));
                    result = AddSingleuser(Newuser);
                    write(nsd, &result, sizeof(result));
                }
                else if (type == 2)
                {
                    struct Jointuser Newuser;
                    read(nsd, &Newuser, sizeof(Jointuser));
                    result = AddJointuser(Newuser);
                    write(nsd, &result, sizeof(result));
                }
            }
            else if (select == 2)
            {
                if (type == 1)
                {
                    int deleteUserid;
                    read(nsd, &deleteUserid, sizeof(int));
                    printf("User ID: %d\n", deleteUserid);
                    result = Deleteuser(deleteUserid);
                    write(nsd, &result, sizeof(result));
                }
                else if (type == 2)
                {
                    int deleteUserid;
                    read(nsd, &deleteUserid, sizeof(int));
                    result = DeleteJointuser(deleteUserid);
                    write(nsd, &result, sizeof(result));
                }
            }
            else if (select == 3)
            {
                if (type == 1)
                {
                    struct Singleuser ModifyUser;
                    read(nsd, &ModifyUser, sizeof(Singleuser));
                    result = ModifySingleuser(ModifyUser);
                    write(nsd, &result, sizeof(result));
                }
                else if (type == 2)
                {
                    struct Jointuser ModifyUser;
                    read(nsd, &ModifyUser, sizeof(Jointuser));
                    result = ModifyJointuser(ModifyUser);
                    write(nsd, &result, sizeof(result));
                }
            }
            else if (select == 4)
            {
                if (type == 1)
                {
                    struct Singleuser user;
                    int uid;
                    read(nsd, &uid, sizeof(int));
                    user = getSingleuser(uid);
                    write(nsd, &user, sizeof(Singleuser));
                }
                else if (type == 2)
                {
                    struct Jointuser user;
                    int uid;
                    read(nsd, &uid, sizeof(int));
                    user = getJointuser(uid);
                    write(nsd, &user, sizeof(Jointuser));
                }
            }
            else if (select == 5)
                break;

        }
    }
    close(nsd);
    return;
}

void *connections(void *nsd)
{
    int Nsd = *(int *)nsd;
    Server1(Nsd);
}

int main()
{
    struct sockaddr_in server, client;
    int sd, nsd, Clienttemp;
    pthread_t threads;
    bool result;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5710);
    bind(sd, (struct sockaddr *)&server, sizeof(server));
    listen(sd, 5);
    write(1, "Waiting for the client\n", sizeof("Waiting for the client\n"));
    while (1)
    {
        Clienttemp = sizeof(client);
        nsd = accept(sd, (struct sockaddr *)&client, &Clienttemp);

        write(1, "Connected to the client\n", sizeof("Connected to the client\n"));
        if (pthread_create(&threads, NULL, connections, (void *)&nsd) < 0)
        {
            perror("Cannot create thread");
            return 1;
        }
    
    }
    pthread_exit(NULL);
    close(sd);
    return 0;
}