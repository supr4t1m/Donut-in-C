#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "matrix_operation.h"

#define PI M_PI

#define screen_width 40
#define screen_height 80

float light_direction[] = { 0, 1, -1 };
// horizontal and vertical spreads of the rectangular cross-section
// (in positive direction)
const float H = 1;
const float V = 1;
const float K2 = 10;
// the maximum width the rectangle can have on the screen should be screen_width*3/8
// screen_width*3/8 = K1*H/(K2+0)
const float K1 = K2*screen_width*3/(12*H);

void render_cube_frame(float phix, float phiy, float phiz) {
    float sinphix = sin(phix), cosphix = cos(phix);
    float sinphiy = sin(phiy), cosphiy = cos(phiy);
    float sinphiz = sin(phiz), cosphiz = cos(phiz);

    char output[screen_width*screen_height];
    float zbuffer[screen_width*screen_height];
    float normal[3];

    for (int i=0; i<screen_width; i++)
        for (int j=0; j<screen_height; j++) { 
            output[i*screen_height + j] = ' ';
            zbuffer[i*screen_height + j] = 0;
        }

    float rotx[][3] = {{ 1, 0, 0 },
            { 0, cosphix, -sinphix},
            { 0, sinphix, cosphix}};

    float roty[][3] = {{ cosphiy, 0, sinphiy },
                       { 0, 1, 0},
                       { -sinphiy, 0, cosphiy }};

    float rotz[][3] = {{ cosphiz, -sinphiz, 0 },
                       { sinphiz, cosphiz, 0 },
                       { 0, 0, 1}};

    for (float h=-H; h<H; h+=0.001) {
        for (float v=-V; v<V; v+=0.001) {
            float x = 0;
            float y = 0;
            float z = 0;

            for (int side=0; side<6; side++) { // [0, 0, 1]
                if (side==0) {
                    x = h;
                    y = v;
                    z = 1;
                    normal[0] = 0; normal[1] = 0; normal[2] = 1;
                } else if (side == 1) { // [0, 0, -1]
                    x = h;
                    y = v;
                    z = -1;
                    normal[0] = 0; normal[1] = 0; normal[2] = -1;
                } else if (side == 2) { // [1, 0, 0]
                    x = 1;
                    y = v;
                    z = h;
                    normal[0] = 1; normal[1] = 0; normal[2] = 0;
                } else if (side == 3) { // [-1, 0, 0]
                    x = -1;
                    y = v;
                    z = h;
                    normal[0] = -1; normal[0] = 0; normal[2] = 0;
                } else if (side == 4) { // [0, 1, 0]
                    x = h;
                    y = 1;
                    z = v;
                    normal[0] = 0; normal[1] = 1; normal[2] = 0;
                } else { // [0, -1, 0]
                    x = h;
                    y = -1;
                    z = v;
                    normal[0] = 0; normal[1] = -1; normal[2] = 0;
                }

                float point[] = { x, y, z };
                rotate(&point, rotx);
                rotate(&point, roty);
                rotate(&point, rotz);

                rotate(&normal, rotx);
                rotate(&normal, roty);
                rotate(&normal, rotz);
                point[2] += K2;
                float ooz = 1/point[2];
                
                int xp = (int) (screen_width/2 + K1*ooz*point[0]);
                int yp = (int) (screen_height/2 - K1*ooz*point[1]);

                float L = luminance(normal, light_direction);

                if (L > 0) {
                    if (ooz > zbuffer[xp*screen_height + yp]) {
                        zbuffer[xp*screen_height + yp] = ooz;
                        
                        int luminance_index = L*8;
                        output[xp*screen_height + yp] = ".,-~:;=!*#$@"[luminance_index];
                    }
                } else {
                	if (ooz > zbuffer[xp*screen_height + yp]) {
                		zbuffer[xp*screen_height + yp] = ooz;
                		output[xp*screen_height + yp] = ' ';
                	}
                }
            }

        }
    }

    printf("\x1b[H");
    for (int i=0; i<screen_width; i++) {
        for (int j=0; j<screen_height; j++) {
            putchar(output[i*screen_height + j]);
            putchar(' ');
        }
        putchar('\n');
    }
}

int main() {

    float A = 0, B = 0, C = 0;

    while(1) 
        render_cube_frame(A, B+=0.3, C+=0.3);

    return 0;
}
