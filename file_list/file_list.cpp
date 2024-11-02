#include <algorithm>
#include <iostream>
#include <filesystem>
#include <SDL_wrapper/wrapper.h>
#include <vector>

std::vector<std::filesystem::path> arr;

uint64_t diretry_total_size(std::filesystem::path path) {
    auto it = std::filesystem::recursive_directory_iterator(path);
    uint64_t size = 0;
    for (const auto& entry : it) {
        if (entry.is_regular_file())  {
            size += entry.file_size();
        }        
    }
    return size;
}

void iterate(std::filesystem::path path) {
    auto it = std::filesystem::directory_iterator(path);
    uint64_t size = 0;
    uint64_t cnt = 0;
    std::vector<std::pair<std::filesystem::path, uint64_t>> arr;
    for (const auto& entry : it) {
        if (entry.is_regular_file())  {
            arr.push_back(std::pair<std::filesystem::path, uint64_t>(entry.path(), entry.file_size()));
        } else if (entry.is_directory()) {
            uint64_t size = diretry_total_size(entry.path());
            arr.push_back(std::pair<std::filesystem::path, uint64_t>(entry.path(), size));
        }      
    }
    
    std::sort(arr.begin(), arr.end(), [](std::pair<std::filesystem::path, uint64_t> &a, std::pair<std::filesystem::path, uint64_t> &b) {
        return a.second > b.second;
    });
    
    for (auto const &pair:arr) {
        if (pair.second < 1024) {
            std::cout << pair.first.filename() << "        " << pair.second << "B\n";
        } else if (pair.second < 1024 * 1024) {
            std::cout << pair.first.filename() << "        " << pair.second / 1024.0  << "KB\n";
        } else if (pair.second < 1024 * 1024 * 1024) {
            std::cout << pair.first.filename() << "        " << pair.second / 1024.0 / 1024  << "MB\n";
        } else {
            std::cout << pair.first.filename() << "        " << pair.second / 1024.0 / 1024 / 1024 << "GB\n";
        }
    }
}


int main() {
    try {
        iterate("/home/kirin");
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << e.what() << std::endl;
        std::terminate();
    } 
}

