#include "MainWindow.h"
#include <QHBoxLayout>
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent) : QWidget(parent){
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    QHBoxLayout *row1 = new QHBoxLayout();
    QHBoxLayout *row2 = new QHBoxLayout();

    download_path = (QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));

    setFixedSize(512, 300);
    
    row1->addWidget(&folder_display);
    row1->addWidget(&folder_button);

    row2->addWidget(&url_input);
    row2->addWidget(&download_button);

    layout->addLayout(row1);
    layout->addLayout(row2);
    layout->addWidget(&progress_bar);


    setWindowTitle("MVD");
    download_button.setText("Download");
    folder_button.setText("Folder");

    url_input.setPlaceholderText("Enter URL here...");

    folder_display.setText(download_path);
    folder_display.setEnabled(false);
    

    connect(&download_button, &QPushButton::clicked, this, &MainWindow::onDownloadClick);
    connect(&folder_button, &QPushButton::clicked, this, &MainWindow::onFolderSelect);
    connect(&download_process, &QProcess::readyReadStandardOutput, this, &MainWindow::onProgressUpdate);
    connect(&download_process, &QProcess::finished, &download_process, &QProcess::close);
    connect(&download_process, &QProcess::readyReadStandardError, this, &MainWindow::onErrorOutput);
}

void MainWindow::onDownloadClick(){
    if(download_process.state() != QProcess::NotRunning)
        return;
    progress_bar.setValue(0);
    QDir().mkpath(download_path);
    QString outputPath = download_path + "/%(title)s.%(ext)s";

    qDebug() << QCoreApplication::applicationDirPath();
    #ifdef Q_OS_WIN
        QString ytdlp = QCoreApplication::applicationDirPath() + "/bin/yt-dlp.exe";
    #else
        QString ytdlp = QCoreApplication::applicationDirPath() + "/bin/yt-dlp";
    #endif

    QStringList args = {url_input.text(), "--no-playlist", "-o", outputPath, "--merge-output-format", "mp4", "--force-overwrites"};
    download_process.start(ytdlp, args);

    
}

void MainWindow::onFolderSelect(){
    QString selected = QFileDialog::getExistingDirectory(this, "Selects Download Folder", download_path);
    if(!selected.isEmpty()){
        download_path = selected;
        folder_display.setText(selected);
    }



}

void MainWindow::onProgressUpdate(){
    QString output = download_process.readAllStandardOutput();
    
    int percentIndex = output.indexOf("%");
    if(percentIndex != -1){
        int start = output.lastIndexOf(' ', percentIndex) + 1;
        int value = (output.mid(start, percentIndex - start)).trimmed().toDouble();
        progress_bar.setValue(value);
    }
}

void MainWindow::onErrorOutput(){
    qDebug() << download_process.readAllStandardError();
}

void MainWindow::closeEvent(QCloseEvent *event){
    download_process.kill();
    download_process.waitForFinished();
    event->accept();
}