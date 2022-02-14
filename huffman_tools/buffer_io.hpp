#ifndef BUFFERIO_H
#define BUFFERIO_H
#include "code.hpp"
#include "huffman_table.hpp"
#include <fstream>
#include <iostream>

constexpr uint8_t map[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

union Byte {
	uint8_t byte = 0;
	char ch;
    static uint8_t getBit(int pos, const Byte& byte) { return byte.byte & map[pos]; }
    static void flipBit(int pos, Byte& byte) { byte.byte ^= map[pos]; }
};

class BufferWrite {
public:
	BufferWrite() = default;
	void writeAllBuffer(std::ostream& os) {
		os.write(&buffer_.ch, 1);
		pos = 0;
		buffer_.byte = 0;
	}
	void write(Code code, std::ostream& os) {
		while (pos > 0) {
			if (pos == 8) {
				writeAllBuffer(os);
				break;
			}
			if (code.getSize() == 0) {
				break;
			}
			uint8_t temp = code.getBit(0) ? 1 : 0;
			code.popLowBit();
			if (temp) {
                Byte::flipBit(pos, buffer_);
			}
			++pos;
		}
		while (code.getSize() >= 8) {
			buffer_.ch = code.getLowByte();
			code.popLowByte();
			writeAllBuffer(os);
		}
		if (code.getSize() > 0) {
			buffer_.ch = code.getLowByte();
			pos = code.getSize();
		}
	}

private:
	Byte buffer_;
	int pos = 0;
};

class BufferRead {
public:
    using BiTree = BinaryTree<Node>;
	BufferRead(std::istream &is, const BiTree *huffman_tree) : huffman_tree_(huffman_tree), is_(is) {
	    renewBuffer();
	}
	char getNextChar() {
        char temp = traverseTree(huffman_tree_);
        return temp;
	}
	bool eof() const {
        return eof_;
    }
    int getBitCount() const {
	    return bit_count_;
	}
private:
	Byte buffer_;
	int pos = -1;
	int bit_count_ = 0;
	const BiTree *huffman_tree_;
	std::istream &is_;
	bool eof_ = false;

	void renewBuffer() {
        is_.read(&buffer_.ch, 1);
        pos = 0;
        if(is_.eof()) {
            eof_ = true;
        }
	}
	void increasePos() {
	    ++pos;
	    ++bit_count_;
	    if(pos < 0 || pos >= 8) {
	        renewBuffer();
	        pos = 0;
	    }
	}
    char traverseTree(const BiTree *current_node) {
        if(current_node->isLeaf()) {
            return current_node->getData().ch;
        }
        if(eof_) {
            return EOF;
        }
        if(Byte::getBit(pos, buffer_) == 0) {
            increasePos();
            return traverseTree(current_node->getLeft());
        } else {
            increasePos();
            return traverseTree(current_node->getRight());
        }
    }
};
#endif
