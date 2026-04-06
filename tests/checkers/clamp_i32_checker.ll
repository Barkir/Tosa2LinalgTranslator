
// CHECK-LABEL: func.func @test_clamp_i32
// CHECK-DAG: %[[MIN:.*]] = arith.constant dense<-5> : tensor<4xi32>
// CHECK-DAG: %[[MAX:.*]] = arith.constant dense<5> : tensor<4xi32>
// CHECK: %[[STEP1:.*]] = arith.maxsi %[[MIN]], %arg0 : tensor<4xi32>
// CHECK: %[[RESULT:.*]] = arith.minsi %[[MAX]], %[[STEP1]] : tensor<4xi32>
// CHECK: return %[[RESULT]]
