#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTextEdit>
#include <QMap>
#include <QList>
#include <QPair>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QComboBox *startBox;
    QComboBox *endBox;
    QTextEdit *resultBox;

    QMap<QString, QList<QPair<QString,int>>> graph;

    void makeGraph();
    void findPath();
};

#endif