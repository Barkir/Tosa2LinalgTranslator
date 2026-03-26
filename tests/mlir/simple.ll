func.func @test_simple_i32(%arg0: tensor<1xi32>, %unsigned: tensor<1xui32>, %unsigned64: tensor<1xui64>) -> () {
  %0 = tosa.add %arg0, %arg0 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %1 = tosa.sub %arg0, %arg0 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %shift1 = "tosa.const"() <{values = dense<0> : tensor<1xi8>}> : () -> tensor<1xi8>
  %2 = tosa.mul %arg0, %arg0, %shift1 : (tensor<1xi32>, tensor<1xi32>, tensor<1xi8>) -> tensor<1xi32>
  %shift2 = "tosa.const"() <{values = dense<2> : tensor<1xi8>}> : () -> tensor<1xi8>
  %3 = tosa.mul %arg0, %arg0, %shift2: (tensor<1xi32>, tensor<1xi32>, tensor<1xi8>) -> tensor<1xi32>
  %4 = tosa.intdiv %arg0, %arg0 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %in_zp = "tosa.const"() <{values = dense<0> : tensor<1xi32>}> : () -> tensor<1xi32>
  %out_zp = "tosa.const"() <{values = dense<0> : tensor<1xi32>}> : () -> tensor<1xi32>
  %5 = tosa.negate %arg0, %in_zp, %out_zp : (tensor<1xi32>, tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %6 = tosa.bitwise_and %arg0, %arg0 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %7 = tosa.bitwise_or %arg0, %arg0 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %8 = tosa.bitwise_xor %arg0, %arg0 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %9 = tosa.logical_left_shift %arg0, %arg0 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %10 = tosa.logical_right_shift %arg0, %arg0 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %11 = tosa.arithmetic_right_shift %arg0, %arg0 {round = 0 : i1} : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %12 = tosa.arithmetic_right_shift %arg0, %arg0 {round = 1 : i1} : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %13 = tosa.clz %arg0 : (tensor<1xi32>) -> tensor<1xi32>
  %14 = tosa.greater %0, %1 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi1>
  %15 = tosa.greater_equal %0, %1 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi1>
  %16 = tosa.select %14, %0, %1 : (tensor<1xi1>, tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %17 = tosa.maximum %0, %1 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %18 = tosa.minimum %0, %1 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
  %19 = tosa.clamp %0 {min_val = 1 : i32, max_val = 5 : i32} : (tensor<1xi32>) -> tensor<1xi32>
  %u0 = tosa.clamp %unsigned {min_val = 4 : ui32, max_val = 32 : ui32} : (tensor<1xui32>) -> tensor<1xui32>
  %20 = tosa.cast %0 : (tensor<1xi32>) -> tensor<1xi16>
  %21 = tosa.cast %0 : (tensor<1xi32>) -> tensor<1xi64>
  %22 = tosa.cast %0 : (tensor<1xi32>) -> tensor<1xi1>
  %23 = tosa.cast %0 : (tensor<1xi32>) -> tensor<1xf32>
  %24 = tosa.abs %arg0 : (tensor<1xi32>) -> tensor<1xi32>

  return
}
