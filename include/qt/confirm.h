#ifndef CONFIRM_H
#define CONFIRM_H

#include <QWidget>
#include <QTimer>


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
    QTimer *timer;

signals:
    //void closeWindow();
    void closed(bool applyed);
    void open();
    void cancelSignal();
    void applySignal();

private slots:
    void on_cancelButton_clicked();
    void on_applyButton_clicked();
    void on_modalWindowOpen();
    //void on_Confirm_destroyed();
    void timerTimeout();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // CONFIRM_H
