#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#include <iomanip>
#include <ctime>

class FileHandler{
    public:
    static std::vector<std::string> getFileList(std::string directory);
    static std::string formatSize(uintmax_t fileSize);
    static std::string formatTime(std::time_t timestamp);
    static std::string packInfo(std::string fileName, uintmax_t fileSize, std::time_t time);
    static std::vector<std::string> parseFileInfo(std::string fileInfo);
    static std::vector<std::string> getFileInfo(std::string filepath);
    static std::string extractFilename(std::string filepath);
};

#endif