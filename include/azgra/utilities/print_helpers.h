#pragma once

#include <cstdio>
#include <cstdarg>

// Terminal colors
#define ANSI_RESET       "\x1B[0m"
#define ANSI_RED         "\x1B[0;31m"
#define ANSI_GREEN       "\x1B[0;32m"
#define ANSI_YELLOW      "\x1B[0;33m"
#define ANSI_BLUE        "\x1B[0;34m"
#define ANSI_MAGENTA     "\x1B[0;35m"
#define ANSI_CYAN        "\x1B[0;36m"


namespace azgra
{
    enum ConsoleColor
    {
        ConsoleColor_Red,
        ConsoleColor_Green,
        ConsoleColor_Yellow,
        ConsoleColor_Blue,
        ConsoleColor_Magenta,
        ConsoleColor_Cyan,
    };

    static void send_color_to_file_ptr(FILE *__restrict__ file, ConsoleColor color)
    {
        switch (color)
        {
            case ConsoleColor_Red:
                fprintf(file, ANSI_RED);
                return;
            case ConsoleColor_Green:
                fprintf(file, ANSI_GREEN);
                return;
            case ConsoleColor_Yellow:
                fprintf(file, ANSI_YELLOW);
                return;
            case ConsoleColor_Blue:
                fprintf(file, ANSI_BLUE);
                return;
            case ConsoleColor_Magenta:
                fprintf(file, ANSI_MAGENTA);
                return;
            case ConsoleColor_Cyan:
                fprintf(file, ANSI_CYAN);
                return;
        }
    }

    static void reset_color(FILE *__restrict__ file)
    {
        fprintf(file, ANSI_RESET);
    }

    inline void print_colorized(ConsoleColor color, const char *format, ...)
    {
        va_list argptr;
        send_color_to_file_ptr(stdout, color);
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
        reset_color(stdout);
    }


    inline void print_if(const bool precondition, const char *format, ...)
    {
        if (precondition)
        {
            va_list argptr;
            va_start(argptr, format);
            vfprintf(stdout, format, argptr);
            va_end(argptr);
        }
    }

    inline void debug_printf(const char *format, ...)
    {
#if DEBUG
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
#endif
    }

    inline void print_error_if(const bool precondition, const char *format, ...)
    {
        if (precondition)
        {
            va_list argptr;
            va_start(argptr, format);
            vfprintf(stderr, format, argptr);
            va_end(argptr);
        }
    }
}
