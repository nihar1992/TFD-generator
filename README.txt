################################
# Carnegie Mellon University
# M.S. Research
# Automatic Hex-Dominant meshing
# for complex flow problems
################################

This program generates a TFD file, which is used by the bubble mesher developed at CERLAB, CMU, to generate meshes for 3D geometries.
The TFD file is a plain text file which stores the mesh size data at all the points on the boundary and in the volume of 3D CAD model.
Goal of this program is read a CAD file (in .srf format), input mesh sizing and use it to create the TFD file.
This code is a part of my research project, which is being performed under the guidance of Professor Kenji Shimada, Professor Satbir Singh and Doctor Soji Yamakawa, Department of Mechanical Engineering, Carnegie Mellon University.



********************
Running the program
********************
Compile and run the make_tfd.cpp file to generate a TFD file for the workbench.srf geometry. The current version of the program works only for a particular geometry. With a few modifications the user will be able to input the geometry name, geometry dimensions and mesh sizing and the program can make a TFD file for any general geometry. 