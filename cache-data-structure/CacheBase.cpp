#include "CacheBase.hpp"

CacheBase::CacheBase(int capacity)
:   m_capacity(capacity)
{
    m_size = 0;
    m_tracking_rank = false;
    
    t_chrono        = std::chrono::microseconds(0);
    t_age           = 0;
    t_hits          = 0;
    t_deleted_ranks = std::vector<int>(101);
}

void CacheBase::track_rank(bool tracking)
{
    m_tracking_rank = tracking;
}

void CacheBase::get_tracking(int w)
{
    std::cout << std::setw(w) << get_name();
    std::cout << std::setw(w) << m_capacity;
    std::cout << std::setw(w) << get_space();
    std::cout << std::setw(w) << double(t_hits)/double(t_age);
    std::cout << std::setw(w) << std::setw(w) << std::chrono::duration<double, std::micro>(t_chrono).count() / double(t_age) << std::endl;
}

void CacheBase::get_ranking(std::string dataset_name)
{
    std::cout << dataset_name << " ";
    std::cout << get_name() << " ";
    std::cout << m_capacity << " ";
    
    for (long count : t_deleted_ranks) std::cout << count << " ";
    std::cout << std::endl;
}
