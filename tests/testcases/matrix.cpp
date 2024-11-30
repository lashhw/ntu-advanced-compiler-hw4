void matrix(
    // input matrix 1
    float a11, float a12,
    float a21, float a22,

    // input matrix 2
    float b11, float b12,
    float b21, float b22,

    // output
    float* c11, float* c12,
    float* c21, float* c22
) {
    float m1 = a11 * b11;
    float m2 = a12 * b21;
    float t1 = m1 + m2;
    *c11 = t1;

    float m3 = a11 * b12;
    float m4 = a12 * b22;
    float t2 = m3 + m4;
    *c12 = t2;

    float m5 = a21 * b11;
    float m6 = a22 * b21;
    float t3 = m5 + m6;
    *c21 = t3;

    float m7 = a21 * b12;
    float m8 = a22 * b22;
    float t4 = m7 + m8;
    *c22 = t4;
}