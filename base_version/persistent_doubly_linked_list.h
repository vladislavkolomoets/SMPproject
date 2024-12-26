#ifndef PERSISTENT_DOUBLY_LINKED_LIST_H
#define PERSISTENT_DOUBLY_LINKED_LIST_H

#include <iostream>
#include <vector>

// doubly-linked-list node
template <typename T>
struct DL_node 
{
    T value; 
    DL_node<T>* prev; 
    DL_node<T>* next; 

    DL_node(T data) : value(data), prev(nullptr), next(nullptr) {}
};

template <typename T>
class PersistentDoublyLinkedList
{
private:
    std::vector<DL_node<T>*> versions{};
    
    DL_node<T>* copyList(DL_node<T>* root) 
    {
        if (!root) return nullptr;

        DL_node<T>* newHead = new DL_node<T>(root->value);
        DL_node<T>* current = root->next;
        DL_node<T>* newCurrent = newHead;

        while (current) 
        {
            newCurrent->next = new DL_node<T>(current->value);
            newCurrent->next->prev = newCurrent;
            newCurrent = newCurrent->next;
            current = current->next;
        }
        return newHead;
    }

    void printList(DL_node<T>* root) 
    {
        DL_node<T>* current = root;
        std::cout << "{";
        while (current) 
        {
            std::cout << "'" << current->value << "'";
            if (current->next) 
            {
                std::cout << ", ";
            }
            current = current->next;
        }
        std::cout << "}";
    }


public:
    PersistentDoublyLinkedList(T* arr, int size) 
    {
        if (size <= 0) return;

        // First version
        DL_node<T>* head = new DL_node<T>(arr[0]);
        DL_node<T>* current = head;
        for (int i = 1; i < size; ++i) 
        {
            current->next = new DL_node<T>(arr[i]);
            current->next->prev = current;
            current = current->next;
        }
        versions.push_back(head); // add a new version
    }
    
    void addVersion(int root_position, int change_index, T new_value) 
    {
        if (root_position < 0 || root_position >= versions.size()) 
        {
            std::cout << "Invalid root position!" << std::endl;
            return;
        }

        DL_node<T>* root = versions[root_position];
        DL_node<T>* new_head = copyList(root); 

        // Find node to change
        DL_node<T>* current = new_head;
        for (int i = 0; current && i < change_index; ++i) 
        {
            current = current->next;
        }

        if (!current) 
        {
            std::cout << "Invalid change index!" << std::endl;
            return;
        }

        current->value = new_value;
        versions.push_back(new_head); 
    }
    
    void printAllVersions() 
    {
        for (size_t i = 0; i < versions.size(); i++) 
        {
            std::cout << "Version [" << i << "]:\t";
            printList(versions[i]);
            std::cout << std::endl;
        }
    }

    ~PersistentDoublyLinkedList() 
    {
        for (auto head : versions) 
        {
            DL_node<T>* current = head;
            while (current) 
            {
                DL_node<T>* next = current->next;
                delete current;
                current = next;
            }
        }
    }
};


#endif // PERSISTENT_DOUBLY_LINKED_LIST_H