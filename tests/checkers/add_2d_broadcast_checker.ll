// CHECK-LABEL: func.func @test_add_2d_broadcast
// CHECK-SAME:  (%[[ARG0:.*]]: tensor<2x1xf32>, %[[ARG1:.*]]: tensor<1x1xf32>) -> tensor<2x1xf32> {
// CHECK:         %[[INIT:.*]] = tensor.empty() : tensor<2x1xf32>
// CHECK:         %[[RES:.*]] = linalg.generic {
// CHECK-SAME:    indexing_maps = [affine_map<(d0, d1) -> (d0, d1)>, affine_map<(d0, d1) -> (0, d1)>, affine_map<(d0, d1) -> (d0, d1)>],
// CHECK-SAME:    iterator_types = ["parallel", "parallel"]
// CHECK-SAME:    } ins(%[[ARG0]], %[[ARG1]] : tensor<2x1xf32>, tensor<1x1xf32>) outs(%[[INIT]] : tensor<2x1xf32>) {
// CHECK:         ^bb0(%[[IN0:.*]]: f32, %[[IN1:.*]]: f32, %[[OUT:.*]]: f32):
// CHECK:           %[[ADD:.*]] = arith.addf %[[IN0]], %[[IN1]] : f32
// CHECK:           linalg.yield %[[ADD]] : f32
// CHECK:         } -> tensor<2x1xf32>
// CHECK:         return %[[RES]] : tensor<2x1xf32>
// CHECK:       }
