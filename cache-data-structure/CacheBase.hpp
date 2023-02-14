#ifndef CacheBase_hpp
#define CacheBase_hpp

#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <map>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <set>
#include <cmath>
#include <functional>

using Timer         = std::chrono::high_resolution_clock;
using TimerMeasure  = std::chrono::time_point<Timer>;

class CacheBase
{
public:
    CacheBase(int capacity);
    virtual ~CacheBase() = default;
    
    void track_rank(bool tracking);
    
    virtual void        insert(const std::string& key)  = 0;
    virtual std::string to_string() = 0;
    virtual int         get_space() = 0;
    virtual std::string get_name() = 0;
    
    void get_tracking(int w);
    void get_ranking(std::string dataset_name);
    
protected:
    const int   m_capacity;
    int         m_size;
    
    bool        m_tracking_rank;
    
    // Tracking
    std::chrono::duration<double, std::micro> t_chrono;
    int t_age;
    int t_hits;
    std::vector<int> t_deleted_ranks;
};

#endif /* CacheBase_hpp */
