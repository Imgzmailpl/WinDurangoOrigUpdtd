#include "WinDurango.Common/WinDurango.h"

#undef CreateFile

namespace wd::common
{
    std::shared_ptr<WinDurango> WinDurango::GetInstance()
    {
        static std::shared_ptr<WinDurango> Instance = std::make_shared<WinDurango>();
        return Instance;
    }

    bool WinDurango::inited()
    {
        return _inited;
    }

    void WinDurango::Init(std::shared_ptr<interfaces::storage::Directory> root)
    {
        if (this->_inited)
        {
            return;
        }
        try
        {
            if (!root)
                return; // Safety check

            rootDir = root;
            rootDir->open();

            std::time_t timestamp = std::time(nullptr);
            std::tm datetm = *std::localtime(&timestamp);

            char buf[512];
            std::strftime(buf, sizeof(buf), "%d.%m.%Y", &datetm);
            std::string date(buf);

            WinDurangoRoot = rootDir->CreateFolder("WinDurango");
            if (!WinDurangoRoot)
                return; // Safety check
            WinDurangoRoot->open();

            std::shared_ptr<interfaces::storage::File> LogFile =
                WinDurangoRoot->CreateFile("windurango_log_" + date + ".log");

            std::shared_ptr<interfaces::storage::File> ConfigFile = WinDurangoRoot->CreateFile("windurango.json");

            if (ConfigFile)
            {
                config = Config(ConfigFile);
                config.parse();
            }

            if (LogFile)
            {
                log = Logging(LogFile);
                log.Initialize();
            }

            this->_inited = true;
        }
        catch (const std::exception &e)
        {
            std::cout << "[WinDurango::Common::WinDurango.exception] - Critical: " << e.what() << "\n";
        }
        catch (...)
        {
            std::cout << "[WinDurango::Common::WinDurango.(...))] - Critical: Unknown Error\n";
        }
    }
} // namespace wd::common