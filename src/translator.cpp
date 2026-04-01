#include "translator.hpp"

// --------------------------------------------------
//              Translator classes
// --------------------------------------------------

struct NegateOpConversion : public mlir::OpConversionPattern<mlir::tosa::NegateOp> {
    using OpConversionPattern<mlir::tosa::NegateOp>::OpConversionPattern;
    mlir::LogicalResult matchAndRewrite(mlir::tosa::NegateOp op, OpAdaptor adaptor,
                                        mlir::ConversionPatternRewriter& rewriter) const override {
        auto loc = rewriter.getUnknownLoc();
        mlir::Value res = adaptor.getInput1();

        mlir::Operation* newOp = nullptr;

        auto resType = mlir::dyn_cast<mlir::RankedTensorType>(op.getType());
        if (!resType) {
            return rewriter.notifyMatchFailure(op, "not a ranked tensor");
        }

        auto eltype = resType.getElementType();
        if (mlir::isa<mlir::FloatType>(eltype)) {
            newOp = rewriter.create<mlir::arith::NegFOp>(loc, resType, res);
        } else if (mlir::isa<mlir::IntegerType>(eltype)) {

        }

        return mlir::success();
    }
};

struct MulOpConversion : public mlir::OpConversionPattern<mlir::tosa::MulOp> {
    using OpConversionPattern<mlir::tosa::MulOp>::OpConversionPattern;
    mlir::LogicalResult matchAndRewrite(mlir::tosa::MulOp op, OpAdaptor adaptor,
                                        mlir::ConversionPatternRewriter& rewriter) const override {
        MSG("TOSA-TO-LINALG: MUL\n");
        auto loc = rewriter.getUnknownLoc();

        mlir::Value lhs = adaptor.getInput1();
        mlir::Value rhs = adaptor.getInput2();

        mlir::Operation* newOp = nullptr;

        auto resType = mlir::dyn_cast<mlir::RankedTensorType>(op.getType());
        if (!resType) {
            return rewriter.notifyMatchFailure(op, "not a ranked tensor");
        }

        auto eltype = resType.getElementType();
        if (mlir::isa<mlir::FloatType>(eltype)) {
            newOp = rewriter.create<mlir::arith::MulFOp>(loc, lhs, rhs);
        } else if (mlir::isa<mlir::IntegerType>(eltype)) {
            newOp = rewriter.create<mlir::arith::MulIOp>(loc, lhs, rhs);
        }

        if (!newOp) return mlir::failure();

        rewriter.replaceOp(op, newOp->getResults());
        return mlir::success();
    }
};

struct SubOpConversion : public mlir::OpConversionPattern<mlir::tosa::SubOp> {
    using OpConversionPattern<mlir::tosa::SubOp>::OpConversionPattern;
    mlir::LogicalResult matchAndRewrite(mlir::tosa::SubOp op, OpAdaptor adaptor,
                                        mlir::ConversionPatternRewriter& rewriter) const override {
        MSG("TOSA-TO-LINALG: SUB\n");
        auto loc = rewriter.getUnknownLoc();

        mlir::Value lhs = adaptor.getInput1();
        mlir::Value rhs = adaptor.getInput2();

        mlir::Operation* newOp = nullptr;

        auto resType = mlir::dyn_cast<mlir::RankedTensorType>(op.getType());
        if (!resType) {
            return rewriter.notifyMatchFailure(op, "not a ranked tensor");
        }

        auto eltype = resType.getElementType();
        if (mlir::isa<mlir::FloatType>(eltype)) {
            newOp = rewriter.create<mlir::arith::SubFOp>(loc, lhs, rhs);
        } else if (mlir::isa<mlir::IntegerType>(eltype)) {
            newOp = rewriter.create<mlir::arith::SubIOp>(loc, lhs, rhs);
        }

        if (!newOp) return mlir::failure();

        rewriter.replaceOp(op, newOp->getResults());
        return mlir::success();
    }
};

struct AddOpConversion : public mlir::OpConversionPattern<mlir::tosa::AddOp> {
    using OpConversionPattern<mlir::tosa::AddOp>::OpConversionPattern;
    mlir::LogicalResult matchAndRewrite(mlir::tosa::AddOp op, OpAdaptor adaptor,
                                        mlir::ConversionPatternRewriter& rewriter) const override {
        MSG("TOSA-TO-LINALG: ADD\n");
        auto loc = rewriter.getUnknownLoc();

        mlir::Value lhs = adaptor.getInput1();
        mlir::Value rhs = adaptor.getInput2();

        mlir::Operation* newOp = nullptr;

        auto resType = mlir::dyn_cast<mlir::RankedTensorType>(op.getType());
        if (!resType) {
            return rewriter.notifyMatchFailure(op, "not a ranked tensor");
        }

        auto eltype = resType.getElementType();
        if (mlir::isa<mlir::FloatType>(eltype)) {
            newOp = rewriter.create<mlir::arith::AddFOp>(loc, lhs, rhs);
        } else if (mlir::isa<mlir::IntegerType>(eltype)) {
            newOp = rewriter.create<mlir::arith::AddIOp>(loc, lhs, rhs);
        }

        if (!newOp) return mlir::failure();

        rewriter.replaceOp(op, newOp->getResults());
        return mlir::success();
    }
};

struct MatMulConversion : public mlir::OpConversionPattern<mlir::tosa::MatMulOp> {
    using OpConversionPattern<mlir::tosa::MatMulOp>::OpConversionPattern;
    mlir::LogicalResult matchAndRewrite(mlir::tosa::MatMulOp op, OpAdaptor adaptor,
                                        mlir::ConversionPatternRewriter& rewriter) const override {
        MSG("TOSA-TO-LINALG: MATMUL\n");
        auto loc = rewriter.getUnknownLoc();

        mlir::Value lhs = adaptor.getB();
        mlir::Value rhs = adaptor.getA();

        MSG("GOT LHS AND RHS\n");
        auto resType = mlir::dyn_cast<mlir::RankedTensorType>(op.getType());
        if (!resType) {
            return rewriter.notifyMatchFailure(op, "resType in add is not RankedTensor :(");
        }

        mlir::Operation* newOp = nullptr;

        auto eltype = resType.getElementType();

        llvm::SmallVector<mlir::Value> dynamicDims;
        dynamicDims.resize(resType.getRank());

        MSG("DEALING WITH DYNAMIC DIMS\n");
        if (!resType.hasRank() || resType.isDynamicDim(0)) {
            dynamicDims[0] = rewriter.create<mlir::tensor::DimOp>(loc, op.getOperand(0), 0);
        }

        if (!resType.hasRank() || resType.isDynamicDim(1)) {
            dynamicDims[1] = rewriter.create<mlir::tensor::DimOp>(loc, op.getOperand(0), 1);
        }

        if (!resType.hasRank() || resType.isDynamicDim(2)) {
            dynamicDims[2] = rewriter.create<mlir::tensor::DimOp>(loc, op.getOperand(1), 2);
        }

        MSG("FILTERING DIMS\n");
        auto filteredDims = MLIRDialectTranslator::condenseValues(dynamicDims);

        MSG("CREATING ZERO TENSOR\n");
        auto zeroTensor = MLIRDialectTranslator::createZeroTensor(rewriter, resType, filteredDims);

        rewriter.replaceOpWithNewOp<mlir::linalg::BatchMatmulOp>(
        op, mlir::TypeRange{op.getType()},
        mlir::ValueRange{lhs, rhs}, zeroTensor);
        return mlir::success();
    }

};

// --------------------------------------------- //

mlir::LogicalResult MLIRDialectTranslator::translateTOSATOLINALG(mlir::ModuleOp& module) {
    auto loc = builder_.getUnknownLoc();

    MSG("Got module to translate ---> ");
    ON_DEBUG(llvm::errs() << module << "\n");
    MSG("-------------------------------------\n");


    MSG("G3TTING CONTEXT\n");

    auto modCtx = module.getContext();

    if (!modCtx) {
        MSG("nullptr context :(\n");
        return mlir::failure();
    }

    MSG("1\n");
    mlir::ConversionTarget target(*modCtx);
    MSG("2\n");
    mlir::RewritePatternSet patterns(modCtx);
    MSG("3\n");
    mlir::TypeConverter typeConverter = createTosaToLinalgTypeConverter();
    MSG("SET CONVERSION TARGET, REWRITER PATTERNS, TYPE CONVERTER\n");

    setDialects(target);
    setPatterns(patterns, typeConverter);

    MSG("SET ILLEGAL/LEGAL DIALECTS, PATTERNS");

    auto modOp = module.getOperation();
    llvm::errs() << "module operation " << *modOp << "\n";

    MSG("TRANSLATING OPERATIONS\n");
    auto result = mlir::applyFullConversion(modOp, target, std::move(patterns));
    MSG("TRANSLATED OPERATIONS\n");
    llvm::errs() << "result = " << static_cast<unsigned>(result.succeeded()) << "\n";

    if (mlir::failed(result)) {
        MSG("FAILED TO TRANSLATE!\n");
        return mlir::failure();
    }

    TranslatorGreen("/ / / / / / / / RESULT / / / / / / / /\n");
    llvm::errs() << module << "\n";
    TranslatorGreen("---------------------------------------\n");
    return mlir::success();

}

// -------------------------------------------
//              mlir - specific
// -------------------------------------------

mlir::TypeConverter MLIRDialectTranslator::createTosaToLinalgTypeConverter() {
    mlir::TypeConverter converter;

    converter.addConversion([&](mlir::Type type) -> std::optional<mlir::Type> {
        if (type.isUnsignedInteger()) {
        return mlir::IntegerType::get(type.getContext(), type.getIntOrFloatBitWidth(),
                              mlir::IntegerType::SignednessSemantics::Signless);
        }
        return type;
    });

    converter.addConversion([&](mlir::TensorType type) -> std::optional<mlir::Type> {
        auto converted = converter.convertType(type.getElementType());
        if (!converted)
        return {};
        return type.clone(converted);
    });

    converter.addSourceMaterialization([&](mlir::OpBuilder &builder, mlir::Type resultType,
                                         mlir::ValueRange inputs,
                                         mlir::Location loc) -> mlir::Value {
        if (inputs.size() != 1)
        return mlir::Value();

        return builder.create<mlir::UnrealizedConversionCastOp>(loc, resultType, inputs)
            .getResult(0);
    });

    converter.addTargetMaterialization([&](mlir::OpBuilder &builder, mlir::Type resultType,
                                         mlir::ValueRange inputs,
                                         mlir::Location loc) -> mlir::Value {
        if (inputs.size() != 1)
        return mlir::Value();

        return builder.create<mlir::UnrealizedConversionCastOp>(loc, resultType, inputs)
            .getResult(0);
    });

    return converter;
}

mlir::Value MLIRDialectTranslator::createZeroTensor(mlir::RankedTensorType type, llvm::SmallVector<mlir::Value> dims) {
    auto loc = builder_.getUnknownLoc();
    auto zeroAttr = builder_.getZeroAttr(type.getElementType());
    mlir::Value zero = builder_.create<mlir::arith::ConstantOp>(loc, zeroAttr);
    auto emptyTensor = builder_.create<mlir::tensor::EmptyOp>(loc, type.getShape(), type.getElementType(), dims);
    auto zeroTensor = builder_.create<mlir::linalg::FillOp>(loc, mlir::ValueRange{zero}, mlir::ValueRange{emptyTensor}).result();

    return zeroTensor;
}

mlir::Value MLIRDialectTranslator::createZeroTensor(mlir::PatternRewriter& rewriter, mlir::RankedTensorType type, llvm::SmallVector<mlir::Value> dims) {
    auto loc = rewriter.getUnknownLoc();
    auto zeroAttr       = rewriter.getZeroAttr(type.getElementType());
    mlir::Value zero    = rewriter.create<mlir::arith::ConstantOp>(loc, zeroAttr);
    auto emptyTensor    = rewriter.create<mlir::tensor::EmptyOp>(loc, type.getShape(), type.getElementType(), dims);
    auto zeroTensor     = rewriter.create<mlir::linalg::FillOp>(loc, mlir::ValueRange{zero}, mlir::ValueRange{emptyTensor}).result();
    return zeroTensor;
}


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

llvm::SmallVector<mlir::Value> MLIRDialectTranslator::condenseValues(llvm::SmallVector<mlir::Value>& values) {
    llvm::SmallVector<mlir::Value> retVec;
    for (auto value : values) {
        if (value) {
            retVec.push_back(value);
        }
    }
    return retVec;
}

void MLIRDialectTranslator::setDialects(mlir::ConversionTarget& target) {
    target.addIllegalDialect<mlir::tosa::TosaDialect>();

    target.addLegalDialect<mlir::linalg::LinalgDialect>();
    target.addLegalDialect<mlir::func::FuncDialect>();
    target.addLegalDialect<mlir::tensor::TensorDialect>();
    target.addLegalDialect<mlir::arith::ArithDialect>();
    target.addLegalDialect<mlir::BuiltinDialect>();

    target.addLegalOp<mlir::tosa::ApplyScaleOp>();
    target.addLegalOp<mlir::tosa::IfOp>();
    target.addLegalOp<mlir::tosa::ConstOp>();
    target.addLegalOp<mlir::tosa::ConstShapeOp>();
    target.addLegalOp<mlir::tosa::WhileOp>();
    target.addLegalOp<mlir::tosa::ConcatOp>();
    target.addLegalOp<mlir::tosa::SliceOp>();
    target.addLegalOp<mlir::tosa::ReshapeOp>();
    target.addLegalOp<mlir::tosa::PadOp>();

    target.markUnknownOpDynamicallyLegal([](mlir::Operation *) { return true; });

}

void MLIRDialectTranslator::setPatterns(mlir::RewritePatternSet& patterns, mlir::TypeConverter& converter) {
    patterns.add<AddOpConversion>(converter, patterns.getContext());
    patterns.add<SubOpConversion>(converter, patterns.getContext());
    patterns.add<MulOpConversion>(converter, patterns.getContext());
    patterns.add<MatMulConversion>(converter, patterns.getContext());

    TranslatorGreen("Got patterns set!");
}


//NOTE - DEPRECATED
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
    auto res = op.getResult(0);
    auto resType = mlir::dyn_cast<mlir::RankedTensorType>(res.getType());
    if (!resType) {
        TranslatorWarn("resType in add is not RankedTensor :(");
        return;
    }

    mlir::Operation* newOp = nullptr;

    auto eltype = resType.getElementType();


    llvm::SmallVector<mlir::Value> dynamicDims;
    dynamicDims.resize(resType.getRank());

    if (!resType.hasRank() || resType.isDynamicDim(0)) {
      dynamicDims[0] = builder_.create<mlir::tensor::DimOp>(loc, op.getOperand(0), 0);
    }

    if (!resType.hasRank() || resType.isDynamicDim(1)) {
      dynamicDims[1] = builder_.create<mlir::tensor::DimOp>(loc, op.getOperand(0), 1);
    }

    if (!resType.hasRank() || resType.isDynamicDim(2)) {
      dynamicDims[2] = builder_.create<mlir::tensor::DimOp>(loc, op.getOperand(1), 2);
    }

    auto filteredDims = condenseValues(dynamicDims);
    auto zeroTensor = createZeroTensor(resType, filteredDims);

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

