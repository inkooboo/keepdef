//
//  event_test.hpp
//  keepdef
//
//  Created by inkooboo on 8/9/12.
//
//

#ifndef keepdef_event_test_hpp
#define keepdef_event_test_hpp

#include "unit_test.hpp"

#include "../common/event.hpp"

#include <iostream>

struct event_unit_test_t : public unit_test_t
{
    event_unit_test_t() : unit_test_t("Events") {}
    
    virtual test_result_t run()
    {
        event_t start(EV_CLIENT_START);
        TEST_TRUE(start.description().id() == EV_CLIENT_START);
        TEST_TRUE(start.description().has_parameter("login"));
        TEST_TRUE(start.description().has_parameter("password"));
        
        start.set<std::string>("login", "inkooboo");
        start.set<std::string>("password", "megapass890");
        
        std::string serialized = start.serialize();
        
        std::cout << serialized << std::endl;
        
        event_t parsed_event(serialized.c_str(), serialized.length());
        
        TEST_TRUE(parsed_event.description().id() == EV_CLIENT_START);
        TEST_TRUE(parsed_event.get<std::string>("login") == "inkooboo");
        TEST_TRUE(parsed_event.get<std::string>("password") == "megapass890");
        
        return TEST_OK;
    }
};

#endif
