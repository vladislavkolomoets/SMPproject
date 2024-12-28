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
class PersistentAssociativeArray {
private:
    std::vector<std::shared_ptr<AA_node<KeyType, ValueType>>> versions{};

    std::shared_ptr<AA_node<KeyType, ValueType>> insert(std::shared_ptr<AA_node<KeyType, ValueType>> root, KeyType key, ValueType value) {
        if (!root) {
            return std::make_shared<AA_node<KeyType, ValueType>>(key, value);
        }

        if (key < root->key) {
            root->left = insert(root->left, key, value);
        }
        else if (key > root->key) {
            root->right = insert(root->right, key, value);
        }
        else {
            root->value = value; // Обновляем значение при совпадении ключа
        }
        //std::cout << root->value << "\t";
        return root; // Возвращаем корень для использования
    }

    // Рекурсивная функция для копирования дерева
    std::shared_ptr<AA_node<KeyType, ValueType>> copyTree(std::shared_ptr<AA_node<KeyType, ValueType>> root) {
        if (!root) {
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

        std::shared_ptr<AA_node<KeyType, ValueType>> root = nullptr; // Строка, с которой вы сказали, что проблема

        for (size_t i = 0; i < keys.size(); ++i)
        {
            root = insert(root, keys[i], values_array[i]);
        }

        versions.push_back(root);

        current_version = 0;

        this->keys = keys;
    }

    // Функция для добавления новой версии с изменением значения
    void addVersion(int root_position, KeyType change_key, ValueType new_value) {
        if (current_version < 0 || current_version >= versions.size() ||
            root_position < 0 || root_position >= versions.size()) {
            throw std::out_of_range("Invalid root position");
        }

        // Копируем дерево по указанной версии
        auto new_root = copyTree(versions[root_position]);

        // Вставляем новое значение в скопированное дерево
        insert(new_root, change_key, new_value); 

        // Добавляем новую версию в вектор
        versions.push_back(new_root);

        current_version++;
    }

    // Тестовая функция для вывода
    void print() {
        for (const auto& version : versions) {
            // Печать информации о каждом узле
            std::cout << "Version root key: " << version->key << ", value: " << version->value << std::endl;
        }
    }

    // Method to make UNDO-action
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

    // Method to make REDO-action
    void redo()
    {
        if (current_version >= versions.size() - 1) {
            std::cout << "No actions to redo!" << std::endl;
            return;
        }

        current_version++;  // Переход к следующей версии
        versions.push_back(versions[current_version]);
    }

    // Функция для печати всех версий
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

    ValueType findValueInNode(std::shared_ptr<AA_node<KeyType, ValueType>> root, const KeyType& key) {
        if (!root) {
            throw std::runtime_error("Key not found"); // или возвращаем значение по умолчанию
        }

        if (key < root->key) {
            return findValueInNode(root->left, key);
        }
        else if (key > root->key) {
            return findValueInNode(root->right, key);
        }
        else {
            return root->value; // Найден узел с соответствующим ключом
        }
    }
};

#endif // PERSISTENT_ASSOCIATIVE_ARRAY_H