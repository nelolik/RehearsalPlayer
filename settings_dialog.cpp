#include "settings_dialog.h"
#include "ui_settings_dialog.h"

Settings_Dialog::Settings_Dialog(bool Stop2ndPanel, bool FadingStop, int FadingTime, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_Dialog)
{
    ui->setupUi(this);

    this->stop2ndPanel = Stop2ndPanel;
    this->fadingStop = FadingStop;
    this->fadingTime = FadingTime;
    ui->fadeTimeSlider->setMinimum(0);
    ui->fadeTimeSlider->setMaximum(10);
    ui->fadeTimeSlider->setValue(FadingTime);
    ui->fadeTimeSlider->setEnabled(FadingStop);
    ui->fadeTimeSpinBox->setValue(FadingTime);
    ui->fadeTimeSpinBox->setMinimum(0);
    ui->fadeTimeSpinBox->setMaximum(10);
    ui->fadeTimeSpinBox->setEnabled(FadingStop);

    ui->turnofCheckBox->setChecked(Stop2ndPanel);
    ui->fadeoutCheckBox->setChecked(FadingStop);

    ui->laguageBox->addItem(QString("Русский"));

    connect(ui->turnofCheckBox, SIGNAL(toggled(bool)), this, SLOT(onStopCheckbox(bool)));
    connect(ui->fadeoutCheckBox, SIGNAL(toggled(bool)), this, SLOT(onFadingCheckbox(bool)));
    connect(ui->fadeTimeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onFadingTimeChanged(double)));
    connect(ui->fadeTimeSlider, SIGNAL(sliderMoved(int)), this, SLOT(onFadingTimeChanged(int)));
}

Settings_Dialog::~Settings_Dialog()
{
    delete ui;
}

bool Settings_Dialog::getStop2ndPanel()
{
    return stop2ndPanel;
}

bool Settings_Dialog::getFadingStop()
{
    return fadingStop;
}

int Settings_Dialog::getFadingTime()
{
    return fadingTime;
}

void Settings_Dialog::onStopCheckbox(bool isChecked)
{
    stop2ndPanel = isChecked;
}

void Settings_Dialog::onFadingCheckbox(bool isChecked)
{
    fadingStop= isChecked;
    ui->fadeTimeSlider->setEnabled(fadingStop);
    ui->fadeTimeSpinBox->setEnabled(fadingStop);
}

void Settings_Dialog::onFadingTimeChanged(int time)
{
    fadingTime = time;
    ui->fadeTimeSpinBox->setValue(fadingTime);
}

void Settings_Dialog::onFadingTimeChanged(double time)
{
    fadingTime = time;
    ui->fadeTimeSlider->setValue(fadingTime);
}
