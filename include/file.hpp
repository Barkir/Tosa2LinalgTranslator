#pragma once
#include "llvm/Support/ErrorOr.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include "mlir/IR/AsmState.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Parser/Parser.h"
#include "llvm/Support/SourceMgr.h"

#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/Dialect/Tosa/IR/TosaOps.h"

#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <system_error>
#include <utility>

class MLIRModuleSerializer {
public:
  // constructor
  MLIRModuleSerializer(mlir::MLIRContext &context);

  // load an MLIR module from a file
  mlir::LogicalResult load(llvm::StringRef fname, mlir::OwningOpRef<mlir::ModuleOp>& module);

private:
  mlir::MLIRContext &context_;
  llvm::SourceMgr sourceMgr;
};
