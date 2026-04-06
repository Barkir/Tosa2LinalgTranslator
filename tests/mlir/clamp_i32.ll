func.func @test_clamp_i32(%arg0: tensor<4xi32>) -> tensor<4xi32> {
  %0 = "tosa.clamp"(%arg0) {
    min_int = -5 : i64,
    max_int = 5 : i64,
    min_fp = -5.0 : f32,
    max_fp = 5.0 : f32
  } : (tensor<4xi32>) -> tensor<4xi32>
  return %0 : tensor<4xi32>
}
