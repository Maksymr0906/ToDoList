# ToDoList
ToDoList - a simple implementation of To-do list using QT framework and PostgreSQL. You can add, edit, delete, and mark tasks as completed, and all your data is stored in a PostgreSQL database for persistence.
Also you have a possibility to change the language. Available languages: Ukrainian and English.
# Preparing Your Machine
Before you can build a project you'll need to ensure that your system has the necessary tools installed.
* Visual Studio. Ensure that you have Visual Studio installed on your system, and the C++ development tools are available. \
If you don't have Visual Studio, you can download and install it from the official Visual Studio website.
* PostgreSQL shell. You need the PostgreSQL database server installed, and the PostgreSQL shell available in your PATH. \
If you haven't already installed PostgreSQL, you can download it from the official PostgreSQL website.
* Qt Extension in VS. To work with Qt in Visual Studio, you should have the Qt Visual Studio Tools extension installed. If you haven't installed this extension, you can do so by following these steps:
  * Open Visual Studio.
  * Navigate to "Extensions" > "Manage Extensions."
  * Search for "Qt Visual Studio Tools" and install it.
# Installation instructions
These instructions are based on PostgreSQL 15.4.
1. Download this repository. You can use this command:
```
git clone https://github.com/Maksymr0906/ToDoList
```
2. Open PostgreSQL shell (psql) and create a new database:
```
create database <database_name>;
```
3. Then connect to your database, using this command:
```
\c <database_name>
```
4. Then create a sequence, to generate unique id for every task (put your table name instead of table_name):
```
create sequence table_name_id_seq START 1;
```
5. After that you need to create a table. Use the following command:
```
create table table_name (
  id integer DEFAULT nextval('table_name_id_seq') PRIMARY KEY,
  task_name varchar(255),
  deadline date,
  responsible varchar(255),
  email varchar(255),
  status integer,
  is_important boolean,
  is_my_day boolean
);
```
6. Lastly, open ToDoList.cpp and change arguments for functions setHostName, setDataBaseName, setUserName and setPassword to your settings:
```
 10.    db.setHostName("localhost");
 11.    db.setDatabaseName("todolist");
 12.    db.setUserName("postgres");
 13.    db.setPassword("12345");
```
# Features
* **Task Management:** Easily create, edit, complete, and delete tasks.
* **Database Integration:** Tasks are stored in a PostgreSQL database for persistence.
* **Intuitive User Interface:** A user-friendly interface built with Qt for smooth task management.
