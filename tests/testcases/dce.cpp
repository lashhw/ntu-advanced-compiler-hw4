void dce(
    // input
    float a,
    float b,
    float c,
    float d,

    // output
    float *e
) {
    float x = a * b;
    float y = c + a;
    float z = x + d; // dead
    *e = y;
}