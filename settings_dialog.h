#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>

namespace Ui {
class Settings_Dialog;
}

class Settings_Dialog : public QDialog
{
    Q_OBJECT

private:
    bool stop2ndPanel;
    bool fadingStop;
    int fadingTime;

public:
    explicit Settings_Dialog(bool stop2ndPanel, bool fadingStop, int fadingTime, QWidget *parent);
    ~Settings_Dialog();
    bool getStop2ndPanel();
    bool getFadingStop();
    int getFadingTime();

private:
    Ui::Settings_Dialog *ui;

private slots:
    void onStopCheckbox(bool);
    void onFadingCheckbox(bool);
    void onFadingTimeChanged(int);
    void onFadingTimeChanged(double);
};

#endif // SETTINGS_DIALOG_H
