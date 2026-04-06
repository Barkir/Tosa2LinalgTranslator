// CHECK-LABEL: func.func @test_clamp_f32
// CHECK-DAG: %[[MIN:.*]] = arith.constant dense<-5.000000e+00> : tensor<4xf32>
// CHECK-DAG: %[[MAX:.*]] = arith.constant dense<5.000000e+00> : tensor<4xf32>
// CHECK: %[[CLAMP_STEP:.*]] = arith.minimumf %arg0, %[[MAX]] : tensor<4xf32>
// CHECK: %[[CLAMPED:.*]] = arith.maximumf %[[CLAMP_STEP]], %[[MIN]] : tensor<4xf32>
// CHECK: %[[IS_NAN:.*]] = arith.cmpf uno, %arg0, %arg0 : tensor<4xf32>
// CHECK: %[[RESULT:.*]] = arith.select %[[IS_NAN]], %[[MIN]], %[[CLAMPED]] : tensor<4xi1>, tensor<4xf32>
// CHECK: return %[[RESULT]]
