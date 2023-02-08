#include "CacheBase.hpp"

CacheBase::CacheBase(int capacity)
:   m_capacity(capacity)
{
    m_tracking = false;
    
    t_chrono        = std::chrono::microseconds(0);
    t_age           = 0;
    t_hits          = 0;
    t_deleted_ranks = std::vector<int>(101);
}

void CacheBase::track(bool tracking)
{
    m_tracking = tracking;
}
