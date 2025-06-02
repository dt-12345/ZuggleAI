#include "Logger.h"

#include "util.h"
#include "lib.hpp"

namespace util
{
    void Logger::Log(const char* Fmt, ...)
    {
        va_list args;
        va_start(args, Fmt);

        char buffer[0x500] = {};

        if (nn::util::VSNPrintf(buffer, sizeof(buffer), Fmt, args) > 0) {
            svcOutputDebugString(buffer, strlen(buffer));
        }

        va_end(args);
    }
} // namespace util
