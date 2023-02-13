#include "LRU.hpp"
#include "MTR.hpp"

int main(int argc, const char * argv[])
{
    auto START = std::chrono::high_resolution_clock::now();
    
    std::string path = "/Users/stephen/Desktop/Research Assistant/splay stream/data/";
    std::string dataset = "newswire";
    
    int n = 1 << 12;
    
    LRU lru(n);
    lru.track_rank(false);
    
    MTR mtr(n);
    mtr.track_rank(false);
    
    const int LINE_BUFFER_SIZE = 1 << 10;
    std::FILE* fp = std::fopen((path + dataset).c_str(), "r");
    
    char line_buffer[LINE_BUFFER_SIZE];
        
    while (std::fgets(line_buffer, sizeof(line_buffer), fp))
    {
        std::string key(line_buffer);
        if (key.back() == '\n') key.pop_back();
        
        //std::cout << "INSERT\t" << key << std::endl;
        
        lru.insert(key);
        mtr.insert(key);
        
        //std::cout << mtr.to_string() << std::endl;
    }
        
    std::fclose(fp);
    
    int w = 11;
    std::cout
    << std::setw(w) << "name"
    << std::setw(w) << "n"
    << std::setw(w) << "space"
    << std::setw(w) << "hit rate"
    << std::setw(w) << "time(us)" << std::endl;
    lru.get_tracking(w);
    mtr.get_tracking(w);
    
    auto END = std::chrono::high_resolution_clock::now();
    
    std::cout << "execution time\t" << std::chrono::duration<double>(END-START).count() << "s" << std::endl;
    
    //std::cout << mtr.to_string() << std::endl;
    
    return 0;
}
