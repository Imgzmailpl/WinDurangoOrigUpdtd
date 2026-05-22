/*
 * Logging File
 * WinDurango.Common::Logging
 */
#pragma once

#include "WinDurango.Common/Interfaces/Storage/File.h"
#include "WinDurango.Common/exports.h"
#include <iostream>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>

namespace wd::common
{
    class WD_API Logging
    {
      public:
        Logging() : pFile(nullptr), isConstructed(false)
        {
        }
        Logging(std::shared_ptr<interfaces::storage::File> file) : pFile(file), isConstructed(true)
        {
        }

        void Initialize();
        void AddLogger(std::string codespace);

        template <typename... Args> void Log(std::string codespace, fmt::format_string<Args...> fmt, Args &&...args)
        {
            if (!isInitialized)
                return;
            try
            {
                if (log.find(codespace) == log.end() || !log[codespace])
                {
                    AddLogger(codespace);
                }

                // SAFEGUARD: Only dereference if valid. Fallback to main logger if AddLogger failed!
                if (log[codespace])
                {
                    log[codespace]->info(fmt, std::forward<Args>(args)...);
                }
                else if (log["WinDurango"])
                {
                    log["WinDurango"]->info(fmt, std::forward<Args>(args)...);
                }
            }
            catch (...)
            {
            }
        }

        template <typename... Args> void Warn(std::string codespace, fmt::format_string<Args...> fmt, Args &&...args)
        {
            if (!isInitialized)
                return;
            try
            {
                if (log.find(codespace) == log.end() || !log[codespace])
                {
                    AddLogger(codespace);
                }

                if (log[codespace])
                {
                    log[codespace]->warn(fmt, std::forward<Args>(args)...);
                }
                else if (log["WinDurango"])
                {
                    log["WinDurango"]->warn(fmt, std::forward<Args>(args)...);
                }
            }
            catch (...)
            {
            }
        }

        template <typename... Args> void Error(std::string codespace, fmt::format_string<Args...> fmt, Args &&...args)
        {
            if (!isInitialized)
                return;
            try
            {
                if (log.find(codespace) == log.end() || !log[codespace])
                {
                    AddLogger(codespace);
                }

                if (log[codespace])
                {
                    log[codespace]->error(fmt, std::forward<Args>(args)...);
                }
                else if (log["WinDurango"])
                {
                    log["WinDurango"]->error(fmt, std::forward<Args>(args)...);
                }
            }
            catch (...)
            {
            }
        }

      private:
        std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> log;
        std::shared_ptr<interfaces::storage::File> pFile;

      public:
        bool isConstructed = false;
        bool isInitialized = false;
    };
} // namespace wd::common