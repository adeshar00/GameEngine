
#About

This is a C library I'm working on, which simplifies the process of making 3D games with OpenGL and SDL.  Currently, it can open a window, display 3D graphics, detect user input, and import models (only those made with Blender and saved with the [Blender add-on](https://github.com/adeshar00/BlenderAddon) I made).  It can be called from Python, which allows for rapid prototyping and tool development.   A lot of the code for this was taken from the code for [Skygunner](https://github.com/adeshar00/Skygunner).


#To Use

The library requires OpenGL ES 2.0, SDL2, and GLEW.  On Debian systems these can be downloaded with the following commands:

```
sudo apt-get install libgles2-mesa-dev
sudo apt-get install libglew-dev
sudo apt-get install libsdl2-dev
```

If using C, include gen.h.  If Python, import gen.py.

In the "examples" folder there is source code (in both C and Python) for an extremely simple application, which lets you move a model around with the arrow keys.  
