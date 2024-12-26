#ifndef PERSISTENT_ARRAY_H
#define PERSISTENT_ARRAY_H

template <typename T>
class PersistentArray
{
private:
    std::vector<std::vector<T>> versions{};
  
public:
    PersistentArray(T* arr, int size)
    {
        std::vector<T> base(arr, arr + size);
        versions.push_back(base);
    }

    void addVersion(int root_position, int change_index, T new_value)
    {
        if (root_position < 0 || root_position >= versions.size() ||
            change_index < 0 || change_index >= versions[root_position].size()) 
        {
            std::cout << "Invalid root position or change index!" << std::endl;
            return;
        }
        
        auto temp = versions[root_position]; 
        temp[change_index] = new_value;     
        versions.push_back(temp);              
    }
    
    void printAllVersions()
    {
	    for (size_t i = 0; i < versions.size(); i++) 
	    {
            std::cout << "Version [" << i << "]: \t{";
            for (size_t j = 0; j < versions[i].size(); j++) 
            {
                std::cout << "'" << versions[i][j] << "'";
                if (j < versions[i].size() - 1) 
                {
                    std::cout << ", ";
                }
            }
            std::cout << "}\n";
        }
    }
};


#endif // PERSISTENT_ARRAY_H