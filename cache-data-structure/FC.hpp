#ifndef FC_hpp
#define FC_hpp

#include "CacheBase.hpp"

class FC : public CacheBase
{
public:
    FC(int capacity);
    
    void        insert(const std::string& key);
    
    int         get_space();
    std::string get_name();
    std::string to_string();
    
private:
    std::unordered_map<std::string, void*> m_map;
};

#endif /* FC_hpp */
