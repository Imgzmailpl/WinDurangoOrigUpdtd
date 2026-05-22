#include "Logging.h"

void wd::common::Logging::Initialize()
{
    if (!isConstructed)
    {
        std::cout << "[WinDurango::Common::Logging.Initialize] - Critical: Logging isn't Constructed\n";
        return;
    }

    std::string logFilePath = "WinDurango.log";
    if (pFile)
    {
        try
        {
            pFile->open();
            logFilePath = pFile->fullfilepath().string();
        }
        catch (const std::exception &e)
        {
            std::cout << "[WinDurango::Common::Logging.Initialize] - Warning: pFile->open() failed: " << e.what()
                      << "\n";
        }
    }
    else
    {
        std::cout
            << "[WinDurango::Common::Logging.Initialize] - Warning: pFile is nullptr. Defaulting log destination to "
            << logFilePath << "\n";
    }

    try
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);

        console_sink->set_pattern("[%n] [%H:%M:%S] [thread %t] - %^%l%$: %v");
        file_sink->set_pattern("[%n] [%H:%M:%S] [thread %t] - %^%l%$: %v");

        std::shared_ptr<spdlog::logger> logg =
            std::make_shared<spdlog::logger>("WinDurango", spdlog::sinks_init_list{console_sink, file_sink});

        log["WinDurango"] = logg;
        log["WinDurango"]->set_pattern("[%n] [%H:%M:%S] [thread %t] - %^%l%$: %v");
        log["WinDurango"]->info("Setting Log File: {}", logFilePath);
        log["WinDurango"]->flush_on(spdlog::level::info);
        isInitialized = true;
    }
    catch (const spdlog::spdlog_ex &e)
    {
        std::cout << "[WinDurango::Common::Logging.spdlog] - Critical: " << e.what() << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "[WinDurango::Common::Logging.exception] - Critical: " << e.what() << "\n";
    }
    catch (...)
    {
        std::cout << "[WinDurango::Common::Logging.(...))] - Critical: Unknown Error\n";
    }
}

void wd::common::Logging::AddLogger(std::string codespace)
{
    if (!isInitialized)
    {
        std::cout << "[WinDurango::Common::Logging.AddLogger] - Critical: Logging isn't Initialized\n";
        return;
    }

    std::string logFilePath = "WinDurango.log";
    if (pFile)
    {
        logFilePath = pFile->fullfilepath().string();
    }

    try
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);

        console_sink->set_pattern("[%n] [%H:%M:%S] [thread %t] - %^%l%$: %v");
        file_sink->set_pattern("[%n] [%H:%M:%S] [thread %t] - %^%l%$: %v");

        std::shared_ptr<spdlog::logger> logg =
            std::make_shared<spdlog::logger>(codespace, spdlog::sinks_init_list{console_sink, file_sink});

        log[codespace] = logg;
        log[codespace]->set_pattern("[%n] [%H:%M:%S] [thread %t] - %^%l%$: %v");
        log[codespace]->flush_on(spdlog::level::info);
    }
    catch (const spdlog::spdlog_ex &e)
    {
        std::cout << "[WinDurango::Common::Logging.spdlog] - Critical: " << e.what() << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "[WinDurango::Common::Logging.exception] - Critical: " << e.what() << "\n";
    }
    catch (...)
    {
        std::cout << "[WinDurango::Common::Logging.(...))] - Critical: Unknown Error\n";
    }
}