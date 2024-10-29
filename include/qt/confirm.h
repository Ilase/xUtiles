#ifndef CONFIRM_H
#define CONFIRM_H

#include <QWidget>

namespace Ui {
class Confirm;
}

class Confirm : public QWidget
{
    Q_OBJECT

public:
    explicit Confirm(QWidget *parent = nullptr);
    ~Confirm();


private:
    Ui::Confirm *ui;
    bool applyed;

signals:
    //void closeWindow();
    void closed(bool applyed);

private slots:
    void on_cancelButton_clicked();
    void on_applyButton_clicked();
};

#endif // CONFIRM_H
