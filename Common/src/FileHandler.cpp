#include "../FileHandler.hpp"

std::vector<std::string> FileHandler::getFileList(std::string directory)
{
    std::vector<std::string> fileList;

    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file())
        {
            std::string filePath = entry.path().string();
            struct stat fileStats;
            stat(filePath.c_str(), &fileStats);
            std::string info = packInfo(filePath, fileStats.st_size, fileStats.st_mtime);
            fileList.push_back(info);
        }
    }

    return fileList;
}

std::vector<std::string> FileHandler::getDetailedFileList(std::string directory)
{
    std::vector<std::string> fileList;

    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file())
        {
            std::string filePath = entry.path().string();
            struct stat fileStats;
            stat(filePath.c_str(), &fileStats);
            std::string info = packDetailedInfo(filePath, fileStats.st_size, fileStats.st_mtime, fileStats.st_atime, fileStats.st_ctime);
            fileList.push_back(info);
        }
    }

    return fileList;
}

std::string FileHandler::packDetailedInfo(std::string fileName, uintmax_t fileSize, std::time_t modificationTime, std::time_t access_time, std::time_t creation_time)
{
    return "File: " + fileName + "\n" + "|" + "File size: " + formatSize(fileSize) + "\n" + "|" + "Last modified: " + formatTime(modificationTime) + "\n" + "|" + "Last accessed: " + formatTime(access_time) + "\n" + "|" + "Creation Time: " + formatTime(creation_time) + "\n";
}

std::string FileHandler::packInfo(std::string fileName, uintmax_t fileSize, std::time_t time)
{
    std::string fileInfo = fileName + "|" + formatSize(fileSize) + "|" + formatTime(time);
    return fileInfo;
}

std::string FileHandler::formatSize(uintmax_t fileSize)
{
    constexpr uintmax_t KB = 1024;
    constexpr uintmax_t MB = KB * KB;
    constexpr uintmax_t GB = MB * KB;

    std::ostringstream oss;

    if (fileSize >= GB)
    {
        oss << std::fixed << std::setprecision(2) << static_cast<double>(fileSize) / GB << " GB";
    }
    else if (fileSize >= MB)
    {
        oss << std::fixed << std::setprecision(2) << static_cast<double>(fileSize) / MB << " MB";
    }
    else if (fileSize >= KB)
    {
        oss << std::fixed << std::setprecision(2) << static_cast<double>(fileSize) / KB << " KB";
    }
    else
    {
        oss << fileSize << " bytes";
    }

    return oss.str();
}

std::string FileHandler::formatTime(std::time_t timestamp)
{
    std::tm *tm = std::localtime(&timestamp);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::vector<std::string> FileHandler::parseFileInfo(std::string fileInfoString)
{

    std::vector<std::string> fileInfo;

    std::istringstream iss(fileInfoString);
    std::string token;
    while (std::getline(iss, token, '|'))
    {
        fileInfo.push_back(token);
    }

    return fileInfo;
}

std::vector<std::string> FileHandler::getFileInfo(std::string filepath)
{
    struct stat fileInfo;
    stat(filepath.c_str(), &fileInfo);
    std::string info = packInfo(filepath, fileInfo.st_size, fileInfo.st_mtime);
    return parseFileInfo(info);
}

std::string FileHandler::extractFilename(std::string filepath)
{
    size_t lastSlashIndex = filepath.find_last_of('/');
    if (lastSlashIndex != std::string::npos)
    {
        return filepath.substr(lastSlashIndex + 1);
    }
    return filepath;
}

bool FileHandler::deleteFile(std::string filepath)
{
    return std::remove(filepath.c_str()) != 0;
}

std::string FileHandler::monitorDirectory(std::string syncDir)
{
    int inotifyFd = inotify_init();
    if (inotifyFd == -1)
    {
        std::cerr << "[-] Failed to initialize inotify" << std::endl;
        return nullptr;
    }
    int watchDescriptor = inotify_add_watch(inotifyFd, syncDir.c_str(), IN_CREATE | IN_MODIFY | IN_DELETE);
    if (watchDescriptor == -1)
    {
        std::cerr << "[-] Failed to add watch to " << syncDir << std::endl;
        return nullptr;
    }

    char buffer[4096];
    while (true)
    {
        int bytesRead = read(inotifyFd, buffer, 4096);
        if (bytesRead == -1)
        {
            std::cerr << "[-] Failed to read inotify events" << std::endl;
            return nullptr;
        }
        struct inotify_event *event = nullptr;
        for (char *ptr = buffer; ptr < buffer + bytesRead; ptr += sizeof(struct inotify_event) + event->len)
        {
            event = reinterpret_cast<struct inotify_event *>(ptr);

            if (event->mask & IN_CREATE)
            {
                sleep(1);
                std::string fileName = event->name;
                return "insert:" + fileName;
            }

            if (event->mask & IN_MODIFY)
            {
                sleep(1);
                std::string fileName = event->name;
                return "update:" + fileName;
            }

            if (event->mask & IN_DELETE)
            {
                sleep(1);
                std::string fileName = event->name;
                return "delete:" + fileName;
            }
        }
    }
}