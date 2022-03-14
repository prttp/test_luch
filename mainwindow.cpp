#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_uploadAction_triggered()
{
    QString str = QFileDialog::getOpenFileName(this, "Открыть изображение", QCoreApplication::applicationDirPath(), "Изображения (*.png *.xpm *.jpg)");
    ui->picWidget->uploadPicture(str); //загрузка выбранного изображения
    this->setFixedSize( ui->picWidget->size()); //изменения размера окна под виджет изображения
}



void MainWindow::on_aboutAction_triggered()
{
    QMessageBox::about(this, "О программе", "Тестовое задание Мишин");
}

