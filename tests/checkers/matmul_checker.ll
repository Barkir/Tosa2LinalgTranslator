// CHECK-LABEL: func.func @matmul
// CHECK-SAME:  (%[[ARG0:.*]]: tensor<1x5x3xf32>, %[[ARG1:.*]]: tensor<1x3x6xf32>)

// CHECK: %[[C0:.*]] = arith.constant 0.000000e+00 : f32
// CHECK: %[[INIT:.*]] = tensor.empty() : tensor<1x5x6xf32>
// CHECK: %[[FILLED:.*]] = linalg.fill ins(%[[C0]] : f32) outs(%[[INIT]] : tensor<1x5x6xf32>) -> tensor<1x5x6xf32>

// CHECK: %[[RESULT:.*]] = linalg.batch_matmul
// CHECK-SAME: ins(%arg0, %arg1 : tensor<1x5x3xf32>, tensor<1x3x6xf32>)
// CHECK-SAME: outs(%[[INIT:.*]] : tensor<1x5x6xf32>) -> tensor<1x5x6xf32>

// CHECK: return %[[RESULT:.*]] : tensor<1x5x6xf32>
