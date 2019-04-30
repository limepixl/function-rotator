# function-rotator
A program that visualizes a given function in 2D and has the ability to rotate it along an axis, to generate a 3D mesh.

### Controls
- **Space** - Toggle the mesh view
- **R** - Toggle automatic rotation
- **P** - Toggle projection mode (perspective vs orthographic) 
- **Left Mouse Button** - Click and drag to rotate manually
- **Scroll Wheel** - Zoom in and out

### Dependencies:
* [GLEW](https://github.com/nigels-com/glew)
* [GLFW](https://github.com/glfw/glfw)
* [glm](https://github.com/g-truc/glm)

### Images
![sin(x)](https://github.com/limepixl/function-rotator/blob/master/img/sinx.png)
![x*x*x](https://github.com/limepixl/function-rotator/blob/master/img/x*x*x.png)

### Todo
* Indexed rendering for 3D mesh
* Choice between axis of rotation
* (Optional) Function parsing instead of hardcoded function
