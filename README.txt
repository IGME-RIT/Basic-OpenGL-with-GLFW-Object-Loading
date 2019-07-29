Documentation Author: Niko Procopi 2019

This tutorial was designed for Visual Studio 2017 / 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

Welcome to the OBJ loader Tutorial!
Prerequesites: Draw a cube with a texture and a light

Comparison Screenshot:
Left:   texture
Middle: texture + normal map
Right:  texture + normal map + mipmapping

When to import OBJ:
If you are a beginner, and loading models for the first time
If you are prototyping your engine, and want an easy way to load models

When not to use OBJ
OBJ files do not hold any animation data
If you want to optimize file sizes 
	OBJ ASCII files can be huge in comparison to binary files
When rendering textures or sprites, each texture needs a quad
	This quad shouldn't be OBJ, it should be procedurally generated
	
Where do I get an OBJ?
Build something in Blender, then export to OBJ
On the lower-left corner, uncheck all options, then check the following:
-Triangulate faces
-Write UVs
-Include Normals
	
How OBJ works:
You can open OBJ and read it like any other file, it is all text-based (ASCII)
Here is a sample of what an OBJ file looks like:
v -4.000000 0.000000 4.000000
v 4.000000 0.000000 4.000000
v -4.000000 0.000000 -4.000000
...
vt 0.032113 0.009619
vt 0.940815 0.986474
vt 0.032113 0.986474
...
f 2/1/1 3/2/1 1/3/1
f 6/4/2 7/5/2 5/6/2
f 8/7/3 11/8/3 7/9/3
...
A "vertex" is a combination of a position, a texture coordinate, and a normal
The confusing thing about OBJ is that it uses the same word for multiple things
'v' looks like it should be "vertex" but it is not a "vertex", 
'v' is a "vertex position". Its stupid, I know. It should've been 'vp' instead of 'v',
but I did not write the OBJ format, so we deal with it.
The three numbers after the 'v' are X,Y,Z positions for that one vertex.
The data after 'vt' is a Texture Coordinate, giving an X and Y position for a pixel
on a texture. The data after 'vn' is the Vertex Normal. A Vertex Normal is the
direction that the polygon faces. "Normal" does not mean "ordinary". "Normal" is the 
perpendicular vector from the face it connects to. If one point is attached to multiple 
polygons, then there will be multiple vertices at that point, each with its own Normal.
The data after 'f' is a face, and a face is made of 3 vertices
After the first 'f', you see:
2/1/1 3/2/1 1/3/1
Lets  break that down. Each face has three vertices
One vertex is 2/1/1
One vertex is 3/2/1
One vertex is 1/3/1
And those three vertices connect to make a face
But what the heck is 2/1/1?
To make it easier, lets use 21/35/12 (randomly chosen)
The 21 means 21st position, or the 21st 'v' in the file
The 35 means 35th texture coordinate, or the 35th 'vt' in the file
The 12 means 12th normal, or the 12th 'vn' in the file
That makes one vertex 

Common mistake:
In OBJ, '1' means the first 'v', 'vn', or 'vt' in the file
in C++, '0' means the first element of an array
So when you read x/y/z after 'f', subtract '1' from x, y, and z

When you're done, you should have an array of Vertex, which you can then turn into about
Vertex Buffer, then you can draw the object, like any other triangle or square
The object will draw, it will be unoptimized
The vertex buffer will have duplicates of "Vertex", which is a waste of processing
because every "Vertex" has to be calculated in the vertex shader
We can use an index buffer to reuse a "Vertex" that has already been calculated

This tutorial uses an optimized index buffer, which decreases the size of the vertex buffer
Other tutorials use fake index buffers (0,1,2,3...) which are useless
By reading comments in the code, you can see how it works

Problem with this loader:
If you try to load "kitten.obj" it will take an hour.
This loader checks to see if a vertex already exists before adding
it to the array of vertices, otherwise there will be duplicate 
vertices in the Vertex Buffer, and the frame rate will drop.
Alternatively, the Assimp library can remove duplicate vertices
in less than a second.

What not to do:
Do not remove the checks for duplicate vertices in the OBJ loader.
Doing so will make the loading very fast, however, in a full 3D game, 
duplicate vertices can hurt frame rate

How to improve:
If you want to improve loading, try Assimp tutorial
If you want to build an environment, try loading multiple objects
If you want to improve graphics, try normal mapping
If you want to try animation, try loading FBX files
If you want to try binary files (rather than ASCII), try FBX
