#include<iostream>
#include <vector>

#include "persistent_array.h"
#include "persistent_doubly_linked_list.h"
#include "persistent_associative_array.h"

int main()
{
    int values[] = { 100, 200, 300 };
    int values_size = sizeof(values) / sizeof(int);
    
    std::cout << "TESTING SIMPLIFIED PERSISTENT ARRAY\n";
    
    PersistentArray<int> my_persistent_array(values, values_size); // Version[0]
    
    my_persistent_array.addVersion(0, 1, 250); // Version[1]
    my_persistent_array.addVersion(1, 0, 150); // Version[2]
    my_persistent_array.printAllVersions();
    
    std::cout << "\n==============================================\n";
    std::cout << "TESTING SIMPLIFIED PERSISTENT DOUBLY LINKED LIST\n";
    
    PersistentDoublyLinkedList<int> my_persistent_list(values, values_size);
    
    my_persistent_list.addVersion(0, 1, 250); 
    my_persistent_list.addVersion(1, 0, 150); 
    my_persistent_list.printAllVersions();
    
    std::cout << "\n==============================================\n";
    std::cout << "TESTING SIMPLIFIED PERSISTENT ASSOCIATIVE ARRAY\n";
    
    std::vector<std::string> keys = {"one", "two", "three"};

    PersistentAssociativeArray<std::string, int> myPersistentMap(keys, values, values_size);
    //myPersistentMap.printAllVersions();

    myPersistentMap.addVersion(0, "four", 4); 
    myPersistentMap.addVersion(1, "two", 20); 
    //myPersistentMap.addVersion(2, "five", 5); 

    myPersistentMap.printAllVersions();
    
    
    return 0;
}
