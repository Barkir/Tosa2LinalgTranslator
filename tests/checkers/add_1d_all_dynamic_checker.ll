// CHECK-LABEL: func.func @test_add_1d_all_dynamic
// CHECK-SAME:  (%[[ARG0:.*]]: tensor<?xf32>, %[[ARG1:.*]]: tensor<?xf32>) -> tensor<?xf32> {
// CHECK-DAG:     %[[C0:.*]] = arith.constant 0 : index
// CHECK-DAG:     %[[DIM:.*]] = tensor.dim %[[ARG0]], %[[C0]] : tensor<?xf32>
// CHECK:         %[[INIT:.*]] = tensor.empty(%[[DIM]]) : tensor<?xf32>
// CHECK:         %[[RES:.*]] = linalg.generic {
// CHECK-SAME:    indexing_maps = [affine_map<(d0) -> (d0)>, affine_map<(d0) -> (d0)>, affine_map<(d0) -> (d0)>],
// CHECK-SAME:    iterator_types = ["parallel"]
// CHECK-SAME:    } ins(%[[ARG0]], %[[ARG1]] : tensor<?xf32>, tensor<?xf32>) outs(%[[INIT]] : tensor<?xf32>) {
// CHECK:         ^bb0(%[[IN0:.*]]: f32, %[[IN1:.*]]: f32, %[[OUT:.*]]: f32):
// CHECK:           %[[ADD:.*]] = arith.addf %[[IN0]], %[[IN1]] : f32
// CHECK:           linalg.yield %[[ADD]] : f32
// CHECK:         } -> tensor<?xf32>
// CHECK:         return %[[RES]] : tensor<?xf32>
// CHECK:       }
