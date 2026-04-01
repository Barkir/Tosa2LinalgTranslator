#include "file.hpp"
#include "err.hpp"
#include "log.hpp"

// Constructor
MLIRModuleSerializer::MLIRModuleSerializer(mlir::MLIRContext &context)
    : context_(context) {
  context_.loadDialect<mlir::arith::ArithDialect>();
  context_.loadDialect<mlir::tensor::TensorDialect>();
  context_.loadDialect<mlir::func::FuncDialect>();
  context_.loadDialect<mlir::linalg::LinalgDialect>();
  context_.loadDialect<mlir::tosa::TosaDialect>();
}

// Load an MLIR module from a file
mlir::LogicalResult MLIRModuleSerializer::load(llvm::StringRef fname, mlir::OwningOpRef<mlir::ModuleOp>& module) {
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> fileOrErr =
      llvm::MemoryBuffer::getFileOrSTDIN(fname);
  if (std::error_code ec = fileOrErr.getError()) {
    llvm::errs() << "Could not open input file: " << ec.message() << "\n";
    return mlir::failure();
  }

  sourceMgr.AddNewSourceBuffer(std::move(*fileOrErr), llvm::SMLoc());
  module = mlir::parseSourceFile<mlir::ModuleOp>(sourceMgr, &context_);
  if (!module) {
    MSG("Failed to parse module\n");
    return mlir::failure();
  }
  // llvm::errs() << "Got smth with name " << parsed.get()->getName() << "\n";

  TranslatorGreen("Successfuly loaded file to func!");
  return mlir::success();
}
