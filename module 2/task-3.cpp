#include <iostream>
#include <set>
#include <cassert>

template <class Tree>

size_t GetDepth(const Tree &);

template <class T>

struct IsLess {
public:
    bool operator()(const T &one, const T &other) {
        return one < other;
    }
};

template <class Key, class Prior, class KeyCompr = IsLess<Key>, class PriorCompr = IsLess<Prior>>

class Treap {
private:
    struct Node {
        Node* left;
        Node* right;
        Node* parent;
        Key prior;
        Prior key;
        bool IsNil();
    };
    KeyCompr key_compr;
    PriorCompr prior_compr;
    Node* head;
    size_t num_of_elements;
    void split(Node*, const Key&, Node*&, Node*&);
    Node* merge(Node*, Node*);
    void find_head(Node*);
    template <class Tree>
    friend size_t GetDepth(const Tree&);
    Node* CopyNode(Node *, Node *);
public:
    Treap();
    Treap(const Treap &);
    Treap(Treap &&) noexcept;
    Treap& operator = (const Treap &);
    Treap& operator = (Treap &&) noexcept;
    void Insert(const Key&, const Prior&);
    void Remove(const Key&);
    void Clear();
    ~Treap();
};

template <class Key, class Prior, class KeyCompr, class PriorCompr>

bool Treap<Key, Prior, KeyCompr, PriorCompr>::Node::IsNil() {
    return (!left && !right);
}

template <class Key, class Prior, class KeyCompr, class PriorCompr>

Treap<Key, Prior, KeyCompr, PriorCompr>::Treap() {
    num_of_elements = 0;
    head = nullptr;
}

template <class Key, class Prior, class KeyCompr, class PriorCompr>

Treap<Key, Prior, KeyCompr, PriorCompr>::Treap(const Treap<Key, Prior, KeyCompr, PriorCompr> &obj) {
    num_of_elements = obj.num_of_elements;
    if (num_of_elements)
        head = CopyNode(obj.head, head);
}

template <class Key, class Prior, class KeyCompr, class PriorCompr>

Treap<Key, Prior, KeyCompr, PriorCompr>::Treap(Treap<Key, Prior, KeyCompr, PriorCompr> &&obj) noexcept {
    num_of_elements = obj.num_of_elements;
    head = obj.head;
    obj.head = nullptr;
    obj.num_of_elements = 0;
}

template <class Key, class Prior, class KeyCompr, class PriorCompr>

typename Treap<Key, Prior, KeyCompr, PriorCompr>::Node* Treap<Key, Prior, KeyCompr, PriorCompr>::CopyNode(Node *from, Node *to) {
    to = new Node;
    to->key = from->key;
    to->prior = from->prior;
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

template <class Key, class Prior, class KeyCompr, class PriorCompr>

Treap<Key, Prior, KeyCompr, PriorCompr>&
Treap<Key, Prior, KeyCompr, PriorCompr>::operator = (const Treap &obj) {
    Treap<Key, Prior, KeyCompr, PriorCompr>* copy = new Treap<Key, Prior, KeyCompr, PriorCompr>(obj);
    *this = *copy;
    return *this;
}

template <class Key, class Prior, class KeyCompr, class PriorCompr>

Treap<Key, Prior, KeyCompr, PriorCompr>&
Treap<Key, Prior, KeyCompr, PriorCompr>::operator = (Treap &&obj) noexcept{
    Treap<Key, Prior, KeyCompr, PriorCompr>* copy = new Treap<Key, Prior, KeyCompr, PriorCompr>(std::move(obj));
    *this = *copy;
    return *this;
}

template <class Key, class Prior, class KeyCompr, class PriorCompr>

void Treap<Key, Prior, KeyCompr, PriorCompr>::split(typename Treap<Key, Prior, KeyCompr, PriorCompr>::Node* curr,
                                                    const Key& key,
                                                    typename Treap<Key, Prior, KeyCompr, PriorCompr>::Node*& T1,
                                                    typename Treap<Key, Prior, KeyCompr, PriorCompr>::Node*& T2) {
    if (!curr) {
        T1 = nullptr;
        T2 = nullptr;
        return;
    }
    else {
        if (key_compr(key, curr->key)) {
            split(curr->left, key, T1, curr->left);
            T2 = curr;
            if (T2->left)
                T2->left->parent = T2;
            if (T2->right)
                T2->right->parent = T2;
            if (T1) {
                if (T1->left)
                    T1->left->parent = T1;
                if (T1->right)
                    T1->right->parent = T1;
            }
            return;
        }
        else {
            split(curr->right, key, curr->right, T2);
            T1 = curr;
            if (T2) {
                if (T2->left)
                    T2->left->parent = T2;
                if (T2->right)
                    T2->right->parent = T2;
            }
            if (T1->left)
                T1->left->parent = T1;
            if (T1->right)
                T1->right->parent = T1;
            return;
        }
    }
}

template <class Key, class Prior, class KeyCompr, class PriorCompr>

typename Treap<Key, Prior, KeyCompr, PriorCompr>::Node*
    Treap<Key, Prior, KeyCompr, PriorCompr>::merge(typename Treap<Key, Prior, KeyCompr, PriorCompr>::Node* T1,
                                                   typename Treap<Key, Prior, KeyCompr, PriorCompr>::Node* T2) {
    if (!T1)
        return T2;
    if (!T2)
        return T1;
    if (prior_compr(T1->prior, T2->prior)) {
        T2->left = merge(T1, T2->left);
        if (T2->left)
            T2->left->parent = T2;
        return T2;
    }
    else {
        T1->right = merge(T1->right, T2);
        if (T1->right)
            T1->right->parent = T1;
        return T1;
    }
}

template <class Key, class Prior, class KeyCompr, class PriorCompr>

void Treap<Key, Prior, KeyCompr, PriorCompr>::find_head(Node *curr) {
    while(curr->parent)
        curr = curr->parent;
    head = curr;
}

template <class Key, class Prior, class KeyCompr, class PriorCompr>

void Treap<Key, Prior, KeyCompr, PriorCompr>::Insert(const Key &key, const Prior &prior) {
    Node* tmp = head;
    Node* par = nullptr;
    while (tmp) {
        if (prior_compr(tmp->prior,prior))
            break;
        if (key_compr(key, tmp->key)) {
            par = tmp;
            tmp = tmp->left;
        }
        else {
            par = tmp;
            tmp = tmp->right;
        }
    }
    if (!tmp) {
        tmp = new Node;
        tmp->prior = prior;
        tmp->key = key;
        tmp->left = nullptr;
        tmp->right = nullptr;
        tmp->parent = par;
        if (par) {
            if (key_compr(key, par->key))
                par->left = tmp;
            else
                par->right = tmp;
        }
        if (!num_of_elements)
            head = tmp;
        else
            find_head(tmp);
    }
    else {
        Node* T1;
        Node* T2;
        split(tmp, key, T1, T2);
        Node* new_node = new Node;
        if(par) {
            if(par->left == tmp)
                par->left = new_node;
            else
                par->right = new_node;
        }
        new_node->key = key;
        new_node->prior = prior;
        new_node->left = T1;
        new_node->right = T2;
        if (T1)
            T1->parent = new_node;
        if (T2)
            T2->parent = new_node;

        new_node->parent = par;
        find_head(new_node);
    }
    ++num_of_elements;
}

template <class Key, class Prior, class KeyCompr, class PriorCompr>

void Treap<Key, Prior, KeyCompr, PriorCompr>::Remove(const Key &key) {
    if (!num_of_elements)
        return;
    Node* tmp = head;
    while (tmp) {
        if (tmp->key == key)
            break;
        if (key_compr(key, tmp->key))
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }
    if (tmp) {
        Node* par = tmp->parent;
        Node* new_node = merge(tmp->left, tmp->right);
        if (par) {
            if (par->left == tmp)
                par->left = new_node;
            else
                par->right = new_node;
        }
        if (new_node->right)
            new_node->right->parent = new_node;
        if (new_node->left)
            new_node->left->parent = new_node;
        new_node->parent = par;
        tmp->left = nullptr;
        tmp->right = nullptr;
        tmp->parent = nullptr;
        delete tmp;
        find_head(new_node);
        --num_of_elements;
    }
}

template <class Key, class Prior, class KeyCompr, class PriorCompr>

void Treap<Key, Prior, KeyCompr, PriorCompr>::Clear() {
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

template <class Key, class Prior, class KeyCompr, class PriorCompr>

Treap<Key, Prior, KeyCompr, PriorCompr>::~Treap() {
    Clear();
}

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
    template <class Tree>
    friend size_t GetDepth(const Tree &);
public:
    BinarySearchTree();
    BinarySearchTree(const BinarySearchTree &);
    BinarySearchTree(BinarySearchTree &&) noexcept ;
    BinarySearchTree& operator = (const BinarySearchTree &);
    BinarySearchTree& operator = (BinarySearchTree &&) noexcept ;
    void Add(const T &);
    void Remove(const T &);
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

template<class Tree>

size_t GetDepth(const Tree &obj) {
    if (!obj.num_of_elements)
        return 0;
    typename Tree::Node* tmp = obj.head;
    size_t max_depth = 1;
    size_t tmp_depth = 1;
    std::set<typename Tree::Node*> visited;
    for (int i =0; i < obj.num_of_elements; ++i) {
        while (!tmp->IsNil()) {
            if (tmp->left && visited.find(tmp->left) == visited.end()) {
                tmp = tmp->left;
                ++tmp_depth;
            }
            else if ((!tmp->left || visited.find(tmp->left) != visited.end()) &&
                     tmp->right && visited.find(tmp->right) == visited.end()) {
                tmp = tmp->right;
                ++tmp_depth;
            }
            else if ((visited.find(tmp->left) != visited.end() || !tmp->left) &&
                     (visited.find(tmp->right) != visited.end() || !tmp->right))
                break;
        }
        visited.insert(tmp);
        if (max_depth < tmp_depth)
            max_depth = tmp_depth;
        tmp = tmp->parent;
        --tmp_depth;
    }
    return max_depth;
}

int main()
{
    Treap<int, int> treap;
    BinarySearchTree<int> tree;
    int num_of_operations;
    std::cin >> num_of_operations;
    assert(num_of_operations < 1000000);
    int op1, op2;
    for (int i = 0; i < num_of_operations; ++i) {
        std::cin >> op1 >> op2;
        treap.Insert(op1, op2);
        tree.Add(op1);
    }
    std::cout << abs(GetDepth(treap)- GetDepth(tree));
    return 0;
}