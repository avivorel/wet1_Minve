#ifndef MAIN_CPP_AVLTTREE_H
#define MAIN_CPP_AVLTTREE_H
#include <iostream>
#include "LinkedList.h"

template<class T>
class AVLNode {
    T data;
    AVLNode<T> *left;
    AVLNode<T> *right;
    AVLNode<T> *parent;
    int height;
public:
    AVLNode() : data(nullptr), left(nullptr), right(nullptr), parent(nullptr), height(0) {}
    explicit AVLNode(const T &value) : data(value), left(nullptr), right(nullptr), parent(nullptr), height(0) {}
    ~AVLNode();
    const AVLNode<T> *GetMinimumNode() const;
    T Get() const { return data; }
    void Set(const T &new_data);
    void SetLeft(AVLNode<T> *newleft) { left = newleft; }
    AVLNode<T> *GetLeft() const { return left; }
    void SetRight(AVLNode<T> *newright) { right = newright; }
    AVLNode<T> *GetRight() const { return right; }
    void SetParent(AVLNode<T> *parent_) { parent = parent_; }
    AVLNode<T> *GetParent() const { return parent; }
    void ChangeHeight(int new_height) { this->height = new_height; }
    int GetHeight() const { return this->height; }
    void UpdateHeight();
    int GetBF() const;
    AVLNode<T> *GetGreatParent();
    AVLNode<T> *GetGreatParent_Use_Auxilary(AVLNode<T> *node);
    void TreeToArray(T *array, int *location);
};
template<class T>
class AVLTree {
    AVLNode<T> *root;
    int (*compare)(const T &a, const T &b);
    void RotateL(AVLNode<T> *B);
    void RotateR(AVLNode<T> *B);
    void InsertToTree(AVLNode<T> *node, AVLNode<T> *new_node);
    void IfNeededRotate(AVLNode<T> *node);
public:
    explicit AVLTree(int (*comp)(const T &a, const T &b)) : root(nullptr), compare(comp) {}
    ~AVLTree();
    AVLNode<T> *RemoveBin(AVLNode<T> *to_delete, const T& data);
    bool Insert(const T &data);
    bool Remove(const T &data);
    bool CheckEmpty();
    const AVLNode<T> *MinNode() const;
    AVLNode<T> *Find(const T &value) const;
    AVLNode<T> *Find_Auxilary(AVLNode<T> *node, const T &value) const;
    AVLNode<T> *FindAndGetSmallestInTree();
    void MergeArrays(T *arr1, T *arr2, T *merge_to, int arr1_size, int arr2_size) const;
    AVLNode<T> *GetRoot();
    AVLNode<T> *FindMinimumValueInTree(AVLNode<T> *node);
    AVLNode<T> *FindMaxValueInTree(AVLNode<T> *node);
    int GetTreeSize();
    int GetSizeAux(AVLNode<T> *node);
    void ToArray(T *array);
    AVLNode<T>*  find_closest_next(AVLNode<T>* root, AVLNode<T>* node);
    void inorder_search_ll(AVLNode<T>* root, LinkedList<T> *list, T minval, T maxval);
    AVLNode<T>*  find_closest_prev(AVLNode<T>* root, AVLNode<T>* node);
    void deleteTree(AVLNode<T>* node);
    void SetRoot(AVLNode<T> *new_root);
    static AVLNode<T> *ArrToTree(T *array, int start, int end, int *to_fill);
    static AVLTree<T> *ArrToTree(T *array, int start, int size, int (*comp)(const T &, const T &));


};

template<class T>
void AVLNode<T>::TreeToArray(T *array, int *location) {
    if (this->GetLeft() != nullptr)
        this->GetLeft()->TreeToArray(array, location);
    array[*location] = this->Get();
    *location = *location + 1;
    if (this->GetRight() != nullptr)
        this->GetRight()->TreeToArray(array, location);
}

template<class T>
void AVLNode<T>::UpdateHeight() {
    int left_height = 0;
    int right_height = 0;

    if (right == nullptr) {
        right_height = -1;
    } else {
        right_height = right->GetHeight();
    }

    if (left == nullptr) {
        left_height = -1;
    } else {
        left_height = left->GetHeight();
    }

    this->height = (std::max(left_height, right_height) + 1);
}


template<class T>
int AVLNode<T>::GetBF() const {
    int left_height, right_height;
    if (left == nullptr) {
        left_height = -1;
    } else {
        left_height = left->GetHeight();
    }
    if (right == nullptr) {
        right_height = -1;
    } else {
        right_height = right->GetHeight();
    }
    return left_height - right_height;
}

template<class T>
AVLNode<T> *AVLNode<T>::GetGreatParent() {
    if (this->parent == nullptr)
        return this;
    return GetGreatParent_Use_Auxilary(this->parent);
}

template<class T>
AVLNode<T> *AVLNode<T>::GetGreatParent_Use_Auxilary(AVLNode<T> *node) {
    if (node->parent == nullptr)
        return node;
    return GetGreatParent_Use_Auxilary(node->parent);
}

template<class T>
AVLNode<T>::~AVLNode() {
    delete left;
    delete right;
}

template<class T>
void AVLNode<T>::Set(const T &new_data) {
    this->data = new_data;
}

template<class T>
const AVLNode<T> *AVLNode<T>::GetMinimumNode() const {
    if (GetLeft() == nullptr)
        return this;
    else
        return GetLeft()->GetMinimumNode();
}


template<class T>
AVLNode<T> *AVLTree<T>::Find(const T &value) const {
    AVLNode<T> *tmp = this->root;
    return Find_Auxilary(tmp, value);
}


template<class T>
void AVLTree<T>::RotateL(AVLNode<T> *B) {
    if (B == nullptr) { return; }
    AVLNode<T> *A = B->GetRight();
    AVLNode<T> *A_L = A->GetLeft();
    B->SetRight(A_L);
    A->SetLeft(B);

    if (B->GetParent() == nullptr) {
        this->root = A;
    } else {
        if (B->GetParent()->GetLeft() == B) {
            B->GetParent()->SetLeft(A);
        } else {
            B->GetParent()->SetRight(A);
        }
    }
    A->SetParent(B->GetParent());
    B->SetParent(A);
    if (A_L != nullptr) {
        A_L->SetParent(B);
    }
    B->UpdateHeight();
    A->UpdateHeight();
}

template<class T>
void AVLTree<T>::RotateR(AVLNode<T> *B) {
    if (B == nullptr) { return; }

    AVLNode<T> *A = B->GetLeft();


    AVLNode<T> *A_R = A->GetRight();
    B->SetLeft(A_R);
    if (A_R != nullptr) {
        A_R->SetParent(B);
    }
    A->SetRight(B);

    if (B->GetParent() == nullptr) {
        this->root = A;
    } else {
        if (B->GetParent()->GetLeft() == B) {
            B->GetParent()->SetLeft(A);
        } else {
            B->GetParent()->SetRight(A);
        }
    }
    A->SetParent(B->GetParent());
    B->SetParent(A);

    B->UpdateHeight();
    A->UpdateHeight();
}


template<class T>
bool AVLTree<T>::Insert(const T &data) {

    if (this->Find(data)) {
        return false;
    }

    auto *new_node = new AVLNode<T>(data);


    if (new_node == nullptr) {
        return false;
    }

    if (this->root == nullptr) {
        this->root = new_node;
    } else {
        InsertToTree(this->root, new_node);
    }

    return true;
}


template<class T>
void AVLTree<T>::InsertToTree(AVLNode<T> *node, AVLNode<T> *new_node) {
    if (compare(node->Get(), new_node->Get()) == 1) {

        if (node->GetLeft() == nullptr) {
            node->SetLeft(new_node);
            new_node->SetParent(node);
        } else {
            InsertToTree(node->GetLeft(), new_node);
        }
    } else {
        if (node->GetRight() == nullptr) {
            node->SetRight(new_node);
            new_node->SetParent(node);
        } else {
            InsertToTree(node->GetRight(), new_node);
        }
    }
    IfNeededRotate(node);

}

template<class T>
AVLNode<T> *AVLTree<T>::FindMinimumValueInTree(AVLNode<T> *node) {
    if (node == nullptr)
        return node;

    if (node->GetLeft() == nullptr)
        return node;
    return FindMinimumValueInTree(node->GetLeft());
}


template <class T>
AVLNode<T>* AVLTree<T>::RemoveBin(AVLNode<T>* node, const T& data)
{
    AVLNode<T>* to_delete = Find(data);
    AVLNode<T>* big_parent = nullptr;

    if (to_delete->GetLeft() && to_delete->GetRight()) {
        AVLNode<T>* new_root = FindMinimumValueInTree(to_delete->GetRight());

        to_delete->Set(new_root->Get());
        to_delete = new_root;
        if (to_delete->GetRight()) {
            big_parent = to_delete->GetRight();
        }
    }

    else if (to_delete->GetLeft()) {
        big_parent = to_delete->GetLeft();
    } else if (to_delete->GetRight()) {
        big_parent = to_delete->GetRight();
    }

    AVLNode<T>* parent = to_delete->GetParent();
    if (parent == nullptr) {
        root = big_parent;
    } else if (to_delete->GetParent()->GetLeft() == to_delete) {
        to_delete->GetParent()->SetLeft(big_parent);
    } else {
        to_delete->GetParent()->SetRight(big_parent);
    }
    if (big_parent != nullptr) {
        big_parent->SetParent(parent);
    }

    to_delete->SetLeft(nullptr);
    to_delete->SetRight(nullptr);
    delete to_delete;
    return parent;
}


template<class T>
void AVLTree<T>::IfNeededRotate(AVLNode<T> *node) {
    int balance_root = node->GetBF();

    if (balance_root == 2 && (node->GetLeft())->GetBF() >= 0) {
        RotateR(node);
    } else if (balance_root == 2 && (node->GetLeft())->GetBF() == -1) {
        RotateL(node->GetLeft());
        RotateR(node);
        return;
    } else if (balance_root == -2 && (node->GetRight())->GetBF() == 1) {
        RotateR(node->GetRight());
        RotateL(node);
        return;
    } else if (balance_root == -2 && (node->GetRight())->GetBF() <= 0) {
        RotateL(node);
        return;
    }

    node->UpdateHeight();
}


template<class T>
bool AVLTree<T>::Remove(const T &data) {
    if (root == nullptr)
        return false;


    AVLNode<T> *to_delete = this->Find(data);
    if (to_delete == nullptr)
    {
        return false;
    }

    AVLNode<T> *parent_of_deleted = RemoveBin(root, data);

    while (parent_of_deleted != nullptr) {
        IfNeededRotate(parent_of_deleted);
        parent_of_deleted = parent_of_deleted->GetParent();
    }
    return true;

}


template<class T>
int AVLTree<T>::GetTreeSize() {
    return GetSizeAux(this->root);
}

template<class T>
int AVLTree<T>::GetSizeAux(AVLNode<T> *node) {
    if (node == nullptr)
        return 0;

    return 1 + GetSizeAux(node->GetLeft()) + GetSizeAux(node->GetRight());
}

template<class T>
void AVLTree<T>::ToArray(T *array) {
    int location = 0;
    if (root != nullptr)
        root->TreeToArray(array, &location);
}

template<class T>
AVLTree<T> *AVLTree<T>::ArrToTree(T *array, int start, int size, int (*comp)(const T &a, const T &b)) {
    auto tree = new AVLTree<T>(comp);
    int to_fill = size;
    tree->root = ArrToTree(array, start, size, &to_fill);
    return tree;
}

template <class T>
AVLNode<T> *AVLTree<T>::ArrToTree(T *array, int start, int end, int* to_fill) {

    if (*to_fill ==0 || start >end) {
        return nullptr;
    }
    int mri = (start + end) / 2;
    auto* mr = new AVLNode<T>(array[mri]);
    mr->SetParent(nullptr);
    *to_fill = *to_fill -1;

    if (start-end == 0)
    {
        mr->SetLeft(nullptr);
        mr->SetRight(nullptr);
        mr->ChangeHeight(0);
        return mr;
    }
    AVLNode<T>* left_son = ArrToTree(array, start, mri - 1, to_fill);
    AVLNode<T>* right_son = ArrToTree(array, mri + 1, end, to_fill);
    mr->SetLeft(left_son);
    mr->SetRight(right_son);
    if (left_son!= nullptr)
    {
        left_son->SetParent(mr);
    }
    if (right_son != nullptr)
    {
        right_son->SetParent(mr);
    }

    mr->UpdateHeight();
    return mr;
}


template<class T>
AVLNode<T> *AVLTree<T>::Find_Auxilary(AVLNode<T> *node, const T &value) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (compare(node->Get(), value) == 0) {
        return node;
    }
    if (compare(value, node->Get()) == 1) {
        return Find_Auxilary(node->GetRight(), value);
    } else {
        return Find_Auxilary(node->GetLeft(), value);
    }
}

template<class T>
bool AVLTree<T>::CheckEmpty() {
    return this->root == nullptr;
}

template<class T>
AVLNode<T> *AVLTree<T>::FindAndGetSmallestInTree() {
    if (root == nullptr)
        return nullptr;

    AVLNode<T> *smallest = root;
    while (smallest->GetLeft() != nullptr) {
        smallest = smallest->GetLeft();
    }
    return smallest;
}

template<class T>
AVLNode<T> *AVLTree<T>::GetRoot() {
    return this->root;
}

template<class T>
void AVLTree<T>::SetRoot(AVLNode<T> *new_root) {
    this->root = new_root;
}

template<class T>
AVLNode<T> *AVLTree<T>::FindMaxValueInTree(AVLNode<T> *node) {
    if (node->GetRight() == nullptr && node->GetLeft() == nullptr)
        return node;
    return FindMaxValueInTree(node->GetRight());
}


template<class T>
AVLTree<T>::~AVLTree() {
    delete root;
}


template<class T>
void AVLTree<T>::MergeArrays(T *arr1, T *arr2, T *merge_to, int arr1_size, int arr2_size) const {

    while (arr1_size != 0 && arr2_size != 0) {
        if (compare(*arr1, *arr2) == -1) {
            *merge_to = *arr1;
            arr1 = arr1 + 1;
            arr1_size = arr1_size - 1;
            merge_to = merge_to + 1;
        } else {
            *merge_to = *arr2;
            arr2 = arr2 + 1;
            arr2_size = arr2_size - 1;
            merge_to = merge_to + 1;
        }
    }

    while (arr1_size != 0) {
        *merge_to = *arr1;
        arr1 = arr1 + 1;
        arr1_size = arr1_size - 1;
        merge_to = merge_to + 1;
    }

    while (arr2_size != 0) {
        *merge_to = *arr2;
        arr2 = arr2 + 1;
        arr2_size = arr2_size - 1;
        merge_to = merge_to + 1;
    }
}

template<class T>
const AVLNode<T> *AVLTree<T>::MinNode() const {
    if (root == nullptr)
        return nullptr;
    return root->GetMinimumNode();
}


template<class T>
AVLNode<T>* AVLTree<T>::find_closest_next(AVLNode<T>* root, AVLNode<T>* node)
{
    if (node->GetRight() != nullptr)
        return FindMinimumValueInTree(node->GetRight());

    AVLNode<T>* next = nullptr;

    while (root != nullptr)
    {
        if (compare(node->Get() , root->Get()) == -1)
        {
            next = root;
            root = root->GetLeft();
        }

        else if (compare(node->Get() , root->Get()) == 1)
            root = root->GetRight();

        else
            break;
    }
    return next;
}

template<class T>
AVLNode<T>* AVLTree<T>:: find_closest_prev(AVLNode<T>* root, AVLNode<T>* node)
{
    if (node->GetLeft() != nullptr)
        return FindMaxValueInTree(node->GetLeft());
    AVLNode<T>* prev = nullptr;
    while (root != nullptr)
    {
        if (compare(node->Get() , root->Get()) == -1)
        {
            root = root->GetLeft();
        }
        else if (compare(node->Get() , root->Get()) == 1)
        {
                prev = root;
                root = root->GetRight();
        }
        else
            break;
    }
    return prev;
}
template<class T>
void AVLTree<T>::inorder_search_ll(AVLNode<T>* root,LinkedList<T> *list, T minval, T maxval){
    if (root == nullptr) return;

    if (compare(root->Get(), minval) == 1)
        inorder_search_ll(root->GetLeft(),list, minval, maxval);

    if (compare(root->Get(), minval) != -1 && compare(root->Get(), maxval) != 1){
        list->insert(root->Get());
    }

    if (compare(root->Get(), maxval) == -1)
        inorder_search_ll(root->GetRight(),list, minval, maxval);
}

template <typename T>
void AVLTree<T>::deleteTree(AVLNode<T>* node)
{
    if (node == nullptr) return;
    deleteTree(node->GetLeft());
    deleteTree(node->GetRight());
    delete node;
}

#endif
