#include "TreeCacheBase.hpp"

TreeCacheBase::TreeCacheBase(int capacity)
: CacheBase(capacity)
{
    m_block = new TreeNode[m_capacity];
    m_root = nullptr;
    m_leaf_queue_front = nullptr;
    m_leaf_queue_back = nullptr;
    
    for (int i = 0; i < m_capacity; i++)
    {
        m_block[i].key      = new std::string;
        m_block[i].parent   = nullptr;
        m_block[i].left     = nullptr;
        m_block[i].right    = nullptr;
    }
}

TreeCacheBase::~TreeCacheBase()
{
    for (int i = 0; i < m_capacity; i++) delete m_block[i].key;
    
    delete [] m_block;
}

TreeNode* TreeCacheBase::find(const std::string &key)
{
    TreeNode* node = m_root;
    
    while (node && !node->leaf && *node->key != key)
    {
        TreeNode* next;
        if (key <= *node->key)  next = node->left;
        else                    next = node->right;
        
        if (next == nullptr) return node;
        
        node = next;
    }
    
    return node;
}


void TreeCacheBase::insert(const std::string &key)
{
    // Tracking
    t_age++;
    
    TimerMeasure START = Timer::now();
    
    TreeNode* node = find(key);
    
    // Cache hit
    if (node != nullptr && *node->key == key)   insert_hit(key, node);
    
    // Cache miss
    else                                        insert_miss(key, node);
    
    TimerMeasure END = Timer::now();
    
    // Tracking
    t_chrono += END - START;
}

void TreeCacheBase::insert_miss(const std::string &key, TreeNode *node)
{
    TreeNode* new_node;
    
    // If the tree is full
    if (m_size == m_capacity)
    {
        // Free space by removing a leaf
        new_node = pop_leaf_queue();
                
        // Increase rank of all the nodes that arrived after removed node
        if (m_tracking_rank)
        {
            TimerMeasure CP1 = Timer::now();
            
            std::set<TreeNode*>::iterator found_it = t_timestamp_heap.find(new_node);
            for (auto it = found_it; it != t_timestamp_heap.end(); it++) (*it)->rank++;
            t_timestamp_heap.erase(new_node);
            
            TimerMeasure CP2 = Timer::now();
            t_chrono -= CP2 - CP1;
        }
                
        // If the removed node was the parent of the to-be-inserted node, use it directly
        if (new_node == node)
        {
            *new_node->key = key;
            new_node->timestamp = t_age;
            new_node->rank = 1;
            
            if (m_tracking_rank)
            {
                TimerMeasure CP1 = Timer::now();
                t_timestamp_heap.insert(new_node);
                TimerMeasure CP2 = Timer::now();
                t_chrono -= CP2 - CP1;
            }
            
            move_to_root(new_node);
            return;
        }
        
        // Otherwise, completely detach the node from the tree
        TreeNode* to_remove_parent = new_node->parent;
        if (to_remove_parent->left == new_node) to_remove_parent->left = nullptr;
        else                                    to_remove_parent->right = nullptr;
        
        // If parent becomes a leaf, push it into the leaf queue
        if (to_remove_parent->left == nullptr && to_remove_parent->right == nullptr) push_leaf_queue(to_remove_parent);
    }
    else
    {
        new_node = &m_block[m_size];
        m_size++;
        
        if (m_tracking_rank)
        {
            TimerMeasure CP1 = Timer::now();
            
            for (auto it = t_timestamp_heap.begin(); it != t_timestamp_heap.end(); it++) (*it)->rank++;
            
            TimerMeasure CP2 = Timer::now();
            t_chrono -= CP2 - CP1;
        }
    }
    
    *new_node->key = key;
    new_node->leaf = false;
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = node;
    new_node->timestamp = t_age;
    new_node->rank = 1;
    
    if (m_tracking_rank)
    {
        TimerMeasure CP1 = Timer::now();
        t_timestamp_heap.insert(new_node);
        TimerMeasure CP2 = Timer::now();
        t_chrono -= CP2 - CP1;
    }
    
    if (m_root == nullptr)
    {
        m_root = new_node;
        push_leaf_queue(m_root);
    }
    else
    {
        attach(new_node);
        move_to_root(new_node);
    }
}

void TreeCacheBase::insert_hit(const std::string &key, TreeNode *node)
{
    // Tracking
    t_hits++;
    
    if (m_tracking_rank)
    {
        // Increase the rank of all the nodes that arrived after the inserted node
        std::set<TreeNode*>::iterator it_found = t_timestamp_heap.find(node);
        for (auto it = it_found; it != t_timestamp_heap.end(); it++) (*it)->rank++;
        
        // Update the rank of the inserted node to 1
        t_timestamp_heap.erase(node);
        
        node->rank = 1;
        node->timestamp = t_age;
        
        t_timestamp_heap.insert(node);
    }
        
    move_to_root(node);
}

void TreeCacheBase::attach(TreeNode *node)
{
    TreeNode* parent    = node->parent;
    TreeNode* gparent   = node->parent->parent;
    
    parent->parent = node;
    node->parent = gparent;
    
    if (*parent->key <= *node->key) node->left = parent;
    else                            node->right = parent;
    
    if (gparent)
    {
        if (gparent->left == parent)    gparent->left = node;
        else                            gparent->right = node;
    }
    
    if (gparent == nullptr) m_root = node;
}

void TreeCacheBase::move_to_root(TreeNode* x)
{
    if (x->leaf) remove_leaf_queue(x);
    
    while (x != m_root) rotate_up(x);
}

void TreeCacheBase::push_leaf_queue(TreeNode* node)
{
    node->leaf = true;
    node->left = nullptr;
    node->right = nullptr;
    
    if (m_leaf_queue_front == nullptr)
    {
        m_leaf_queue_front = node;
        m_leaf_queue_back = node;
        
        node->left = nullptr;
        node->right = nullptr;
    }
    else
    {
        node->left = m_leaf_queue_back;
        node->right = nullptr;
        
        m_leaf_queue_back->right = node;
        
        m_leaf_queue_back = node;
    }
}

TreeNode* TreeCacheBase::pop_leaf_queue()
{
    if (m_leaf_queue_front == nullptr) return nullptr;
        
    TreeNode* node = m_leaf_queue_front;
    
    m_leaf_queue_front = m_leaf_queue_front->right;
    if (m_leaf_queue_front != nullptr) m_leaf_queue_front->left = nullptr;
    
    node->leaf = false;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

void TreeCacheBase::remove_leaf_queue(TreeNode* node)
{
    if (!node->leaf) return;
        
    if (m_leaf_queue_front == m_leaf_queue_back)
    {
        m_leaf_queue_front = nullptr;
        m_leaf_queue_back = nullptr;
    }
    else if (node == m_leaf_queue_front)
    {
        m_leaf_queue_front = m_leaf_queue_front->right;
        if (m_leaf_queue_front != nullptr) m_leaf_queue_front->left = nullptr;
    }
    else if (node == m_leaf_queue_back)
    {
        m_leaf_queue_back = m_leaf_queue_back->left;
        if (m_leaf_queue_back != nullptr) m_leaf_queue_back->right = nullptr;
    }
    else
    {
        if (node->left != nullptr)  node->left->right = node->right;
        if (node->right != nullptr) node->right->left = node->left;
    }
    
    node->leaf = false;
    node->left = nullptr;
    node->right = nullptr;
}

std::string TreeCacheBase::to_string()
{
    return m_root->to_string("", true);
}
