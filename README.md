# Computer Graphics Engine — Solar System

[![C++](https://img.shields.io/badge/C%2B%2B-OpenGL-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/build-CMake-064F8C.svg)](https://cmake.org/)
[![OpenGL](https://img.shields.io/badge/rendering-OpenGL-red.svg)](https://www.opengl.org/)
[![GLUT](https://img.shields.io/badge/windowing-GLUT-orange.svg)](https://freeglut.sourceforge.net/)
[![GLEW](https://img.shields.io/badge/extensions-GLEW-4B8BBE.svg)](https://glew.sourceforge.net/)
[![TinyXML](https://img.shields.io/badge/XML-TinyXML-green.svg)](https://sourceforge.net/projects/tinyxml/)

## 1. Project overview

This project implements a small real-time 3D graphics pipeline for the Computer Graphics
course. The problem is to represent geometric primitives and hierarchical scenes in a
portable, editable format, then render those scenes with a controllable camera, animation,
lighting and materials.

The solution is split into two native C++ programs:

- `generator` creates triangulated `.3d` meshes from primitive parameters or Bézier patch
  files.
- `engine` parses a scene XML file, loads the generated meshes and renders the scene with
  OpenGL.

The repository contains the incremental implementations delivered in `Phase1` through
`Phase4`. `Phase4` is the latest implementation and includes the complete feature set.

> **Final grade: 17/20**

## 2. Features and technical architecture

### Geometry generation

The generator currently supports:

- Planes: `plane <length> <divisions> <output.3d>`
- Rings: `ring <inner-radius> <outer-radius> <slices> <output.3d>`
- Cones: `cone <radius> <height> <slices> <stacks> <output.3d>`
- Spheres: `sphere <radius> <slices> <stacks> <output.3d>`
- Boxes: `box <length> <grid> <output.3d>`
- Bézier patches: `patch <patch-file> <tessellation> <output.3d>`

Meshes contain triangle vertices and the data required by the renderer, including normals
and texture coordinates where applicable.

### Scene engine

Scenes are described by XML files rooted at `<world>`. The parser supports:

- Window and perspective-camera configuration (`position`, `lookAt`, `up`, `fov`, `near`,
  and `far`).
- Hierarchical groups with nested subgroups.
- Static translation, rotation and scaling.
- Time-based rotations and Catmull–Rom translations, with optional tangent alignment and
  orbit display.
- Point, directional and spot lights.
- Per-model diffuse, ambient, specular and emissive colours, shininess and textures.
- Vertex, normal and texture-coordinate buffers rendered as OpenGL triangles.

The code is organised by responsibility:

- `gen/`: primitive and Bézier-patch mesh generation.
- `data_structs/`: scene, model, transform, colour, light and matrix data structures,
  including XML scene parsing.
- `engine/`: OpenGL window creation, camera/input handling, lighting and rendering.
- `TinyXML/`: the XML parser used by the scene loader.
- `test_files/`: course test scenes and Bézier patch inputs.
- `3d/`: sample generated meshes used by the XML scenes.

## 3. Installation and execution

### Requirements

Install the following before building:

- C++ compiler with C++ support.
- CMake 3.10 or newer.
- OpenGL development libraries.
- GLUT/freeglut, GLEW and DevIL development libraries.

On Linux, the `Phase4/outputs/CMakeLists.txt` file locates these through CMake packages
(`OpenGL`, `GLUT`, `GLEW` and `DevIL`). On macOS, the project also expects the relevant
OpenGL/GLUT and DevIL libraries available to CMake. On Windows, the CMake file expects
the GLUT, GLEW and DevIL headers, libraries and DLLs under
`Phase4/toolkits/toolkits`.

### Build the latest phase

The CMake project for each phase is intentionally located in its `outputs` directory.
For the latest implementation:

```bash
cd Phase4/outputs
cmake -S . -B .
cmake --build .
```

The build produces the `generator` and `engine` executables in the build directory.
To build an earlier submission, replace `Phase4` in the commands with `Phase1`, `Phase2`
or `Phase3`.

### Generate a mesh

Run the generator from `Phase4/outputs`. The output filename is written to `Phase4/3d`
by the implementation, so the filename must match the one referenced by the scene XML.

```bash
./generator plane 2 3 plane_2_3.3d
./generator ring 0.5 1.0 32 ring.3d
./generator cone 1 2 32 16 cone.3d
./generator sphere 1 32 16 sphere.3d
./generator box 2 3 box.3d
./generator patch ../test_files/teapot.patch 10 teapot.3d
```

The accepted command-line forms are:

```text
./generator plane  <length> <divisions> <file.3d>
./generator ring   <inner-radius> <outer-radius> <slices> <file.3d>
./generator cone   <radius> <height> <slices> <stacks> <file.3d>
./generator sphere <radius> <slices> <stacks> <file.3d>
./generator box    <length> <grid> <file.3d>
./generator patch  <patch-file> <tessellation> <file.3d>
```

### Run a scene

The engine receives one XML scene path. From `Phase4/outputs`, use one of the bundled
test scenes:

```bash
./engine ../test_files/test_files_phase_1/test_1_1.xml
./engine ../test_files/test_files_phase_4/test_4_6.xml
./engine ../demo_scenes/solar_system.xml
```

The sample scenes reference meshes in `Phase4/3d`, which is the path resolved by the
loader when the engine is started from `Phase4/outputs`.

### Runtime controls

While the window is focused:

| Input | Action |
| --- | --- |
| `W` / `S` | Move the camera forward / backward |
| `A` / `D` | Strafe the camera left / right |
| `+` / `-` | Move the camera up / down |
| Left mouse button + drag | Look around |
| `M` | Cycle fill, wireframe and point rendering |
| `P` | Toggle coordinate axes |
| `C` | Toggle Catmull–Rom orbit curves |

## 4. Authorship

This repository was developed collaboratively by students as part of the Computer
Graphics coursework in the Bachelor's programme at the University of Minho
(UMinho). The repository preserves the progressive deliverables from the course phases,
including the final implementation and the original course test assets.
