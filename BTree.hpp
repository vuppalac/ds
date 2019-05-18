#pragma once

template <typename K, typename V>
struct BTreeNode
{
    K key;
    V value;
    BTreeNode<T> *left, *right;

    BTreeNode(K key, V value): key(key), value(value), left(nullptr), right(nullptr) {}
    ~BTree() { left = nullptr; right = nullptr; }

    static BTreeNode<K, V>* newInst(K key, V value) {
        return new BTreeNode(key, value);
    }

    static void deleteInst(BTreeNode<K, V> *pNode) {
        delete pNode;
    }
};

template<typename K, typename V>
class BTree
{
    private:
        BTreeNode<K, V> *pRoot;

        BTreeNode<K, V>* findMin(BTreeNode<K, V> *pNode)
        {
            BTreeNode<K, V> *pCurrent = pNode;

            while(true) {

                if(!(pCurrent->left)) {
                    return pCurrent;
                }

                pCurrent = pCurrent->left;
            }
        }

        static bool remove(BTreeNode<K, V> *pNode, K key, V& value)
        {
            bool rc = true;
            BTreeNode<K, V> *pCurrent = pNode;
            BTreeNode<K, V> *pParent = nullptr;

            while(true) {
                if(!pCurrent) return false;

                if(pCurrent->key > key) {
                    pParent = pCurrent;
                    pCurrent = pCurrent->left;
                } else if(pCurrent->key < key) {
                    pParent = pCurrent;
                    pCurrent = pCurrent->right;
                } else if(pCurrent->key == key) {
                    if(pCurrent->left && pCurrent->right) {
                        BTreeNode<K, V> *pMinNode = findMin(pCurrent->right);
                        value = pCurrent->value;
                        pCurrent->key = pMinNode->key;
                        return remove(pCurrent->right, pCurrent->key, pCurrent->value);
                    } else if(pCurrent->left && !pCurrent->right) {
                        if(pParent->left == pCurrent) {
                            pParent->left = pCurrent->left;
                        } else if(pParent->right == pCurrent) {
                            pParent->right = pCurrent->left;
                        }
                        pCurrent->left == nullptr;
                        value = pCurrent->value;
                        BTreeNode<K, V>::deleteInst(pCurrent);
                        return true;
                    } else if(!pCurrent->left && pCurrent->right) {
                        if(pParent->left == pCurrent) {
                            pParent->left = pCurrent->right;
                        } else if(pParent->right == pCurrent) {
                            pParent->right = pCurrent->right;
                        }
                        pCurrent->right == nullptr;
                        value = pCurrent->value;
                        BTreeNode<K, V>::deleteInst(pCurrent);
                        return true;
                    } else if(!pCurrent->left && !pCurrent->right) {
                        if(pParent->left == pCurrent) {
                            pParent->left = nullptr;
                        } else if(pParent->right == pCurrent) {
                            pParent->right = nullptr;
                        }
                        value = pCurrent->value;
                        BTreeNode<K, V>::deleteInst(pCurrent);
                        return true;
                    }
                }
            }

            return false;
        }

    public:
        BTree(): pRoot(nullptr) {}
        ~BTree() { /* cleanup tree */ }

        bool insert(K key, V value)
        {
            bool rc = true;
            BTreeNode<K, V> *pChild = nullptr;
            BTreeNode<K, V> *pParent = nullptr;

            if(!pRoot) {
                pRoot = BTreeNode<K, V>::newInst(key, value);
                return rc;
            } else {
                while(true) {
                    pParent = pChild;
                    if(pChild->key > key) {
                        pChild = pParent->left;
                        if(!pChild) {
                            pParent->left = BTreeNode<K, V>::newInst(key, value);
                            break;
                        }
                    } else if(pChild->key < key) {
                        pChild = pParent->right;
                        if(!pChild) {
                            pParent->right = BTreeNode<K, V>::newInst(key, value);
                            break;
                        }
                    } else if(pChild->key == key) {
                        rc = false;
                        break;
                    } else {
                        /* should never happen */
                        rc = false;
                        break;
                    }
                }
            }

            return rc;
        }

        bool find(K key, V &value)
        {
            bool rc = true;
            BTreeNode<K, V> *pCurrent = pRoot;

            while(true) {
                if(!pCurrent) return false;

                if(pCurrent->key > key) {
                    pCurrent = pCurrent->left;
                } else if(pCurrent->key < key) {
                    pCurrent = pCurrent->right;
                } else if(pCurrent->key == key) {
                    value = pCurrent->value;
                    break;
                }
            }

            return rc;
        }

        bool remove(K key, V& value)
        {
            return remove(pRoot, key, value);
        }
};
