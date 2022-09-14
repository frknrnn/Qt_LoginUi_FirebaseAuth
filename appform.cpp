#include "appform.h"
#include "ui_appform.h"

Appform::Appform(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Appform)
{
    ui->setupUi(this);
}

Appform::~Appform()
{
    delete ui;
}
