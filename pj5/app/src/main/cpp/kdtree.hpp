#include <tuple>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stdexcept>

/**
 * An abstract template base of the KDTree class
 */
template<typename...>
class KDTree;

/**
 * A partial template specialization of the KDTree class
 * The time complexity of functions are based on n and k
 * n is the size of the KDTree
 * k is the number of dimensions
 * @typedef Key         key type
 * @typedef Value       value type
 * @typedef Data        key-value pair
 * @static  KeySize     k (number of dimensions)
 */
template<typename ValueType, typename... KeyTypes>
class KDTree<std::tuple<KeyTypes...>, ValueType> {
public:
    typedef std::tuple<KeyTypes...> Key;
    typedef ValueType Value;
    typedef std::pair<const Key, Value> Data;
    static inline constexpr size_t KeySize = std::tuple_size<Key>::value;
    static_assert(KeySize > 0, "Can not construct KDTree with zero dimension");
protected:
    struct Node {
        Data data;
        Node *parent;
        Node *left = nullptr;
        Node *right = nullptr;

        Node(const Key &key, const Value &value, Node *parent) : data(key, value), parent(parent) {}

        const Key &key() { return data.first; }

        Value &value() { return data.second; }
    };

public:
    /**
     * A bi-directional iterator for the KDTree
     * ! ONLY NEED TO MODIFY increment and decrement !
     */
    class Iterator {
    private:
        KDTree *tree;
        Node *node;

        Iterator(KDTree *tree, Node *node) : tree(tree), node(node) {}

        /**
         * Increment the iterator
         * Time complexity: O(log n)
         */
        void increment() {
            if (node->right){
                node=node->right;
                while (node->left){
                    node=node->left;
                }
            }
            else{
                while (node->parent){
                    if (node->parent->left == node){
                        node = node->parent;
                        return;
                    }
                    node = node->parent;
                }
                if (!node->parent)
                    node = nullptr;
            }
        }

        /**
         * Decrement the iterator
         * Time complexity: O(log n)
         */
        void decrement() {
            if(node->left){
                node=node->left;
                while(node->right)
                    node=node->right;
            }
            else{
                while(node->parent && node->parent->right != node)
                    node=node->parent;
                if(!node->parent)
                    node= nullptr;
                else
                    node=node->parent;
            }
        }

    public:
        friend class KDTree;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            increment();
            return temp;
        }

        Iterator &operator--() {
            decrement();
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            decrement();
            return temp;
        }

        bool operator==(const Iterator &that) const {
            return node == that.node;
        }

        bool operator!=(const Iterator &that) const {
            return node != that.node;
        }

        Data *operator->() {
            return &(node->data);
        }

        Data &operator*() {
            return node->data;
        }
    };

protected:                      // DO NOT USE private HERE!
    Node *root = nullptr;       // root of the tree
    size_t treeSize = 0;        // size of the tree

    /**
     * Find the node with key
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param node
     * @return the node with key, or nullptr if not found
     */
    template<size_t DIM>
    Node *find(const Key &key, Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node)
            return nullptr;
        if (key==node->key())
            return node;
        if (std::less<>{}(std::get<DIM>(key), std::get<DIM>(node->key())))
            return find<DIM_NEXT>(key, node->left);
        else
            return find<DIM_NEXT>(key, node->right);
    }

    /**
     * Insert the key-value pair, if the key already exists, replace the value only
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param value
     * @param node
     * @param parent
     * @return whether insertion took place (return false if the key already exists)
     */
    template<size_t DIM>
    bool insert(const Key &key, const Value &value, Node *&node, Node *parent) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node){
            node = new Node(key, value, parent);
            treeSize++;
            return 1;
        }
        if (key==node->key()){
            node->data.second=value;
            return 0;
        }
        else if (std::less<>{}(std::get<DIM>(key),std::get<DIM>(node->key())))
            return insert<DIM_NEXT>(key, value, node->left, node);
        else
            return insert<DIM_NEXT>(key, value, node->right, node);
    }

    /**
     * Compare two keys on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return relationship of two keys on a dimension with the compare function
     */
    template<size_t DIM, typename Compare>
    static bool compareKey(const Key &a, const Key &b, Compare compare = Compare()) {
        if (std::get<DIM>(a) != std::get<DIM>(b)){
            return compare(std::get<DIM>(a), std::get<DIM>(b));
        }
        return compare(a, b);
    }

    /**
     * Compare two nodes on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return the minimum / maximum of two nodes
     */
    template<size_t DIM, typename Compare>
    static Node *compareNode(Node *a, Node *b, Compare compare = Compare()) {
        if (!a) return b;
        if (!b) return a;
        return compareKey<DIM, Compare>(a->key(), b->key(), compare) ? a : b;
    }

    /**
     * Find the minimum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the minimum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMin(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node)
            return nullptr;
        Node* min=findMin<DIM_CMP, DIM_NEXT>(node->left);
        if (DIM_CMP!=DIM){
            Node* rightMin=findMin<DIM_CMP, DIM_NEXT>(node->right);
            min=compareNode<DIM_CMP, std::less<>>(min, rightMin);
        }
        return compareNode<DIM_CMP, std::less<>>(min, node);
    }

    /**
     * Find the maximum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the maximum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMax(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node)
            return nullptr;
        Node* max=findMax<DIM_CMP, DIM_NEXT>(node->right);
        if (DIM_CMP!=DIM){
            Node* leftMax=findMax<DIM_CMP, DIM_NEXT>(node->left);
            max=compareNode<DIM_CMP, std::greater<>>(max, leftMax);
        }
        return compareNode<DIM_CMP, std::greater<>>(max, node);
    }

    template<size_t DIM>
    Node *findMinDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMin<DIM, 0>(root);
        return findMinDynamic<DIM_NEXT>(dim);
    }

    template<size_t DIM>
    Node *findMaxDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMax<DIM, 0>(root);
        return findMaxDynamic<DIM_NEXT>(dim);
    }

    /**
     * Erase a node with key (check the pseudocode in project description)
     * Time Complexity: max{O(k log n), O(findMin)}
     * @tparam DIM current dimension of node
     * @param node
     * @param key
     * @return nullptr if node is erased, else the (probably) replaced node
     */
    template<size_t DIM>
    Node *erase(Node *node, const Key &key) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node)
            return nullptr;
        if (key==node->key()){
            if (!node->left && !node->right){
                delete node;
                treeSize--;
                return nullptr;
            }
            else if (node->right){
                Node* minNode=findMin<DIM, DIM_NEXT>(node->right);
                Key &temp=const_cast<Key &>(node->data.first);
                temp=minNode->key();
                node->data.second=minNode->value();
                node->right=erase<DIM_NEXT>(node->right, minNode->key());
            }
            else if (node->left){
                Node* maxNode=findMax<DIM, DIM_NEXT>(node->left);
                Key &temp=const_cast<Key &>(node->data.first);
                temp=maxNode->key();
                node->data.second=maxNode->value();
                node->left=erase<DIM_NEXT>(node->left, maxNode->key());
            }
        }
        else{
            if (std::less<>{}(std::get<DIM>(key), std::get<DIM>(node->key())))
                node->left=erase<DIM_NEXT>(node->left, key);
            else
                node->right=erase<DIM_NEXT>(node->right, key);
        }
        return node;
    }

    template<size_t DIM>
    Node *eraseDynamic(Node *node, size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return erase<DIM>(node, node->key());
        return eraseDynamic<DIM_NEXT>(node, dim);
    }

    template<size_t DIM>
    static bool nodeCmp(const std::pair<Key,Value> &node1, const std::pair<Key,Value> &node2){
        return compareKey<DIM,std::less<>>(node1.first,node2.first);
    }

    static bool nodeEqual(const std::pair<Key,Value> &node1, const std::pair<Key,Value> &node2){
        return node1==node2;
    }

    template<size_t DIM>
    void balancedInsert(std::vector<std::pair<Key, Value>>& v){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (v.empty())
            return ;
        auto nth=v.begin() + (v.size()-1)/2;
        std::nth_element(v.begin(), nth, v.end(), nodeCmp<DIM>);
        insert(nth->first, nth->second);
        std::vector<std::pair<Key, Value>> left(v.begin(), nth);
        std::vector<std::pair<Key, Value>> right(nth+1, v.end());
        balancedInsert<DIM_NEXT>(left);
        balancedInsert<DIM_NEXT>(right);
    }

    template <size_t DIM>
    void exCopyTree(Node *&thisNode, Node *thatNode, Node *parent){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        thisNode = new Node(thatNode->data.first, thatNode->data.second, parent);
        if (thatNode->left)
            exCopyTree<DIM_NEXT>(thisNode->left, thatNode->left, thisNode);
        if (thatNode->right)
            exCopyTree<DIM_NEXT>(thisNode->right, thatNode->right, thisNode);
    }

    void copyFrom(const KDTree &that){
        exCopyTree<0>(root, that.root, nullptr);
        treeSize=that.treeSize;
    }

    void destruct(Node* node){
        if (!node){
            return ;
        }
        if (node->left)
            destruct(node->left);
        if (node->right)
            destruct(node->right);
        delete node;
    }


    std::vector<Value> *addList(Node * node, std::vector<Value> * ans){
        if (!node)
            return ans;
        ans->push_back(node->value());
        ans=addList(node->left, ans);
        ans=addList(node->right, ans);
        return ans;
    }

    template<size_t DIM>//current dimension of the node
    std::vector<Value> *Range_helper(Node* node, Key key, int distance, Key treeRange[2][2], std::vector<Value> * ans) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node) return ans;//node is null
        if (std::get<0>(treeRange[0][0]) > std::get<0>(key)+distance //BB(T) does not overlap query range
            || std::get<0>(treeRange[0][1]) < std::get<0>(key)-distance
            || std::get<1>(treeRange[1][0]) > std::get<1>(key)+distance
            || std::get<1>(treeRange[1][1]) < std::get<1>(key)-distance){
            return ans;
        }
        if (std::get<0>(treeRange[0][0]) >= std::get<0>(key)-distance //BB(T) inside query range
            && std::get<0>(treeRange[0][1]) <= std::get<0>(key)+distance
            && std::get<1>(treeRange[1][0]) >= std::get<1>(key)-distance
            && std::get<1>(treeRange[1][1]) <= std::get<1>(key)+distance){
            return addList(node, ans);
        }

        if(std::get<0>(node->key()) >= std::get<0>(key)-distance //node->data->Key inside the query range
           && std::get<0>(node->key()) <= std::get<0>(key)+distance
           && std::get<1>(node->key()) >= std::get<1>(key)-distance
           && std::get<1>(node->key()) <= std::get<1>(key)+distance){
            ans->push_back(node->value());
        }

        if(node->left){
            Key leftTreeRange[2][2];
            leftTreeRange[0][0]=findMin<0,DIM_NEXT>(node->left)->key();
            leftTreeRange[0][1]=findMax<0,DIM_NEXT>(node->left)->key();
            leftTreeRange[1][0]=findMin<1,DIM_NEXT>(node->left)->key();
            leftTreeRange[1][1]=findMax<1,DIM_NEXT>(node->left)->key();
            ans=Range_helper<DIM_NEXT>(node->left, key, distance, leftTreeRange, ans);
        }
        if(node->right){
            Key rightTreeRange[2][2];
            rightTreeRange[0][0]=findMin<0,DIM_NEXT>(node->right)->key();
            rightTreeRange[0][1]=findMax<0,DIM_NEXT>(node->right)->key();
            rightTreeRange[1][0]=findMin<1,DIM_NEXT>(node->right)->key();
            rightTreeRange[1][1]=findMax<1,DIM_NEXT>(node->right)->key();
            ans=Range_helper<DIM_NEXT>(node->right, key, distance, rightTreeRange, ans);
        }
        return ans;
    }




public:
    KDTree() = default;

    /**
     * Time complexity: O(kn log n)
     * @param v we pass by value here because v need to be modified
     */
    explicit KDTree(std::vector<std::pair<Key, Value>> v) {
        if (v.empty())
            return ;
        std::stable_sort(v.begin(),v.end(), nodeCmp<0>);
        auto it=std::unique(v.rbegin(), v.rend(), nodeEqual);
        v.erase(v.begin(), it.base());
        balancedInsert<0>(v);
        treeSize=v.size();
    }

    /**
     * Time complexity: O(n)
     */
    KDTree(const KDTree &that) {
        if (!that.root){
            root=nullptr;
            treeSize=0;
        }
        else{
            copyFrom(that);
        }
    }

    /**
     * Time complexity: O(n)
     */
    KDTree &operator=(const KDTree &that) {
        if (root){
            destruct(root);
        }
        if (!that.root){
            root=nullptr;
            treeSize=0;
        }
        else{
            copyFrom(that);
        }
        return *this;
    }

    /**
     * Time complexity: O(n)
     */
    ~KDTree() {
        if (root){
            destruct(root);
        }
    }

    Iterator begin() {
        if (!root) return end();
        auto node = root;
        while (node->left) node = node->left;
        return Iterator(this, node);
    }

    Iterator end() {
        return Iterator(this, nullptr);
    }

    Iterator find(const Key &key) {
        return Iterator(this, find<0>(key, root));
    }

    void insert(const Key &key, const Value &value) {
        insert<0>(key, value, root, nullptr);
    }

    template<size_t DIM>
    Iterator findMin() {
        return Iterator(this, findMin<DIM, 0>(root));
    }

    Iterator findMin(size_t dim) {
        return Iterator(this, findMinDynamic<0>(dim));
    }

    template<size_t DIM>
    Iterator findMax() {
        return Iterator(this, findMax<DIM, 0>(root));
    }

    Iterator findMax(size_t dim) {
        return Iterator(this, findMaxDynamic<0>(dim));
    }

    bool erase(const Key &key) {
        auto prevSize = treeSize;
        erase<0>(root, key);
        return prevSize > treeSize;
    }

    Iterator erase(Iterator it) {
        if (it == end()) return it;
        auto node = it.node;
        if (!it.node->left && !it.node->right) {
            it.node = it.node->parent;
        }
        size_t depth = 0;
        auto temp = node->parent;
        while (temp) {
            temp = temp->parent;
            ++depth;
        }
        eraseDynamic<0>(node, depth % KeySize);
        return it;
    }

    size_t size() const { return treeSize; }

    /**
     * find the nodes within the distance around a certain point
     * @param key: the location of the point
     * @param distance: the distance
     * @return a vector contains the value of nodes that satisfy the requirement
     */
    std::vector<Value> *Range_Search(Key key, int distance) {
        std::vector<Value> * ans=new std::vector<Value>();
        if (!root)
            return ans;
        Key treeRange[2][2];
        treeRange[0][0]=findMin<0,0>(root)->key();
        treeRange[0][1]=findMax<0,0>(root)->key();
        treeRange[1][0]=findMin<1,0>(root)->key();
        treeRange[1][1]=findMax<1,0>(root)->key();
        return Range_helper<0>(root ,key ,distance, treeRange,ans);

    }
};
