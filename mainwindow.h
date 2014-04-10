#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void savefilenoerror();
public slots:
    void openfile();
    void newfile();
    void changecata();
    void savefile();
    void pushfile();
    void openblog();
private:
    Ui::MainWindow *ui;
    std::string path;
    bool saved;
};

#endif // MAINWINDOW_H
