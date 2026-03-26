#include "err.hpp"

void TranslatorErrorCtx::printErrorCode() {
    switch(code) {
        case TranslatorErrorCode::UNKNOWN_ERROR_CODE:
            std::cerr << "UNKNOWN_ERROR_CODE" << "\n";
            break;

        case TranslatorErrorCode::UNKNOWN_TRANSLATOR_TYPE:
            std::cerr << "UNKNOWN_TRANSLATOR_TYPE" << "\n";
            break;

        case TranslatorErrorCode::SUCCESS:
            std::cerr << "SUCCESS" << "\n";
            break;

        default:
            std::cerr << "UNKNOWN_ERROR_CODE" << "\n";
            break;
    }
}
