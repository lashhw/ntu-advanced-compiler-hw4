void lagrange(
    // input points
    float x0, float y0,
    float x1, float y1, 
    float x2, float y2,

    // input interpolation point
    float x,

    // output interpolated value
    float* result
) {
    // compute basis terms
    float l0_t1 = (x - x1) * (x - x2);
    float l0_t2 = (x0 - x1) * (x0 - x2);
    float l1_t1 = (x - x0) * (x - x2);
    float l1_t2 = (x1 - x0) * (x1 - x2);
    float l2_t1 = (x - x0) * (x - x1);
    float l2_t2 = (x2 - x0) * (x2 - x1);
    
    // compute basis functions
    float l0 = l0_t1 / l0_t2;
    float l1 = l1_t1 / l1_t2;
    float l2 = l2_t1 / l2_t2;
    
    // multiply with y values
    float term0 = y0 * l0;
    float term1 = y1 * l1;
    float term2 = y2 * l2;
    
    // sum all terms
    float sum = term0 + term1;
    *result = sum + term2;
}