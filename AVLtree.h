#include <iostream>
#include <tuple>

typedef long long ll;

class AVLtree {

    struct Node {
        Node(int val) { key = val; sum = key; }

        int key;
        int height{ 1 };
        ll sum;
        Node* parent = nullptr, * left = nullptr, * right = nullptr;
    };

public:

    AVLtree() {
        root = nullptr;
    }
    ~AVLtree() {
        clear(root);
    }

    bool search(int val)
    {
        return find(root, val);
    }

    void print_tree(bool print_height = false) {
        print(root, "", false, print_height);
    }

    void insert(int val) {
        root = insert(root, val);
    }

    void remove(int val) {
        root = remove(root, val);
    }

    //void merge(AVLtree* tree) {
    //    if (tree == this)
    //        return;
    //    root = merge(root, tree->root);
    //    tree->root = nullptr;
    //    delete tree;
    //}

    ll sum(int l, int r) {

        bool first_split_root = find(root, l - 1);
        bool second_split_root = find(root, r + 1);

        auto first_split = split(root, l - 1);
        auto second_split = split(first_split.second, r + 1);

        ll result = sum(second_split.first);

        Node* second_merge = nullptr;
        if (second_split_root)
            second_merge = avl_merge_with_root(second_split.first, second_split.second, new Node(r + 1));
        else
            second_merge = merge(second_split.first, second_split.second);

        if (first_split_root)
            root = avl_merge_with_root(first_split.first, second_merge, new Node(l - 1));
        else
            root = merge(first_split.first, second_merge);

        return result;
    }

private:

    Node* root;

    int height(Node* node) {
        if (node)
            return node->height;
        return 0;
    }

    ll sum(Node* node) {
        if (node)
            return node->sum;
        return 0;
    }

    std::pair<Node*, Node*> split(Node* v, int val) {
        if (v == nullptr)
            return std::make_pair(nullptr, nullptr);
        if (val < v->key) {
            auto tmp_split = split(v->left, val);
            if (tmp_split.first) tmp_split.first->parent = nullptr;
            if (tmp_split.second) tmp_split.second->parent = nullptr;
            if (v->right) v->right->parent = nullptr;
            Node* tmp_v = avl_merge_with_root(tmp_split.second, v->right, v);
            return std::make_pair(tmp_split.first, tmp_v);
        }
        else if (val > v->key) {
            auto tmp_split = split(v->right, val);
            if (tmp_split.first) tmp_split.first->parent = nullptr;
            if (tmp_split.second) tmp_split.second->parent = nullptr;
            if (v->left) v->left->parent = nullptr;
            Node* tmp_v = avl_merge_with_root(v->left, tmp_split.first, v);
            return std::make_pair(tmp_v, tmp_split.second);
        }
        else {
            return std::make_pair(v->left, v->right);
        }
    }

    Node* merge(Node* v1, Node* v2) {
        if (v1 == nullptr)
            return v2;
        if (v2 == nullptr)
            return v1;
        Node* max_node = new Node(get_max(v1)->key);
        v1 = remove(v1, max_node->key);
        return avl_merge_with_root(v1, v2, max_node);
    }

    Node* avl_merge_with_root(Node* v1, Node* v2, Node* new_root) {
        if (abs(height(v1) - height(v2)) <= 1) {
            new_root = merge_with_root(v1, v2, new_root);
            update(new_root);
            return new_root;
        }
        else if (height(v1) > height(v2)) {
            Node* tmp_root = avl_merge_with_root(v1->right, v2, new_root);
            v1->right = tmp_root;
            tmp_root->parent = v1;
            return rebalance_tree(v1);
        }
        else {
            Node* tmp_root = avl_merge_with_root(v1, v2->left, new_root);
            v2->left = tmp_root;
            tmp_root = v2;
            return rebalance_tree(v2);
        }
    }

    Node* merge_with_root(Node* v1, Node* v2, Node* new_root) {
        new_root->left = v1;
        new_root->right = v2;
        if (v1) v1->parent = new_root;
        if (v2) v2->parent = new_root;
        return new_root;
    }

    void clear(Node* node)
    {
        if (node)
        {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    void print(Node* node, const std::string& prefix, bool is_left, bool print_height = false)
    {
        if (node == nullptr)
            return;

        std::cout << prefix;

        std::cout << (is_left ? "├──" : "└──");

        int value = print_height ? height(node) : node->key;
        std::cout << value << std::endl;

        if (node->left)
            print(node->left, prefix + (is_left ? "│   " : "    "), true, print_height);
        if (node->right)
            print(node->right, prefix + (is_left ? "│   " : "    "), false, print_height);
    }

    Node* find(Node* node, int val)
    {
        if (node == nullptr)
            return nullptr;
        if (node->key == val)
            return node;
        if (node->left && node->key > val)
            return find(node->left, val);
        if (node->right && node->key < val)
            return find(node->right, val);
        return nullptr;
    }

    Node* insert(Node* node, int val)
    {
        if (node == nullptr) {
            node = new Node(val);
        }
        else if (node->key > val) {
            node->left = insert(node->left, val);
            node->left->parent = node;
        }
        else if (node->key < val) {
            node->right = insert(node->right, val);
            node->right->parent = node;
        }
        update(node);
        node = rebalance_tree(node);
        return node;
    }

    Node* remove(Node* node, int val) {
        if (node == nullptr) {
            return nullptr;
        }
        else if (node->key > val) {
            node->left = remove(node->left, val);
        }
        else if (node->key < val) {
            node->right = remove(node->right, val);
        }
        else if (node->left && node->right) {
            node->key = get_max(node->left)->key;
            node->left = remove(node->left, node->key);
        }
        else
        {
            Node* tmp = node;
            node = node->left ? node->left : node->right;
            delete tmp;
            return node;
        }
        update(node);
        node = rebalance_tree(node);
        return node;
    }

    Node* get_min(Node* node) {
        if (node == nullptr)
            return nullptr;
        return node->left ? get_min(node->left) : node;
    }

    Node* get_max(Node* node) {
        if (node == nullptr)
            return nullptr;
        return node->right ? get_max(node->right) : node;
    }

    void update(Node* node) {
        update_height(node);
        update_sum(node);
    }

    void update_height(Node* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    void update_sum(Node* node) {
        node->sum = sum(node->left) + sum(node->right) + (ll)node->key;
    }

    Node* rebalance_tree(Node* node) {
        Node* new_root = node;
        if (abs(height(node->left) - height(node->right)) > 1)
        {
            if (height(node->left) < height(node->right) && height(node->right->left) <= height(node->right->right))
                new_root = rotate_left(node, node->right);
            else if (height(node->left) < height(node->right) && height(node->right->left) > height(node->right->right))
                new_root = rotate_right_left(node, node->right);
            else if (height(node->right) < height(node->left) && height(node->left->right) <= height(node->left->left))
                new_root = rotate_right(node, node->left);
            else if (height(node->right) < height(node->left) && height(node->left->right) > height(node->left->left))
                new_root = rotate_left_right(node, node->left);
        }
        update(node);
        return new_root;
    }

    Node* rotate_left(Node* x, Node* z)
    {
        x->right = z->left;
        if (z->left) z->left->parent = x;
        z->left = x;
        z->parent = x->parent;
        x->parent = z;
        update(x);
        update(z);
        return z;
    }

    Node* rotate_right_left(Node* x, Node* z)
    {
        Node* y = z->left;
        z->left = y->right;
        if (y->right) y->right->parent = z;

        y->right = z;
        z->parent = y;

        x->right = y->left;
        if (y->left) y->left->parent = x;

        y->left = x;
        x->parent = y;

        update(x);
        update(z);
        update(y);

        return y;
    }

    Node* rotate_right(Node* x, Node* z)
    {
        x->left = z->right;
        if (z->right) z->right->parent = x;
        z->right = x;
        z->parent = x->parent;
        x->parent = z;
        update(x);
        update(z);
        return z;
    }

    Node* rotate_left_right(Node* x, Node* z)
    {
        Node* y = z->right;
        z->right = y->left;
        if (y->left) y->left->parent = z;

        y->left = z;
        z->parent = y;

        x->left = y->right;
        if (y->right) y->right->parent = x;

        y->right = x;
        x->parent = y;

        update(x);
        update(z);
        update(y);

        return y;
    }

};