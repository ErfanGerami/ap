#include <iostream>
#include <string>
using namespace std;
const int MAX = 100;

template<typename T,typename F>
int myfind(T* begin,T* end,F f) {
    for (T* itr = begin; itr != end; itr++) {
        if (f(*itr) == 1) {
            return itr - begin;
        }
    }
    return -1;

}

class AbstractUser {
public:
    AbstractUser(const string& username, const string& password)
        : username(username), password(password) {}

    virtual ~AbstractUser() = default;

    virtual void displayRole() const = 0;
    virtual bool hasAccessToSee(AbstractUser*) const = 0;

    string getUsername() const {
        return username;
    }
    bool isPasswordCorrect(const string& pass) {
        return (pass == password);
    }

protected:
    string username;
    string password;
};

class Student : public AbstractUser {
public:
    Student(const string& username, const string& password, int grade)
        : AbstractUser(username, password), grade(grade) {}

    void displayRole() const override {
        cout << "Student: " << username << ", Grade: " << grade << endl;
    }
    bool hasAccessToSee(AbstractUser* user) const override {
        return false;//doesnt have access for any type
    }

private:
    int grade;
};

class Staff : public AbstractUser {
public:
    Staff(const string& username, const string& password, int salary)
        : AbstractUser(username, password), salary(salary) {}

    void displayRole() const override {
        cout << "Staff: " << username << ", Salary: " << salary << endl;
    }
    bool hasAccessToSee(AbstractUser* user) const override {
        if (dynamic_cast<Student *>(user) != nullptr) {
            return true;
        }

        return false;
    }

private:
    int salary;
};

class Admin : public Staff {
public:
    Admin(const string& username, const string& password, int salary)
        : Staff(username, password, salary) {}

    void displayRole() const override {

        cout << "Admin ";
        Staff::displayRole();
    }
    bool hasAccessToSee(AbstractUser* user) const override {
        if (dynamic_cast<Admin *>(user) == nullptr) {
            return true;
        }
        return false;
    }
};

AbstractUser* G_current_user;
AbstractUser* G_users[100];
int G_user_cnt;

void print() {
    if (!G_current_user) {
        return;
    }
    for (AbstractUser* user : G_users) {
        if (G_current_user->hasAccessToSee(user) == true) {
            if (user == nullptr) {
                return;
            }
            user->displayRole();
        }
    }
}

void addStudent(string username, string password, int grade) {
    int index = myfind(G_users, G_users + G_user_cnt, [username](const AbstractUser *us){ if (username == us->getUsername()){return 1;} else {return 0;}});
    if (index != -1) {
        return;
    }
    if (dynamic_cast<Admin*>(G_current_user) == nullptr){
        cout << "permision denied"<<endl;
        return;
    }
    Student* st = new Student(username, password, grade);
    G_users[G_user_cnt ++] = st;
}

int main() {
    Student s("erfan", "pass1", 12);
    Student s2("rasool", "pass2", 12);
    Staff st("alireza", "staffpass", 1000);
    Staff st2("kiyan", "staffpass", 1000);
    Admin a("yalda", "adminpass",1200);
    G_user_cnt = 5;
    G_users[0] = &s;
    G_users[1] = &s2;
    G_users[2] = &st;
    G_users[3] = &st2;
    G_users[4] = &a;

    while (1) {
        string username, password;
        cout << "username: ";
        cin >> username;
        cout << "password:";
        cin >> password;
        int index = myfind(G_users, G_users + G_user_cnt, [username](const AbstractUser *us){ if (username == us->getUsername()){return 1;} else {return 0;}});
        if (index!=-1) {
            G_current_user = G_users[index];
            break;
        }
    }
    int choice;
    while (1) {
        cout << "1.print" << endl;
        cout<<"2.add student"<<endl;
        cin >> choice;
        if (choice == 1) {
            print();
        }
        else {
            string user, pass;
            int grade;
            cin >> user >> pass >> grade;
            addStudent(user, pass, grade);
        }

    }

    return 0;
}
