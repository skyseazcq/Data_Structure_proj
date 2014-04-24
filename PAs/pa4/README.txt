To successfully compile P4, we do the following:

1. create a ui_mainwindow.h from mainwindow.ui, by running:
	uic mainwindow.ui -o ui_mainwindow.h

2. generate .pro file, by running:
	qmake -project

3. modify .pro file:
   a.  add the following two lines in the beginning:
	
	QMAKE_CXXFLAGS += -std=c++11
	QT += core gui widgets	

   b.  delete the following file from SOURCES:
	bogtest.cpp	

4. generate Makefile, by running:
	qmake

5. compile the project, by running:
	make

And finally we can see a binary file to run.
Note, when we run the program, we might see the following information: 

Qt at-spi: error getting the accessibility dbus address:  "The name org.a11y.Bus was not provided by any .service files" 
Accessibility DBus not found. Falling back to session bus.

This message can be ignored.
