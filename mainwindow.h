#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QObject>
#include <QWidget>

#ifdef mywidgets_EXPORTS
#define MYWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define MYWIDGETS_EXPORT Q_DECL_IMPORT
#endif

class MYWIDGETS_EXPORT MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
