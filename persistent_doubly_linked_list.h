#ifndef PERSISTENT_DOUBLY_LINKED_LIST_H
#define PERSISTENT_DOUBLY_LINKED_LIST_H

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

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
    std::vector<std::shared_ptr<DL_node<T>>> versions; // Store head versions of the list
    //std::unordered_map<T, std::shared_ptr<DL_node<T>>> node_map; // For storing already created nodes

    int current_version;

public:
    // Constructor that accepts an array and its size
    PersistentDoublyLinkedList(T* arr, int size)
    {
        if (size <= 0) {
            return; // If the array is empty, just return
        }

        // Create the head of the list
        auto head = std::make_shared<DL_node<T>>(arr[0]);
        std::shared_ptr<DL_node<T>> current = head; // Store pointer to the current node


        // Iterate through the array and create the doubly linked list
        for (int i = 1; i < size; ++i)
        {
            auto new_node = std::make_shared<DL_node<T>>(arr[i]);
            current->next = new_node; // Attach the new node to the current
            new_node->prev = current;  // Set the previous node reference
            current = new_node; // Move to the new node

        }

        // Store the head of the list in the versions vector
        versions.push_back(head);
        current_version = 0;
    }

    PersistentDoublyLinkedList(const std::vector<T>& vec, int vec_size)
    {
        int size = vec_size;
        if (size <= 0)
        {
            return; // If the vector is empty, just return
        }

        // Create the head of the list
        auto head = std::make_shared<DL_node<T>>(vec[0]);
        std::shared_ptr<DL_node<T>> current = head; // Store pointer to the current node

        // Iterate through the vector and create the doubly linked list
        for (int i = 1; i < size; ++i)
        {
            auto new_node = std::make_shared<DL_node<T>>(vec[i]);
            current->next = new_node; // Attach the new node to the current
            new_node->prev = current;  // Set the previous node reference
            current = new_node; // Move to the new node
        }

        // Store the head of the list in the versions vector
        versions.push_back(head);
        current_version = 0;
    }

    // Method to add a new node to the front of the list
    void push_front(T value)
    {
        auto new_head = std::make_shared<DL_node<T>>(value);
        new_head->next = versions.back(); // The new node points to the current head

        // Update the previous head's pointer if it exists
        if (versions.back() != nullptr)
        {
            versions.back()->prev = new_head;
        }

        // Store the new version (head) of the list
        versions.push_back(new_head);
        current_version++;
    }

    // Method to print all versions of the list without using PrintList
    void printAllVersions()
    {
        for (int i = 0; i < versions.size(); ++i)
        {
            std::cout << "Version " << i << ": {";
            auto current = versions[i];

            // Traverse through all nodes of the current version and print their values
            while (current)
            {
                std::cout << current->value << " (" << &(current->value) << ") ";
                current = current->next;
            }
            std::cout << "} " << std::endl; // Move to a new line after printing one version
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

    // Method to add a new node to the end of the list
    void push_back(T value)
    {
        auto new_node = std::make_shared<DL_node<T>>(value);

        // If this is the first version of the list, the new node will be the head
        if (versions.empty())
        {
            versions.push_back(new_node);
            current_version = 0;
            return;
        }

        auto last = versions.back(); // Get the current version (head)

        // Find the last node (tail) in the current version
        while (last->next)
        {
            last = last->next;
        }

        last->next = new_node; // Attach the new node to the tail
        new_node->prev = last; // Set the previous node reference

        // Store the new version (head) of the list
        versions.push_back(versions.back());
        current_version++;
    }

    std::vector<T> getVersion(size_t idx) const
    {
        if (idx < versions.size())
        {
            std::vector<T> result;
            auto current = versions[idx];

            // Traverse through all nodes of the current version and add their values to the vector
            while (current)
            {
                result.push_back(current->value);
                current = current->next;
            }
            return result; // Return the vector of values
        }
        throw std::out_of_range("Invalid version index");
    }
};

#endif // PERSISTENT_DOUBLY_LINKED_LIST_H