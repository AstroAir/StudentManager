#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTranslator>

#include "ElaWindow.h"

class ElaContentDialog;

class T_About;
class T_Home;
class T_Setting;
class T_StudentPage;

class MainWindow : public ElaWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Q_SLOT void onCloseButtonClicked();

    void initWindow();
    void initEdgeLayout();
    void initContent();

private:
    ElaContentDialog *_closeDialog{nullptr};

    T_Home *_homePage{nullptr};
    T_Setting *_settingPage{nullptr};
    T_StudentPage *_studentPage{nullptr};

    QString _aboutKey{""};
    QString _settingKey{""};
};
#endif  // MAINWINDOW_H
