func.func @test_conv2d_simple(%input: tensor<1x14x14x16xf32>, %weights: tensor<32x3x3x16xf32>, %bias: tensor<32xf32>) -> tensor<1x14x14x32xf32> {

  %0 = tosa.conv2d %input, %weights, %bias {
    pad = array<i64: 1, 1, 1, 1>,
    stride = array<i64: 1, 1>,
    dilation = array<i64: 1, 1>
  } : (tensor<1x14x14x16xf32>, tensor<32x3x3x16xf32>, tensor<32xf32>) -> tensor<1x14x14x32xf32>

  return %0 : tensor<1x14x14x32xf32>
}

