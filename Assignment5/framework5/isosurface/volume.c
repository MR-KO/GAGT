/* Computer Graphics, Assignment, Volume rendering with cubes/points/isosurface
 *
 * Student name .... Timo Dobber & Kevin Ouwehand
 * Student email ... tdob3000@gmail.com & c0nd3mn3d@hotmail.com
 * Collegekaart .... 10386726 & 10420908
 * Date ............ Tue March 18
 * Comments ........
 *
 * (always fill in these fields before submitting!!)
 */

#include <stdio.h>
#include <stdlib.h>

#include "volume.h"

/* The voxels of the volume dataset, stored as a one-dimensional array */
unsigned char *volume;

/* The dimensions of the volume dataset */
int nx, ny, nz;

/* The size of a voxel */
float sizex, sizey, sizez;

/* Utility function to convert the index of a voxel
   into an index in the volume array above */
int voxel2idx(int i, int j, int k) {
	return (k*ny + j)*nx + i;
}

/* Extract a cell from the volume, so that datapoint 0 of the
   cell corresponds to voxel (i, j, k), datapoint 1 to voxel (i+1, j, k),
   etc. See the assignment. */
cell get_cell(int i, int j, int k) {
	cell c;

	/* Store the 8 corner positions. */
	c.p[0] = v3_create(i, 		j, 		k);
	c.p[1] = v3_create(i + 1,	j, 		k);
	c.p[2] = v3_create(i, 		j + 1, 	k);
	c.p[3] = v3_create(i + 1, 	j + 1, 	k);
	c.p[4] = v3_create(i, 		j, 		k + 1);
	c.p[5] = v3_create(i + 1, 	j, 		k + 1);
	c.p[6] = v3_create(i, 		j + 1, 	k + 1);
	c.p[7] = v3_create(i + 1, 	j + 1, 	k + 1);

	/* Get their corresponding values. */
	c.value[0] = volume[voxel2idx(i, 		j, 		k)];
	c.value[1] = volume[voxel2idx(i + 1, 	j, 		k)];
	c.value[2] = volume[voxel2idx(i, 		j + 1, 	k)];
	c.value[3] = volume[voxel2idx(i + 1, 	j + 1, 	k)];
	c.value[4] = volume[voxel2idx(i, 		j, 		k + 1)];
	c.value[5] = volume[voxel2idx(i + 1,	j, 		k + 1)];
	c.value[6] = volume[voxel2idx(i, 		j + 1,	k + 1)];
	c.value[7] = volume[voxel2idx(i + 1,	j + 1,	k + 1)];

	/* Calculate their 8 normal vectors, pointing away from the corners. */
	c.n[0] = v3_normalize(v3_subtract(v3_negate(v3_add(v3_add(c.p[1], c.p[2]), c.p[4])), c.p[0]));
	c.n[1] = v3_normalize(v3_subtract(v3_negate(v3_add(v3_add(c.p[0], c.p[3]), c.p[5])), c.p[1]));
	c.n[2] = v3_normalize(v3_subtract(v3_negate(v3_add(v3_add(c.p[0], c.p[3]), c.p[6])), c.p[2]));
	c.n[3] = v3_normalize(v3_subtract(v3_negate(v3_add(v3_add(c.p[1], c.p[2]), c.p[7])), c.p[3]));
	c.n[4] = v3_normalize(v3_subtract(v3_negate(v3_add(v3_add(c.p[0], c.p[5]), c.p[6])), c.p[4]));
	c.n[5] = v3_normalize(v3_subtract(v3_negate(v3_add(v3_add(c.p[1], c.p[4]), c.p[7])), c.p[5]));
	c.n[6] = v3_normalize(v3_subtract(v3_negate(v3_add(v3_add(c.p[2], c.p[4]), c.p[7])), c.p[6]));
	c.n[7] = v3_normalize(v3_subtract(v3_negate(v3_add(v3_add(c.p[3], c.p[5]), c.p[6])), c.p[7]));

	return c;
}

/* Utility function to read a volume dataset from a VTK file.
   This will store the data in the "volume" array and update the dimension
   and size values. */

void read_volume(const char *fname) {
	FILE *f;
	char s[256];
	int nvoxels;

	printf("Reading %s\n", fname);
	f = fopen(fname, "rb");

	if (!f) {
		fprintf(stderr, "read_volume(): Could not open file '%s' for reading!\n", fname);
		exit(-1);
	}

	// header line
	fgets(s, 255, f);

	// comment line
	fgets(s, 255, f);

	// BINARY
	fgets(s, 255, f);

	// DATASET STRUCTURED_POINTS
	fgets(s, 255, f);

	// DIMENSIONS %d %d %d
	fscanf(f, "%s %d %d %d\n", s, &nx, &ny, &nz);
	printf("%d x %d x %d voxels\n", nx, ny, nz);

	// ASPECT_RATIO/SPACING %f %f %f
	fscanf(f, "%s %f %f %f\n", s, &sizex, &sizey, &sizez);
	printf("voxel sizes: %.3f, %.3f, %.3f\n", sizex, sizey, sizez);

	// ORIGIN ...
	fgets(s, 255, f);

	// POINT_DATA ...
	fgets(s, 255, f);

	// SCALARS ...
	fgets(s, 255, f);

	// LOOKUP_TABLE ...
	fgets(s, 255, f);

	// allocate memory to hold the volume data and read it from file
	nvoxels = nx * ny * nz;
	volume = (unsigned char*)malloc(nvoxels);

	if (fread(volume, 1, nvoxels, f) < (size_t)nvoxels) {
		printf("WARNING: not all data could be read!\n");
	}

	fclose(f);
}
