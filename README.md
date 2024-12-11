## Procedural Prism Generation using OpenGL

This project is a simple implementation of procedural generation by which a prism of given parameter is generated and rendered using OpenGL. Each edge of the generated prism is assigned a unique colour to make it easier to distinguish between the vertices and edges. Additional features implemented include camera movement, object translation and rotation, and transformation of the generated prism into a pyramid.

### Dependencies

- OpenGL
- GLFW
- GLAD

### Compiling

To compile the project, run the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

### Running

The program can be run by executing the following command:

```bash
./build/render <num_sides>
```

where `<num_sides>` is the number of sides of the prism to be generated. Any integer value greater than or equal to 3 can be used.

### Controls

Camera Movement:

- 'W' - Move the camera forward
- 'S' - Move the camera backward
- 'A' - Move the camera left
- 'D' - Move the camera right
- 'Q' - Move the camera up
- 'E' - Move the camera down

Object Translation and Rotation:

- 'I' - Move the object forward
- 'K' - Move the object backward
- 'J' - Move the object left
- 'L' - Move the object right
- 'U' - Move the object up
- 'O' - Move the object down
- 'R' - Rotate the object clockwise

Shape Conversion:

- 'T' - Transform the prism into a pyramid

Close Program:

- 'Esc' - Exit the program