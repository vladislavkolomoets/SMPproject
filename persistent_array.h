#ifndef PERSISTENT_ARRAY_H
#define PERSISTENT_ARRAY_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include <mutex>

template <typename T>
class PersistentArray
{
private:
    // Using a vector of pointers to elements
    using Ptr = std::shared_ptr<T>;
    std::vector<std::vector<Ptr>> versions; // All versions will be stored here

    int current_version;

public:
    // Constructor, accepts an array and its size
    PersistentArray(T* arr, int size)
        : current_version(0)
    {
        std::vector<Ptr> base;
        for (int i = 0; i < size; ++i)
        {
            base.push_back(std::make_shared<T>(arr[i])); // Copy elements into shared_ptr
        }
        versions.push_back(base); // Store the base version
    }

    PersistentArray(std::vector<T> vec, int size)
        : current_version(0)
    {
        std::vector<Ptr> base;
        for (const auto& val : vec)
        {
            base.push_back(std::make_shared<T>(val)); // Copy elements into shared_ptr
        }
        versions.push_back(base); // Store the base version
    }

    // Method to add a new version of the array
    void addVersion(int root_position, int change_index, T new_value)
    {
        // Check the validity of indices
        if (current_version < 0 || current_version >= versions.size() ||
            root_position < 0 || root_position >= versions.size() ||
            change_index < 0 || change_index >= versions[root_position].size())
        {
            throw std::out_of_range("Invalid root position");
        }

        std::vector<Ptr> new_version = versions[root_position]; // Copy pointers from the previous version
        new_version[change_index] = std::make_shared<T>(new_value); // Replace the value with the new one

        versions.push_back(new_version); // Store the new version
        current_version++;
    }

    // Method to undo the last action
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

    // Method to redo an action
    void redo()
    {
        if (current_version >= versions.size() - 1)
        {
            std::cout << "No actions to redo!" << std::endl;
            return;
        }

        current_version++;
        versions.push_back(versions[current_version]);
    }

    // Method to print all versions
    void printAllVersions()
    {
        for (size_t i = 0; i < versions.size(); i++)
        {
            std::cout << "Version [" << i << "]: \t{";
            for (size_t j = 0; j < versions[i].size(); j++)
            {
                std::cout << *(versions[i][j]) << " (" << versions[i][j].get() << ")";
                if (j < versions[i].size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << "}\n";
        }
    }

    std::vector<T> getVersion(size_t idx) const
    {
        if (idx < versions.size())
        {
            std::vector<T> result;
            for (const auto& ptr : versions[idx])
            {
                result.push_back(*ptr); // Copy value from shared_ptr
            }
            return result; // Return the vector of values
        }
        throw std::out_of_range("Invalid version index");
    }

};

int double_num(int number)
{
    return number * 2;
}

#endif // PERSISTENT_ARRAY_H