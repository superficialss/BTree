#pragma once

template<typename K, size_t M = 3>
struct BTreeNode
{
	K _keys[M];							   //关键字
	BTreeNode<K, M>* _Subs[M + 1];         //孩子
	BTreeNode<K, M>* _parent;              //父亲
	size_t _size;						   //关键字的个数

	BTreeNode()
	{
		memset(_Subs, 0, sizeof(_Subs));
		_parent = NULL;
		_size = 0;
	}
};

template<typename K, size_t M = 3>
class BTree
{
	typedef BTreeNode<K, M> Node;
public:
	BTree()
		:_root(NULL)
	{}

	pair<Node*, int> Find(const K& key)
	{
		Node* cur = _root;
		Node* parent = NULL;
		size_t i = 0;
		while (cur)
		{
			for (i = 0; i < cur->_size; ++i)
			{
				if (cur->_keys[i] < key)
				{
					++i;
				}
				else if(cur->_keys[i] > key)
				{
					break;
				}
				else
				{
					return make_pair(cur, i);
				}
			}
			parent = cur;
			cur = cur->_Subs[i];
		}
		return make_pair(parent, -1);
	}

	void InsertKey(Node* cur, const K& key, Node* Sub)
	{
		assert(cur);
		int end = cur->_size - 1;
		for (; end >= 0; --end)
		{
			if (cur->_keys[end] < key)
			{
				break;
			}
			else
			{
				cur->_keys[end + 1] = cur->_keys[end];
				cur->_Subs[end + 2] = cur->_Subs[end + 1];
			}
		}
		cur->_keys[end + 1] = key;
		cur->_Subs[end + 2] = Sub;
		if (Sub)
		{
			Sub->_parent = cur;
		}
		cur->_size++;
	}

	bool Insert(const K& key)
	{
		if (_root == NULL)
		{
			_root = new Node;
			_root->_keys[0] = key;
			_root->_size = 1;
			return true;
		}
		pair<Node*, int> ret = Find(key);
		if (ret.second >= 0)
		{
			return false;
		}
		Node* cur = ret.first;
		K NewKey = key;
		Node* Sub = NULL;
		while(1)
		{
			InsertKey(cur, NewKey, Sub);
			if (cur->_size < M)
			{
				return true;
			}
			Node* NewNode = new Node;
			size_t index = 0;
			size_t mid = M / 2;
			size_t i = mid + 1;
			for (; i < cur->_size; ++i)
			{
				NewNode->_keys[index] = cur->_keys[i];
				NewNode->_Subs[index] = cur->_Subs[i];
				if (cur->_Subs[i])
				{
					cur->_Subs[i]->_parent = NewNode;
				}
				++index;
				NewNode->_size++;
			}
			NewNode->_Subs[index] = cur->_Subs[i];
			if (cur->_Subs[i])
			{
				cur->_Subs[i]->_parent = NewNode;
			}
			cur->_size = cur->_size - NewNode->_size - 1;
			if (cur->_parent == NULL)
			{
				_root = new Node;
				_root->_keys[0] = cur->_keys[mid];
				_root->_Subs[0] = cur;
				_root->_Subs[1] = NewNode;
				cur->_parent = _root;
				NewNode->_parent = _root;
				_root->_size = 1;
			}
			else
			{
				NewKey = cur->_keys[mid];
				cur = cur->_parent;
				Sub = NewNode;
			}
		}
	}

	void InOrder()
	{
		return _InOrder(_root);
		cout << endl;
	}

protected:
	void _InOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		for (size_t i = 0; i < root->_size; ++i)
		{
			if (root->_Subs[i])
			{
				_InOrder(root->_Subs[i]);
			}
			cout << root->_keys[i] << " ";
		}
	}

protected:
	Node* _root;
};

void TestBTree()
{
	int a[] = { 53, 75, 139, 49, 145, 36, 101 };
	BTree<int> bt;
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		bt.Insert(a[i]);
	}
	bt.InOrder();
}