#include <gtest/gtest.h>
#include <filesystem>
#include "translator.hpp"
#include "err.hpp"
#include "file.hpp"

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

static std::string get_file_path(std::string path) {
    // llvm::errs() << "got path : " << path << "\n";
    auto res = std::filesystem::path(TEST_DATA_DIR) / "tests" / path;
    // llvm::errs() << "res is > :" << res << "\n";
    return res;
}

static std::string get_checker_path(std::string path) {
  auto toSys = std::filesystem::path(path);
  auto checker_res = toSys.stem();
  llvm::errs() << "res_stem = " << checker_res << "\n";
  auto new_checker = checker_res.string() + "_checker.ll";
  auto new_path = std::filesystem::path(TEST_DATA_DIR) / "tests/checkers" / new_checker;

  llvm::errs() << "got new_path " << new_path << "\n";

  return new_path;
}

static void VerifyIR(mlir::ModuleOp module, std::string checkFilePath) {
    std::string output;
    llvm::raw_string_ostream os(output);

    mlir::OpPrintingFlags flags;
    flags.useLocalScope();
    flags.printGenericOpForm(false);
    flags.enableDebugInfo(false);
    flags.shouldPrintDebugInfoPrettyForm();
    flags.elideLargeElementsAttrs();

    module.print(os, flags);

    llvm::errs() << "-----------------------\nGot module !!!!!!\n" << module << "\n------------------------\n";

    auto command = "echo '" + output + "' | FileCheck " + checkFilePath;
    EXPECT_EQ(std::system(command.c_str()), 0);
}

TEST(add0d, LoadWithoutErrors) {
    auto file = get_file_path("mlir/add_0d.ll");
    auto check = get_checker_path(file);


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

    VerifyIR(moduleGet, check);

}

TEST(add1d, LoadWithoutErrors) {
    auto file = get_file_path("mlir/add_1d_all_dynamic.ll");
    auto check = get_checker_path(file);


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

    VerifyIR(moduleGet, check);
}

TEST(add2d, LoadWithoutErrors) {
    auto file = get_file_path("mlir/add_2d_broadcast.ll");
    auto check = get_checker_path(file);


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

    VerifyIR(moduleGet, check);
}

TEST(matmul, LoadWithoutErrors) {
    auto file = get_file_path("mlir/matmul.ll");
    auto check = get_checker_path(file);


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

    VerifyIR(moduleGet, check);
}

TEST(clampf32, LoadWithoutErrors) {
    auto file = get_file_path("mlir/clamp_f32.ll");
    auto check = get_checker_path(file);


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

    VerifyIR(moduleGet, check);

}

TEST(clampi32, LoadWithoutErrors) {
    auto file = get_file_path("mlir/clamp_i32.ll");
    auto check = get_checker_path(file);


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

    VerifyIR(moduleGet, check);

}

// TEST(qmatmul, LoadWithoutErrors) {
//     auto file = get_file_path("mlir/matmul_quantized.ll");
//     auto check = get_checker_path(file);
//
//
//     mlir::MLIRContext context;
//     MLIRModuleSerializer ms(context);
//
//     mlir::OwningOpRef<mlir::ModuleOp> module;
//     auto result = ms.load(file, module);
//     if (mlir::failed(result)) {
//       llvm::errs() << BG_BRIGHT_RED << "Failed to load file" << file << TOSA2LINALG_RESET << "\n";
//       ASSERT_NE(0, 0);
//     }
//
//     MLIRDialectTranslator translator;
//     auto moduleGet = module.get();
//     // llvm::errs() << moduleGet << "\n";
//     auto translationResult = translator.translate(moduleGet, TranslatorTypes::TOSA_TO_LINALG);
//     ASSERT_NE(translationResult.succeeded(), 0);
//
//     VerifyIR(moduleGet, check);
// }
//
// TEST(simple, LoadWithoutErrors) {
//     auto file = get_file_path("mlir/simple.ll");
//     auto check = get_checker_path(file);
//
//
//     mlir::MLIRContext context;
//     MLIRModuleSerializer ms(context);
//
//     mlir::OwningOpRef<mlir::ModuleOp> module;
//     auto result = ms.load(file, module);
//     if (mlir::failed(result)) {
//       llvm::errs() << BG_BRIGHT_RED << "Failed to load file" << file << TOSA2LINALG_RESET << "\n";
//       ASSERT_NE(0, 0);
//     }
//
//     MLIRDialectTranslator translator;
//     auto moduleGet = module.get();
//     // llvm::errs() << moduleGet << "\n";
//     auto translationResult = translator.translate(moduleGet, TranslatorTypes::TOSA_TO_LINALG);
//     ASSERT_NE(translationResult.succeeded(), 0);
//
//     VerifyIR(moduleGet, check);
// }






