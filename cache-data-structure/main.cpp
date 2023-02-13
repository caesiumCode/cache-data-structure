#include "LRU.hpp"
#include "MTR.hpp"
#include "ST.hpp"
#include "FC.hpp"

int main(int argc, const char * argv[])
{
    TimerMeasure START = Timer::now();
    
    std::string path = "/Users/stephen/Desktop/Research Assistant/splay stream/data/";
    std::string dataset = "apw-parsed";
    
    int n = 1 << 10;
    
    LRU lru(n);
    lru.track_rank(false);
    
    MTR mtr(n);
    mtr.track_rank(false);
    
    ST st(n);
    st.track_rank(false);
    
    FC fc(n);
    fc.track_rank(false);
    
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
        st.insert(key);
        fc.insert(key);
        
        //std::cout << st.to_string() << std::endl;
    }
        
    std::fclose(fp);
    
    int w = 12;
    std::cout
    << std::setw(w) << "name"
    << std::setw(w) << "n"
    << std::setw(w) << "space"
    << std::setw(w) << "hit rate"
    << std::setw(w) << "time(us)" << std::endl << std::endl;
    lru.get_tracking(w);
    mtr.get_tracking(w);
    st.get_tracking(w);
    fc.get_tracking(w);
    
    TimerMeasure END = Timer::now();
    
    std::cout << std::endl;
    std::cout << "execution time\t" << std::chrono::duration<double>(END-START).count() << "s" << std::endl;
    
    //std::cout << mtr.to_string() << std::endl;
    
    return 0;
}
