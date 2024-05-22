# vtk-implementation
This repo includes tools developed by me for Befem CAE software of VTK in C++, the corresponding functions files are in test/ folder and the relative research reports are in study_material folder.
This repo has already finished functions:
1. specify the rotation center
2. fix the object to an absolute position of a window
3. extrude the 2D polygons along a vector(double T cells工字梁结构) --- done, a line --- done
4. animation
# vtk-implementation

<img src="/vtk_logo.png" alt="Alt text" title="VTK">

<p align="center">
  <a href="#key-features">Key Features</a> •
  <a href="#set-up-the-local-database">Set up the local database</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#required-library">Required library</a> •
  <a href="#reference">Reference</a> •
  <a href="#changes">Changes</a> •
  <a href="#license">License</a>
</p>



## Key Features

This repo includes tools developed by me for Befem CAE software based on VTK(Visual Tool Kits), in C++, built with Msys2 Mingw64 environment

* /doc : report of functions
* /test : test code

## Set up the local database
* download mysqlworkbench
* create a new connection, set the hostname to 127.0.0.1, port to 3306(mainly 3306 but it depends on your port), set up the password and username
* create a new schema called "db"
* run table.sql and then data.sql

## How To Use

To clone and run this application, you'll need [Git](https://git-scm.com), python3 and mySQLworkbench 8.0.31

* change the line in main.py, app.py: mysql+pymysql://username:password@localhost:port/db to what you set in the "Set up the Local database" section
From your command line:

```bash
# Clone this repository
$ git clone https://github.com/Celia-zwx/Movie-Theatre-application.git

# Go into the repository
$ cd vtk-implementation

# Go into the test folder
$ cd test/Animation/Animation

# build the project
$ cmake -B build
$ cd build
$ ninja




# Run the app
$ ./Animation.cxx



```



## Required library
* flask 
* datetime
* sqlalchemy.orm
* sqlalchemy.ext
* sqlalchemy
* mysql.connector
* pymysql

## Reference
# Study guide for vtk
https://docs.vtk.org/en/latest/getting_started/index.html

# what is CAE?
https://en.wikipedia.org/wiki/Computer-aided_engineering

# Introduction for BeFem CAE software
https://bda.pku.edu.cn/info/1003/2467.htm


## License

MIT

---

> GitHub [@Celia-zwx](https://github.com/Celia-zwx) &nbsp;&middot;&nbsp;


