//
//  main.cpp
//  keepdef
//
//  Created by inkooboo on 8/9/12.
//
//

#include "event_unit_test.hpp"

#include <iostream>
#include <vector>
#include <memory>

int main()
{
    std::vector<std::unique_ptr<unit_test_t>> tests;
    
    tests.push_back(std::unique_ptr<unit_test_t>(new event_unit_test_t));
    

    for (auto &test : tests)
    {
        std::cout << "TEST: " << test->name() << std::endl;
        test_result_t res = test->run();
        std::cout << (TEST_OK == res ? "succeed" : "FAILED!") << std::endl;
    }
    
    
    return 0;
}