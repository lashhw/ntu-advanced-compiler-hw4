void copy_prop(
    // input
    float a,
    float b,

    // output
    float *c
) {
    float x = a + b;
    float y = x;  // intermediate copy
    float z = y;  // intermediate copy
    *c = z;
}