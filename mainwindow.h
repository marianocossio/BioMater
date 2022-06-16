#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QErrorMessage>

#include <QCloseEvent>

#include <QActionGroup>

#include "system.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    QActionGroup *availablePorts;

    System system;

    int baudrate;

#if defined(Q_OS_LINUX)
    QTimer searchPortTicker;
#endif

private slots:
    void selectPort(QAction *selectedPort);
    void connectionError();
    void updatePorts();
    void updatePortsFromMenu();
    void on_pausePushButton_clicked();
    void on_playPushButton_clicked();
    void on_stopPushButton_clicked();
    void on_recPushButton_clicked();
    void on_screenPushButton_clicked();
};

#endif // MAINWINDOW_H
