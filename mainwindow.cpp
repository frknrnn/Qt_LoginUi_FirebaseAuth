#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "authhandler.h"
#include <QDebug>
#include <QMovie>
#include <QThread>
#include <appform.h>
#include <iostream>
#include <QPropertyAnimation>
#include <QRect>
#include <QEasingCurve>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent,Qt::FramelessWindowHint)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    auth =new AuthHandler();
    auth->setAPIKey("AIzaSyCbeSxOcaxXwqGEWouM9MZEo2uW2Bue5jw");
    connect(auth,&AuthHandler::userCreated,this,&MainWindow::completeCreateNewUser);
    connect(auth,&AuthHandler::userCreatedError,this,&MainWindow::createNewUserError);
    initialUi();


}

void MainWindow::initialUi(){
    ui->label_loginInfo->hide();
    ui->label_createUserInfo->hide();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createNewUser()
{
    auth->createNetworkAccessManager();
    auth->signUserUp(newUserEmail,newUserPassword);

}

void MainWindow::signInUser()
{
    auth->createNetworkAccessManager();
    auth->signUserIn(userEmail,userPassword);
}

void MainWindow::loginLabelInfoStyle(int status)
{
    if(status == 0){
        ui->label_loginInfo->setStyleSheet("QLabel{\n"
                                           "color: rgb(200, 100, 100);\n"
                                           "font: 87 7pt \"Segoe UI Black\";\n"
                                           "border:none;\n"
                                           "}");
    }else{
        ui->label_loginInfo->setStyleSheet("QLabel{\n"
                                           "color: rgb(100, 200, 100);\n"
                                           "font: 87 7pt \"Segoe UI Black\";\n"
                                           "border:none;\n"
                                           "}");
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mouseClickMove_X = event->x();
    mouseClickMove_Y = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalX()-mouseClickMove_X,event->globalY()-mouseClickMove_Y);
}




void MainWindow::on_pushButton_quitLogin_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_signup_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_signupCancel_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_signup_create_clicked()
{
    newUserName = ui->lineEdit_signup_name->text();
    newUserSurname = ui->lineEdit_signup_surname->text();
    newUserEmail = ui->lineEdit_signup_email->text();
    newUserPassword = ui->lineEdit_signup_password->text();
    newUserConfirmPassword = ui->lineEdit_signup_confirmpassword->text();
    bool status = checkNewUserInfo();

    qDebug()<< "Check Status" << status;

    if(status){
        qDebug()<< "Create New user";
        showLoadingPage();
        createNewUser();

    }
    else{
        qDebug()<< "Error";
        ui->label_createUserInfo->show();
    }


}

bool MainWindow::checkNewUserInfo()
{
    qDebug()<< newUserPassword <<" : "<< newUserConfirmPassword;
    if(newUserPassword!=newUserConfirmPassword){
        return false;
    }
    else{
        return true;
    }
}


void MainWindow::showLoadingPage()
{
    ui->stackedWidget->setCurrentIndex(2);
    QMovie *loadingGifMovie =new QMovie(":/gif/icons/Spinner-0.5s-100px.gif");
    ui->label_gif->setMovie(loadingGifMovie);
    loadingGifMovie->start();
}

void MainWindow::showLogin()
{
    loginPageAnimation =new QPropertyAnimation(ui->frame_login,"geometry");
    loginPageAnimation->setDuration(500);
    loginPageAnimation->setEndValue(QRect(20,10,ui->frame_login->width(),ui->frame_login->height()));
    loginPageAnimation->setStartValue(QRect(20,-300,ui->frame_login->width(),ui->frame_login->height()));
    loginPageAnimation->setEasingCurve(QEasingCurve::InOutQuart);
    loginPageAnimation->start();
}

void MainWindow::showForgotPasswordPage()
{
    forgotPasswordPageAnimation =new QPropertyAnimation(ui->frame_login,"geometry");
    forgotPasswordPageAnimation->setDuration(500);
    forgotPasswordPageAnimation->setStartValue(QRect(20,10,ui->frame_login->width(),ui->frame_login->height()));
    forgotPasswordPageAnimation->setEndValue(QRect(20,-300,ui->frame_login->width(),ui->frame_login->height()));
    forgotPasswordPageAnimation->setEasingCurve(QEasingCurve::InOutQuart);
    forgotPasswordPageAnimation->start();
}

void MainWindow::completeCreateNewUser()
{
    ui->stackedWidget->setCurrentIndex(0);
    loginLabelInfoStyle(1);
    ui->label_loginInfo->setText("User Created.");
    ui->label_loginInfo->show();
    ui->label_createUserInfo->hide();
}

void MainWindow::completeSignInUser()
{
    Appform *newApp = new Appform();
    newApp->show();
    this->hide();
}

void MainWindow::createNewUserError()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->label_createUserInfo->setText("Error");
    ui->label_createUserInfo->show();
}

void MainWindow::on_pushButton_login_clicked()
{
    userEmail = ui->lineEdit_login_email->text();
    userPassword = ui->lineEdit_login_password->text();
    signInUser();
    showLoadingPage();

}









void MainWindow::on_pushButton_forgotPasswordCancel_clicked()
{
    showLogin();
}

void MainWindow::on_pushButton_forgotPassword_clicked()
{
    showForgotPasswordPage();
}
