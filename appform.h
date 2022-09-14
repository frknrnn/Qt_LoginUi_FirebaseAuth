#ifndef APPFORM_H
#define APPFORM_H

#include <QMainWindow>

namespace Ui {
class Appform;
}

class Appform : public QMainWindow
{
    Q_OBJECT

public:
    explicit Appform(QWidget *parent = nullptr);
    ~Appform();

private:
    Ui::Appform *ui;
};

#endif // APPFORM_H
