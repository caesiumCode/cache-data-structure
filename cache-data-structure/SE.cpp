#include "SE.hpp"

SE::SE(int capacity)
: LeafQueueBase(capacity)
{
    
}

void SE::move_heuristic(TreeNode* node)
{
    if (node->leaf) remove_leaf_queue(node);
    
    if (node != m_root) rotate_up(node);
}

void SE::rotate_up(TreeNode *x)
{
    TreeNode* y         = x->parent;
    TreeNode* ancestor  = y->parent;
    
    /*
     Zig
     
           y                   x
          / \                 / \
         /   \               /   \
        x     R     ===>    L     y
       / \                       / \
      /   \                     /   \
     L     M                   M     R
     
     */
    if (y->left == x)
    {
        TreeNode* M = x->right;
        
        // Rotate
        x->right = y;
        x->parent = y->parent;
        y->left = M;
        y->parent = x;
        if (M) M->parent = y;
        
        // If y becomes a leaf
        if (!y->left and !y->right) push_leaf_queue(y);
    }
    /*
     Zag
     
           y                    x
          / \                  / \
         /   \                /   \
        L     x     ===>     y     R
             / \            / \
            /   \          /   \
           M     R        L     M
     
     */
    else
    {
        TreeNode* M = x->left;
        
        
        // Rotate
        x->left = y;
        x->parent = y->parent;
        y->right = M;
        y->parent = x;
        if (M) M->parent = y;
        
        // If y becomes a leaf
        if (!y->left and !y->right) push_leaf_queue(y);
    }
    
    if (ancestor == nullptr) m_root = x;
    else
    {
        if (ancestor->left == y)    ancestor->left = x;
        else                        ancestor->right = x;
    }
}

std::string SE::get_name()
{
    return "SE Tree";
}
