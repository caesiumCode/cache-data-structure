#include "LRU.hpp"

LRU::LRU(int capacity)
: CacheBase(capacity)
{
    m_block = new ListNode[m_capacity];
    m_front = nullptr;
    m_back = nullptr;
    
    for (int i = 0; i < m_capacity; i++)
    {
        m_block[i].key      = new std::string;
        m_block[i].left     = nullptr;
        m_block[i].right    = nullptr;
    }
}

LRU::~LRU()
{
    for (int i = 0; i < m_capacity; i++) delete m_block[i].key;
    
    delete [] m_block;
}

void LRU::inserted(const std::string &key)
{
    if (m_tracking) t_age++;
    
    auto CP1 = std::chrono::high_resolution_clock::now();
    
    std::unordered_map<std::string, ListNode*>::iterator hint = m_map.find(key);
    
    /*
     Select a node
     */
    
    ListNode* node;
    
    if (hint == m_map.end())
    {
        if (m_size < m_capacity)
        {
            node = &m_back[m_size];
            m_size++;
        }
        else
        {
            node = m_back;
            
            if (m_tracking) t_deleted_ranks[100]++;
        }
    }
    else
    {
        node = hint->second;
        
        if (m_tracking) t_hits++;
    }
    
    /*
     Remove the node
     */
    
    
    if (node == m_back)  m_back  = node->left;
    if (node == m_front) m_front = node->right;
    
    if (node->left)     node->left->right = node->right;
    if (node->right)    node->right->left = node->left;
    
    /*
     Attach the node
     */
    
    *(node->key)    = std::string(key);
    node->left      = nullptr;
    node->right     = m_front;
    
    if (m_front) m_front->left = node;
    
    if (m_size == 1) m_back = node;
    
    m_front = node;
    m_size++;
    
    auto CP2 = std::chrono::high_resolution_clock::now();
    
    if (m_tracking) t_chrono = CP2 - CP1;
}
