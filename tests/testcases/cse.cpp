void cse(
    // input
    float a,
    float b,
    float c,
    float d,

    // output
    float *e,
    float *f
) {
    float x = (a + b) * c;
    float y = (a + b) * d;
    *e = x;
    *f = y;
}