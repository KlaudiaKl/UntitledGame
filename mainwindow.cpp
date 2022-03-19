#include <iostream>

#include "mainwindow.h"
#include "window.h"

using namespace std;

MainWindow::MainWindow()
{  

    setWindowTitle(tr("Gra"));
    setCentralWidget(new Window(this));
}





