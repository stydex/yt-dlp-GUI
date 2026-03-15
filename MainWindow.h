#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QProcess>
#include <QLineEdit>
#include <QProgressBar>
#include <QStandardPaths>
#include <QCloseEvent>
#include <QDir>
#include <QCoreApplication>


class MainWindow: public QWidget{
    Q_OBJECT

    protected:
    void closeEvent(QCloseEvent *event) override;

    public:
    MainWindow(QWidget *parent = nullptr);

    public slots:
        void onDownloadClick();
        void onFolderSelect();
        void onProgressUpdate();
        void onErrorOutput();
    private:
        QLineEdit url_input, folder_display;
        QPushButton download_button, folder_button;
        QProcess download_process;
        QProgressBar progress_bar;
        QString download_path;

};

#endif