func.func @test_clamp_f32(%arg0: tensor<4xf32>) -> tensor<4xf32> {
  %0 = "tosa.clamp"(%arg0) {min_fp = -5.0 : f32, max_fp = 5.0 : f32, max_int = 5 : i64, min_int = -5 : i64} : (tensor<4xf32>) -> tensor<4xf32>
  return %0 : tensor<4xf32>
}
