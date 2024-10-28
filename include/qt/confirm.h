#ifndef CONFIRM_H
#define CONFIRM_H

#include <QFrame>

namespace Ui {
class confirm;
}

class confirm : public QFrame
{
    Q_OBJECT

public:
    explicit confirm(QWidget *parent = nullptr);
    ~confirm();

private:
    Ui::confirm *ui;
};

#endif // CONFIRM_H
