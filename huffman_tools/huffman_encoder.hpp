#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H
#define CODE_SIZE 32
#define BUF_SIZE 3000
#include "buffer_io.hpp"
#include "code.hpp"
#include "huffman_table.hpp"
#include <fstream>
#include <iostream>

class HuffmanEncoder {
public:
	HuffmanEncoder() = default;
	~HuffmanEncoder() {
	    
	}
	HuffmanTable *getTable() { return table_; }
	virtual void bufferIn(std::istream& is, size_t sz = BUF_SIZE, bool gen_freq_table = false) {
	    char temp = 0;
	    count = 0;
	    while(!is.eof()) {
            is.read(&temp, 1);
            if(gen_freq_table) {
                freq_table_[temp]++;
            }
            buffer[count++] = temp;
	    }
	    if(gen_freq_table) {
            freq_table_[EOF] = 1;
        }
	}
    virtual void genFreqTable(std::istream& is, int sz = -1) {
	    char temp = 0;
	    int temp_count = 0;
	    while(!is.eof() && (temp_count < sz || sz == -1)) {
	        is.read(&temp, 1);
	        freq_table_[temp]++;
	        ++temp_count;
	    }
        freq_table_[EOF] = 1;
	}
	virtual void encodeAll(std::istream &is, std::ostream &os, size_t sz = BUF_SIZE) {
	    while(!is.eof()) {
            is.read(buffer, sz);
            count = is.gcount();
            writeOut(os);
        }
        bufw_.write((*table_)[EOF], os);
        bufw_.writeAllBuffer(os);
	}
	virtual void genTable(bool clear_previous = false) {
	    if(!table_) {
            table_ = new HuffmanTable();
	    }
		if (clear_previous) {
			table_->clear();
		}
		for (auto p : freq_table_) {
			table_->addNode(Node(p.first, p.second));
		}
		table_->genTable();
	}
	void setTable(HuffmanTable *table) {
	    delete table_;
	    table_ = table;
	}
	virtual void writeOut(std::ostream& os) {
		for (int i = 0; i <= count; ++i) {
			bufw_.write((*table_)[buffer[i]], os);
		}
	}
	void loadTableFromFile(const char* filename = "table.bin") {
		std::ifstream ifs(filename, std::ios::binary);
		if(!table_) {
		    table_ = new HuffmanTable();
		}
		table_->loadTableFromFile(ifs);
		ifs.close();
	}
	void saveTableToFile(const char* filename = "table.bin") {
		std::ofstream ofs(filename, std::ios::binary);
		table_->writeTableToFile(ofs);
		ofs.close();
	}

private:
	HuffmanTable *table_ = nullptr;
	FreqMap freq_table_;
	char buffer[BUF_SIZE];
	int count = 0;
	BufferWrite bufw_;
};
#endif
