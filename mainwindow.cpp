#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <boost/filesystem.hpp>
#include <set>
#include <fstream>
#include <sstream>
#include <string>
#include <QString>
#include <unistd.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time.hpp>
#include <QDesktopServices>
#include <QFontComboBox>
#include <QFont>
#include <QDragEnterEvent>

using namespace boost::filesystem;
using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    saved(0),
    ui(new Ui::MainWindow)
{
    ::system("cd ~ && pwd >> /tmp/filepath");
    ifstream pa("/tmp/filepath");
    pa >> path;
    path = path + "/mysite/source/_posts";
    set<string> vec;
    directory_iterator end;                                                               //迭代器返回的对象是directory_entry
    for(directory_iterator pos(path); pos != end; pos++)
    {
        ifstream in(pos->path().c_str());
        string tmp;
        getline(in, tmp);
        getline(in, tmp);
        in >> tmp;
        tmp.clear();
        in >> tmp;
        vec.insert(tmp);
        in.close();
    }
    ui->setupUi(this);
    ui->lineEdit_2->setVisible(0);
    for(set<string>::iterator i = vec.begin(); i != vec.end(); i++)
    {
        ui->comboBox->addItem((*i).c_str());
    }
    ui->comboBox->addItem("新建类别");
    connect((ui->action_2), SIGNAL(triggered()), this, SLOT(openfile()));
    connect((ui->action_3), SIGNAL(triggered()), this, SLOT(pushfile()));
    connect((ui->comboBox), SIGNAL(currentIndexChanged(int)), this, SLOT(changecata()));
    connect((ui->Baocun), SIGNAL(triggered()), this, SLOT(savefile()));
    connect((ui->action), SIGNAL(triggered()), this, SLOT(newfile()));
    connect((ui->pushButton), SIGNAL(clicked()), this, SLOT(newfile()));
    connect((ui->pushButton_2), SIGNAL(clicked()), this, SLOT(openfile()));
    connect((ui->pushButton_3), SIGNAL(clicked()), this, SLOT(savefile()));
    connect((ui->pushButton_4), SIGNAL(clicked()), this, SLOT(pushfile()));
    connect((ui->pushButton_5), SIGNAL(clicked()), this, SLOT(openblog()));
    changecata();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::openfile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open paper"), this->path.c_str(), tr("text Files (*.md)"));
    if(path.length() == 0)
    {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }
    else
    {
        //QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
        string temp;
        ifstream in(path.toStdString().c_str());
        getline(in, temp);istringstream str(temp);
        str >> temp;temp = str.get();
        getline(str, temp);
        ui->lineEdit->setText(QString::fromStdString(temp));
        in >> temp;
        in >> temp;
        in >> temp;
        temp = in.get();
        getline(in, temp);
        istringstream str2(temp);
        str2 >> temp;temp = str2.get();
        getline(str2, temp);
        ui->comboBox->setCurrentText(QString::fromStdString(temp));
        getline(in, temp);
        in >> temp;
        char *a = new char[100000];
        //in.get(a,1,'\n');
        in.get(a, 100000, EOF);
        string tmp(a);
        tmp.erase(0, 1);
        ui->textEdit->setText(QString::fromStdString(tmp));
        delete[] a;
    }
}
void MainWindow::newfile()
{
    ui->lineEdit->setText("");
    ui->comboBox->setCurrentText("");
    ui->textEdit->setHtml("");
}
void MainWindow::changecata()
{
    if(ui->comboBox->currentText() == "新建类别")
    {
        ui->lineEdit_2->setHidden(0);
    }
    else
    {
        ui->lineEdit_2->setHidden(1);
    }
}
void MainWindow::savefile()
{
    ptime p = second_clock::local_time();
    string tags;
    if(ui->comboBox->currentText() == "新建类别")
    {
        tags = ui->lineEdit_2->text().toStdString();
        ui->comboBox->addItem(ui->lineEdit_2->text());
    }
    else
    {
        tags = ui->comboBox->currentText().toStdString();
    }
    string tmp = path.c_str() + string("/") + (ui->lineEdit->text().toStdString()) + '-' + tags + ".md";
    if(tags.length() != 0 && ui->lineEdit->text().length() != 0 && ui->textEdit->toPlainText().length() != 0)
    {
        ofstream out(tmp.c_str());
        out << "title: " << (ui->lineEdit->text().toStdString()) << '\n' << "date: " << to_iso_extended_string(p.date()) << ' '
            << to_simple_string(p.time_of_day()) << '\n' << "categories: " << tags << endl << "tags: "
            << tags << endl << "---" << endl;
        string tmp = (ui->textEdit->toPlainText().toStdString());
        /*
        for(int i = 0; i != tmp.length() - 3; i++)
        {
            if((tmp[i] != ' ' || tmp[i + 1] != ' ') && (tmp[i + 2] == '\n' && tmp[i + 3] != '\n'))
                tmp.insert(i + 2, 2, ' ');
        }
        */
        while(tmp.find("file://") != string::npos)
        {
            string::size_type sib = tmp.find("file://");
            string::size_type sie = sib;
            for(; ((tmp[sie] != '\n') && (sie <= tmp.length() + 1)) ; sie++);

            while(tmp[sie - 1] == ' ' ) {
                sie--;
            }
            string pathtmp(tmp, sib + 7, sie - sib - 7);
            boost::filesystem::path dirpath(pathtmp);
            boost::filesystem::path endle(this->path);
            endle = endle.parent_path() / "img" / dirpath.filename();
            try
            {
                copy_file(pathtmp, endle);
            }
            catch(...) {}
            string picture = "![";
            picture = picture + dirpath.filename().string() + "](" + "/img/" + dirpath.filename().string() + ")";
            tmp.replace((tmp.begin() + sib), (tmp.begin() + sie), picture);
        }
        out << tmp;
        out.close();
        QMessageBox msgBox;
        msgBox.setText("保存成功,喵~!");
        msgBox.exec();
    }
    else
    {
        QMessageBox::information(NULL, tr("Path"), tr("请把所有信息填满再保存,笨喵~!"));
    }
}

void MainWindow::pushfile()
{


    system("source ~/.bash_profile && cd ~/mysite && hexo generate && hexo deploy");
    QMessageBox *msgBox = new QMessageBox;
    msgBox->setText("提交成功,喵~!");
    msgBox->exec();
    delete msgBox;
}
void MainWindow::openblog()
{
    QDesktopServices::openUrl(QUrl("http://hebei1992.github.io/", QUrl::TolerantMode));
}

void MainWindow::savefilenoerror()
{
    ptime p = second_clock::local_time();
    string tags;
    if(ui->comboBox->currentText() == "新建类别")
    {
        tags = ui->lineEdit_2->text().toStdString();
        ui->comboBox->addItem(ui->lineEdit_2->text());
    }
    else
    {
        tags = ui->comboBox->currentText().toStdString();
    }
    string tmp = path.c_str() + string("/") + (ui->lineEdit->text().toStdString()) + '-' + tags + ".md";
    if(tags.length() != 0 && ui->lineEdit->text().length() != 0 && ui->textEdit->toPlainText().length() != 0)
    {
        ofstream out(tmp.c_str());
        out << "title: " << (ui->lineEdit->text().toStdString()) << '\n' << "date: " << to_iso_extended_string(p.date()) << ' '
            << to_simple_string(p.time_of_day()) << '\n' << "categories: " << tags << endl << "tags: "
            << tags << endl << "---" << endl;
        string tmp = (ui->textEdit->toPlainText().toStdString());
        for(int i = 0; i != tmp.length() - 3; i++)
        {
            if((tmp[i] != ' ' || tmp[i + 1] != ' ') && (tmp[i + 2] == '\n' && tmp[i + 3] != '\n'))
                tmp.insert(i + 2, 2, ' ');
        }
        while(tmp.find("file://") != string::npos)
        {
            string::size_type sib = tmp.find("file://");
            string::size_type sie = sib;
            for(; (tmp[sie] != ' ' && tmp[sie] != '/n') && sie <= tmp.length() + 1 ; sie++);
            string pathtmp(tmp, sib + 7, sie - sib - 7);
            boost::filesystem::path dirpath(pathtmp);
            boost::filesystem::path endle(this->path);
            endle = endle.parent_path() / "img" / dirpath.filename();
            try
            {
                copy_file(pathtmp, endle);
            }
            catch(...) {}
            string picture = "![";
            picture = picture + dirpath.filename().string() + "](" + "/img/" + dirpath.filename().string() + ")";
            tmp.replace((tmp.begin() + sib), (tmp.begin() + sie), picture);
        }
        out << tmp;
        out.close();
        QMessageBox msgBox;
        msgBox.setText("保存成功,喵~!");
        msgBox.exec();
    }
}

