// CHECK-LABEL: func.func @test_add_0d
// CHECK-SAME:  (%[[ARG0:.*]]: tensor<f32>, %[[ARG1:.*]]: tensor<f32>)

// CHECK: %[[INIT:.*]] = tensor.empty() : tensor<f32>

// CHECK: %[[RES:.*]] = linalg.generic
// CHECK-SAME: indexing_maps = [affine_map<() -> ()>, affine_map<() -> ()>, affine_map<() -> ()>]
// CHECK-SAME: iterator_types = []
// CHECK-SAME: ins(%[[ARG0]], %[[ARG1]] : tensor<f32>, tensor<f32>)
// CHECK-SAME: outs(%[[INIT]] : tensor<f32>)

// CHECK: ^bb0(%[[VAL0:.*]]: f32, %[[VAL1:.*]]: f32, %[[VAL_OUT:.*]]: f32):
// CHECK-NEXT: %[[ADD:.*]] = arith.addf %[[VAL0]], %[[VAL1]] : f32
// CHECK-NEXT: linalg.yield %[[ADD]] : f32

// CHECK: return %[[RES]] : tensor<f32>
