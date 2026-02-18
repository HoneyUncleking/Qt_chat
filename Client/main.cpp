#include <QApplication>
#include <QWidget>
#include "ClientSocket.h"
#include "pages/LoginPage.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Create main window
    MainWindow w;
    w.show();
    
    return app.exec();
}