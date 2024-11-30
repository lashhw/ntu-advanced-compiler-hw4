void sobel(
    // input constant
    float two,
    // input matrix
    float p00, float p01, float p02,
    float p10, float p11, float p12,
    float p20, float p21, float p22,

    // output
    float* magnitude 
) {
    // Sobel X gradient
    float gx1 = p00 + (two * p10) + p20;
    float gx2 = p02 + (two * p12) + p22;
    float gx = gx2 - gx1;

    // Sobel Y gradient
    float gy1 = p00 + (two * p01) + p02;
    float gy2 = p20 + (two * p21) + p22;
    float gy = gy2 - gy1;

    // Magnitude computation
    float gx_squared = gx * gx;
    float gy_squared = gy * gy;
    float sum = gx_squared + gy_squared;
    *magnitude = sum;
}