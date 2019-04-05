#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

    #include <QMainWindow>
    #include <QTreeWidgetItem>
    #include <QDialogButtonBox>
    #include <QLineEdit>
    #include <QTableView>

    #include "shapeHandler.h"
    #include "qdbftablemodel.h"

    namespace Ui {
    class MainWindow;
    }

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    protected:
        Crit3DShapeHandler shapeHandler;

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;
        QTableView m_tableView;
        QDbf::QDbfTableModel m_model;
        QDialogButtonBox m_buttonBox;

    private slots:
        void onFileOpen();
        void onSelectShape(QTreeWidgetItem *item, int column);
        void on_dbfButton_clicked();
    };

#endif // MAINWINDOW_HPP
