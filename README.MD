# Library Management App using C++ & MySQL

## Prior Setup

1. Start the MySQL service:

   ```bash
   brew services start mysql

   ```
2. Log into MySQL:

    mysql -u root -p
    Enter password

3. Create the database:

    CREATE DATABASE mydb;
    USE mydb;

4. Create the necessary tables:

    CREATE TABLE lib (
        Name VARCHAR(255) NOT NULL,
        Quantity INT NOT NULL
    );

    CREATE TABLE student (
        Id VARCHAR(255) NOT NULL
    );

5. Compile the C++ program:

    g++ -o main main.cpp -I/opt/homebrew/opt/mysql/include -L/opt/homebrew/opt/mysql/lib -lmysqlclient

6. Run the program:

    ./main
