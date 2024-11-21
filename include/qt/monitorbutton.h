#ifndef MONITORBUTTON_H
#define MONITORBUTTON_H

#include <QWidget>
namespace Ui {
class MonitorButton;
}

class MonitorButton : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorButton(QWidget *parent = nullptr, int = 0, QString = "");
    ~MonitorButton();

private slots:
    void on_monitorButton_clicked();
signals:
    void changedScreen(int);
private:
    Ui::MonitorButton *ui;
    int m_id;
    QString m_name;
};

#endif // MONITORBUTTON_H
