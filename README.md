# AVLtree
C++ implementation of AVL binary search tree.

All operations take O(logn) time.

Interface:

`void insert(int val)` inserts a new `val` to the tree. Does nothing if the key is already in the tree.

`void remove(int val)` removes `val`. Does nothing if there is no such key in the tree.

`bool search(int val)` returns true if val is in tree.

`ll sum(int l, int r)` return a sum of all elements in a range `[l,r]`.

`void print_tree(bool print_height = false)` print a tree keys in the console. If `print_height == true` prints heights of the nodes.


