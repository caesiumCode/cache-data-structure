#ifndef CacheBase_hpp
#define CacheBase_hpp

#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <set>

using Timer         = std::chrono::high_resolution_clock;
using TimerMeasure  = std::chrono::time_point<Timer>;

struct ListNode
{
    std::string*    key;
    void*           value;
    
    ListNode* left;
    ListNode* right;
};

struct TreeNode
{
    std::string*    key;
    void*           value;
    
    bool leaf;
    
    TreeNode* parent;
    TreeNode* left;
    TreeNode* right;
    
    // Tracking
    int timestamp;
    mutable int rank;
    
    // Display
    std::string to_string(std::string indent, bool last)
    {
        std::string node_string = "+-" + *key + "(" + std::to_string(rank) + ")\n";
        
        if (!leaf)
        {
            std::string new_indent = indent + (last ? "   " : "|  ");
            
            if      (left != nullptr && right == nullptr)   return indent + node_string + left->to_string(new_indent, true);
            else if (left == nullptr && right != nullptr)   return indent + node_string + right->to_string(new_indent, true);
            else if (left != nullptr && right != nullptr)   return indent + node_string + left->to_string(new_indent, false) + right->to_string(new_indent, true);
        }
        
        return indent + node_string;
    }
};

auto tree_node_comp = [](TreeNode* a, TreeNode* b) { return a->timestamp < b->timestamp; };

class CacheBase
{
public:
    CacheBase(int capacity);
    
    void track_rank(bool tracking);
    
    virtual void        insert(const std::string& key)  = 0;
    virtual std::string to_string() = 0;
    virtual int         get_space() = 0;
    virtual std::string get_name() = 0;
    
    void get_tracking(int w);
    
    // Tracking
    std::chrono::duration<double, std::micro> t_chrono;
    int t_age;
    int t_hits;
    std::vector<int> t_deleted_ranks;
    
    
protected:
    const int   m_capacity;
    int         m_size;
    
    bool        m_tracking_rank;
};

#endif /* CacheBase_hpp */
