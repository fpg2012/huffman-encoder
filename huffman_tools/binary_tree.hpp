#ifndef BINARY_TREE_H
#define BINARY_TREE_H
template <typename T> class BinaryTree {
public:
	BinaryTree(T data, BinaryTree* left = nullptr, BinaryTree* right = nullptr,
		BinaryTree* parent = nullptr)
		: data_(data), left_(left), right_(right), parent_(parent) {}
	BinaryTree(const BinaryTree&) = default;
	~BinaryTree() {
		if(!left_)
			delete left_;
		if(!right_)
			delete right_;
	}
	BinaryTree* getLeft() const { return left_; }
	BinaryTree* getRight() const { return right_; }
	BinaryTree* getParent() const { return parent_; }
	T getData() const { return data_; }
	bool isLeaf() const { return left_ == nullptr && right_ == nullptr; }
	void setData(T data) { data_ = data; }
	void setParent(BinaryTree* parent) { parent_ = parent; }
	void setLeft(BinaryTree* bitr) {
		left_ = bitr;
		bitr->parent_ = this;
	}
	void setRight(BinaryTree* bitr) {
		right_ = bitr;
		bitr->parent_ = this;
	}
	bool operator<(const BinaryTree& bitr) const { return data_ < bitr.data_; }
	bool operator<=(const BinaryTree& bitr) const { return data_ <= bitr.data_; }
	bool operator>=(const BinaryTree& bitr) const { return data_ >= bitr.data_; }
	bool operator>(const BinaryTree& bitr) const { return data_ > bitr.data_; }

private:
	T data_;
	BinaryTree* left_ = nullptr, * right_ = nullptr, * parent_ = nullptr;
};
#endif
