#ifndef CODE_H
#define CODE_H
#include <bitset>
#include <string>
class Code {
public:
	Code() = default;
	Code(const Code& code) = default;
	Code pushBit(uint8_t lowbit) {
		bits_ <<= 1;
		bits_.set(0, lowbit);
		++size_;
		return *this;
	}
	Code pushBitHigh(uint8_t highbit) {
	    if(highbit) {
            bits_.flip(size_);
	    }
        ++size_;
	    return *this;
	}
	Code afterPushBitHigh(uint8_t highbit) {
	    Code temp(*this);
	    return temp.pushBitHigh(highbit);
	}
	Code afterPushBit(uint8_t lowbit) {
		Code temp(*this);
		return temp.pushBit(lowbit);
	}
	void pushByte(uint8_t lowbyte) {
		bits_ <<= 8;
		bits_ |= lowbyte;
		size_ += 8;
	}
	void pushByteHigh(uint8_t highbyte) {
	    for(int i = 0;i < 8;++i) {
	        pushBitHigh(highbyte & 1u);
	        highbyte >>= 1u;
	    }
	}
	size_t getSize() const { return size_; }
	bool getBit(size_t pos) const { return bits_.test(pos); }
	std::string toStr() const {
		std::string temp;
		for (int i = size_ - 1; i >= 0; --i) {
			temp += ('0' + getBit(i));
		}
		return temp;
	}
	uint8_t getLowByte() {
		uint8_t temp = 0;
		for (int i = 7; i >= 0; --i) {
			temp <<= 1;
			if (getBit(i)) {
				temp += 1;
			}
		}
		return temp;
	}
	void popLowByte() {
		bits_ >>= 8;
		if (size_ < 8) {
			size_ = 0;
		}
		else {
			size_ -= 8;
		}
	}
	void popLowBit() {
		bits_ >>= 1;
		size_ -= 1;
	}
	void shiftLeft(int bits) {
		bits_ <<= bits;
	}
	void shiftRight(int bits) {
		bits_ >>= bits;
	}
	void concatLowBits(uint8_t bits, int bits_sz) {
		bits_ <<= bits_sz;
		bits_ |= bits;
		size_ += bits_sz;
	}
    void concatHighBits(uint8_t bits, int bits_sz) {
        for(int i = 0;i < bits_sz;++i) {
            pushBitHigh(bits & 1u);
            bits >>= 1;
        }
    }
private:
	size_t size_ = 0;
	std::bitset<256> bits_ = 0u;
};
#endif
