#include <iostream>
#include <string>
using namespace std;

const int MAX = 100;

template<typename T,typename F>
int myfind(T* begin,T* end,F f) {
    for (T* itr = begin; itr != end; itr++) {
        if (f(*itr)) {
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
        return false; // دانشجوها به کسی دسترسی ندارن
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
        if (dynamic_cast<Student*>(user) != nullptr) {
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
        return true; // ادمین به همه دسترسی داره
    }
};

AbstractUser* current_user = nullptr;
AbstractUser* users[100];
int user_cnt = 0;

void print() {
    if (!current_user) {
        return;
    }
    for (int i = 0; i < user_cnt; i++) {
        if (current_user->hasAccessToSee(users[i]) || current_user == users[i]) {
            users[i]->displayRole();
        }
    }
}

void addStudent(string username, string password, int grade) {
    int index = myfind(users, users + user_cnt, [&](AbstractUser* u){
        return u->getUsername() == username;
    });
    if (index != -1) {
        cout << "User already exists!" << endl;
        return;
    }
    if (dynamic_cast<Admin*>(current_user) == nullptr) {
        cout << "Permission denied" << endl;
        return;
    }
    Student* st = new Student(username, password, grade);
    users[user_cnt++] = st;
}

int main() {
    Student s("erfan", "pass1", 12);
    Student s2("rasool", "pass2", 12);
    Staff st("alireza", "staffpass", 1000);
    Staff st2("kiyan", "staffpass", 1000);
    Admin a("yalda", "adminpass", 1200);

    user_cnt = 5;
    users[0] = &s;
    users[1] = &s2;
    users[2] = &st;
    users[3] = &st2;
    users[4] = &a;

    while (true) {
        string username, password;
        cout << "username: ";
        cin >> username;
        cout << "password: ";
        cin >> password;

        int index = myfind(users, users + user_cnt, [&](AbstractUser* u){
            return u->getUsername() == username && u->isPasswordCorrect(password);
        });

        if (index != -1) {
            current_user = users[index];
            break;
        }
        else {
            cout << "Invalid username or password. Try again." << endl;
        }
    }

    int choice;
    while (true) {
        cout << "1. Print users" << endl;
        cout << "2. Add student" << endl;
        cin >> choice;

        if (choice == 1) {
            print();
        }
        else if (choice == 2) {
            string user, pass;
            int grade;
            cout << "Enter username password grade: ";
            cin >> user >> pass >> grade;
            addStudent(user, pass, grade);
        }
    }

    return 0;
}
