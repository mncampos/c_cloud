#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#include <iomanip>
#include <ctime>
#include <sys/inotify.h>
#include <unistd.h>
#include <fcntl.h>

class FileHandler{
    public:
    static std::vector<std::string> getFileList(std::string directory);
    static std::vector<std::string> getDetailedFileList(std::string directory);
    static std::string formatSize(uintmax_t fileSize);
    static std::string formatTime(std::time_t timestamp);
    static std::string packInfo(std::string fileName, uintmax_t fileSize, std::time_t time);
    static std::string packDetailedInfo(std::string fileName, uintmax_t fileSize, std::time_t modificationTime, std::time_t access_time, std::time_t creation_time);
    static std::vector<std::string> parseFileInfo(std::string fileInfo);
    static std::vector<std::string> getFileInfo(std::string filepath);
    static std::string extractFilename(std::string filepath);
    static bool deleteFile(std::string filepath);
    static std::string monitorDirectory(std::string syncDir);
};

#endif