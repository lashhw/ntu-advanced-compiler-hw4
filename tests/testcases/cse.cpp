void cse(
    // input
    float a,
    float b,
    float c,
    float d,

    // output
    float *e,
    float *f,
    float *g
) {
    float x = (a + b) * (c + d);
    float y = (a + b) * (a + b);
    float z = (c + d) * (a + b);
    *e = x;
    *f = y;
    *g = z;
}
