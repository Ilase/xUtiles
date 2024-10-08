//#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <QListWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QString>
#include <X11/Xlib.h>
#include <GL/glx.h>



int main(int argc, char *argv[])
{
    // QApplication a(argc, argv);
    // MainWindow w;
    // //QBoxLayout *layout = new QBoxLayout(&w);

    // w.show();
    // return a.exec();
    // Display *display = XOpenDisplay(0);
    // if(!display){
    //     std::cerr << "Error occured while open display session!" << std::endl;
    //     return 1;
    // }

    // int screen DefaultScreen(display);

    // const char *glxExtensions = glXQueryExtensionsString(display,screen);
    // if(!glxExtensions){
    //     std::cerr << "GLX Extensions is not supported" << std::endl;
    //     XCloseDisplay(display);
    //     return 1;
    // }

    // std::cout << "GLX extensions: " << glxExtensions << std::endl;

    // const unsigned char *glRenderer = glGetString(GL_RENDERER);
    // size_t glRendererSize = sizeof(glRenderer);
    // QByteArray byteArray(reinterpret_cast<const char*>(glRenderer), glRendererSize);
    // QString QglRenderer = QString::fromUtf8(byteArray);


    //const unsigned char *glVendor = glGetString(GL_VENDOR);
    //const unsigned char *glVersion = glGetString(GL_VERSION);


    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("XDrive");
    QVBoxLayout *layout = new QVBoxLayout(&window);

    QListWidget *listWidget = new QListWidget();
    //listWidget->addItem(QglRenderer);
    listWidget->addItem("Item 2");
    listWidget->addItem("Item 3");
    layout->addWidget(listWidget);

    window.setLayout(layout);
    window.show();

    return app.exec();

}
