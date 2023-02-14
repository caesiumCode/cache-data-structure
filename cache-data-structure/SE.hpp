#ifndef SE_hpp
#define SE_hpp

#include "LeafQueueBase.hpp"

class SE : public LeafQueueBase
{
public:
    SE(int capacity);
        
    std::string get_name();
    
protected:
    void move_heuristic(TreeNode* node);
    
private:
    void rotate_up(TreeNode* node);
};

#endif /* SE_hpp */
