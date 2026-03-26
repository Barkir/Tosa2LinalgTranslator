#include "translator.hpp"

mlir::ModuleOp MLIRDialectTranslator::translateTOSATOLINALG(mlir::ModuleOp& module) {
    auto loc = builder_.getUnknownLoc();

    MSG("Got module to translate ---> ");
    ON_DEBUG(llvm::errs() << module << "\n");
    MSG("-------------------------------------\n");

    // setting insertion point to build a translated function
    mlir::ModuleOp new_module = mlir::ModuleOp::create(builder_.getUnknownLoc());
    builder_.setInsertionPointToStart(new_module.getBody());

    MSG("Iterating over functions...\n");
    for (auto&& oper : module) {
        if (auto f = mlir::dyn_cast<mlir::func::FuncOp>(oper)) {
            MSG("cast: Operation -> Func. Successful\n");

            auto newFuncOp = createEmptyFunc(loc, f);
            auto& entry = f.front();

            for (auto&& op : entry) {
                llvm::errs() << "Got op -> " << op.getName() << "\n";
                tosaToLinalgOp(op);
            }
        }

    }

    TranslatorGreen("/ / / / / / / / RESULT / / / / / / / /\n");
    llvm::errs() << new_module << "\n";
    TranslatorGreen("---------------------------------------\n");
    return new_module;

}

// -------------------------------------------
//              mlir - specific
// -------------------------------------------

mlir::func::FuncOp MLIRDialectTranslator::createEmptyFunc(mlir::Location loc, mlir::func::FuncOp& old_func) {
    auto opTypes = old_func.getArgumentTypes();
    auto resTypes = old_func.getResultTypes();
    auto newFuncType = mlir::FunctionType::get(&ctx_, opTypes, resTypes);
    auto newFuncOp = builder_.create<mlir::func::FuncOp>(loc, old_func.getName(), newFuncType);

    auto* newEntryBlock = newFuncOp.addEntryBlock();
    builder_.setInsertionPointToStart(newEntryBlock);

    auto numOp = old_func.getNumArguments();                   llvm::errs() << "old func got " << numOp << " arguments" << "\n";
    auto newNumOp = newEntryBlock->getNumArguments();   llvm::errs() << "new func got " << newNumOp << " arguments" << "\n";
    for (unsigned i = 0; i < numOp; ++i) {
        valueMapper.map(old_func.getArgument(i), newEntryBlock->getArgument(i));
    }


    llvm::errs() << "created new func op: " << newFuncOp << "\n";

    return newFuncOp;
}

// --------------------------------------------------
//              Translators
// --------------------------------------------------

void MLIRDialectTranslator::tosaToLinalgOp(mlir::Operation& op) {
    auto name = op.getName().getStringRef().str();
    auto found = tosaToLinalgMap.find(name);
    if (found == tosaToLinalgMap.end()) {
        throw TranslatorErr("haven't found any translation for this op " + name);
        return;
    }
    LOG("found tosaToLinalg func conversion in map for op {} - {}!\n", name, found->first);
    auto funcPtr = found->second;

    (this->*funcPtr)(op);
    return;
}

void MLIRDialectTranslator::createTosaToLinalgAdd(mlir::Operation& op) {
    MSG("TOSA-TO-LINALG: ADD\n");
    auto loc = builder_.getUnknownLoc();
    auto res = op.getResult(0);
    auto resType = mlir::dyn_cast<mlir::RankedTensorType>(res.getType());
    if (!resType) {
        TranslatorWarn("resType in add is not RankedTensor :(");
        return;
    }


    mlir::Value lhs = valueMapper.lookupOrDefault(op.getOperand(0));
    mlir::Value rhs = valueMapper.lookupOrDefault(op.getOperand(1));

    mlir::Operation* newOp = nullptr;

    auto eltype = resType.getElementType();
    if (mlir::isa<mlir::FloatType>(eltype)) {
        newOp = builder_.create<mlir::arith::AddFOp>(loc, lhs, rhs);
    } else if (mlir::isa<mlir::IntegerType>(eltype)) {
        newOp = builder_.create<mlir::arith::AddIOp>(loc, lhs, rhs);
    }

    for (unsigned i = 0; i < op.getNumResults(); ++i) {
        valueMapper.map(op.getResult(i), newOp->getResult(i));
    }
}

void MLIRDialectTranslator::createTosaToLinalgMatMul(mlir::Operation& op) {
    MSG("TOSA-TO-LINALG: MATMUL\n");
    auto loc = builder_.getUnknownLoc();

    mlir::Value lhs = valueMapper.lookupOrDefault(op.getOperand(0));
    mlir::Value rhs = valueMapper.lookupOrDefault(op.getOperand(1));

    mlir::Operation* newOp = nullptr;


}

void MLIRDialectTranslator::createTosaToLinalgReturn(mlir::Operation& op) {
    MSG("TOSA-TO-LINALG: RETURN\n");
    auto loc = builder_.getUnknownLoc();

    auto returnValues = op.getOperands();
    llvm::SmallVector<mlir::Value, 4> newReturnValues;
    for (auto val : returnValues) {
        llvm::errs()  << "val = " << val << "\n";
        auto newVal = valueMapper.lookupOrDefault(val);
        newReturnValues.push_back(newVal);
    }

    builder_.create<mlir::func::ReturnOp>(loc, newReturnValues);
}

