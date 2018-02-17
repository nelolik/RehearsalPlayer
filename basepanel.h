#ifndef BASEPANEL_H
#define BASEPANEL_H

#include <QFrame>

class BasePanel : public QFrame
{
    Q_OBJECT

private:
    bool global_focus;
    void setFrameParams();

public:
    explicit BasePanel(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent*) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

signals:
    void keySpacePressed();
    void signalFocusIn();
    void signalFocusOut();

public slots:
    void setFocusState(bool state);
};

#endif // BASEPANEL_H
