#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    availablePorts = new QActionGroup(this);

    baudrate = 115200;

    updatePorts();

    setWindowTitle("Configuración");
    setFixedSize(size());

    system.playPhonogram(false);

    setFixedSize(size());

    ui->playPushButton->setToolTip("Reproducir Audio");
    ui->pausePushButton->setToolTip("Pausar Audio");
    ui->recPushButton->setToolTip("Grabar en Archivo");
    ui->stopPushButton->setToolTip("Finalizar Grabación");
    ui->screenPushButton->setToolTip("Mostrar/Ocultar Gráfica");

    ui->playPushButton->setVisible(true);
    ui->pausePushButton->setVisible(false);
    ui->recPushButton->setVisible(true);
    ui->stopPushButton->setVisible(false);

    ui->centralWidget->setEnabled(false);

    system.setPhonogramVolume(ui->volumeDial->value());

    connect(ui->volumeDial, SIGNAL(valueChanged(int)), &system, SLOT(setPhonogramVolume(int)));
    connect(availablePorts, SIGNAL(triggered(QAction*)), this, SLOT(selectPort(QAction*)));
    connect(&system, SIGNAL(portConnectionError()), this, SLOT(connectionError()));
    connect(ui->action_Filtrar, SIGNAL(toggled(bool)), &system, SLOT(enableFilter(bool)));
#if defined(Q_OS_LINUX)
    connect(&searchPortTicker, SIGNAL(timeout()), this, SLOT(updatePorts()));
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    system.stop();
    exit(0);
}

void MainWindow::selectPort(QAction *selectedPort)
{qDebug(selectedPort->text().toLatin1());
    if (system.receivingData())
        system.stop();

    if (system.start(selectedPort->text(), baudrate))
    {

        ui->centralWidget->setEnabled(true);

        ui->playPushButton->setVisible(false);
        ui->pausePushButton->setVisible(true);

#if defined(Q_OS_LINUX)
        searchPortTicker.stop();
#endif

    }
}

void MainWindow::connectionError()
{
    QErrorMessage errorMessage;

    updatePorts();

#if defined(Q_OS_LINUX)
    searchPortTicker.start(2000);
#endif

    errorMessage.showMessage("Error de conexión de puerto!");

    errorMessage.exec();
}

void MainWindow::updatePorts()
{
    for (int portIndex = 0; portIndex < ui->menu_Port->actions().size(); portIndex++)
            availablePorts->removeAction(ui->menu_Port->actions()[portIndex]);

    ui->menu_Port->clear();

    for (int portIndex = 0; portIndex < system.availablePorts().size(); portIndex++)
    {
        ui->menu_Port->addAction(system.availablePorts()[portIndex]);
        ui->menu_Port->actions()[portIndex]->setCheckable(true);

        availablePorts->addAction(ui->menu_Port->actions()[portIndex]);
    }

    ui->menu_Port->addSeparator();

    ui->menu_Port->addAction("&Actualizar Puertos");

    connect(ui->menu_Port->actions()[ui->menu_Port->actions().size() - 1], SIGNAL(triggered(bool)), this, SLOT(updatePortsFromMenu()));
}

void MainWindow::updatePortsFromMenu()
{
    updatePorts();

    ui->menu_Tools->show();
    ui->menu_Port->show();

    ui->menu_Tools->setFocus();
    ui->menu_Port->setFocus();
}

void MainWindow::on_pausePushButton_clicked()
{
    system.playPhonogram(false);

    ui->playPushButton->setVisible(true);
    ui->pausePushButton->setVisible(false);
}

void MainWindow::on_playPushButton_clicked()
{
    system.playPhonogram(true);

    ui->playPushButton->setVisible(false);
    ui->pausePushButton->setVisible(true);
}

void MainWindow::on_stopPushButton_clicked()
{
    if (system.isRecording())
        system.stopRecording();

    ui->recPushButton->setVisible(true);
    ui->stopPushButton->setVisible(false);
}

void MainWindow::on_recPushButton_clicked()
{
    system.startRecording();

    ui->recPushButton->setVisible(false);
    ui->stopPushButton->setVisible(true);
}

void MainWindow::on_screenPushButton_clicked()
{
    system.toggleGraphVisibility();
}
