#define WIDTH 1920
#define HEIGHT 1080
#define DEPTH 3

#include<math.h>
#include<stdio.h>

enum color_band {R, G, B};

const size_t FRAME_SZ = WIDTH * HEIGHT;
const size_t TOTAL_SZ = DEPTH * WIDTH * HEIGHT;
const char *FILE_NAME = "image.ppm";
const unsigned char MAX_COLOR = 255;

float distance(float p1[], float p2[]);

int
main(int argc, char *argv[])
{
	FILE *file_pointer = fopen(FILE_NAME, "wb");
	unsigned char *pxs = malloc(TOTAL_SZ);
	float center[2] = {WIDTH / 2, HEIGHT / 2};

	for (int h = 0 ; h < HEIGHT; ++h) {
		for (int w = 0; w < WIDTH; ++w) {
			for (int d = 0; d < DEPTH; ++d) {
				unsigned char px;
				float point[2] = {w, h}; 		
				float dist = distance(point, center);
				float circular_gradient = sin((1.0 - dist / HEIGHT) * 50);
				
				float adj = center[0] - point[0];
				float adj_over_hyp = adj / dist;
				switch (d) {
					case R:
						px = 0.5 * circular_gradient * MAX_COLOR;
						break;
					case G:
						px = circular_gradient * MAX_COLOR;
						break;
					case B:
						px = 0.5 * sin(circular_gradient * 100) * MAX_COLOR;
						break;
					default:
						px = 0;
				}
				int resolution = 1000;
				float ang = acos(adj_over_hyp);
				ang *= (float) resolution;
				int ang_int = (int) ang;
				ang_int %= (resolution / 2);
				ang = ((float) ang_int) / (float) resolution;
				px -= 200 * cos(ang * 10);
				
				pxs[(h * WIDTH * DEPTH) + (w * DEPTH) + d] = px;
			}
		}
	}
	
	fprintf(file_pointer, "P6 %d %d %d\n", WIDTH, HEIGHT, MAX_COLOR);
	fwrite(pxs, TOTAL_SZ, 1, file_pointer);
	fclose(file_pointer);
}

float
distance(float p1[], float p2[])
{
	return sqrt(pow(p2[0] - p1[0], 2) + pow(p2[1] - p1[1], 2));
}