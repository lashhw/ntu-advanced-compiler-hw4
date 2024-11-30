void ray_trig(
    // input
    float one,
    float o_x,
    float o_y,
    float o_z,
    float d_x,
    float d_y,
    float d_z,
    float p0_x,
    float p0_y,
    float p0_z,
    float e1_x,
    float e1_y,
    float e1_z,
    float e2_x,
    float e2_y,
    float e2_z,

    // output
    float *t,
    float *u,
    float *v
) {
    float n_x = e1_y * e2_z - e1_z * e2_y;
    float n_y = e1_z * e2_x - e1_x * e2_z;
    float n_z = e1_x * e2_y - e1_y * e2_x;

    float c_x = p0_x - o_x;
    float c_y = p0_y - o_y;
    float c_z = p0_z - o_z;

    float r_x = d_y * c_z - d_z * c_y;
    float r_y = d_z * c_x - d_x * c_z;
    float r_z = d_x * c_y - d_y * c_x;

    float inv_det = one / (d_x * n_x + d_y * n_y + d_z * n_z);

    *t = inv_det * (c_x * n_x + c_y * n_y + c_z * n_z);
    *u = inv_det * (e2_x * r_x + e2_y * r_y + e2_z * r_z);
    *v = inv_det * (e1_x * r_x + e1_y * r_y + e1_z * r_z);
}