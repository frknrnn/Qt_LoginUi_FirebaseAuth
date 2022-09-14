#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>
#include "authhandler.h"
#include <QThread>
#include <QPropertyAnimation>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_quitLogin_clicked();

    void on_pushButton_signup_clicked();

    void on_pushButton_signupCancel_clicked();

    void on_pushButton_signup_create_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_forgotPasswordCancel_clicked();

    void on_pushButton_forgotPassword_clicked();

private:
    Ui::MainWindow *ui;

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void initialUi();
    bool checkNewUserInfo();
    void showLoadingPage();
    void showLogin();
    void showForgotPasswordPage();
    void completeCreateNewUser();
    void completeSignInUser();
    void createNewUserError();
    void createNewUser();
    void signInUser();
    void loginLabelInfoStyle(int status);
    int mouseClickMove_X;
    int mouseClickMove_Y;


    QString newUserName;
    QString newUserSurname;
    QString newUserEmail;
    QString newUserPassword;
    QString newUserConfirmPassword;

    QString userEmail;
    QString userPassword;

    AuthHandler * auth;
    QThread *authThread;


    QPropertyAnimation *loginPageAnimation;
    QPropertyAnimation *forgotPasswordPageAnimation;

};
#endif // MAINWINDOW_H
