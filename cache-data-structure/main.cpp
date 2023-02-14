#include <memory>

#include "LRU.hpp"
#include "SplayStream.hpp"
#include "MTR.hpp"
#include "SE.hpp"
#include "FixedCache.hpp"

/*
 ./program <path> <dataset> <n> <model> <ranking>
 */

int main(int argc, const char * argv[])
{
    if (argc != 6)
    {
        std::cout << "INVALID ARGUMENT: Expected 5 arguments, " << argc-1 << " were given" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Set stream
    std::string path    = std::string(argv[1]);
    std::string dataset = std::string(argv[2]);
    
    const int   LINE_BUFFER_SIZE = 1 << 10;
    char        line_buffer[LINE_BUFFER_SIZE];
    std::FILE* fp = std::fopen((path + dataset).c_str(), "r");
    
    // Set cache
    int         n       = std::atoi(argv[3]);
    std::unique_ptr<CacheBase> cache;
    
    if      (std::string(argv[4]) == "lru") cache = std::make_unique<LRU>(n);
    else if (std::string(argv[4]) == "st")  cache = std::make_unique<SplayStream>(n);
    else if (std::string(argv[4]) == "mtr") cache = std::make_unique<MTR>(n);
    else if (std::string(argv[4]) == "se")  cache = std::make_unique<SE>(n);
    else if (std::string(argv[4]) == "fc")  cache = std::make_unique<FixedCache>(n);
    else
    {
        std::cout << "INVALID ARGUMENT: Unknown model " << argv[4] << std::endl;
        return EXIT_FAILURE;
    }
    
    bool track_ranking;
    if (std::atoi(argv[5]) == 0)    track_ranking = false;
    else                            track_ranking = true;
    
    cache->track_rank(track_ranking);
    
    // Test cache
    while (std::fgets(line_buffer, sizeof(line_buffer), fp))
    {
        std::string key(line_buffer);
        if (key.back() == '\n') key.pop_back();
        
        cache->insert(key);
    }
        
    std::fclose(fp);
    
    // Output
    if (track_ranking)  cache->get_ranking(dataset);
    else                cache->get_tracking(dataset);
    
    return EXIT_SUCCESS;
}
