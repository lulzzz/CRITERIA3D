#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

class Plot;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushRunAllPeriod_clicked();  
    void on_pushLoadFileSoil_clicked();
    void on_pushLoadFileMeteo_clicked();
    void on_pushCopyOutput_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *selItem);
    void on_chkUseInputMeteo_clicked();
    void on_chkUseInputSoil_clicked();

    bool initializeModel();

private:
    Ui::MainWindow *ui;

private:
    Plot * outPlot;

};

#endif // MAINWINDOW_H
