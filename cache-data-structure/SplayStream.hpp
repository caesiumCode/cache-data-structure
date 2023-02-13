#ifndef SplayStream_hpp
#define SplayStream_hpp

#include "LeafQueueBase.hpp"

class SplayStream : public LeafQueueBase
{
public:
    SplayStream(int capacity);
        
    std::string get_name();
    
protected:
    void rotate_up(TreeNode* node);
};

#endif /* SplayStream_hpp */
