#include "translator.hpp"

// --------------------------------------------------
//              Translator classes
// --------------------------------------------------

// mlir::LogicalResult helpMatchAndRewriteElementWiseOp(mlir::ConversionPatternRewriter& rewriter, Operation* op)
//
// template <typename ElementWiseOp>
// struct ElementWiseConverter : public mlir::OpConversionPattern<ElementWiseOp> {
// public:
//     using OpConversionPattern<SrcOp>::OpConversionPattern;
//     using typename OpConversionPattern<ElementWiseOp>::OpAdaptor;
//
//     mlir::LogicalResult matchAndRewrite(ElementWiseOp op, OpAdaptor adaptor,
//                                         mlir::ConversionPatternRewriter& rewriter) const override {
//         return helpMatchAndRewriteElementWiseOp(rewriter, op, adaptor.getOperands());
//     }
// }

// struct Conv2DConversion : public mlir::OpConversionPattern<mlir::tosa::Conv2DOp> {
//     using OpConversionPattern<mlir::tosa::Conv2DOp>::OpConversionPattern;
//     mlir::LogicalResult matchAndRewrite(mlir::tosa::Conv2DOp op, OpAdaptor adaptor,
//                                         mlir::ConversionPatternRewriter& rewriter) const override {
//         MSG("TOSA-TO-LINALG : CONV2D");
//         auto loc = rewriter.getUnknownLoc();
//         auto input = adaptor.getInput();
//         auto weight = adaptor.getWeight();
//         auto bias = adaptor.getBias();
//
//         auto pad = op.getPadAttr();
//         auto stride = op.getStrideAttr();
//         auto dilation = op.getDilationAttr();
//
//         // tosa::Conv2D uses nhwc layout by default, so we generate linalgConv
//         auto linalgConv = rewriter.create<mlir::linalg::Conv2DNhwcFhwcOp>(
//             loc
//         );
//
//         return mlir::success();
//     }
// };

struct ClampConversion : public mlir::OpConversionPattern<mlir::tosa::ClampOp> {
    using OpConversionPattern<mlir::tosa::ClampOp>::OpConversionPattern;
    mlir::LogicalResult matchAndRewrite(mlir::tosa::ClampOp op, OpAdaptor adaptor,
                                        mlir::ConversionPatternRewriter& rewriter) const override {

        MSG("TOSA-TO-LINALG : RELU\n");
        auto loc = rewriter.getUnknownLoc();
        auto input = adaptor.getInput();
        auto type = mlir::cast<mlir::RankedTensorType>(input.getType());
        auto eltype = type.getElementType();

        mlir::Value newOp;

        if (mlir::isa<mlir::FloatType>(eltype)) {
            MSG("GOT FLOAT TYPE IN INPUT\n");
            auto inputTy = mlir::cast<mlir::TensorType>(input.getType());

            mlir::APFloat minApf = mlir::cast<mlir::FloatAttr>(op->getAttr("min_fp")).getValue();
            mlir::APFloat maxApf = mlir::cast<mlir::FloatAttr>(op->getAttr("max_fp")).getValue();

            auto minAttr = mlir::DenseElementsAttr::get(
                inputTy,
                minApf
            );

            auto maxAttr = mlir::DenseElementsAttr::get(
                inputTy,
                maxApf
            );

            MSG("GOT minApf, maxApf\n");
            auto min = rewriter.create<mlir::arith::ConstantOp>(
                loc, inputTy, minAttr
            );

            auto max = rewriter.create<mlir::arith::ConstantOp>(
                loc, inputTy, maxAttr
            );

            MSG("GOT MAX AND MIN\n");
            auto result = mlir::tosa::clampFloatHelper(loc, input, min, max, rewriter);
            mlir::Value isNaN = rewriter.create<mlir::arith::CmpFOp>(
                op->getLoc(), mlir::arith::CmpFPredicate::UNO, input, input);


            auto newFloatOp = rewriter.create<mlir::arith::SelectOp>(op->getLoc(), isNaN, min, result);
            rewriter.replaceOp(op, newFloatOp);
            MSG("REPLACED OP\n");
        } else if (mlir::isa<mlir::IntegerType>(eltype)) {
            MSG("GOT INPUT TYPE IN INPUT\n");
            auto intTy = mlir::cast<mlir::IntegerType>(eltype);
            auto inputTy = mlir::cast<mlir::TensorType>(input.getType());

            int64_t min = mlir::cast<mlir::IntegerAttr>(op->getAttr("min_int")).getValue().getSExtValue();
            int64_t max = mlir::cast<mlir::IntegerAttr>(op->getAttr("max_int")).getValue().getSExtValue();

            auto minAttr = mlir::DenseElementsAttr::get(
                inputTy,
                llvm::APInt(intTy.getIntOrFloatBitWidth(), min, true)
            );

            auto maxAttr = mlir::DenseElementsAttr::get(
                inputTy,
                llvm::APInt(intTy.getIntOrFloatBitWidth(), max, true)
            );

            auto minVal = rewriter.create<mlir::arith::ConstantOp>(loc, inputTy, minAttr);
            auto maxVal = rewriter.create<mlir::arith::ConstantOp>(loc, inputTy, maxAttr);
            MSG("GOT MAX AND MIN\n");
            auto newIntOp = mlir::tosa::clampIntHelper(loc, input, minVal, maxVal, rewriter,
                                    intTy.isUnsignedInteger());
            rewriter.replaceOp(op, newIntOp);
            MSG("REPLACED OP\n");
        } else {
            return mlir::failure();
        }
        return mlir::success();
    }
};

struct NegateOpConversion : public mlir::OpConversionPattern<mlir::tosa::NegateOp> {
    using OpConversionPattern<mlir::tosa::NegateOp>::OpConversionPattern;
    mlir::LogicalResult matchAndRewrite(mlir::tosa::NegateOp op, OpAdaptor adaptor,
                                        mlir::ConversionPatternRewriter& rewriter) const override {
        MSG("TOSA-TO-LINALG : NEGATE\n");
        auto loc = rewriter.getUnknownLoc();
        mlir::Value res = adaptor.getInput1();

        mlir::Operation* newOp = nullptr;

        auto resType = mlir::dyn_cast<mlir::RankedTensorType>(op.getType());
        if (!resType) {
            return rewriter.notifyMatchFailure(op, "not a ranked tensor");
        }

        auto args = adaptor.getOperands();

        auto eltype = resType.getElementType();
        if (mlir::isa<mlir::FloatType>(eltype)) {
            newOp = rewriter.create<mlir::arith::NegFOp>(loc, resType, res);
        }

        // add integer type

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
        auto operands = mlir::ValueRange{lhs, rhs};

        auto resType = mlir::dyn_cast<mlir::RankedTensorType>(op.getType());
        if (!resType) {
            return rewriter.notifyMatchFailure(op, "not a ranked tensor");
        }

        auto output = MLIRDialectTranslator::createEmptyTensor(rewriter, resType, lhs);
        auto affineMaps = MLIRDialectTranslator::getAffineMaps(operands, resType, rewriter);

        auto genericOp = rewriter.create<mlir::linalg::GenericOp>(
            loc,
            resType,
            mlir::ValueRange{lhs, rhs},
            mlir::ValueRange{output},
            affineMaps,
            mlir::tosa::getNParallelLoopsAttrs(static_cast<unsigned>(resType.getRank())),
            [&](mlir::OpBuilder &b, mlir::Location l, mlir::ValueRange args) {
                auto lh = args[0];
                auto rh = args[1];
                mlir::Value res;

                auto eltype = lh.getType();
                if (mlir::isa<mlir::FloatType>(eltype)) {
                    res = b.create<mlir::arith::MulFOp>(l, lh, rh);
                } else if (mlir::isa<mlir::IntegerType>(eltype)) {
                    res = b.create<mlir::arith::MulIOp>(l, lh, rh);
                }
                b.create<mlir::linalg::YieldOp>(l, res);
            });


        rewriter.replaceOp(op, genericOp->getResults());
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
        auto operands = mlir::ValueRange{lhs, rhs};

        auto resType = mlir::dyn_cast<mlir::RankedTensorType>(op.getType());
        if (!resType) {
            return rewriter.notifyMatchFailure(op, "not a ranked tensor");
        }

        auto output = MLIRDialectTranslator::createEmptyTensor(rewriter, resType, lhs);
        auto affineMaps = MLIRDialectTranslator::getAffineMaps(operands, resType, rewriter);

        auto genericOp = rewriter.create<mlir::linalg::GenericOp>(
            loc,
            resType,
            mlir::ValueRange{lhs, rhs},
            mlir::ValueRange{output},
            affineMaps,
            mlir::tosa::getNParallelLoopsAttrs(static_cast<unsigned>(resType.getRank())),
            [&](mlir::OpBuilder &b, mlir::Location l, mlir::ValueRange args) {
                auto lh = args[0];
                auto rh = args[1];
                mlir::Value res;

                auto eltype = lh.getType();
                if (mlir::isa<mlir::FloatType>(eltype)) {
                    res = b.create<mlir::arith::SubFOp>(l, lh, rh).getResult();
                } else if (mlir::isa<mlir::IntegerType>(eltype)) {
                    res = b.create<mlir::arith::SubIOp>(l, lh, rh).getResult();
                }
                b.create<mlir::linalg::YieldOp>(l, res);
            });


        rewriter.replaceOp(op, genericOp->getResults());
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
        auto operands = mlir::ValueRange{lhs, rhs};

        auto resType = mlir::dyn_cast<mlir::RankedTensorType>(op.getType());
        if (!resType) {
            return rewriter.notifyMatchFailure(op, "not a ranked tensor");
        }

        auto output = MLIRDialectTranslator::createEmptyTensor(rewriter, resType, lhs);
        auto affineMaps = MLIRDialectTranslator::getAffineMaps(operands, resType, rewriter);

        auto genericOp = rewriter.create<mlir::linalg::GenericOp>(
            loc,
            resType,
            mlir::ValueRange{lhs, rhs},
            mlir::ValueRange{output},
            affineMaps,
            mlir::tosa::getNParallelLoopsAttrs(static_cast<unsigned>(resType.getRank())),
            [&](mlir::OpBuilder &b, mlir::Location l, mlir::ValueRange args) {
                auto lh = args[0];
                auto rh = args[1];
                mlir::Value res;

                auto eltype = lh.getType();
                if (mlir::isa<mlir::FloatType>(eltype)) {
                    res = b.create<mlir::arith::AddFOp>(l, lh, rh).getResult();
                } else if (mlir::isa<mlir::IntegerType>(eltype)) {
                    res = b.create<mlir::arith::AddIOp>(l, lh, rh).getResult();
                }
                b.create<mlir::linalg::YieldOp>(l, res);
            });


        rewriter.replaceOp(op, genericOp->getResults());
        return mlir::success();
    }
};

struct MatMulConversion : public mlir::OpConversionPattern<mlir::tosa::MatMulOp> {
    using OpConversionPattern<mlir::tosa::MatMulOp>::OpConversionPattern;
    mlir::LogicalResult matchAndRewrite(mlir::tosa::MatMulOp op, OpAdaptor adaptor,
                                        mlir::ConversionPatternRewriter& rewriter) const override {
        MSG("TOSA-TO-LINALG: MATMUL\n");
        auto loc = rewriter.getUnknownLoc();

        mlir::Value lhs = adaptor.getA();
        mlir::Value rhs = adaptor.getB();

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

        LOG("got filteredDims with size {}\n", filteredDims.size());

        MSG("CREATING ZERO TENSOR\n");
        auto zeroTensor = MLIRDialectTranslator::createZeroTensor(rewriter, resType, lhs);

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

    loadDialects(modCtx);

    mlir::ConversionTarget target(*modCtx);
    mlir::RewritePatternSet patterns(modCtx);
    mlir::TypeConverter typeConverter = createTosaToLinalgTypeConverter();
    MSG("SET CONVERSION TARGET, REWRITER PATTERNS, TYPE CONVERTER\n");

    setDialectsLegality(target);
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
    std::string output;
    llvm::raw_string_ostream os(output);
    mlir::OpPrintingFlags flags;
    flags.printGenericOpForm(false);

    for (auto d : (module.getContext())->getLoadedDialects()) {
        llvm::errs() << d->getNamespace() << "\n";
    }

    module.print(os, flags);
    llvm::errs() << output << "\n";
    TranslatorGreen("---------------------------------------\n");
    return mlir::success();

}

// -------------------------------------------
//              mlir - specific
// -------------------------------------------

void MLIRDialectTranslator::loadDialects(mlir::MLIRContext* ctx) {
    ctx->loadDialect<mlir::arith::ArithDialect>();
    ctx->loadDialect<mlir::tensor::TensorDialect>();
    ctx->loadDialect<mlir::func::FuncDialect>();
    ctx->loadDialect<mlir::linalg::LinalgDialect>();
    ctx->loadDialect<mlir::tosa::TosaDialect>();
    ctx->loadAllAvailableDialects();
}

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

mlir::Value MLIRDialectTranslator::createEmptyTensor(mlir::PatternRewriter& rewriter, mlir::RankedTensorType type, mlir::Value input) {
    llvm::SmallVector<mlir::Value> dynamicSizes;
    auto loc = rewriter.getUnknownLoc();

    for (auto en : llvm::enumerate(type.getShape())) {
        if (mlir::ShapedType::isDynamic(en.value())) {
            auto dimOp = rewriter.create<mlir::tensor::DimOp>(loc, input, en.index());
            dynamicSizes.push_back(dimOp);
        }
    }
    auto value = rewriter.create<mlir::tensor::EmptyOp>(
        loc,
        type,
        dynamicSizes
    );

    return value;
}


llvm::SmallVector<mlir::AffineMap> MLIRDialectTranslator::getAffineMaps(mlir::ValueRange operands, mlir::RankedTensorType resType, mlir::PatternRewriter& rewriter) {
    auto rank = static_cast<unsigned>(resType.getRank());
    auto affineMaps = llvm::map_to_vector(operands, [&](mlir::Value operand) {
    auto shape = mlir::cast<mlir::ShapedType>(operand.getType()).getShape();
    llvm::SmallVector<mlir::AffineExpr> affineExprs;
    for (auto it : llvm::enumerate(shape)) {
        bool requiresBroadcast = (it.value() == 1 && resType.getDimSize(static_cast<unsigned>(it.index())) != 1);
        auto affineExpr = requiresBroadcast ? rewriter.getAffineConstantExpr(0) : rewriter.getAffineDimExpr(static_cast<unsigned>(it.index()));
            affineExprs.push_back(affineExpr);
        }
        return mlir::AffineMap::get(rank, 0, affineExprs, rewriter.getContext());
    });
    affineMaps.push_back(rewriter.getMultiDimIdentityMap(rank));
    return affineMaps;
}

mlir::Value MLIRDialectTranslator::createZeroTensor(mlir::RankedTensorType type, llvm::SmallVector<mlir::Value> dims) {
    auto loc = builder_.getUnknownLoc();
    auto zeroAttr = builder_.getZeroAttr(type.getElementType());
    mlir::Value zero = builder_.create<mlir::arith::ConstantOp>(loc, zeroAttr);
    auto emptyTensor = builder_.create<mlir::tensor::EmptyOp>(loc, type, dims);
    auto zeroTensor = builder_.create<mlir::linalg::FillOp>(loc, mlir::ValueRange{zero}, mlir::ValueRange{emptyTensor}).result();

    return zeroTensor;
}

mlir::Value MLIRDialectTranslator::createZeroTensor(mlir::PatternRewriter& rewriter, mlir::RankedTensorType type, mlir::Value input) {
    auto loc = rewriter.getUnknownLoc();
    auto zeroAttr       = rewriter.getZeroAttr(type.getElementType());
    mlir::Value zero    = rewriter.create<mlir::arith::ConstantOp>(loc, zeroAttr);
    auto emptyTensor    = MLIRDialectTranslator::createEmptyTensor(rewriter, type, input);
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

void MLIRDialectTranslator::setDialectsLegality(mlir::ConversionTarget& target) {

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
    patterns.add<NegateOpConversion>(converter, patterns.getContext());
    patterns.add<SubOpConversion>(converter, patterns.getContext());
    patterns.add<MulOpConversion>(converter, patterns.getContext());
    patterns.add<MatMulConversion>(converter, patterns.getContext());
    patterns.add<ClampConversion>(converter, patterns.getContext());

    TranslatorGreen("Got patterns set!");
}


