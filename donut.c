#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define PI 3.1416

#define screen_width 40
#define screen_height 80 

const float theta_spacing = 0.07;
const float phi_spacing = 0.02;

/*
 * Here R1 is radius of torus cross-section, which is a circle
 * R2 is the distance of the center of the torus to center of the circle of cross-section
 * K2 is the distance of the donut from the viewer
 * K1 is Scale factor
 */

const float R1 = 0.5;
const float R2 = 1;
const float K2 = 5;
const float K1 = screen_width*K2*3/(8*(R1+R2));

void render_frame(float A, float B) {
	float cosA = cos(A), sinA = sin(A);
	float cosB = cos(B), sinB = sin(B);
/*
 * Output will contain our character buffer of generated points on the torus
 * zbuffer will keep track of any character being placed on top of another
 */
	char output[screen_width*screen_height];
	float zbuffer[screen_width*screen_height];

    for (int i=0; i<screen_width; i++) {
        for (int j=0; j<screen_height; j++) {
            output[i*screen_height + j] = ' ';
            zbuffer[i*screen_height + j] = 0;
        }
    }
/*
 * Theta will generate the cross-section,
 * Phi will rotate the cross-section to make it 3D using 3 dimensional rotation matrix (around y axis)
 * The other two angles A and B are for rotating the torus around x and z axis, with the help of rotation matrix
 */
	for (float theta=0; theta<2*PI; theta+=theta_spacing) {
		float costheta=cos(theta), sintheta=sin(theta);

		for (float phi=0; phi<2*PI; phi+=phi_spacing) {
			float cosphi = cos(phi), sinphi = sin(phi);


			float circlex = R2 + R1*costheta;
			float circley = R1*sintheta;
// Here x, y and z are points on the surface of the torus after calculating the matrix multiplication of rotation
			float x = circlex*(cosB*cosphi + sinA*sinB*sinphi) - circley*cosA*sinB;
			float y = circlex*(sinB*cosphi - sinA*cosB*sinphi) + circley*cosA*cosB;
			float z = K2 + cosA*circlex*sinphi + circley*sinA;
			float ooz = 1/z; // one over z

			int xp = (int) (screen_width/2 + K1*ooz*x);
			int yp = (int) (screen_height/2 - K1*ooz*y);
			
/*
 * Here the luminance is calculated against a light in the direction [0, 1, -1]
 * which is above the viewer and behind screen
 * Luminance is achieved by the dot product of surface normal at that point and light direction
 * since surface normal is a unit vector and the light direction vector has a magnitude of sqrt(2)
 * we can see that the dot product will have a value between 0 and sqrt(2)
 *
 * floor(8 * sqrt(2)) = 11
 * Here we take a string of ascii characters of length 12 (0... 11)
 * they are sorted according the increasing luminance value
 * luminance > 0 means we have bright point facing the light
 * luminance < 0 means we have a dark point away from the light
 */
			float L = cosphi*costheta*sinB - cosA*costheta*sinphi - sinA*sintheta + cosB*(cosA*sintheta - costheta*sinA*sinphi);
			
/* 
 * Here we check the depth using z^-1,
 * z^-1=0 corresponds to z->inf, meaning infinite depth
 * z^-1=large value, corresponds to z->0 which means the point is closer to the viewer
 * if the point is closer to the viewer, we will override the previous output buffer at this point
 */
			if (L > 0) {
				if (ooz > zbuffer[screen_height*xp + yp]) {
                    zbuffer[screen_height*xp + yp] = ooz;
					int luminance_index = L*8;
					output[screen_height*xp + yp] = ".,-~:;=!*#$@"[luminance_index];
				}
			}
		}
	}
// ASCII escape to bring the cursor to home, 
// It is essentially H() function
	printf("\x1b[H");
	for (int i=0; i<screen_width; i++) {
		for (int j=0; j<screen_height; j++) {
			putchar(output[screen_height*i + j]);
		}
		putchar('\n');
	}
}

int main() {
    float A=0, B=0;
    while (1) {
	    render_frame(A+=0.001, B+=0.001);
    }
	return 0;
}
