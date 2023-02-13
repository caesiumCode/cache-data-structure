#include "LRU.hpp"
#include "MTR.hpp"
#include "SplayStream.hpp"
#include "FixedCache.hpp"

int main(int argc, const char * argv[])
{
    TimerMeasure START = Timer::now();
    
    std::string path = "/Users/jauds/Desktop/Datasets/ss/";
    std::string dataset = "apw-parsed";
    
    int n = 1 << 10;
    
    LRU lru(n);
    lru.track_rank(false);
    
    SplayStream splay_stream(n);
    splay_stream.track_rank(false);
    
    MTR mtr(n);
    mtr.track_rank(false);
    
    FixedCache fixed_cache(n);
    fixed_cache.track_rank(false);
    
    const int LINE_BUFFER_SIZE = 1 << 10;
    std::FILE* fp = std::fopen((path + dataset).c_str(), "r");
    
    char line_buffer[LINE_BUFFER_SIZE];
        
    while (std::fgets(line_buffer, sizeof(line_buffer), fp))
    {
        std::string key(line_buffer);
        if (key.back() == '\n') key.pop_back();
        
        //std::cout << "INSERT\t" << key << std::endl;
        
        lru.insert(key);
        splay_stream.insert(key);
        mtr.insert(key);
        fixed_cache.insert(key);
        
        //std::cout << st.to_string() << std::endl;
    }
        
    std::fclose(fp);
    
    int w = 14;
    std::cout
    << std::setw(w) << "name"
    << std::setw(w) << "n"
    << std::setw(w) << "space"
    << std::setw(w) << "hit rate"
    << std::setw(w) << "time(us)" << std::endl << std::endl;
    lru.get_tracking(w);
    splay_stream.get_tracking(w);
    mtr.get_tracking(w);
    fixed_cache.get_tracking(w);
    
    TimerMeasure END = Timer::now();
    
    std::cout << std::endl;
    std::cout << "execution time\t" << std::chrono::duration<double>(END-START).count() << "s" << std::endl;
    
    //std::cout << mtr.to_string() << std::endl;
    
    return 0;
}
