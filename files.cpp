#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif //_WIN32

#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include "files.h"

void ReadTerminal(std::FILE*& file){
    file = stdin;
    return;
}

void WriteTerminal(std::FILE*& file){
    file = stdout;
    return;
}

void ReadFile(std::FILE*& file, const std::string& filename){
    file = std::fopen(filename.c_str(), "r");
    if(file == nullptr){
        throw std::runtime_error(filename+": No such file or directory");
    }
    return;
}

void WriteFile(std::FILE*& file, const std::string& filename){
    std::size_t pos{filename.find_last_of("/\\")};
    if(pos != std::string::npos){
        std::filesystem::create_directories(filename.substr(0, pos));
    }
    file = std::fopen(filename.c_str(), "w");
    return;
}

void AppendToFile(std::FILE*& file, const std::string& filename){
    std::size_t pos{filename.find_last_of("/\\")};
    if(pos != std::string::npos){
        std::filesystem::create_directories(filename.substr(0, pos));
    }
    file = std::fopen(filename.c_str(), "a");
    return;
}

void ReadBinaryFile(std::FILE*& file, const std::string& filename){
    file = std::fopen(filename.c_str(), "rb");
    if(file == nullptr){
        throw std::runtime_error(filename+": No such file or directory");
    }
    return;
}

void WriteBinaryFile(std::FILE*& file, const std::string& filename){
    std::size_t pos{filename.find_last_of("/\\")};
    if(pos != std::string::npos){
        std::filesystem::create_directories(filename.substr(0, pos));
    }
    file = std::fopen(filename.c_str(), "wb");
    return;
}


void AppendToBinaryFile(std::FILE*& file, const std::string& filename){
    std::size_t pos{filename.find_last_of("/\\")};
    if(pos != std::string::npos){
        std::filesystem::create_directories(filename.substr(0, pos));
    }
    file = std::fopen(filename.c_str(), "ab");
    return;
}

void CloseReadFile(std::FILE* file){
    if(file != nullptr && file != stdin){
        std::fclose(file);
    }
    return;
}

void CloseWriteFile(std::FILE* file){
    if(file != nullptr && file != stdout){
        std::fflush(file);
        std::fclose(file);
    }
    return;
}

void ReadFile(std::ifstream& stream, const std::string& filename){
    stream.open(filename);
    if(!stream){
        throw std::runtime_error(filename+": No such file or directory");
    }
    return;
}

void WriteFile(std::ofstream& stream, const std::string& filename){
    std::size_t pos{filename.find_last_of("/\\")};
    if(pos != std::string::npos){
        std::filesystem::create_directories(filename.substr(0, pos));
    }
    stream.open(filename);
    return;
}

void AppendToFile(std::ofstream& stream, const std::string& filename){
    std::size_t pos{filename.find_last_of("/\\")};
    if(pos != std::string::npos){
        std::filesystem::create_directories(filename.substr(0, pos));
    }
    stream.open(filename, std::ios::app);
    return;
}

void ReadBinaryFile(std::ifstream& stream, const std::string& filename){
    stream.open(filename, std::ios::binary);
    if(!stream){
        throw ENOENT;
    }
    return;
}

void WriteBinaryFile(std::ofstream& stream, const std::string& filename){
    std::size_t pos{filename.find_last_of("/\\")};
    if(pos != std::string::npos){
        std::filesystem::create_directories(filename.substr(0, pos));
    }
    stream.open(filename, std::ios::binary);
    return;
}

void CloseReadFile(std::ifstream& stream){
    stream.close();
    return;
}

void CloseWriteFile(std::ofstream& stream){
    stream.flush();
    stream.close();
    return;
}

std::string GetFilesInDirectory(const std::string& path, const std::string& prep){
    std::string output{};
    for(const std::filesystem::directory_entry& file :
        std::filesystem::directory_iterator(path)){
        if(std::filesystem::is_directory(file)){
            output.append(prep+GetFilesInDirectory(file.path().string(),
                                                   file.path().string()
                                                   .substr(path.length(),
                                                   file.path().string()
                                                   .length()-path.length())));
        } else {
            output.append(prep+file.path().string()
                          .substr(path.length(),
                                  file.path().string()
                                  .length()-path.length()));
            output.append("\n");
        }
    }
    return output;
}
