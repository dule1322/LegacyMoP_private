/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Errors.h"

#include <cstdio>
#include <cstdlib>
#include <thread>
#include <cstdarg>

namespace Trinity
{

void Assert(char const* file, int line, char const* function, char const* message)
{
    fprintf(stderr, "\n%s:%i in %s ASSERTION FAILED:\n  %s\n", file, line, function, message);
    *static_cast<volatile int*>(NULL) = 0;
    exit(1);
}

void Assert(char const* file, int line, char const* function, char const* message, char const* format, ...)
{
    va_list args;
    va_start(args, format);

    fprintf(stderr, "\n%s:%i in %s ASSERTION FAILED:\n  %s ", file, line, function, message);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    fflush(stderr);

    va_end(args);
    *static_cast<volatile int*>(NULL) = 0;
    exit(1);
}

void Fatal(char const* file, int line, char const* function, char const* message, ...)
{
    va_list args;
    va_start(args, message);

    fprintf(stderr, "\n%s:%i in %s FATAL ERROR:\n  ", file, line, function);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    fflush(stderr);

    std::this_thread::sleep_for(std::chrono::seconds(10));
    *static_cast<volatile int*>(NULL) = 0;
    exit(1);
}

void Error(char const* file, int line, char const* function, char const* message)
{
    fprintf(stderr, "\n%s:%i in %s ERROR:\n  %s\n", file, line, function, message);
    *static_cast<volatile int*>(NULL) = 0;
    exit(1);
}

void Warning(char const* file, int line, char const* function, char const* message)
{
    fprintf(stderr, "\n%s:%i in %s WARNING:\n  %s\n", file, line, function, message);
}

void Abort(char const* file, int line, char const* function)
{
    fprintf(stderr, "\n%s:%i in %s ABORTED.\n", file, line, function);
    *static_cast<volatile int*>(NULL) = 0;
    exit(1);
}

void AbortHandler(int /*sigval*/)
{
    // nothing useful to log here, no way to pass args
    *static_cast<volatile int*>(NULL) = 0;
    exit(1);
}

} // namespace Trinity