#ifndef CONVERT_H
#define CONVERT_H

#include "persistent_array.h"
#include "persistent_doubly_linked_list.h"

template <typename T>
class Convert {
public:
    // Конвертация из PersistentArray в PersistentDoublyLinkedList
    static PersistentDoublyLinkedList<T> convertArrayToList(const PersistentArray<T>& array, size_t idx = 0) {
        // Получаем базовую версию массива
        const auto& base_version = array.getVersion(idx);
        return PersistentDoublyLinkedList<T>(base_version, base_version.size());
    }

    // Конвертация из PersistentDoublyLinkedList в PersistentArray
    static PersistentArray<T> convertListToArray(const PersistentDoublyLinkedList<T>& list, size_t idx = 0) {
        // Получаем базовую версию списка
        const auto head = list.getVersion(idx);

        std::vector<T> array;
        auto current = head;
        while (current) {
            array.push_back(current->value);
            current = current->next;
        }

        return PersistentArray<T>(array.data(), array.size());
    }
};

#endif // CONVERT_H