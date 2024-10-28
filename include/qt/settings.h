#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QDesktopWidget>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_checkBoxTearing_clicked();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
