#pragma once

#include <iostream>
#include <format>

#define ENABLE_LOGGING

#define HAS_FORMAT 1


#ifdef ENABLE_LOGGING

#define ON_DEBUG(msg) msg
#include "llvm/Support/raw_ostream.h"

#define MSG(msg)                                                               \
    do                                                                         \
    {                                                                          \
        std::clog << __FUNCTION__ << ": " << msg;                              \
    }                                                                          \
    while (false)

#if HAS_FORMAT
#define LOG(msg, ...)                                                          \
    do                                                                         \
    {                                                                          \
        std::clog << __FUNCTION__ << ": ";                                     \
        std::clog << std::format(msg, __VA_ARGS__);                            \
    }                                                                          \
    while (false)
#else

#define LOG(msg, ...)                                                          \
    do                                                                         \
    {                                                                          \
        std::clog << __FUNCTION__ << ": " << msg;                              \
        std::clog << msg;                                                      \
    }                                                                          \
    while (false)
#endif

#define LLVM_MSG(msg)                                                          \
    do                                                                         \
    {                                                                          \
        llvm::errs() << msg;                                                   \
    }                                                                          \
    while (false)

#define LLVM_MSGLN(msg)                                                        \
    do                                                                         \
    {                                                                          \
        llvm::errs() << msg << "\n";                                           \
    }                                                                          \
    while (false)

#define LLVM_PRINT(value)                                                         \
    do                                                                         \
    {                                                                          \
        (value)->print(llvm::errs());                                          \
    }                                                                          \
    while (false)

#else

#define ON_DEBUG(msg)

#define MSG(msg)                                                               \
    do                                                                         \
    {                                                                          \
    }                                                                          \
    while (false)

#define LOG(msg, ...)                                                          \
    do                                                                         \
    {                                                                          \
    }                                                                          \
    while (false)

#define LLVM_MSG(msg)                                                          \
    do                                                                         \
    {                                                                          \
    }                                                                          \
    while (false)

#define LLVM_MSGLN(msg)                                                        \
    do                                                                         \
    {                                                                          \
    }                                                                          \
    while (false)

#define LLVM_PRINT(value)                                                      \
    do                                                                         \
    {                                                                          \
    }                                                                          \
    while (false)

#endif
