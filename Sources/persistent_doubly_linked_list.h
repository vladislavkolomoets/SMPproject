#ifndef PERSISTENT_DOUBLY_LINKED_LIST_H
#define PERSISTENT_DOUBLY_LINKED_LIST_H

#include <iostream>
#include <vector>
#include <stack>
#include <memory>

// doubly-linked-list node
template <typename T>
struct DL_node
{
    T value;
    std::shared_ptr<DL_node<T>> prev;
    std::shared_ptr<DL_node<T>> next;

    DL_node(T data) : value(data), prev(nullptr), next(nullptr) {}
};

template <typename T>
class PersistentDoublyLinkedList
{
private:
    std::vector<std::shared_ptr<DL_node<T>>> versions;

    int current_version;

    // helper function to print one list 
    void printList(size_t version_idx) const
    {
        auto current = versions[version_idx]; // head of printed list

        std::cout << "{";
        while (current)
        {
            std::cout << current->value;
            current = current->next; // go to the next
            if (current)
            {
                std::cout << ", ";
            }
        }
        std::cout << "}" << std::endl;
    }

public:
    PersistentDoublyLinkedList(T* arr, int size)
    {
        if (size <= 0) 
        {
            return; // Если массив пуст, просто возвращаем
        }

        // Создаём голову списка
        auto head = std::make_shared<DL_node<T>>(arr[0]);
        std::shared_ptr<DL_node<T>> current = head; // Хранить указатель на текущий узел

        // Проходим по массиву и создаём двусвязный список
        for (int i = 1; i < size; ++i) {
            auto new_node = std::make_shared<DL_node<T>>(arr[i]);
            current->next = new_node; // Присоединяем новый узел к текущему
            new_node->prev = current;  // Устанавливаем ссылку на предыдущий узел
            current = new_node; // Переходим к новому узлу
        }

        // Сохраняем голову списка в вектор версий
        versions.push_back(head);
        current_version = 0; // Начинаем с нулевой версии
    }

    PersistentDoublyLinkedList(std::vector<T> vec, int size)
    {
        if (size <= 0)
        {
            return; // Если массив пуст, просто возвращаем
        }

        // Создаём голову списка
        auto head = std::make_shared<DL_node<T>>(vec[0]);
        std::shared_ptr<DL_node<T>> current = head; // Хранить указатель на текущий узел

        // Проходим по массиву и создаём двусвязный список
        for (int i = 1; i < size; ++i) {
            auto new_node = std::make_shared<DL_node<T>>(vec[i]);
            current->next = new_node; // Присоединяем новый узел к текущему
            new_node->prev = current;  // Устанавливаем ссылку на предыдущий узел
            current = new_node; // Переходим к новому узлу
        }

        // Сохраняем голову списка в вектор версий
        versions.push_back(head);
        current_version = 0; // Начинаем с нулевой версии
    }

    void addVersion(int root_position, int change_index, T new_value)
    {
        // Error checking
        if (current_version < 0 || current_version >= versions.size() ||
            root_position < 0 || root_position >= versions.size())
        {
            throw std::out_of_range("Invalid root position");
        }

        auto root = versions[root_position]; // Получаем голову списка для указанной версии
        std::shared_ptr<DL_node<T>> new_head = nullptr;  // Голова нового списка
        std::shared_ptr<DL_node<T>> new_current = nullptr; // Указатель на текущий узел нового списка

        auto current = root;
        int index = 0;

        // Копирование узлов
        while (current) {
            auto new_node = std::make_shared<DL_node<T>>(current->value); // Создаем новый узел
            if (index == change_index) {
                new_node->value = new_value; // Меняем значение, если индекс совпадает
            }

            if (!new_head) {
                new_head = new_node; // Устанавливаем голову нового списка
            }
            else {
                new_current->next = new_node; // Присоединяем новый узел к текущему
                new_node->prev = new_current;  // Устанавливаем ссылку на предыдущий узел
            }

            new_current = new_node; // Переходим к новому узлу
            current = current->next; // Переходим к следующему узлу
            index++; // Увеличиваем индекс
        }

        // Сохраняем новый список как новую версию
        versions.push_back(new_head);

        current_version++;
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

    void printAllVersions()
    {
        for (size_t i = 0; i < versions.size(); i++)
        {
            std::cout << "Version [" << i << "]\t";
            printList(i);

        }
    }

    std::shared_ptr<DL_node<T>> getVersion(size_t idx) const 
    {
        if (idx < versions.size()) 
        {
            return versions[idx];
        }
        throw std::out_of_range("Invalid version index");
    }

};

#endif // PERSISTENT_DOUBLY_LINKED_LIST_H