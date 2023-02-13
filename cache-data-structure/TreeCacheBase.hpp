#ifndef TreeCacheBase_hpp
#define TreeCacheBase_hpp

#include "CacheBase.hpp"

class TreeCacheBase : public CacheBase
{
public:
    TreeCacheBase(int capacity);
    ~TreeCacheBase();
    
    void insert(const std::string& key);
    
    virtual std::string get_name() = 0;
    int         get_space();
    std::string to_string();
    
protected:
    TreeNode* m_block;
    TreeNode* m_root;
    TreeNode* m_leaf_queue_front;
    TreeNode* m_leaf_queue_back;
    
    // Tracking
    std::set<TreeNode*, decltype(tree_node_comp)> t_timestamp_heap;
    
    // Move to root rule
    virtual void rotate_up(TreeNode* x) = 0;
    
private:
    void        attach(TreeNode* node);
    void        move_to_root(TreeNode* node);
    TreeNode*   find(const std::string& key);
    
    void        insert_hit(const std::string& key, TreeNode* node);
    void        insert_miss(const std::string& key, TreeNode* node);
    
protected:
    void        push_leaf_queue(TreeNode* node);
    TreeNode*   pop_leaf_queue();
    void        remove_leaf_queue(TreeNode* node);
};

#endif /* TreeCacheBase_hpp */
