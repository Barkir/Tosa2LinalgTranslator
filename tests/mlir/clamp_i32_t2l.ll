module {
  func.func @test_clamp_i32(%arg0: tensor<4xi32>) -> tensor<4xi32> {
    %cst = arith.constant dense<-5> : tensor<4xi32>
    %cst_0 = arith.constant dense<5> : tensor<4xi32>
    %0 = arith.maxsi %cst, %arg0 : tensor<4xi32>
    %1 = arith.minsi %cst_0, %0 : tensor<4xi32>
    return %1 : tensor<4xi32>
  }
}
