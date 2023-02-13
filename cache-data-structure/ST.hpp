#ifndef ST_hpp
#define ST_hpp

#include "TreeCacheBase.hpp"

class ST : public TreeCacheBase
{
public:
    ST(int capacity);
        
    std::string get_name();
    
protected:
    void rotate_up(TreeNode* node);
};

#endif /* ST_hpp */
