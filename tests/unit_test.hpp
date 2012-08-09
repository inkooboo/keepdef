//
//  test.hpp
//  keepdef
//
//  Created by inkooboo on 8/9/12.
//
//

#ifndef keepdef_test_hpp
#define keepdef_test_hpp

# include <string>


#define TEST_TRUE(expression) \
    do { \
        if (expression) {void(0);} \
        else { \
            std::cout << "FAILED" << #expression << std::endl; \
            std::cout << "file: " << __FILE__ << " line: " << __LINE__ << std::endl; \
            return TEST_FAILED; } \
    } while(false);

enum test_result_t
{
      TEST_OK
    , TEST_FAILED
};

struct unit_test_t
{
    virtual test_result_t run() = 0;
    
    virtual ~unit_test_t() {}

    unit_test_t(const char *name)
        : m_name(name)
    {}
    
    const std::string & name() const
    {
        return m_name;
    }
    
private:
    std::string m_name;
};

#endif
