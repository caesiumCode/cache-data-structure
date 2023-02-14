#include <memory>

#include "LRU.hpp"
#include "MTR.hpp"
#include "SplayStream.hpp"
#include "FixedCache.hpp"

/*
 ./program <path> <dataset> <n> <model>
 */

int main(int argc, const char * argv[])
{
    if (argc != 5)
    {
        std::cout << "INVALID ARGUMENT: Expected 4 arguments, " << argc-1 << " were given" << std::endl;
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
    else if (std::string(argv[4]) == "fc")  cache = std::make_unique<FixedCache>(n);
    else
    {
        std::cout << "INVALID ARGUMENT: Unknown model " << argv[4] << std::endl;
        return EXIT_FAILURE;
    }
    
    // Test cache
    while (std::fgets(line_buffer, sizeof(line_buffer), fp))
    {
        std::string key(line_buffer);
        if (key.back() == '\n') key.pop_back();
        
        cache->insert(key);
    }
        
    std::fclose(fp);
    
    // Output
    cache->get_tracking(14);
    
    return EXIT_SUCCESS;
}
