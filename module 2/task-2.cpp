//Дано число N ≤ 104 и последовательность целых чисел из [-2^31..2^31] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root,
// если root→Key ≤ K, то узел K добавляется в правое поддерево root;
// иначе в левое поддерево root. Выведите элементы в порядке post-order (снизу вверх).
//Рекурсия запрещена.


#include <iostream>
#include <algorithm>
#include <cassert>
#include <set>

template <class T>

struct IsLess {
public:
    bool operator()(const T &one, const T &other) {
        return one < other;
    }
};

template <class T, class Comparator = IsLess<T>>

class BinarySearchTree {
private:
    struct Node {
        T value;
        Node* parent;
        Node* right;
        Node* left;
        bool IsNil();
    };
    Node *head;
    size_t num_of_elements;
    Comparator compr;
    Node* CopyNode(Node*, Node*);
public:
    BinarySearchTree();
    BinarySearchTree(const BinarySearchTree &);
    BinarySearchTree(BinarySearchTree &&) noexcept ;
    BinarySearchTree& operator = (const BinarySearchTree &);
    BinarySearchTree& operator = (BinarySearchTree &&) noexcept ;
    void Add(const T &);
    void Remove(const T &);
    void PostOrder();
    void Clear();
    size_t size() { return num_of_elements; };
    ~BinarySearchTree();
};

template <class T, class Comparator>

BinarySearchTree<T, Comparator>::BinarySearchTree() {
    num_of_elements = 0;
    head = nullptr;
}

template <class T, class Comparator>

BinarySearchTree<T, Comparator>::BinarySearchTree(const BinarySearchTree<T, Comparator> &obj) {
    num_of_elements = obj.num_of_elements;
    if (num_of_elements)
        head = CopyNode(obj.head, head);
}

template <class T, class Comparator>

BinarySearchTree<T, Comparator>::BinarySearchTree(BinarySearchTree &&obj) noexcept {
    num_of_elements = obj.num_of_elements;
    head = obj.head;
    obj.head = nullptr;
    obj.num_of_elements = 0;
}

template <class T, class Comparator>

BinarySearchTree<T, Comparator>& BinarySearchTree<T, Comparator>::operator=(const BinarySearchTree &obj) {
    BinarySearchTree<T, Comparator>* copy = new BinarySearchTree<T, Comparator>(obj);
    *this = *copy;
    return *this;
}

template <class T, class Comparator>

BinarySearchTree<T, Comparator>& BinarySearchTree<T, Comparator>::operator=(BinarySearchTree && obj) noexcept {
    BinarySearchTree<T, Comparator>* copy = new BinarySearchTree<T, Comparator>(std::move(obj));
    *this = *copy;
    return *this;
}

template <class T, class Comparator>

typename BinarySearchTree<T, Comparator>::Node* BinarySearchTree<T, Comparator>::CopyNode(Node *from, Node *to) {
    to = new Node;
    to->value = from->value;
    to->left = nullptr;
    to->right = nullptr;
    if (from->left) {
        to->left = CopyNode(from->left, to->left);
        to->left->parent = to;
    }
    if (from->right) {
        to->right = CopyNode(from->right, to->right);
        to->right->parent = to;
    }
    return to;
}

template <class T, class Comparator>

void BinarySearchTree<T, Comparator>::Add(const T &elem) {
    if (!num_of_elements) {
        head = new Node;
        head->left = nullptr;
        head->right = nullptr;
        head->parent = nullptr;
        head->value = elem;
        ++num_of_elements;
        return;
    }
    Node *tmp = head;
    while (tmp) {
        if (compr(elem, tmp->value)) {
            if (tmp->left)
                tmp = tmp->left;
            else {
                tmp->left = new Node;
                tmp->left->left = nullptr;
                tmp->left->right = nullptr;
                tmp->left->value = elem;
                tmp->left->parent = tmp;
                ++num_of_elements;
                return;
            }
        }
        else {
            if (tmp->right)
                tmp = tmp->right;
            else {
                tmp->right = new Node;
                tmp->right->left = nullptr;
                tmp->right->right = nullptr;
                tmp->right->value = elem;
                tmp->right->parent = tmp;
                ++num_of_elements;
                return;
            }
        }
    }
}

template <class T, class Comparator>

void BinarySearchTree<T, Comparator>::Remove(const T &elem) {
    Node *tmp = head;
    while (tmp) {
        if (tmp->value == elem)
            break;
        if (compr(elem, tmp->value))
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }
    if (!tmp)
        return;
    Node *suitable = tmp;
    if (suitable->left)
        suitable = suitable->left;
    while (suitable->right)
        suitable = suitable->right;
    if (suitable != head) {
        if (suitable->parent->right == suitable)
            suitable->parent->right = nullptr;
        else
            suitable->parent->left = nullptr;
        std::swap(suitable->value, tmp->value);
    }
    delete suitable;
    --num_of_elements;
}

template <class T, class Comparator>

void BinarySearchTree<T, Comparator>::PostOrder() {
    Node* tmp = head;
    std::set<Node*> visited;
    for (int i =0; i < num_of_elements; ++i) {
        while (!tmp->IsNil()) {
            if (tmp->left && visited.find(tmp->left) == visited.end())
                tmp = tmp->left;
            else if ((!tmp->left || visited.find(tmp->left) != visited.end()) &&
                    tmp->right && visited.find(tmp->right) == visited.end())
                tmp = tmp->right;
            else if ((visited.find(tmp->left) != visited.end() || !tmp->left) &&
                             (visited.find(tmp->right) != visited.end() || !tmp->right))
                break;
        }
        int val = tmp->value;
        visited.insert(tmp);
        std::cout << val << ' ';
        tmp = tmp->parent;
    }
}

template <class T, class Comparator>

void BinarySearchTree<T, Comparator>::Clear() {
    Node* tmp = head;
    while (num_of_elements) {
        while (!tmp->IsNil()) {
            if (tmp->left)
                tmp = tmp->left;
            else if (!tmp->left && tmp->right)
                tmp = tmp->right;
        }
        Node* to_delete = tmp;
        if (head != tmp) {
            tmp = tmp->parent;
            if (tmp->left == to_delete)
                tmp->left = nullptr;
            else
                tmp->right = nullptr;
        }
        delete to_delete;
        --num_of_elements;
    }
}

template <class T, class Comparator>

BinarySearchTree<T, Comparator>::~BinarySearchTree() {
    Clear();
}

template <class T, class Comparator>

bool BinarySearchTree<T, Comparator>::Node::IsNil() {
    if (!left && !right)
        return true;
    return false;
}

int main()
{
    BinarySearchTree<int> tree;
    size_t num_of_operands;
    std::cin >> num_of_operands;
    assert(num_of_operands <= 10000);
    int operand;
    for (int i = 0; i < num_of_operands; ++i) {
        std::cin >> operand;
        tree.Add(operand);
    }
    //tree.PostOrder();
    return 0;
}