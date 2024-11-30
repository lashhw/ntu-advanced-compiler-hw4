void rotation(
    // input constant
    float two,

    // input vector
    float vx, float vy, float vz,

    // input quaternion (normalized)
    float qw, float qx, float qy, float qz,

    // output
    float *rx, float *ry, float *rz
) {
    // Compute quaternion products
    float t1 = qw * qx;
    float t2 = qy * qz;
    float t3 = qw * qy;
    float t4 = qx * qz;
    float t5 = qw * qz;
    float t6 = qx * qy;
    
    // Compute intermediate terms
    float p1 = qw * qw;
    float p2 = qx * qx;
    float p3 = qy * qy;
    float p4 = qz * qz;
    
    // Compute rotation matrix
    float m11 = p1 + p2 - p3 - p4;
    float m12 = two * (t6 - t1);
    float m13 = two * (t4 + t3);
    float m21 = two * (t6 + t1);
    float m22 = p1 - p2 + p3 - p4;
    float m23 = two * (t2 - t5);
    float m31 = two * (t4 - t3);
    float m32 = two * (t2 + t5);
    float m33 = p1 - p2 - p3 + p4;
    
    // Apply rotation
    *rx = m11 * vx + m12 * vy + m13 * vz;
    *ry = m21 * vx + m22 * vy + m23 * vz;
    *rz = m31 * vx + m32 * vy + m33 * vz;
}