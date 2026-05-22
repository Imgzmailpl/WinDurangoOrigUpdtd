#include "Config.h"

bool wd::common::Config::parse()
{
    try
    {
        if (!pFile)
            return false;

        pFile->open();
        std::string jsonData = pFile->read();

        // Fix: Prevent JSON parse_error on empty files
        if (jsonData.empty())
        {
            jsonData = "{}";
        }

        data = nlohmann::json::parse(jsonData);
        return true;
    }
    catch (...)
    {
        // Fix: Ensure data is ALWAYS a valid object, even if parsing completely fails
        data = nlohmann::json::object();
        return false;
    }
}

template <typename T> T &wd::common::Config::operator[](std::string node)
{
    return data[node];
}

template <typename T> bool wd::common::Config::set(std::string node, T type)
{
    data[node] = type;
    return true;
}