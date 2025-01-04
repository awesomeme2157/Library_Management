#include <iostream>
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>
#include <unistd.h> // Use this header for macOS
#include <iomanip>  // Include this for setw (set-width)
#include <sstream>
using namespace std;

const char *HOST = "localhost";
const char *USER = "root";
const char *PW = ""; // Enter your password
const char *DB = "mydb";

class Student
{
private:
    string Id;

public:
    Student() : Id("") {}

    void setId(string id)
    {
        Id = id;
    }

    string getId()
    {
        return Id;
    }
};

class Library
{
private:
    string Name;
    int Quantity;

public:
    Library() : Name(""), Quantity(0) {}

    void setName(string name)
    {
        Name = name;
    }

    void setQuantity(int quantity)
    {
        Quantity = quantity;
    }

    string getName()
    {
        return Name;
    }

    int getQuantity()
    {
        return Quantity;
    }
};

void displayIssuedBooks(MYSQL *conn)
{
    system("clear");
    cout << "Issued Books" << endl;
    cout << "************" << endl;

    // string query = "SELECT * FROM issued_books";
    string query = "SELECT Id, Student_id, Book_name, DATE(issue_date) AS Issue_date FROM issued_books";
    if (mysql_query(conn, query.c_str()))
    {
        cout << "Error: " << mysql_error(conn) << endl;
    }
    else
    {
        MYSQL_RES *res;
        res = mysql_store_result(conn);
        if (res)
        {
            MYSQL_ROW row;
            MYSQL_FIELD *field;
            int num = mysql_num_fields(res);

            // Print column headers
            for (int i = 0; (field = mysql_fetch_field(res)); i++)
            {
                cout << left << setw(15) << field->name; // Set column width to 15
            }
            cout << endl;

            // Print rows
            while ((row = mysql_fetch_row(res)))
            {
                for (int i = 0; i < num; i++)
                {
                    cout << left << setw(15) << (row[i] ? row[i] : "NULL"); // Set column width to 15
                }
                cout << endl;
            }
            mysql_free_result(res);
        }
    }
    usleep(5000000); // Sleep for 5 seconds
}

void admin(MYSQL *conn, Library l, Student s)
{
    bool closed = false;
    while (!closed)
    {
        int choice;
        cout << "1. Add Book." << endl;
        cout << "2. Add Student." << endl;
        cout << "3. Display Issued Books." << endl;
        cout << "0. Exit." << endl;
        cout << "Enter Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            system("clear");
            string name;
            int quantity;

            cout << "Book Name: ";
            cin >> name;
            l.setName(name);

            cout << "Enter Quantity: ";
            cin >> quantity;
            l.setQuantity(quantity);

            int Iq = l.getQuantity();
            stringstream ss;
            ss << Iq;
            string Sq = ss.str();

            string book = "INSERT INTO lib (Name,Quantity) VALUES('" + l.getName() + "', '" + Sq + "') ";
            if (mysql_query(conn, book.c_str()))
            {
                cout << "Error: " << mysql_error(conn) << endl;
            }
            else
            {
                cout << "Book Inserted Successfully" << endl;
            }
        }
        else if (choice == 2)
        {
            system("clear");
            string id;
            cout << "Enter Student ID: ";
            cin >> id;
            s.setId(id);

            string st = "INSERT INTO student (Id) VALUES('" + s.getId() + "')";
            if (mysql_query(conn, st.c_str()))
            {
                cout << "Error: " << mysql_error(conn) << endl;
            }
            else
            {
                cout << "Student Inserted Successfully" << endl;
            }
        }
        else if (choice == 3)
        {
            displayIssuedBooks(conn);
        }
        else if (choice == 0)
        {
            closed = true;
            cout << "System is closing" << endl;
        }
    }
    usleep(3000000); // Sleep for 3 seconds
}

void display(MYSQL *conn)
{
    system("clear");
    cout << "Available Books" << endl;
    cout << "***************" << endl;

    string disp = "SELECT * FROM lib";
    if (mysql_query(conn, disp.c_str()))
    {
        cout << "Error: " << mysql_error(conn) << endl;
    }
    else
    {
        MYSQL_RES *res;
        res = mysql_store_result(conn);
        if (res)
        {
            MYSQL_ROW row;
            MYSQL_FIELD *field;
            int num = mysql_num_fields(res);

            // Print column headers
            for (int i = 0; (field = mysql_fetch_field(res)); i++)
            {
                cout << left << setw(15) << field->name; // Set column width to 15
            }
            cout << endl;

            // Print rows
            while ((row = mysql_fetch_row(res)))
            {
                for (int i = 0; i < num; i++)
                {
                    cout << left << setw(15) << (row[i] ? row[i] : "NULL"); // Set column width to 15
                }
                cout << endl;
            }
            mysql_free_result(res);
        }
    }
    usleep(5000000); // Sleep for 5 seconds
}

int book(MYSQL *conn, string Bname)
{
    string exist = "SELECT Name, Quantity FROM lib WHERE Name = '" + Bname + "'";
    if (mysql_query(conn, exist.c_str()))
    {
        cout << "Error: " << mysql_error(conn) << endl;
    }
    else
    {
        MYSQL_RES *res;
        res = mysql_store_result(conn);
        if (res)
        {
            int num = mysql_num_fields(res);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)))
            {
                for (int i = 0; i < num; i++)
                {
                    if (Bname == row[i])
                    {
                        int quantity = atoi(row[i + 1]);
                        return quantity;
                    }
                    else
                    {
                        cout << "Book Not Found." << endl;
                    }
                }
            }
            mysql_free_result(res);
        }
    }
    return 0;
    usleep(5000000); // Sleep for 5 seconds
}

void user(MYSQL *conn, Library l, Student s)
{
    system("clear");
    display(conn);
    string Sid;
    cout << "Enter Your ID: ";
    cin >> Sid;

    string com = "SELECT * FROM student WHERE Id = '" + Sid + "'";
    if (mysql_query(conn, com.c_str()))
    {
        cout << "Error: " << mysql_error(conn) << endl;
    }
    else
    {
        MYSQL_RES *res;
        res = mysql_store_result(conn);
        if (res)
        {
            int num = mysql_num_rows(res);
            if (num == 1)
            {
                cout << "Student ID Found." << endl;
                string Bname;
                cout << "Enter Book Name: ";
                cin >> Bname;
                if (book(conn, Bname) > 0)
                {
                    int bookQuantity = book(conn, Bname) - 1;
                    stringstream ss;
                    ss << bookQuantity;
                    string Sq = ss.str();

                    string upd = "UPDATE lib SET Quantity ='" + Sq + "' WHERE Name = '" + Bname + "' ";
                    if (mysql_query(conn, upd.c_str()))
                    {
                        cout << "Error: " << mysql_error(conn) << endl;
                    }
                    else
                    {
                        cout << "Book is available. Borrowing Book...." << endl;

                        string issue = "INSERT INTO issued_books (student_id, book_name) VALUES('" + Sid + "', '" + Bname + "')";
                        if (mysql_query(conn, issue.c_str()))
                        {
                            cout << "Error: " << mysql_error(conn) << endl;
                        }
                        else
                        {
                            cout << "Book issued successfully!" << endl;
                        }
                    }
                }
                else
                {
                    cout << "Book is not Available." << endl;
                }
            }
            else if (num == 0)
            {
                cout << "This Student is Not Registered." << endl;
            }
        }
        mysql_free_result(res);
    }
}

int main()
{
    Student s;
    Library l;

    MYSQL *conn;
    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0))
    {
        cout << "Error: " << mysql_error(conn) << endl;
    }
    else
    {
        cout << "Logged In!" << endl;
    }
    usleep(3000000); // Sleep for 3 seconds
    bool exit = false;
    while (!exit)
    {
        system("clear");
        int val;
        cout << "Welcome To Library Management System" << endl;
        cout << "************************************" << endl;
        cout << "1. Administration." << endl;
        cout << "2. User." << endl;
        cout << "0. Exit." << endl;
        cout << "Enter Choice: ";
        cin >> val;

        if (val == 1)
        {
            system("clear");
            admin(conn, l, s);
        }
        else if (val == 2)
        {
            user(conn, l, s);
            usleep(5000000); // Sleep for 5 seconds
        }
        else if (val == 0)
        {
            exit = true;
            cout << "Good Luck!" << endl;
            usleep(3000000); // Sleep for 3 seconds
        }
    }
    mysql_close(conn);
    return 0;
}
