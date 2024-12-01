void dce(
    // input
    float a,
    float b,
    float c,
    float d,

    // output
    float *e
) {
    float x = a * b;  // dead
    float y = a + c;
    float z = x + d;  // dead
    *e = y;
}