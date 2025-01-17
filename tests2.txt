// Test fixture for PersistentArray tests
class PersistentArrayTest : public ::testing::Test 
{
protected:

    PersistentArray<int>* array;

    void SetUp() override 
    {
        int init_arr[] = { 1, 2, 3, 4, 5 };
        array = new PersistentArray<int>(init_arr, 5);
    }

    void TearDown() override 
    {
        delete array;
    }
};


TEST_F(PersistentArrayTest, InitialVersion) 
{
    EXPECT_EQ(array->getVersion(0), std::vector<int>({ 1, 2, 3, 4, 5 }));
}


TEST_F(PersistentArrayTest, AddVersion) 
{
    array->addVersion(0, 0, 10); // Change the first version with 10
    EXPECT_EQ(array->getVersion(1), std::vector<int>({ 10, 2, 3, 4, 5 }));
}


TEST_F(PersistentArrayTest, AddVersionInvalidIndex) 
{
    EXPECT_THROW(array->addVersion(0, 10, 0), std::out_of_range);
}


TEST_F(PersistentArrayTest, Undo) 
{
    array->addVersion(0, 0, 10); // Change the first version with 10
    array->undo(); 
    //array->printAllVersions();
    EXPECT_EQ(array->getVersion(2), std::vector<int>({ 1, 2, 3, 4, 5 }));
}


TEST_F(PersistentArrayTest, Redo) 
{
    array->addVersion(0, 0, 10); // Change the first version with 10
    array->undo(); // Version[1]
    array->redo(); // Version[2]
    //array->printAllVersions();
    EXPECT_EQ(array->getVersion(3), std::vector<int>({ 10, 2, 3, 4, 5 }));
}


TEST_F(PersistentArrayTest, GetInvalidVersion) 
{
    EXPECT_THROW(array->getVersion(10), std::out_of_range);
}


TEST_F(PersistentArrayTest, NoActionToUndo) 
{
    testing::internal::CaptureStdout();
    array->undo(); 
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "No actions to undo!\n");
}

TEST_F(PersistentArrayTest, NoActionToRedo) 
{
    testing::internal::CaptureStdout();
    array->redo(); 
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "No actions to redo!\n");
}

// Test fixture for PersistentDoublyLinkedList tests
class PersistentDoublyLinkedListTest : public ::testing::Test 
{
protected:
    PersistentDoublyLinkedList<int>* list;

    void SetUp() override 
    {
        int init_arr[] = { 1, 2, 3, 4, 5 };
        list = new PersistentDoublyLinkedList<int>(init_arr, 5);
    }

    void TearDown() override 
    {
        delete list;
    }
};

TEST_F(PersistentDoublyLinkedListTest, InitialVersion) 
{
    EXPECT_EQ(list->getVersion(0), std::vector<int>({ 1, 2, 3, 4, 5 }));
}

TEST_F(PersistentDoublyLinkedListTest, PushFront) 
{
    list->push_front(0); // Add 0 to the front
    EXPECT_EQ(list->getVersion(1), std::vector<int>({ 0, 1, 2, 3, 4, 5 }));
}

TEST_F(PersistentDoublyLinkedListTest, PushBack) 
{
    list->push_back(6); // Add 6 to the end
    EXPECT_EQ(list->getVersion(1), std::vector<int>({ 1, 2, 3, 4, 5, 6 }));
}

TEST_F(PersistentDoublyLinkedListTest, Undo) 
{
    list->push_front(0); // Add 0 to the front
    list->undo(); 
    //list->printAllVersions();
    EXPECT_EQ(list->getVersion(2), std::vector<int>({ 1, 2, 3, 4, 5 })); 
}

TEST_F(PersistentDoublyLinkedListTest, Redo) {
    list->push_front(0); // Add 0 to the front
    list->undo(); // Version[1]
    list->redo(); // Version[2]
    //list->printAllVersions();
    EXPECT_EQ(list->getVersion(3), std::vector<int>({ 0, 1, 2, 3, 4, 5 }));
}

TEST_F(PersistentDoublyLinkedListTest, GetInvalidVersion) 
{
    EXPECT_THROW(list->getVersion(10), std::out_of_range);
}

TEST_F(PersistentDoublyLinkedListTest, NoActionToUndo) 
{
    testing::internal::CaptureStdout();
    list->undo(); 
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "No actions to undo!\n");
}

TEST_F(PersistentDoublyLinkedListTest, NoActionToRedo) 
{
    testing::internal::CaptureStdout();
    list->redo(); 
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "No actions to redo!\n");
}

// Test fixture for PersistentAssociativeArray tests
class PersistentAssociativeArrayTest : public ::testing::Test 
{
protected:
    PersistentAssociativeArray<int, std::string>* array;

    void SetUp() override 
    {
        std::vector<int> keys = { 1, 2, 3 };
        std::string values[] = { "A", "B", "C" };
        array = new PersistentAssociativeArray<int, std::string>(keys, values, 3);
    }

    void TearDown() override 
    {
        delete array;
    }
};

TEST_F(PersistentAssociativeArrayTest, InitialVersion) 
{
    EXPECT_EQ(array->getVersion(0), std::vector<std::string>({ "A", "B", "C" }));
}

TEST_F(PersistentAssociativeArrayTest, AddVersionSameKey) 
{
    array->addVersion(0, 2, "D"); // Change the value with key 2 to "D"
    EXPECT_EQ(array->getVersion(1), std::vector<std::string>({ "A", "D", "C" }));
}

TEST_F(PersistentAssociativeArrayTest, AddVersionNewKey) 
{
    array->addVersion(0, 4, "D"); // Add key 4 with value "D" 
    EXPECT_EQ(array->getVersion(1), std::vector<std::string>({ "A", "B", "C", "D" }));
}

TEST_F(PersistentAssociativeArrayTest, AddVersionInvalidIndex) 
{
    EXPECT_THROW(array->addVersion(10, 2, "X"), std::out_of_range);
}

TEST_F(PersistentAssociativeArrayTest, Undo) 
{
    array->addVersion(0, 2, "D"); // Change value for key 2 to "D"
    array->undo(); 
    EXPECT_EQ(array->getVersion(2), std::vector<std::string>({ "A", "B", "C" }));
}

TEST_F(PersistentAssociativeArrayTest, Redo) 
{
    array->addVersion(0, 2, "D"); // Change value for key 2 to "D"
    array->undo(); // Version[1]
    array->redo(); // Version[2]
    EXPECT_EQ(array->getVersion(3), std::vector<std::string>({ "A", "D", "C" }));
}

TEST_F(PersistentAssociativeArrayTest, GetInvalidVersion) 
{
    EXPECT_THROW(array->getVersion(10), std::out_of_range);
}

TEST_F(PersistentAssociativeArrayTest, NoActionToUndo) 
{
    testing::internal::CaptureStdout();
    array->undo(); 
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "No actions to undo!\n");
}

TEST_F(PersistentAssociativeArrayTest, NoActionToRedo) 
{
    testing::internal::CaptureStdout();
    array->redo(); 
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "No actions to redo!\n");
}

// Test fixture for the Convert class tests
class ConvertTest : public ::testing::Test 
{
protected:
    PersistentArray<double>* array;
    PersistentDoublyLinkedList<double>* list;
    PersistentAssociativeArray<int, double>* associative_array;

    void SetUp() override 
    {
        std::vector<double> values2 = { 100.1, 200.2, 300.3, 400.4, 500.5 };
        array = new PersistentArray<double>(values2.data(), values2.size());

        list = new PersistentDoublyLinkedList<double>(values2, values2.size());

        std::vector<int> _keys = { 1, 2, 3, 4, 5 };
        associative_array = new PersistentAssociativeArray<int, double>(_keys, values2.data(), values2.size());
    }

    void TearDown() override 
    {
        delete array;
        delete list;
        delete associative_array;
    }
};

// Test conversion from PersistentArray to PersistentDoublyLinkedList
TEST_F(ConvertTest, ConvertArrayToList) 
{
    PersistentDoublyLinkedList<double> newList = Convert<double>::convertArrayToList(*array, 0);
    EXPECT_EQ(newList.getVersion(0), std::vector<double>({ 100.1, 200.2, 300.3, 400.4, 500.5 }));
}

// Test conversion from PersistentDoublyLinkedList to PersistentArray
TEST_F(ConvertTest, ConvertListToArray) 
{
    PersistentArray<double> newArray = Convert<double>::convertListToArray(*list, 0);
    EXPECT_EQ(newArray.getVersion(0), std::vector<double>({ 100.1, 200.2, 300.3, 400.4, 500.5 }));
}

// Test conversion from PersistentArray to PersistentAssociativeArray
TEST_F(ConvertTest, ConvertArrayToAssociativeArray) 
{
    std::vector<int> keys = { 1, 2, 3, 4, 5 };
    PersistentAssociativeArray<int, double> newAssociativeArray =
        Convert<double>::convertArrayToAssociativeArray<int>(*array, keys, 0);
    EXPECT_EQ(newAssociativeArray.getVersion(0), std::vector<double>({ 100.1, 200.2, 300.3, 400.4, 500.5 }));
}

// Test conversion from PersistentDoublyLinkedList to PersistentAssociativeArray
TEST_F(ConvertTest, ConvertListToAssociativeArray) 
{
    std::vector<int> keys = { 1, 2, 3, 4, 5 };
    PersistentAssociativeArray<int, double> newAssociativeArray =
        Convert<double>::convertListToAssociativeArray<int>(*list, keys, 0);
    EXPECT_EQ(newAssociativeArray.getVersion(0), std::vector<double>({ 100.1, 200.2, 300.3, 400.4, 500.5 }));
}



// Test conversion from PersistentAssociativeArray to PersistentDoublyLinkedList
TEST_F(ConvertTest, ConvertAssociativeArrayToList) 
{
    PersistentDoublyLinkedList<double> newList =
        Convert<double>::convertAssociativeArrayToList(*associative_array);

    std::vector<double> expected_values = associative_array->getVersion(0);

    EXPECT_EQ(newList.getVersion(0), expected_values);
}

// Test conversion from PersistentAssociativeArray to PersistentArray
TEST_F(ConvertTest, ConvertAssociativeArrayToArray) 
{
    PersistentArray<double> newArray =
        Convert<double>::convertAssociativeArrayToArray(*associative_array);

    std::vector<double> expected_values = associative_array->getVersion(0);

    EXPECT_EQ(newArray.getVersion(0), expected_values);
}

// Test for invalid key size in Convert Array to AssociativeArray
TEST_F(ConvertTest, ConvertArrayToAssociativeArrayInvalidKeys) 
{
    std::vector<int> keys = { 1, 2 }; 
    EXPECT_THROW(Convert<double>::convertArrayToAssociativeArray<int>(*array, keys, 0), std::invalid_argument);
}

// Test for invalid key size in Convert List to AssociativeArray
TEST_F(ConvertTest, ConvertListToAssociativeArrayInvalidKeys) 
{
    std::vector<int> keys = { 1, 2 }; 
    EXPECT_THROW(Convert<double>::convertListToAssociativeArray<int>(*list, keys, 0), std::invalid_argument);
}
