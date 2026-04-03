// CHECK-LABEL: func.func @matmul_quantized
// CHECK-SAME:  (%[[ARG0:.*]]: tensor<1x5x3xi8>, %[[ARG1:.*]]: tensor<1x3x6xi8>)

// CHECK: %[[C0:.*]] = arith.constant 0 : i32
// CHECK: %[[INIT:.*]] = tensor.empty() : tensor<1x5x6xi32>
// CHECK: %[[FILLED:.*]] = linalg.fill ins(%[[C0]] : i32) outs(%[[INIT]] : tensor<1x5x6xi32>) -> tensor<1x5x6xi32>

// CHECK: %[[RES:.*]] = linalg.generic
// CHECK-SAME: indexing_maps = [affine_map<(d0, d1, d2, d3) -> (d0, d1, d3)>, affine_map<(d0, d1, d2, d3) -> (d0, d3, d2)>, affine_map<(d0, d1, d2, d3) -> (d0, d1, d2)>]
// CHECK-SAME: iterator_types = ["parallel", "parallel", "parallel", "reduction"]
// CHECK-SAME: ins(%[[ARG0]], %[[ARG1]] : tensor<1x5x3xi8>, tensor<1x3x6xi8>)
// CHECK-SAME: outs(%[[FILLED]] : tensor<1x5x6xi32>)

// CHECK: ^bb0(%[[VAL0:.*]]: i8, %[[VAL1:.*]]: i8, %[[VAL_OUT:.*]]: i32):
// CHECK-NEXT: %[[EXT0:.*]] = arith.extsi %[[VAL0]] : i8 to i32
// CHECK-NEXT: %[[EXT1:.*]] = arith.extsi %[[VAL1]] : i8 to i32
// CHECK-NEXT: %[[MUL:.*]] = arith.muli %[[EXT0]], %[[EXT1]] : i32
// CHECK-NEXT: %[[ADD:.*]] = arith.addi %[[MUL]], %[[VAL_OUT]] : i32
// CHECK-NEXT: linalg.yield %[[ADD]] : i32

// CHECK: return %[[RES]] : tensor<1x5x6xi32>
