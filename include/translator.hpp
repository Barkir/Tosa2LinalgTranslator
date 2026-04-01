#pragma once

#include "llvm/Support/ErrorOr.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include "mlir/IR/AsmState.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/IRMapping.h"
#include "mlir/Parser/Parser.h"
#include "mlir/Dialect/Tosa/IR/TosaOps.h"

#include "llvm/Support/SourceMgr.h"
#include "mlir/Transforms/DialectConversion.h"

#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/Dialect/Tosa/IR/TosaOps.h"

#include "err.hpp"
#include "typeConverter.hpp"

#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <system_error>
#include <utility>
#include <functional>
#include <unordered_map>

enum class TranslatorTypes { TOSA_TO_LINALG };

struct MLIRDialectTranslator {
private:

    mlir::OpBuilder builder_;
    mlir::MLIRContext ctx_;
    mlir::IRMapping valueMapper;
    TranslatorErrorCtx errs;

public:
  MLIRDialectTranslator() : builder_(&ctx_) {
    ctx_.loadDialect<mlir::arith::ArithDialect>();
    ctx_.loadDialect<mlir::tensor::TensorDialect>();
    ctx_.loadDialect<mlir::func::FuncDialect>();
    ctx_.loadDialect<mlir::linalg::LinalgDialect>();
    ctx_.loadDialect<mlir::tosa::TosaDialect>();
  }

public:
    mlir::LogicalResult translateTOSATOLINALG(mlir::ModuleOp& module);

public:
    mlir::LogicalResult translate(mlir::ModuleOp& module, TranslatorTypes type) {
        switch (type) {
            case TranslatorTypes::TOSA_TO_LINALG:
            {
                MSG("TOSA-TO-LINALG TRANSLATION\n");
                return translateTOSATOLINALG(module);
            }

            default:
            {
                errs.setErrorCode(TranslatorErrorCode::UNKNOWN_TRANSLATOR_TYPE);
                return mlir::failure();
            }

        }
    }

public: // static methods for translation


public: // mlir-specific
    mlir::func::FuncOp createEmptyFunc(mlir::Location loc, mlir::func::FuncOp& old_func);
    void setDialects(mlir::ConversionTarget& target);
    void setPatterns(mlir::RewritePatternSet& patterns, mlir::TypeConverter& converter);
    mlir::TypeConverter createTosaToLinalgTypeConverter();
    mlir::Value createZeroTensor(mlir::RankedTensorType type, llvm::SmallVector<mlir::Value> dims);

    // static ones // --- ---
    static llvm::SmallVector<mlir::Value> condenseValues(llvm::SmallVector<mlir::Value>& values);
    static mlir::Value createZeroTensor(mlir::PatternRewriter& rewriter, mlir::RankedTensorType type, llvm::SmallVector<mlir::Value> dims);
    // -----------------------------------------------------------------------------------------------------


public:
    TranslatorErrorCode getErrorCode() { return errs.getErrorCode(); }
    void printErrorCode(void) { return errs.printErrorCode(); }

public: // tosa-to-linalg translation

    void tosaToLinalgOp(mlir::Operation& op);
    void createTosaToLinalgAdd(mlir::Operation& op);
    void createTosaToLinalgMul(mlir::Operation& op);
    void createTosaToLinalgClamp(mlir::Operation& op);
    void createTosaToLinalgSub(mlir::Operation& op);
    void createTosaToLinalgMatMul(mlir::Operation& op);
    void createTosaToLinalgReturn(mlir::Operation& op);

// -------------------------------------------------------------------------
    using translatorFuncPointer = void (MLIRDialectTranslator::*)(mlir::Operation&);
// -------------------------------------------------------------------------

private:
    const inline static std::unordered_map<std::string, translatorFuncPointer> tosaToLinalgMap = {
        {"tosa.add",    &MLIRDialectTranslator::createTosaToLinalgAdd},
        {"func.return", &MLIRDialectTranslator::createTosaToLinalgReturn},
    };

private:
    // TosaToLinalgTypeConverter typeConverter;



};




