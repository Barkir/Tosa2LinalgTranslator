// CHECK-LABEL: func.func @test_add_2d_broadcast
// CHECK-SAME:  (%[[ARG0:.*]]: tensor<2x1xf32>, %[[ARG1:.*]]: tensor<1x1xf32>)

// CHECK: %[[C0:.*]] = arith.constant 0 : index
// CHECK: %[[DIM0:.*]] = tensor.dim %[[ARG0]], %[[C0]] : tensor<2x1xf32>
// CHECK: %[[C1:.*]] = arith.constant 1 : index
// CHECK: %[[DIM1:.*]] = tensor.dim %[[ARG0]], %[[C1]] : tensor<2x1xf32>
// CHECK: %[[INIT:.*]] = tensor.empty(%[[DIM0]], %[[DIM1]]) : tensor<2x1xf32>

// CHECK: %[[RES:.*]] = linalg.generic
// CHECK-SAME: indexing_maps = [affine_map<(d0, d1) -> (d0, d1)>, affine_map<(d0, d1) -> (0, d1)>, affine_map<(d0, d1) -> (d0, d1)>]
// CHECK-SAME: iterator_types = ["parallel", "parallel"]
// CHECK-SAME: ins(%[[ARG0]], %[[ARG1]] : tensor<2x1xf32>, tensor<1x1xf32>)
// CHECK-SAME: outs(%[[INIT]] : tensor<2x1xf32>)

// CHECK: ^bb0(%[[VAL0:.*]]: f32, %[[VAL1:.*]]: f32, %[[VAL_OUT:.*]]: f32):
// CHECK-NEXT: %[[ADD:.*]] = arith.addf %[[VAL0]], %[[VAL1]] : f32
// CHECK-NEXT: linalg.yield %[[ADD]] : f32

// CHECK: return %[[RES]] : tensor<2x1xf32>
