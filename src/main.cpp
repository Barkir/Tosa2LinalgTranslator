#include "err.hpp"
#include "file.hpp"
#include "translator.hpp"

int main(int argc, char* argv[]) {
    mlir::MLIRContext context;
    MLIRModuleSerializer ms(context);


    llvm::StringRef file = argc > 1 ? argv[1] : "../tests/mlir/add_0d.ll";


    mlir::OwningOpRef<mlir::ModuleOp> module;
    auto result = ms.load(file, module);
    if (mlir::failed(result)) {
      TranslatorErr(llvm::Twine("Failed to load file: ") + file);
    }

    MLIRDialectTranslator translator;
    auto moduleGet = module.get();
    translator.translate(moduleGet, TranslatorTypes::TOSA_TO_LINALG);

    return 0;
}
