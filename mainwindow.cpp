#include "mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), qte_(new QTextEdit(this)), encode_btn_(new QPushButton(tr("Encode"), this)),
      decode_btn_(new QPushButton(tr("Decode"), this)), load_table_btn_(new QPushButton(tr("Load Table"), this)),
      gen_table_btn_(new QPushButton(tr("Generate Table"), this)),
      load_text_file_btn_(new QPushButton(tr("Load Text File"), this)),
      save_text_file_btn_(new QPushButton(tr("Save Text File"), this))
{
    qte_->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    qte_->setPlaceholderText(tr("Type anything to encode"));
    qte_->setTabStopDistance(40);

    auto *central_widget = new QWidget(this);
    auto *qvbox_ = new QVBoxLayout(central_widget);
    central_widget->setLayout(qvbox_);

    qvbox_->addWidget(load_text_file_btn_);
    qvbox_->addWidget(save_text_file_btn_);
    qvbox_->addWidget(load_table_btn_);
    qvbox_->addWidget(gen_table_btn_);
    qvbox_->addWidget(qte_);
    qvbox_->addWidget(encode_btn_);
    qvbox_->addWidget(decode_btn_);

    connect(load_table_btn_, &QPushButton::clicked, this, &MainWindow::loadTable);
    connect(gen_table_btn_, &QPushButton::clicked, this, &MainWindow::genTable);
    connect(encode_btn_, &QPushButton::clicked, this, &MainWindow::encode);
    connect(decode_btn_, &QPushButton::clicked, this, &MainWindow::decode);
    connect(load_text_file_btn_, &QPushButton::clicked, this, &MainWindow::loadTextFile);
    connect(save_text_file_btn_, &QPushButton::clicked, this, &MainWindow::saveTextFile);
    statusBar()->showMessage("Ready");
    setCentralWidget(central_widget);
}

MainWindow::~MainWindow()
{
    delete table_;
}

void MainWindow::encode() {
    if(!table_) {
        noTableError();
        return;
    }
    QString filename = fileSave(tr("Save encoded file"), "*.bin");
    if(filename.size() == 0) {
        return;
    }
    auto *he_ = new HuffmanEncoder();
    he_->setTable(table_);
    std::istringstream iss(qte_->toPlainText().toStdString());
    std::ofstream ofs(filename.toStdString().c_str(), std::ios::binary);
    he_->encodeAll(iss, ofs);
    ofs.close();
    statusBar()->showMessage(tr("Encoded successfully."));
}

void MainWindow::decode() {
    if(!table_) {
        noTableError();
        return;
    }
    QString filename = fileOpen(tr("Selecet file to decode"), "*.bin");
    if(filename.size() == 0) {
        return;
    }
    std::ostringstream oss;
    std::ifstream ifs(filename.toStdString().c_str(), std::ios::binary);
    HuffmanDecoder hd_(oss, ifs, table_->getHuffmanTree());
    hd_.readAll();
    qte_->setText(QString::fromStdString(oss.str()));
    ifs.close();
    statusBar()->showMessage(tr("Decoded successfully."));
}

void MainWindow::loadTable() {
    QString filename = fileOpen(tr("Load Table"), "*.tab");
    if(filename.size() == 0) {
        return;
    }
    std::ifstream ifs(filename.toStdString().c_str(), std::ios::binary);
    if(!table_) {
        table_ = new HuffmanTable();
    }
    table_->loadTableFromFile(ifs);
    ifs.close();
    statusBar()->showMessage(tr("Table loaded successfully."));
}

void MainWindow::genTable() {
    QString filename = fileSave(tr("Save table"), "*.tab");
    if(filename.size() == 0) {
        return;
    }
    QString str = qte_->toPlainText();
    std::string stdstr = str.toStdString();
    std::istringstream iss(stdstr);
    HuffmanEncoder he_;
    he_.genFreqTable(iss);
    he_.genTable();
    he_.saveTableToFile(filename.toStdString().c_str());
    table_ = he_.getTable();
    statusBar()->showMessage(tr("Table generated."));
}

void MainWindow::noTableError() {
    QMessageBox::critical(this, tr("Error"), tr("No table generated or loaded!"));
}

void MainWindow::noFileError() {
    QMessageBox::critical(this, tr("Error"), tr("You didn't selecet any file"));
    return;
}

QString MainWindow::fileOpen(const QString &title, const QString &filter) {
    QString filename = QFileDialog::getOpenFileName(this, title, filter);
    if(filename.size() == 0) {
        noFileError();
    }
    return filename;
}

QString MainWindow::fileSave(const QString &title, const QString &filter) {
    QString filename = QFileDialog::getSaveFileName(this, title, filter);
    if(filename.size() == 0) {
        noFileError();
    }
    return filename;
}

void MainWindow::loadTextFile() {
    QString filename = fileOpen(tr("Open Text file"), "*");
    if(filename.size() == 0) {
        return;
    }
    QFile text_file(filename);
    text_file.open(QFile::ReadOnly);
    qte_->setText(QString::fromUtf8(text_file.readAll()));
    text_file.close();
    statusBar()->showMessage(tr("loaded succefully."));
}

void MainWindow::saveTextFile() {
    QString filename = fileSave(tr("Save Text file"), "*");
    if(filename.size() == 0) {
        return;
    }
    QFile text_file(filename);
    text_file.open(QFile::ReadWrite);
    text_file.write(qte_->toPlainText().toUtf8());
    text_file.close();
    statusBar()->showMessage(tr("Saved succefully."));
}
