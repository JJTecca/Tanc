Technologies and Libraries Used:
  -OpenGL (fixed-function pipeline)
  -GLU (OpenGL Utility Library)
  -AUX (Auxiliary toolkit for window and texture handling)
  -C programming language

Key Features and Components:
3D Vehicle Model
	-The main body of the vehicle is constructed using GL_QUADS, representing various parts such as the chassis, hood, and roof.

	-The vehicle’s components are color-coded or texture-mapped to simulate different materials.

Texturing

	-Textures are loaded using the auxDIBImageLoad function from the AUX library.

	-Textures are applied to vehicle surfaces, wheels, and other 3D objects to enhance realism.

Wheels

	-The front and back wheels are modeled using gluCylinder and gluDisk functions from the GLU library.

	-Each wheel is rendered with proper rotation and placement using glPushMatrix, glTranslatef, and glRotatef.

Bézier Surfaces

	-Bézier patches are created using glMap2f, glEnable(GL_MAP2_VERTEX_3), and glEvalCoord2f.

	-These patches are used to model smooth, curved surfaces such as a bell or custom vehicle features.

Lighting and Materials

	-Basic lighting is implemented using GL_LIGHTING, GL_LIGHT0, and GL_COLOR_MATERIAL.

	-Materials are defined to reflect light realistically on various surfaces.

3D Transformations and Controls

	-User-controlled transformations include translation, rotation, and zoom via keyboard input.

	-Functions like rotateLeft, rotateRight, MutaDreapta, and MutaStanga manipulate the camera and object positions.

Custom Bitmap Font Rendering

	-A custom font is rendered using glBitmap and GLubyte arrays representing the letters in the name "MAIOR CRISTIAN ALEXANDRU".

	-Each letter is defined as a 12x16 bitmap and displayed using glRasterPos2f.

Scene Composition

	-The complete scene combines all these elements: a textured 3D vehicle, Bézier-modeled structures, animated wheels, a labeled nameplate, and interactive transformations.
