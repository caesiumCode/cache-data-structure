#include "FC.hpp"

FC::FC(int capacity)
: CacheBase(capacity)
{
    m_map.clear();
}

void FC::insert(const std::string &key)
{
    // Tracking
    t_age++;
    
    TimerMeasure START = Timer::now();
        
    if (!m_map.contains(key))
    {
        if (m_size < m_capacity)
        {
            m_map[key] = nullptr;
            m_size++;
        }
    }
    else t_hits++;
    
    TimerMeasure END = Timer::now();
    
    // Tracking
    t_chrono += END - START;
}

int FC::get_space()
{
    int space_bucket = 0;
    
    for (int i = 0; i < m_map.bucket_count(); i++) space_bucket += m_map.bucket_size(i);
    
    return 8 * space_bucket + m_capacity * 8 * 3;
}

std::string FC::get_name()
{
    return "Fixed Cache";
}

std::string FC::to_string()
{
    std::string str;
    
    for (const auto& [key, value] : m_map) str += key + " ";
    
    return str;
}
