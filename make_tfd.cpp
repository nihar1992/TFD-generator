#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * make_tfd.cpp
 * 
 * Objective:
 * Aim of this program is create a TFD file which stores the data for
 * points on the boundary and the volume of a 3D CAD model and 
 * corresponding mesh sizes
 * eg. x y z s
 * This means that the mesh size at point (x y z) inside the CAD model is s
 * This file is used by the mesher to create a volume mesh for the geometry
 *
 * Method:
 * Create a 3D array of size bx*by*bz where bx, by and bz are number of points
 * in x, y and z directions respectively
 * Assign mesh sizes to these points according to user input
 * At some points in the mesh there will be a sudden transition between the mesh
 * sizes which is bad
 * A maximum gradient limit is enforced between two neighboring points to ensure
 * that the mesh is smooth
 * eg. If neighboring points have mesh size 2 and 3 and the maximum gradient limit
 * is 1.1 the mesh size of the second element will be reduced to 2.2
 * The condition that the gradient for no element exceeds the maximum value is 
 * checked for multiple iterations till it is satisfied for every element 
 */

// Mesh grid class
class grid
{
public:
	double size; //mesh size
};

int main()
{
	FILE *fp;
	int i,j,k,bx,by,bz;
	int state=0;

	// Grid sizes in x,y and z direction
	bx=50;
	by=100;
	bz=100;

	// Maximum gradient limit
	double grad_max = 1.1;

	grid ***mesh=new grid**[bz+1];

	// Create a 3D array for the mesh grid
	for(i=0;i<bz+1;i++)
	{
		mesh[i]=new grid*[by+1];
		for(j=0;j<by+1;j++)
		{
			mesh[i][j]=new grid[bx+1];
		}
	}

	// Set maximum & minimum limits
	double minx,miny,minz,maxx,maxy,maxz;
	minx = -0.150;
	maxx = 0.150;
	miny = -0.150;
	maxy = 0.445;
	minz = -0.300;
	maxz = 0.275;

	double x,y,z,rad1,rad2;

	// Mesh sizing
	double mesh_c1, mesh_c2, mesh_outer;
	mesh_c1 = 0.0006;
	mesh_c2 = 0.0012;
	mesh_outer = 0.0024;

	fp=fopen("flow_bench_fine.tfd","w");
	fprintf(fp,"GRD1\n");
	fprintf(fp,"%d %d %d\n",bx,by,bz);
	fprintf(fp,"%lf %lf %lf %lf %lf %lf\n",minx,miny,minz,maxx,maxy,maxz);

	for(i=0;i<bz+1;i++)
	{
		for(j=0;j<by+1;j++)
		{
			for(k=0;k<bx+1;k++)
			{
				mesh[i][j][k].size=mesh_outer;
			}
		}
	}

	for(i=0;i<bz+1;i++)
	{
		for(j=0;j<by+1;j++)
		{
			for(k=0;k<bx+1;k++)
			{
				x = minx + (maxx - minx)*k/bx;
				y = miny + (maxy - miny)*j/by;
				z = minz + (maxz - minz)*i/bz;
				rad1 = sqrt(x*x + y*y + (z-0.015)*(z-0.015));
				rad2 = sqrt(x*x + y*y + (z-0.030)*(z-0.030));
				if(rad1 <= 0.070)
				{
					mesh[i][j][k].size=mesh_c1;
				}
				else if((rad1 > 0.070) && (rad2 <= 0.170))
				{
					mesh[i][j][k].size=mesh_c2;;
				}
			}
		}
	}

	//Gradient limit - to smooth the mesh
	while(state==0)
	{
		int state2=0;
		for(i=1;i<bz;i++)
		{
			for(j=1;j<by;j++)
			{
				for(k=1;k<bx;k++)
				{
					if(mesh[i+1][j][k].size/mesh[i][j][k].size>grad_max)
					{
						mesh[i+1][j][k].size=mesh[i][j][k].size*grad_max;
						// If there is at least one element which fails 
						// this criteria state2 will equal 1
						state2=1;
					}
					if(mesh[i-1][j][k].size/mesh[i][j][k].size>grad_max)
					{
						mesh[i-1][j][k].size=mesh[i][j][k].size*grad_max;
						state2=1;
					}
					if(mesh[i][j+1][k].size/mesh[i][j][k].size>grad_max)
					{
						mesh[i][j+1][k].size=mesh[i][j][k].size*grad_max;
						state2=1;
					}
					if(mesh[i][j-1][k].size/mesh[i][j][k].size>grad_max)
					{
						mesh[i][j-1][k].size=mesh[i][j][k].size*grad_max;
						state2=1;
					}
					if(mesh[i][j][k+1].size/mesh[i][j][k].size>grad_max)
					{
						mesh[i][j][k+1].size=mesh[i][j][k].size*grad_max;
						state2=1;
					}
					if(mesh[i][j][k-1].size/mesh[i][j][k].size>grad_max)
					{
						mesh[i][j][k-1].size=mesh[i][j][k].size*grad_max;
						state2=1;
					}
				}	
			}
		}
		if(state2==0)
		{
			// Makes sure no element fails the maximum gradient criteria
			state=1;
		}
	}

	//Write to tfd
	for(i=0;i<bz+1;i++)
	{
		for(j=0;j<by+1;j++)
		{
			for(k=0;k<bx+1;k++)
			{
				fprintf(fp,"%lf\n",mesh[i][j][k].size);
			}
		}
	}
	fclose(fp);
}