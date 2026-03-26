func.func @test_add_2d_broadcast(%arg0: tensor<2x1xf32>, %arg1: tensor<1x1xf32>) -> tensor<2x1xf32> {

  %0 = tosa.add %arg0, %arg1 : (tensor<2x1xf32>, tensor<1x1xf32>) -> tensor<2x1xf32>
  return %0 : tensor<2x1xf32>
}
