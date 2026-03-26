#pragma once

#include "mlir/Transforms/DialectConversion.h"

struct TosaToLinalgTypeConverter : public mlir::TypeConverter {
    TosaToLinalgTypeConverter() {
        addConversion([](mlir::Type type) -> mlir::Type {
            return type;
        });


    }
};
