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
    explicit MonitorButton(QWidget *parent = nullptr);
    ~MonitorButton();

private:
    Ui::MonitorButton *ui;
};

#endif // MONITORBUTTON_H
