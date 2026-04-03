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
      llvm::errs() << BG_BRIGHT_RED << "Failed to load file" << file << TOSA2LINALG_RESET << "\n";
      return -1;
    }

    MLIRDialectTranslator translator;
    auto moduleGet = module.get();
    // llvm::errs() << moduleGet << "\n";
    auto translationResult = translator.translate(moduleGet, TranslatorTypes::TOSA_TO_LINALG);

    llvm::errs() << "now module looks like: " << moduleGet << "\n";

    return 0;
}
