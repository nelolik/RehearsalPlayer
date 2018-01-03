#ifndef BASEPANEL_H
#define BASEPANEL_H

#include <QFrame>

class BasePanel : public QFrame
{
    Q_OBJECT
public:
    explicit BasePanel(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent*) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void keySpacePressed();

public slots:
};

#endif // BASEPANEL_H
