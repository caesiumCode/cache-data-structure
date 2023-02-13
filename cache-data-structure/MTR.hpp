#ifndef MTR_hpp
#define MTR_hpp

#include "TreeCacheBase.hpp"

class MTR : public TreeCacheBase
{
public:
    MTR(int capacity);
        
    std::string get_name();
    
protected:
    void rotate_up(TreeNode* node);
};

#endif /* MTR_hpp */
