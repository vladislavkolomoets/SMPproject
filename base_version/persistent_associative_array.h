#ifndef PERSISTENT_ASSOCIATIVE_ARRAY_H
#define PERSISTENT_ASSOCIATIVE_ARRAY_H

#include <iostream>
#include <vector>
#include <memory>
#include <utility>

template <typename KeyType, typename ValueType>
struct AA_node
{
    KeyType key{};
    ValueType value{};
    AA_node<KeyType, ValueType>* left{};
    AA_node<KeyType, ValueType>* right{};

    AA_node(KeyType k, ValueType v) : key(k), value(v), left(nullptr), right(nullptr) {}
};

template <typename KeyType, typename ValueType>
class PersistentAssociativeArray
{
private:
    std::vector<AA_node<KeyType, ValueType>*> versions{};
    
    // helper function to insert
    AA_node<KeyType, ValueType>* insert(AA_node<KeyType, ValueType>* root, KeyType key, ValueType value) 
    {
        if (!root)
        {
            return new AA_node<KeyType, ValueType>(key, value);
        }
        
        if (key < root->key) 
        {
            root->left = insert(root->left, key, value);
        } 
        else if (key > root->key) 
        {
            root->right = insert(root->right, key, value);
        } 
        else 
        {
            root->value = value; // If a key is exist, add a value
        }
        
        return root;
    }
    
    // helper function to copy
    AA_node<KeyType, ValueType>* copyTree(AA_node<KeyType, ValueType>* root) 
    {
        if (!root) return nullptr;
        AA_node<KeyType, ValueType>* newRoot = new AA_node<KeyType, ValueType>(root->key, root->value);
        newRoot->left = copyTree(root->left);
        newRoot->right = copyTree(root->right);
        return newRoot;
    }
    
    // helper function to print
    void printTree(AA_node<KeyType, ValueType>* root) 
    {
        if (!root) return;
        printTree(root->left);
        std::cout << "'" << root->key << "' : '" << root->value << "', ";
        printTree(root->right);
    }
    
	// helper function to delete
    void deleteTree(AA_node<KeyType, ValueType>* root) 
    {
        if (!root) return;
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
public:
    PersistentAssociativeArray(const std::vector<KeyType>& keys, ValueType* values_array, size_t values_array_size) 
    {
        if (keys.size() != values_array_size || keys.empty()) 
        {
            throw std::invalid_argument("Keys and values must have the same non-zero length.");
        }
    
		// First version
        AA_node<KeyType, ValueType>* root = nullptr;
    
        for (size_t i = 0; i < keys.size(); ++i) 
        {
            root = insert(root, keys[i], values_array[i]);
        }
    
        versions.push_back(root); 
    }

    ~PersistentAssociativeArray() 
    {
        for (auto root : versions) 
        {
            deleteTree(root);
        }
    }

    void addVersion(int root_position, KeyType key, ValueType value) 
    {
        if (root_position < 0 || root_position >= versions.size()) {
            std::cout << "Invalid root position!" << std::endl;
            return;
        }
    
        // Copy tree for creating a new version
        AA_node<KeyType, ValueType>* newRoot = copyTree(versions[root_position]); 

        newRoot = insert(newRoot, key, value); // update newRoot
        versions.push_back(newRoot); 
    }

    void printAllVersions() 
    {
        for (size_t i = 0; i < versions.size(); i++) 
        {
            std::cout << "Version [" << i << "]:\t{";
            printTree(versions[i]);
            std::cout <<"}"<< std::endl;
        }
    }
};




#endif // PERSISTENT_ASSOCIATIVE_ARRAY_H