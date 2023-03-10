#ifndef MTR_hpp
#define MTR_hpp

#include "LeafQueueBase.hpp"

class MTR : public LeafQueueBase
{
public:
    MTR(int capacity);
        
    std::string get_name();
    
protected:
    void move_heuristic(TreeNode* node);
    
private:
    void rotate_up(TreeNode* node);
};

#endif /* MTR_hpp */
