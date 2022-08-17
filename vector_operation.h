#ifndef VECTOR_OPERATION_H
#define VECTOR_OPERATION_H
#include <math.h>

float modulus(float vec[]) {
    return sqrt(
            vec[0]*vec[0] +
            vec[1]*vec[1] + 
            vec[2]*vec[2]
            );
}

// (* ptr to array)[size_1][size_2]
void rotate(float (*point)[3], float rotation_matrix[][3]) {

    float temp[3] = {0, 0, 0};
// dereference arr ptr first, then dereference the array
    
    for (int j=0; j<3; j++) {
        for (int k=0; k<3; k++) {
            temp[j] += (*point)[k]*rotation_matrix[k][j];
        }
    }

    for (int i=0; i<3; i++) 
        (*point)[i] = temp[i];
}

float luminance(float normal[], float light_direction[]) {
    float l = 0;
    for (int i=0; i<3; i++)
        l += normal[i]*light_direction[i];
    return l/modulus(normal);
}

#endif // VECTOR_OPERATION_H
