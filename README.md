# Major Features


* Software renderer with a structure based on modern, programmable hardware pipelines
* Software pipeline supports runtime dynamic inputs and outputs
* Allows for user-designed shaders and types
* Also renders in Direct3D and OpenGL, and supports hot swapping between rendering modes

<br />
<br />

# Details

The core of the renderer is a C++ implementation of a 3D rendering system modeled on the Direct3D 10/11 pipeline.  Essentially, it is a simulation in software of a modern, programmable 3D graphics hardware pipeline.

Following this paradigm, the centerpiece is a runtime dynamic/polymorphic pipeline that allows bindable shaders taking heterogeneous inputs and outputs to interoperate in a manner similar to a modern programmable hardware pipeline.  A user has a great deal of flexibility in shader input and output types and shader behavior. The current build supports the classic D3D10 shader types: vertex, geometry, and pixel.  

The general structure of the front end is Scenes that demonstrate particular (D3D-style) Effects that are composed of Shaders and a Technique.  For example, one key demo scene is a textured object scene utilizing an effect with appropriate shaders.

The software pipeline performs, broadly speaking, all the same functions the hardware pipelines do: it assembles inputs; it runs the shader stages and translates data among them; it clips and culls; it performs the necessary transformations through various coordinate spaces (model/world -> homogeneous/clip -> NDC -> screen); it rasterizes the results to a back buffer.  The current default rasterizer is of the contemporary interpolating scanline variety; an edge function rasterizer is due to be added as alternative soon.

The software renderer also allows realtime change of winding direction and culling state.

While the software renderer is the core of the project, another important design decision was to incorporate alternative rendering modes.  Scenes can be rendered via the software pipeline or straight Direct3D or OpenGL.  (Note that in the current build, this feature is only supported by the texture mapped scene.)

On the back end, the software renderer itself writes to a buffer that is then flipped to ether Direct3D or OpenGL for rendering (rather than, say, blitting via Win32).  Because by its very nature the software render does not operate at a high frame rate--it _is_ a simulation/model of a modern hardware pipeline rather than an attempt at a production software rendering layer--this helps keep the floor a little higher.  It is thus the case that there are four distinct rendering states in the application: Software via D3D (the default), Software via OGL, and Direct3D and OpenGL themselves.

<br />

## Keyboard Control Guide

### Renderer Features

1 - Software render mode

2 - Direct3D render mode

3 - OpenGL render mode

4 - Sets software renderer backend to Direct3D

5 - Sets software renderer backend to OpenGL

6 - Clockwise winding

7 - Counter-clockwise winding

8 - Back face culling

9 - Front face culling

0 - Front and back face culling

Right Arrow - Next Scene

Left Arrow - Previous Scene

<br />

### Scene Controls

#### Object Controls

* Q and A rotate the object about the X axis

* W and S rotate the object about the Y axis

* F and R translate the object along the Z axis

* E and D rotate the object about the Z axis

<br />

#### Directional Lights Controls

* I and J rotate the light about the X axis

* I and K rotate the light about the Y axis

* O and L rotate the light about the Z axis
