#ifndef PERSISTENT_ARRAY_H
#define PERSISTENT_ARRAY_H

#include <vector>
#include <iostream>
#include <stack>
#include <tuple>


template <typename T>
class PersistentArray
{
private:
    std::vector<std::vector<T>> versions{}; // fat-node; all versions are stored here

    int current_version{};

public:
    // Constructor
    // takes an array and its size; 
    // creates vector from given array and pushes it as a base version
    PersistentArray(T* arr, int size)
    {
        std::vector<T> base(arr, arr + size); // transform array to vector
        versions.push_back(base); // save base version in the array of versions
        current_version = 0;
    }

    PersistentArray(std::vector<T> vec, int size)
    {
        auto base = vec; // transform array to vector
        versions.push_back(base); // save base version in the array of versions
        current_version = 0;
    }

    // Method to add a new version of array
    // takes an index of a version you want to change;
    // index of element to change and a new value
    // root_position is an index of a version we want change
    void addVersion(int root_position, int change_index, T new_value)
    {
        // Error checking
        if (current_version < 0 || current_version >= versions.size() ||
            root_position < 0 || root_position >= versions.size() ||
            change_index < 0 || change_index >= versions[root_position].size())
        {
            throw std::out_of_range("Invalid root position");
        }

        auto temp = versions[root_position]; // make a copy(temp) of a version we want to change
        temp[change_index] = new_value; // change value by index
        versions.push_back(temp); // add a new version
        current_version++; // increment
    }

    // Method to make UNDO-action
    void undo()
    {
        if (current_version <= 0)
        {
            std::cout << "No actions to undo!" << std::endl;
            return;
        }

        current_version--; // decrement position
        versions.push_back(versions[current_version]);
    }

    // Method to make REDO-action
    void redo()
    {
        if (current_version >= versions.size() - 1) 
        {
            std::cout << "No actions to redo!" << std::endl;
            return;
        }

        current_version++;  // go to the next idx of version
        versions.push_back(versions[current_version]);
    }

    // Method to print all versions
    void printAllVersions()
    {
        // out loop for all veriosns
        for (size_t i = 0; i < versions.size(); i++)
        {
            std::cout << "Version [" << i << "]: \t{";
            //inner loop for elements for a version
            for (size_t j = 0; j < versions[i].size(); j++)
            {
                std::cout << versions[i][j] << "(" << &(versions[i][j]) << ")";
                if (j < versions[i].size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << "}\n";
        }
    }

    // Method to return a specific version by index
    const std::vector<T>& getVersion(size_t idx) const 
    {
        if (idx < versions.size()) 
        {
            return versions[idx];
        }
        throw std::out_of_range("Invalid version index");
    }
};


#endif // PERSISTENT_ARRAY_H