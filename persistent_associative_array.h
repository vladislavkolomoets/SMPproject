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
    std::shared_ptr<AA_node<KeyType, ValueType>> left{};
    std::shared_ptr<AA_node<KeyType, ValueType>> right{};

    AA_node(KeyType k, ValueType v) : key(k), value(v), left(nullptr), right(nullptr) {}
};

template <typename KeyType, typename ValueType>
class PersistentAssociativeArray 
{
private:
    std::vector<std::shared_ptr<AA_node<KeyType, ValueType>>> versions{};

    std::shared_ptr<AA_node<KeyType, ValueType>> insert(std::shared_ptr<AA_node<KeyType, ValueType>> root, KeyType key, ValueType value) {
        if (!root) 
        {
            return std::make_shared<AA_node<KeyType, ValueType>>(key, value);
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
            root->value = value; // Update value when the key matches
        }
        return root; // Return the root for usage
    }

    // Recursive function to copy the tree
    std::shared_ptr<AA_node<KeyType, ValueType>> copyTree(std::shared_ptr<AA_node<KeyType, ValueType>> root) {
        if (!root) 
        {
            return nullptr;
        }
        auto newNode = std::make_shared<AA_node<KeyType, ValueType>>(root->key, root->value);
        newNode->left = copyTree(root->left);
        newNode->right = copyTree(root->right);
        return newNode;
    }

    int current_version{};
    std::vector<KeyType> keys;

public:
    PersistentAssociativeArray(const std::vector<KeyType>& keys, ValueType* values_array, size_t values_array_size)
    {
        if (keys.size() != values_array_size || keys.empty())
        {
            throw std::invalid_argument("Keys and values must have the same non-zero length.");
        }

        std::shared_ptr<AA_node<KeyType, ValueType>> root = nullptr; // Issue occurred here

        for (size_t i = 0; i < keys.size(); ++i)
        {
            root = insert(root, keys[i], values_array[i]);
        }

        versions.push_back(root);
        current_version = 0;
        this->keys = keys;
    }

    PersistentAssociativeArray(const std::vector<KeyType>& keys, const std::vector<ValueType>& values, size_t values_array_size)
    {
        if (keys.size() != values.size() || keys.empty())
        {
            throw std::invalid_argument("Keys and values must have the same non-zero length.");
        }

        std::shared_ptr<AA_node<KeyType, ValueType>> root = nullptr;

        for (size_t i = 0; i < keys.size(); ++i)
        {
            root = insert(root, keys[i], values[i]);
        }

        versions.push_back(root);
        current_version = 0;
        this->keys = keys;
    }

    // Function to add a new version with a value change
    void addVersion(int root_position, KeyType change_key, ValueType new_value) 
    {
        if (current_version < 0 || current_version >= versions.size() ||
            root_position < 0 || root_position >= versions.size()) 
        {
            throw std::out_of_range("Invalid root position");
        }

        // Copy the tree from the specified version
        auto new_root = copyTree(versions[root_position]);

        // Insert the new value into the copied tree
        insert(new_root, change_key, new_value);

        // Add the new version to the vector
        versions.push_back(new_root);
        current_version++;
    }

    // Test function for output
    void print() 
    {
        for (const auto& version : versions) 
        {
            // Print information for each node
            std::cout << "Version root key: " << version->key << ", value: " << version->value << std::endl;
        }
    }

    // Method to make UNDO action
    void undo()
    {
        if (current_version <= 0)
        {
            std::cout << "No actions to undo!" << std::endl;
            return;
        }

        current_version--;
        versions.push_back(versions[current_version]);
    }

    // Method to make REDO action
    void redo()
    {
        if (current_version >= versions.size() - 1) 
        {
            std::cout << "No actions to redo!" << std::endl;
            return;
        }

        current_version++;  // Move to the next version
        versions.push_back(versions[current_version]);
    }

    // Function to print all versions
    void printAllVersions()
    {
        for (size_t i = 0; i < versions.size(); ++i)
        {
            std::cout << "Version [" << i << "]\t";
            std::cout << "{";
            for (size_t j = 0; j < keys.size(); ++j)
            {
                std::cout << "'" << keys[j] << "': " << findValueInNode(versions[i], keys[j]);
                if (j < keys.size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << "}" << std::endl;
        }
    }

    ValueType findValueInNode(std::shared_ptr<AA_node<KeyType, ValueType>> root, const KeyType& key) 
    {
        if (!root) 
        {
            throw std::runtime_error("Key not found"); // or return a default value
        }

        if (key < root->key) 
        {
            return findValueInNode(root->left, key);
        }
        else if (key > root->key) 
        {
            return findValueInNode(root->right, key);
        }
        else 
        {
            return root->value; // Found the node with the corresponding key
        }
    }

    std::vector<ValueType> getVersion(size_t idx) const 
    {
        if (idx < versions.size()) 
        {
            std::vector<ValueType> result;
            collectValues(versions[idx], result);
            return result; // Return the vector of values
        }
        throw std::out_of_range("Invalid version index");
    }

    // Recursive function to collect values from the tree
    void collectValues(std::shared_ptr<AA_node<KeyType, ValueType>> node, std::vector<ValueType>& result) const 
    {
        if (!node) 
        {
            return;
        }

        // First traverse the left subtree
        collectValues(node->left, result);

        // Then add the current value
        result.push_back(node->value);

        // And traverse the right subtree
        collectValues(node->right, result);
    }
};

#endif // PERSISTENT_ASSOCIATIVE_ARRAY_H