#include <gtest/gtest.h>
#include <filesystem>
#include "translator.hpp"
#include "err.hpp"
#include "file.hpp"

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

static std::string get_model_path(std::string path) {
    llvm::errs() << "got path : " << path << "\n";
    auto res = std::filesystem::path(TEST_DATA_DIR) / "tests" / path;
    llvm::errs() << "res is > :" << res << "\n";
    return res;
}

TEST(add0d, LoadWithoutErrors) {
    auto file = get_model_path("mlir/add_0d.ll");
    llvm::errs() << "resulting path:  " << file << "\n";

    mlir::MLIRContext context;
    MLIRModuleSerializer ms(context);

    mlir::OwningOpRef<mlir::ModuleOp> module;
    auto result = ms.load(file, module);
    if (mlir::failed(result)) {
      llvm::errs() << BG_BRIGHT_RED << "Failed to load file" << file << TOSA2LINALG_RESET << "\n";
      ASSERT_NE(0, 0);
    }

    MLIRDialectTranslator translator;
    auto moduleGet = module.get();
    // llvm::errs() << moduleGet << "\n";
    auto translationResult = translator.translate(moduleGet, TranslatorTypes::TOSA_TO_LINALG);
    ASSERT_NE(translationResult.succeeded(), 0);

}

TEST(add1d, LoadWithoutErrors) {
    auto file = get_model_path("mlir/add_1d_all_dynamic.ll");

    mlir::MLIRContext context;
    MLIRModuleSerializer ms(context);

    mlir::OwningOpRef<mlir::ModuleOp> module;
    auto result = ms.load(file, module);
    if (mlir::failed(result)) {
      llvm::errs() << BG_BRIGHT_RED << "Failed to load file" << file << TOSA2LINALG_RESET << "\n";
      ASSERT_NE(0, 0);
    }

    MLIRDialectTranslator translator;
    auto moduleGet = module.get();
    // llvm::errs() << moduleGet << "\n";
    auto translationResult = translator.translate(moduleGet, TranslatorTypes::TOSA_TO_LINALG);
    ASSERT_NE(translationResult.succeeded(), 0);
}

TEST(add2d, LoadWithoutErrors) {
    auto file = get_model_path("mlir/add_2d_broadcast.ll");

    mlir::MLIRContext context;
    MLIRModuleSerializer ms(context);

    mlir::OwningOpRef<mlir::ModuleOp> module;
    auto result = ms.load(file, module);
    if (mlir::failed(result)) {
      llvm::errs() << BG_BRIGHT_RED << "Failed to load file" << file << TOSA2LINALG_RESET << "\n";
      ASSERT_NE(0, 0);
    }

    MLIRDialectTranslator translator;
    auto moduleGet = module.get();
    // llvm::errs() << moduleGet << "\n";
    auto translationResult = translator.translate(moduleGet, TranslatorTypes::TOSA_TO_LINALG);
    ASSERT_NE(translationResult.succeeded(), 0);
}

TEST(matmul, LoadWithoutErrors) {
    auto file = get_model_path("mlir/matmul.ll");

    mlir::MLIRContext context;
    MLIRModuleSerializer ms(context);

    mlir::OwningOpRef<mlir::ModuleOp> module;
    auto result = ms.load(file, module);
    if (mlir::failed(result)) {
      llvm::errs() << BG_BRIGHT_RED << "Failed to load file" << file << TOSA2LINALG_RESET << "\n";
      ASSERT_NE(0, 0);
    }

    MLIRDialectTranslator translator;
    auto moduleGet = module.get();
    // llvm::errs() << moduleGet << "\n";
    auto translationResult = translator.translate(moduleGet, TranslatorTypes::TOSA_TO_LINALG);
    ASSERT_NE(translationResult.succeeded(), 0);
}

TEST(qmatmul, LoadWithoutErrors) {
    auto file = get_model_path("mlir/matmul_quantized.ll");

    mlir::MLIRContext context;
    MLIRModuleSerializer ms(context);

    mlir::OwningOpRef<mlir::ModuleOp> module;
    auto result = ms.load(file, module);
    if (mlir::failed(result)) {
      llvm::errs() << BG_BRIGHT_RED << "Failed to load file" << file << TOSA2LINALG_RESET << "\n";
      ASSERT_NE(0, 0);
    }

    MLIRDialectTranslator translator;
    auto moduleGet = module.get();
    // llvm::errs() << moduleGet << "\n";
    auto translationResult = translator.translate(moduleGet, TranslatorTypes::TOSA_TO_LINALG);
    ASSERT_NE(translationResult.succeeded(), 0);
}

TEST(simple, LoadWithoutErrors) {
    auto file = get_model_path("mlir/simple.ll");

    mlir::MLIRContext context;
    MLIRModuleSerializer ms(context);

    mlir::OwningOpRef<mlir::ModuleOp> module;
    auto result = ms.load(file, module);
    if (mlir::failed(result)) {
      llvm::errs() << BG_BRIGHT_RED << "Failed to load file" << file << TOSA2LINALG_RESET << "\n";
      ASSERT_NE(0, 0);
    }

    MLIRDialectTranslator translator;
    auto moduleGet = module.get();
    // llvm::errs() << moduleGet << "\n";
    auto translationResult = translator.translate(moduleGet, TranslatorTypes::TOSA_TO_LINALG);
    ASSERT_NE(translationResult.succeeded(), 0);
}






