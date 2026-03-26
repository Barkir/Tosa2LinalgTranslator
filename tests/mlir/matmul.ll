func.func @matmul(%arg0: tensor<1x5x3xf32>, %arg1: tensor<1x3x6xf32>) -> (tensor<1x5x6xf32>) {
  %0 = tosa.matmul %arg0, %arg1 : (tensor<1x5x3xf32>, tensor<1x3x6xf32>)  -> tensor<1x5x6xf32>
  return %0 : tensor<1x5x6xf32>
}
