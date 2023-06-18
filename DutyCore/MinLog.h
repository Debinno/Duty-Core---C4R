#pragma once
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

class MinLog
{
private:
    std::wofstream OutputStream;
    bool _isOpen = false;

public:
    MinLog(const char* filename, const char* dir = "")
    {
        Open(filename, dir);
    }

    void Open(const char* filename, const char* dir = "")
    {
        if (_isOpen) return;
        std::filesystem::path const directory = std::filesystem::current_path() / dir;
        std::filesystem::create_directories(directory);
        MinLog::OutputStream = std::wofstream(directory / filename);
        _isOpen = true;
    }

    void Write(const char* value)
    {
        MinLog::OutputStream << value;
        std::cout << value;
    }

    void Write(std::string value)
    {
        auto c = value.c_str();
        const size_t cSize = strlen(c) + 1;
        wchar_t* wc = new wchar_t[cSize];
        size_t outSize;
        mbstowcs_s(&outSize, wc, cSize, c, cSize-1);
        MinLog::OutputStream << wc;
        std::cout << value;

        delete[] wc;
    }

    void Write(std::wstring value)
    {
        MinLog::OutputStream << value;
        std::wcout << value;
    }

    void WriteLine(const char *value)
    {
        Write(value);
        MinLog::OutputStream << std::endl;
        std::cout << std::endl;
    }

    void WriteLine(std::string value)
    {
        Write(value);
        MinLog::OutputStream << std::endl;
        std::cout << std::endl;
    }

    void WriteLine(std::wstring value)
    {
        MinLog::OutputStream << value << std::endl;
        std::wcout << value << std::endl;
    }

    MinLog& operator<<(auto value) {
        MinLog::OutputStream << value;
        std::cout << value;
        return *this;
    }

    void Flush() {
        MinLog::OutputStream.flush();
        std::cout.flush();
    }
};
