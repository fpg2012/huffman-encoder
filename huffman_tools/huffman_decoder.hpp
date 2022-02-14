//
// Created by nth233 on 1/5/21.
//
#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "binary_tree.hpp"
#include "buffer_io.hpp"
#include "huffman_table.hpp"

class HuffmanDecoder {
public:
    HuffmanDecoder(std::ostream &oss, std::ifstream &ifs, const BinaryTree<Node> *bt)
    : oss_(oss), ifs_(ifs), buf_read_(new BufferRead(ifs, bt)) {

    }
    ~HuffmanDecoder() {
        delete buf_read_;
    }
    void setEndChar(char end_ch) {
        end_char_ = end_ch;
    }
    void readAll() {
        while(true) {
            char temp = buf_read_->getNextChar();
            if(buf_read_->eof()) {
                return;
            }
            if(temp == end_char_) {
                eof_ = true;
                return;
            }
            oss_ << temp;
        }
    }
    bool eof() const {
        return eof_;
    }
private:
    std::ostream &oss_;
    std::ifstream &ifs_;
    BufferRead *buf_read_ = nullptr;
    char end_char_ = EOF;
    bool eof_ = false;
};
#endif
