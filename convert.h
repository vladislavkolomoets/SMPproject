#ifndef CONVERT_H
#define CONVERT_H

#include "persistent_array.h"
#include "persistent_doubly_linked_list.h"
#include "persistent_associative_array.h"

template <typename T>
class Convert
{
public:
    // Convert from PersistentArray to PersistentDoublyLinkedList
    static PersistentDoublyLinkedList<T> convertArrayToList(const PersistentArray<T>& array, size_t idx = 0)
    {
        // Get the base version of the array
        auto base_version = array.getVersion(idx);
        return PersistentDoublyLinkedList<T>(base_version, base_version.size());
    }

    // Convert from PersistentDoublyLinkedList to PersistentArray
    static PersistentArray<T> convertListToArray(const PersistentDoublyLinkedList<T>& list, size_t idx = 0)
    {
        // Get the base version of the list
        auto head = list.getVersion(idx);

        return PersistentArray<T>(head, head.size());
    }

    // Convert from PersistentArray to PersistentAssociativeArray
    template<typename KeyType>
    static PersistentAssociativeArray<KeyType, T> convertArrayToAssociativeArray(const PersistentArray<T>& array, const std::vector<KeyType>& keys, size_t idx = 0)
    {
        // Get the base version of the array
        auto base_version = array.getVersion(idx);

        if (keys.size() != base_version.size())
        {
            throw std::invalid_argument("Number of keys must match the number of elements in the array.");
        }

        return PersistentAssociativeArray<KeyType, T>(keys, base_version, base_version.size());
    }

    // Convert from PersistentDoublyLinkedList to PersistentAssociativeArray
    template<typename KeyType>
    static PersistentAssociativeArray<KeyType, T> convertListToAssociativeArray(const PersistentDoublyLinkedList<T>& list, const std::vector<KeyType>& keys, size_t idx = 0)
    {
        // Get the base version of the list
        auto values = list.getVersion(idx);

        if (keys.size() != values.size())
        {
            throw std::invalid_argument("Number of keys must match the number of elements in the list.");
        }

        return PersistentAssociativeArray<KeyType, T>(keys, values, values.size());
    }

    // Convert from PersistentAssociativeArray to PersistentDoublyLinkedList
    template<typename KeyType>
    static PersistentDoublyLinkedList<T> convertAssociativeArrayToList(const PersistentAssociativeArray<KeyType, T>& associative_array, size_t idx = 0)
    {
        std::vector<T> values = associative_array.getVersion(idx);

        return PersistentDoublyLinkedList<T>(values, values.size());
    }

    // Convert from PersistentAssociativeArray to PersistentArray
    template<typename KeyType>
    static PersistentArray<T> convertAssociativeArrayToArray(const PersistentAssociativeArray<KeyType, T>& associative_array, size_t idx = 0) {
        std::vector<T> values = associative_array.getVersion(idx);

        return PersistentArray<T>(values, values.size());
    }
};

#endif // CONVERT_H