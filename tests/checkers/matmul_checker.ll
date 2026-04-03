// CHECK-LABEL: func.func @matmul
// CHECK-SAME:  (%[[ARG0:.*]]: tensor<1x5x3xf32>, %[[ARG1:.*]]: tensor<1x3x6xf32>)

// CHECK: %[[C0:.*]] = arith.constant 0.0 : f32
// CHECK: %[[INIT:.*]] = tensor.empty() : tensor<1x5x6xf32>
// CHECK: %[[FILLED:.*]] = linalg.fill ins(%[[C0]] : f32) outs(%[[INIT]] : tensor<1x5x6xf32>) -> tensor<1x5x6xf32>

// CHECK: %[[RES:.*]] = linalg.generic
// CHECK-SAME: indexing_maps = [affine_map<(d0, d1, d2, d3) -> (d0, d1, d3)>, affine_map<(d0, d1, d2, d3) -> (d0, d3, d2)>, affine_map<(d0, d1, d2, d3) -> (d0, d1, d2)>]
// CHECK-SAME: iterator_types = ["parallel", "parallel", "parallel", "reduction"]
// CHECK-SAME: ins(%[[ARG0]], %[[ARG1]] : tensor<1x5x3xf32>, tensor<1x3x6xf32>)
// CHECK-SAME: outs(%[[FILLED]] : tensor<1x5x6xf32>)

// CHECK: ^bb0(%[[VAL0:.*]]: f32, %[[VAL1:.*]]: f32, %[[VAL_OUT:.*]]: f32):
// CHECK-NEXT: %[[MUL:.*]] = arith.mulf %[[VAL0]], %[[VAL1]] : f32
// CHECK-NEXT: %[[ADD:.*]] = arith.addf %[[MUL]], %[[VAL_OUT]] : f32
// CHECK-NEXT: linalg.yield %[[ADD]] : f32

// CHECK: return %[[RES]] : tensor<1x5x6xf32>
