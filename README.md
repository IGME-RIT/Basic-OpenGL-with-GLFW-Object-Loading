# Basic OpenGL with GLFW: Texturing a Cube

This program introduces basic concepts involved in writing an OpenGL program.

# About

Materials are commonly used within graphics programs to manage data associated with rendering.
With materials it can become easier to draw many different objects.
This example demonstrates a basic implementation of a material class that owns a shader program and textures.
This example also demonstrates the use of reference counting to keep track of textures and shaders.

# Setup

You will need to have CMake installed on your computer, and properly added to your path.
In order to setup, run the following in a shell, then open the project in your preferred editor.
Windows setup has been configured for use with Visual Studio.

Windows:
```
cd path/to/folder
setup.cmd
```
Linux:
```
cd path/to/folder
./setup
```
