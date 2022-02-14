#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QStatusBar>
#include <string>
#include <sstream>

#include "huffman_tools/huffman_encoder.hpp"
#include "huffman_tools/huffman_decoder.hpp"
#include "huffman_tools/huffman_table.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void encode();
    void decode();
    void loadTable();
    void genTable();
    void loadTextFile();
    void saveTextFile();
private:
    QTextEdit *qte_;
    QPushButton *encode_btn_;
    QPushButton *decode_btn_;
    QPushButton *load_table_btn_;
    QPushButton *gen_table_btn_;
    QPushButton *load_text_file_btn_;
    QPushButton *save_text_file_btn_;

    HuffmanTable *table_ = nullptr;

    void noTableError();
    void noFileError();
    QString fileOpen(const QString &title, const QString &filter);
    QString fileSave(const QString &title, const QString &filter);
};
#endif // MAINWINDOW_H
