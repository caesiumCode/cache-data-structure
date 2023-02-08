#ifndef CacheBase_hpp
#define CacheBase_hpp

#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <chrono>

class CacheBase
{
public:
    CacheBase(int capacity);
    void track(bool tracking);
    
    virtual void insert(const std::string& key) = 0;
    
    // Tracking
    std::chrono::duration<double, std::micro> t_chrono;
    int t_age;
    int t_hits;
    std::vector<int> t_deleted_ranks;
    
    
protected:
    const int   m_capacity;
    int         m_size;
    
    bool        m_tracking;
    
    
    
};

#endif /* CacheBase_hpp */
