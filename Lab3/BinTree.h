#pragma once
#include "LinkedList.h"
#include "DynamicArray.h"
#include <string>

typedef DynamicArray<char> String;

template<typename T>
struct Branch
{
	T data;
	int key;
	Branch<T>* left;
	Branch<T>* right;
};


/*
//proto
template <typename T>
class BinTree;

template<typename T>
class Branch
{
	friend BinTree;
public:
	//
private:
	T data;
	Branch* left;
	Branch* right;
};
*/

template<typename T>
class BinTree
{
	// base functions
	void insert(Branch<T>*& _branch, const T& item, int key) {
		if (!_branch) {
			_branch = new Branch<T>;
			_branch->data = item;
			_branch->key = key;
			_branch->left = 0;
			_branch->right = 0;
			return;
		}
		else {
			if (_branch->key > key) {
				insert(_branch->left, item, key);
			}
			else {
				insert(_branch->right, item, key);
			}
		}
	}

	Branch<T>* remove(Branch<T>*& _branch, int key) {
		if (_branch == NULL)
			return _branch;

		if (key == _branch->key) {

			Branch<T>* tempBranch;
			if (_branch->right == NULL)
				tempBranch = _branch->left;
			else {

				Branch<T>* ptr = _branch->right;
				if (ptr->left == NULL) {
					ptr->left = _branch->left;
					tempBranch = ptr;
				}
				else {

					Branch<T>* pmin = ptr->left;
					while (pmin->left != NULL) {
						ptr = pmin;
						pmin = ptr->left;
					}
					ptr->left = pmin->right;
					pmin->left = _branch->left;
					pmin->right = _branch->right;
					tempBranch = pmin;
				}
			}

			delete _branch;
			_branch = tempBranch;
			return tempBranch;
		}
		else if (key < _branch->key)
			_branch->left = remove(_branch->left, key);
		else
			_branch->right = remove(_branch->right, key);
		return _branch;
	}

	Branch<T>* find(Branch<T>*& _branch, int key) {
		if (_branch == NULL)
			return NULL; // empty

		if (key == _branch->key) {
			return _branch;
		}
		else if (key < _branch->key)
			return find(_branch->left, key);
		else
			return find(_branch->right, key);
		//return _branch;
	}

	// map
	void map(T(*func)(T), Branch<T>* _branch) {
		_branch->data = func(_branch->data);
		if (_branch->left == nullptr && _branch->right == nullptr) {
			return;
		}
		if (_branch->left != nullptr) {
			map(func, _branch->left);
		}
		if (_branch->right != nullptr) {
			map(func, _branch->right);
		}
	}

	// where
	void WHERE(std::function<int(T)> func, Branch<T>*& _branch) {
		if (!func(_branch->data)) {
			_branch = remove(_branch, _branch->key);
			if (!_branch) return;
			WHERE(func, _branch);
			return;
		}
		
		if (_branch->left == nullptr && _branch->right == nullptr) {
			return;
		}
		if (_branch->left != nullptr) {
			WHERE(func, _branch->left);
		}
		if (_branch->right != nullptr) {
			WHERE(func, _branch->right);
		}
	}

	// 1.3.1
	// fixed LRR
	void tothread(Branch<T>*& _branch, LinkedList<Branch<T>>& list, String method = 0) {
		if (!_branch) return;
		tothread(_branch->left, list);
		list.Append(*_branch);
		tothread(_branch->right, list);
		return;
	}

	// fixed LRR
	void toList(Branch<T>*& _branch, LinkedList<T>& list, String method = 0) {
		if (!_branch) return;
		toList(_branch->left, list);
		list.Append(_branch->data);
		toList(_branch->right, list);
		return;
	}

	Branch<T>* findsubtree(Branch<T>*& _branch, Branch<T>*& subbranch) {
		if (_branch == NULL)
			return NULL; // empty
		if (subbranch == NULL)
			return NULL; // empty

		if (subbranch->key == _branch->key) {
			if (!findsubtree(_branch->left, subbranch->left)) {
				return NULL;
			}
			if (!findsubtree(_branch->right, subbranch->right)) {
				return NULL;
			}
			return _branch;
		}
		else if (subbranch->key < _branch->key)
			_branch->left = findsubtree(_branch->left, subbranch);
		else
			_branch->right = findsubtree(_branch->right, subbranch);
		//return _branch;
	}

	// copy
	void copy(const Branch<T>* s_branch, Branch<T>*& _branch) {
		if (!s_branch) return;
		_branch = new Branch<T>;
		_branch->data = s_branch->data;
		_branch->key = s_branch->key;
		_branch->left = 0;
		_branch->right = 0;
		copy(s_branch->left, _branch->left);
		copy(s_branch->right, _branch->right);
		return;
	}

	// print
	void print(Branch<T>*& _branch, size_t tabs) {
		if (!_branch) return;
		tabs += 5;
		print(_branch->left, tabs);
		for (size_t i = 0; i < tabs; i++) std::cout << " ";
		std::cout << _branch->data << std::endl;
		print(_branch->right, tabs);
		tabs -= 5;
		return;
	}

	// clear branch
	void clear(Branch<T>*& _branch) {
		if (_branch != NULL) {
			if (_branch->left != NULL) clear(_branch->left);
			if (_branch->right != NULL) clear(_branch->right);
			delete _branch;
			_branch = 0;
		}
	}

public:
	// constructors --------------------------------------------------------
	
	BinTree() {
	}

	BinTree(const T& item) {
		branch = new Branch<T>;
		branch->data = item;	
		branch->left = 0;
		branch->right = 0;
	}
	
	BinTree(const BinTree<T>& bintree) : BinTree() {
		copy(bintree.branch, branch);
	}
	
	// functions -----------------------------------------------------------
	// base functions
	void Insert(const T& item, int key) { insert(branch, item, key); }
	
	void Remove(int key) {
		if (!branch) {
			return;
		}
		remove(branch, key);
	}

	T Find(int key) {
		if (!branch) {
			//std::cout << "дерево пусто" << std::endl;
			return NULL;
		}
		BinTree<T> binTree = BinTree<T>();
		copy(find(branch, key), binTree.branch);
		if (!binTree.branch)
			return NULL;
		return binTree.branch->data;
	}

	// fixed LRR
	LinkedList<T> toList() {
		LinkedList<T> list = LinkedList<T>();
		if (!branch) {
			std::cout << "дерево пусто" << std::endl;
			return list;
		}
		toList(branch, list);
		return list;
	}

	LinkedList<Branch<T>> toThread() {
		LinkedList<Branch<T>> list = LinkedList<Branch<T>>();
		if (!branch) {
			std::cout << "дерево пусто" << std::endl;
			return list;
		}
		tothread(branch, list);
		return list;
	}

	//
	// fixed LRR
	std::string toString() {
		std::string str;
		if (!branch) {
			std::cout << "дерево пусто" << std::endl;
			return str;
		}
		LinkedList<Branch<T>> list = this->toThread();
		for (int i = 0; i < list.GetLength(); i++) {
			str += "(" + std::to_string(i) + ", " + std::to_string(list.Get(i).key) + ")";
			if (i < list.GetLength() - 1) {
				str += ', ';
			}
		}
		return str;
	}

	// 1.2
	BinTree<T> Map(T(*func)(T)) {
		BinTree<T> binTree = BinTree();
		if (!branch) {
			//std::cout << "дерево пусто" << std::endl;
			return binTree;
		}
		copy(branch, binTree.branch);
		map(func, binTree.branch);
		return binTree;
	}

	// int(*func)(T)
	BinTree<T> Where(std::function<int(T)> func) {
		BinTree<T> binTree = BinTree();
		if (!branch) {
			//std::cout << "дерево пусто" << std::endl;
			return binTree;
		}
		copy(branch, binTree.branch);
		WHERE(func, binTree.branch);
		return binTree;
	}
	
	T Reduce(T(*func)(T, T), const T& start) {
		LinkedList<T> list = LinkedList<T>();
		if (!branch) {
			//std::cout << "дерево пусто" << std::endl;
			return 0;
		}
		toList(branch, list);

		T res;
		for (size_t i = 0; i < list.GetLength(); i++) {
			if (i == 0) {
				res = func(list.Get(i), start);
			}
			else {
				res = func(list.Get(i), res);
			}
		}
		return res;
	}

	/*
	// 1.4.1 fixed LRR
	String toString() {
		String string = String();
		if (!branch) {
			std::cout << "дерево пусто" << std::endl;
			return string;
		}
		LinkedList<T> list = this->toList();
		string.Resize(list.GetLength());
		for (size_t i = 0; i < list.GetLength(); i++) {
			string.Set('0' + list[i], i);
		}
		return string;
	}
	*/

	// 1.5
	BinTree<T> GetSubTree(int key) {
		BinTree<T> binTree = BinTree<T>();
		if (!branch) {
			//std::cout << "дерево пусто" << std::endl;
			return binTree;
		}
		copy(find(branch, key), binTree.branch);
		return binTree;
	}

	// 1.6
	bool FindSubTree(BinTree<T>& subtree) {
		if (!branch) {
			//std::cout << "дерево пусто" << std::endl;
			return false;
		}
		if (!findsubtree(this->branch, subtree.branch)) {
			return true;
		}
		return false;
	}

	BinTree<T> Copy() {
		BinTree<T> binTree = BinTree<T>();
		copy(branch, binTree.branch);
		return binTree;
	}

	void Print() {
		if (!branch) { 
			std::cout << "дерево пусто" << std::endl;
			return;
		}
		std::cout << "дерево:" << std::endl;
		print(branch, 0);
	}

	~BinTree()
	{
		if (this->branch != NULL) {
			clear(this->branch);
		}
	}

private:
	Branch<T>* branch;
};