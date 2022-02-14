#ifndef HUFFMAN_TABLE_H
#define HUFFMAN_TABLE_H
#include "code.hpp"
#include "binary_tree.hpp"
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
using TableMap = std::map<char, Code>;
using FreqMap = std::map<char, int>;

template <typename T> union BytesArr {
	T bytes = 0;
	char ch[sizeof(T)];
};

struct Node {
	char ch;
	int freq;
	Node(char ch = '\0', int freq = 0) : ch(ch), freq(freq) {}
	bool operator<(const Node& nd) { return freq < nd.freq; }
	bool operator<=(const Node& nd) { return freq <= nd.freq; }
	bool operator>=(const Node& nd) { return freq >= nd.freq; }
	bool operator>(const Node& nd) { return freq > nd.freq; }
};

class HuffmanTable {
public:
	using BiTree = BinaryTree<Node>;
	HuffmanTable() = default;
	~HuffmanTable() {
	    delete huffman_tree_;
	}
	void addNode(Node nd) {
		auto* new_bitree = new BiTree(nd);
		bitrees_.push_back(new_bitree);
	}
	void genTable() {
		//addNode(Node(EOF, 0));
		for (auto item : bitrees_) {
			q.push(item);
		}
		while (q.size() > 1) {
			auto temp1 = q.top();
			q.pop();
			auto temp2 = q.top();
			q.pop();
			Node temp_node('\0', temp1->getData().freq + temp2->getData().freq);
			auto* bt = new BiTree(temp_node);
			bt->setLeft(temp1);
			bt->setRight(temp2);
			q.push(bt);
		}
		auto* bt = q.top();
		Code init;
		traverseTree(bt, init);
		huffman_tree_ = bt;
	}
	Code getCode(char ch) { return table_[ch]; }
	void printTable() {
		printf("output\n");
		for (auto iter = table_.begin(); iter != table_.end(); ++iter) {
			printf("%d\t-\t%s\n", iter->first, iter->second.toStr().c_str());
		}
	}
	const TableMap& getTable() const { return table_; }
	Code operator[](const char ch) { return table_[ch]; }
	void clear() {
		bitrees_.clear();
		table_.clear();
		while (!q.empty()) {
			q.pop();
		}
	}
	void setTable(const std::map<char, Code>& mp) { table_ = mp; }
	void writeTableToFile(std::ofstream& ofs) {
		for (const auto ccp : table_) {
			char ch = ccp.first;
			Code cd = ccp.second;
			int bytes_len = cd.getSize() / 8;
			if (cd.getSize() % 8 != 0) {
				++bytes_len;
			}
			ofs.write(&ch, 1);
			BytesArr<uint8_t> sz;
			sz.bytes = cd.getSize();
			ofs.write(sz.ch, sizeof(sz));
			BytesArr<uint8_t> temp_byte;
			for (int i = 0; i < bytes_len; ++i) {
				temp_byte.bytes = cd.getLowByte();
				ofs.write(temp_byte.ch, sizeof(temp_byte));
				cd.popLowByte();
			}
		}
	}
	void loadTableFromFile(std::ifstream& ifs) {
	    delete huffman_tree_;
	    huffman_tree_ = new BiTree(Node(0, 0));
		BytesArr<uint8_t> sz_temp;
		char temp_ch = 0;
		int temp_size = 0;
		BytesArr<uint8_t> buf;
		table_.clear();
		while (true) {
			ifs.read(&temp_ch, 1);
			ifs.read(sz_temp.ch, sizeof(sz_temp));
			if(ifs.eof()) {
			    return;
			}
			temp_size = sz_temp.bytes;
			int bytes_to_read = temp_size / 8;
			if (temp_size % 8 > 0) {
				++bytes_to_read;
			}
			Code temp_cd;
			for (int i = 0; i < bytes_to_read && temp_size >= 8;
				++i, temp_size -= 8) {
				ifs.read(buf.ch, 1);
				temp_cd.pushByteHigh(buf.bytes);
			}
			if (temp_size > 0) {
				ifs.read(buf.ch, 1);
				temp_cd.concatHighBits(buf.bytes, temp_size);
			}
			if(temp_ch == 'z') {
			    std::cout << "current code: " << temp_cd.toStr() << std::endl;
			}
			table_[temp_ch] = temp_cd;
			traverseAndGenerateTree(huffman_tree_, temp_cd, temp_ch);
		}
	}
	const BiTree *getHuffmanTree() {
        return huffman_tree_;
	}

private:
	struct MyCmp {
		bool operator()(const BiTree* t1, const BiTree* t2) {
			return t1->getData() > t2->getData();
		}
	};
	using BitreeHeap =
		std::priority_queue<BiTree*, std::vector<BiTree*>, MyCmp>;
	std::vector<BiTree*> bitrees_;
	TableMap table_;
	BitreeHeap q;
    BiTree *huffman_tree_ = nullptr;
	void traverseTree(BiTree* bt, Code code) {
		if (!bt->isLeaf()) {
			traverseTree(bt->getLeft(), code.afterPushBitHigh(0));
			traverseTree(bt->getRight(), code.afterPushBitHigh(1));
			return;
		}
		table_[bt->getData().ch] = code;
	}
	void traverseAndGenerateTree(BiTree *bt, Code code, char ch) {
	    if(code.getSize() == 0) {
	        bt->setData(Node(ch, 0));
	        return;
	    }
	    if(code.getBit(0)) {
	        if(!bt->getRight()) {
	            bt->setRight(new BiTree(Node(0, 0), nullptr, nullptr));
	        }
	        code.popLowBit();
	        traverseAndGenerateTree(bt->getRight(), code, ch);
	    }
	    else {
            if(!bt->getLeft()) {
                bt->setLeft(new BiTree(Node(0, 0), nullptr, nullptr));
            }
            code.popLowBit();
            traverseAndGenerateTree(bt->getLeft(), code, ch);
	    }
	}
};
#endif
