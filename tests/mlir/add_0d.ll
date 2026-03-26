func.func @test_add_0d(%arg0: tensor<f32>, %arg1: tensor<f32>) -> tensor<f32> {
  %0 = tosa.add %arg0, %arg1 : (tensor<f32>, tensor<f32>) -> tensor<f32>
  return %0 : tensor<f32>
}

