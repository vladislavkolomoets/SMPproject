#include <iostream>
//#include "persistent.h"


#include "persistent_array.h"
#include "persistent_doubly_linked_list.h"
#include "persistent_associative_array.h"
#include "convert.h"



int main()
{
    int values[] = { 100, 200, 300, 400, 500 }; // array
    int values_size = sizeof(values) / sizeof(int); // size of array

    std::cout << "TESTING SIMPLIFIED PERSISTENT ARRAY\n";

    PersistentArray<int> my_persistent_array(values, values_size); // Version[0] is made of values 

    my_persistent_array.addVersion(0, 2, -250); // Version[1]
    my_persistent_array.addVersion(1, 3, -150); // Version[2]
    my_persistent_array.printAllVersions();


    my_persistent_array.undo(); std::cout << "\tundo\n"; // reject last version
    my_persistent_array.printAllVersions(); std::cout << "\n";
    my_persistent_array.redo(); std::cout << "\tredo\n"; // repeat last action
    my_persistent_array.printAllVersions(); std::cout << "\n";

    std::cout << "\n==============================================\n";
    std::cout << "TESTING SIMPLIFIED PERSISTENT DOUBLY LINKED LIST\n";

    PersistentDoublyLinkedList<int> my_persistent_list(values, values_size);

    my_persistent_list.printAllVersions();

    my_persistent_list.addVersion(0, 2, -250); my_persistent_list.printAllVersions();
    my_persistent_list.addVersion(1, 3, -150);
    my_persistent_list.printAllVersions();

    my_persistent_list.undo(); std::cout << "\tundo\n"; // reject last version
    my_persistent_list.printAllVersions(); std::cout << "\n";
    my_persistent_list.redo(); std::cout << "\tredo\n"; // repeat last action
    my_persistent_list.printAllVersions(); std::cout << "\n";

    std::cout << "\n==============================================\n";
    std::cout << "TESTING SIMPLIFIED PERSISTENT ASSOCIATIVE ARRAY\n";

    //std::vector<std::string> keys = { "one", "two", "three" };

    //PersistentAssociativeArray<std::string, int> myPersistentMap(keys, values, values_size);
    //myPersistentMap.printAllVersions();

    std::vector<std::string> keys = { "zero", "one", "two", "three", "four" };
    //int values_array[] = { 10, 20, 30 };
    //size_t values_array_size = sizeof(values_array) / sizeof(values_array[0]);

    try {
        PersistentAssociativeArray<std::string, int> assoc_array(keys, values, values_size);
        assoc_array.printAllVersions(); // Вызов функции печати для проверки

        // Добавляем новую версию, изменяя ключ 2 на значение 25
        assoc_array.addVersion(0, "two", -250);
        assoc_array.printAllVersions();
        assoc_array.addVersion(1, "three", -150);
        assoc_array.printAllVersions();
        assoc_array.undo(); std::cout << "\tundo\n"; // reject last version
        assoc_array.printAllVersions(); std::cout << "\n";
        assoc_array.redo(); std::cout << "\tredo\n"; // repeat last action
        assoc_array.printAllVersions(); std::cout << "\n";
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    std::cout << "\n==============================================\n";
    std::cout << "TESTING CONVERTATION\n";

    //double values2[] = { 100.1, 200.2, 300.3, 400.4, 500.5 };
    //int values2_size = sizeof(values2) / sizeof(double);
    std::vector<double> values2 = { 100.1, 200.2, 300.3, 400.4, 500.5 };

    PersistentArray<double> array_to_convert(values2, values2.size());
    array_to_convert.printAllVersions();

    PersistentDoublyLinkedList<double> dll_from_array = Convert<double>::convertArrayToList(array_to_convert, 0);
    dll_from_array.printAllVersions();

    std::vector<double> values3 = { -100.1, -200.2, -300.3, -400.4, -500.5 };

    PersistentDoublyLinkedList<double> dll_to_convert(values3, values3.size());
    dll_to_convert.printAllVersions();

    PersistentArray<double> array_from_dll = Convert<double>::convertListToArray(dll_to_convert, 0);
    array_from_dll.printAllVersions();

    return 0;

}