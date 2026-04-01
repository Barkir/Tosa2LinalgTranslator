### TOSA-LINALG TRANSLATOR
![LLVM Badge](https://img.shields.io/badge/LLVM-262D3A?logo=llvm&logoColor=fff&style=for-the-badge)![C++ Badge](https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus&logoColor=fff&style=for-the-badge)

### Architecture
`ModuleSerializer` - load mlir-code into module
> Code Example
```cpp
mlir::OwningOpRef<mlir::ModuleOp> module;
    auto result = ms.load(file, module);
    if (mlir::failed(result)) {
      llvm::errs() << BG_BRIGHT_RED << "Failed to load file" << file << TOSA2LINALG_RESET << "\n";
      return -1;
    }
```


`MLIRDialectTranslator` - class for dialect-translation

##### Key steps
- load needed dialects
- mark `tosa` as illegal and `linalg, ...` as legal dialects.
- add patterns to `RewritePatternSet`
- add `typeConverter`


> Available tranlsations

| | |
|-|-|
| tosa.add | ☑️|
| tosa.sub | ☑️|
| tosa.mul | ☑️|
| tosa.matmul | ☑️|
| tosa.conv2d | ❌|
| tosa.clamp | ❌|
