
#pragma once

#include "colors.hpp"
#include "log.hpp"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Twine.h"
#include <iostream>
#include <stdexcept>
#include <string>

enum class TranslatorErrorCode {

    SUCCESS,
    UNKNOWN_TRANSLATOR_TYPE,
    UNKNOWN_ERROR_CODE
};

class TranslatorErrorCtx {
private:
    TranslatorErrorCode code;
public:
    TranslatorErrorCtx() = default;

public:
    TranslatorErrorCode getErrorCode() { return code; }
    void setErrorCode(const TranslatorErrorCode code_) { code = code_; }
    void printErrorCode(void);
};

struct TranslatorErr : public std::runtime_error {
    explicit TranslatorErr(const std::string& msg) : std::runtime_error(BG_BRIGHT_RED + msg + TOSA2LINALG_RESET) {}
    explicit TranslatorErr(const llvm::Twine& msg) : std::runtime_error(BG_BRIGHT_RED + msg.str() + TOSA2LINALG_RESET) {}
};

struct TranslatorWarn {
    explicit TranslatorWarn(const std::string& msg) {
        ON_DEBUG(std::cout << FG_GOLD << msg << TOSA2LINALG_RESET << "\n");
    }
};

struct TranslatorGreen {
    explicit TranslatorGreen(const std::string& msg) {
        ON_DEBUG(std::cout << BOLD_GREEN << msg << TOSA2LINALG_RESET << "\n");
    }
};
