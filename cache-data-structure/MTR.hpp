#ifndef MTR_hpp
#define MTR_hpp

#include "CacheBase.hpp"

class MTR : public CacheBase
{
public:
    MTR(int capacity);
    ~MTR();
    
    void        insert(const std::string& key);
    
    int         get_space();
    std::string get_name();
    std::string to_string();
    
private:
    TreeNode* m_block;
    TreeNode* m_root;
    TreeNode* m_leaf_queue_front;
    TreeNode* m_leaf_queue_back;
    
    // Tracking
    std::set<TreeNode*, decltype(tree_node_comp)> t_timestamp_heap;
    
private:
    void        attach(TreeNode* node);
    void        move_to_root(TreeNode* node);
    TreeNode*   find(const std::string& key);
    
    void        insert_hit(const std::string& key, TreeNode* node);
    void        insert_miss(const std::string& key, TreeNode* node);
    
    void        rotate_up(TreeNode* x);
    
    void        push_leaf_queue(TreeNode* node);
    TreeNode*   pop_leaf_queue();
    void        remove_leaf_queue(TreeNode* node);
};

#endif /* MTR_hpp */
