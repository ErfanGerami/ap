#include <iostream>
#include <string>

using namespace std;

const int MAX = 100;

template<typename T, typename F>
int myfind(T* begin, T* end, F f) {
    for (T* itr = begin; itr != end; itr++) {
        bool matched = f(*itr);
        if (matched) {
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
    bool isPasswordCorrect(const string& pass) const {
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
        return false;
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
    bool hasAccessToSee(AbstractUser*) const override {
        return true;
    }
};

AbstractUser* current_user = nullptr;
AbstractUser* users[MAX];
int user_cnt = 0;

void print() {
    if (!current_user) {
        return;
    }
    for (int i = 0; i < user_cnt; ++i) {
        if (current_user->hasAccessToSee(users[i])) {
            users[i]->displayRole();
        }
    }
}

void addStudent(string username, string password, int grade) {
    int index = myfind(users, users + user_cnt, [&](AbstractUser* h) {
        return h->getUsername() == username;
    });

    if (index != -1) {
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
    Student s("st1", "pass1", 12);
    Student s2("st2", "pass2", 12);
    Staff st("staff1", "staffpass", 1000);
    Admin a("admin1", "adminpass", 1000);
    users[user_cnt++] = &s;
    users[user_cnt++] = &s2;
    users[user_cnt++] = &st;
    users[user_cnt++] = &a;

    while (true) {
        string username, password;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        int index = myfind(users, users + user_cnt, [&](AbstractUser* t) {
            return t->getUsername() == username && t->isPasswordCorrect(password);
        });

        if (index != -1) {
            current_user = users[index];
            break;
        }
        cout << "Invalid credentials, try again." << endl;
    }

    int choice;
    while (true) {
        cout << "1. Print users\n2. Add user\n";
        cin >> choice;

        if (choice == 1) {
            print();
        } else if (choice == 2) {
            string user, pass;
            int grade;
            cout << "Enter username, password, and grade: ";
            cin >> user >> pass >> grade;
            addStudent(user, pass, grade);
        } else {
            cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}