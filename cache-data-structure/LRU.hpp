#ifndef LRU_hpp
#define LRU_hpp

#include "CacheBase.hpp"

struct ListNode
{
    std::string*    key;
    void*           value;
    
    ListNode* left;
    ListNode* right;
};

class LRU : protected CacheBase
{
public:
    LRU(int capacity);
    ~LRU();
    
    void inserted(const std::string& key);
    
private:
    ListNode* m_block;
    ListNode* m_front;
    ListNode* m_back;
    
    std::unordered_map<std::string, ListNode*> m_map;
};

#endif /* LRU_hpp */
