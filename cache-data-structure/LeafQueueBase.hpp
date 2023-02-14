#ifndef LeafQueueBase_hpp
#define LeafQueueBase_hpp

#include "CacheBase.hpp"

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




class LeafQueueBase : public CacheBase
{
public:
    LeafQueueBase(int capacity);
    ~LeafQueueBase();
    
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
    std::function<bool (TreeNode*, TreeNode*)> tree_node_comp = [](TreeNode* a, TreeNode* b) { return a->timestamp < b->timestamp; };
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

#endif /* LeafQueueBase_hpp */
