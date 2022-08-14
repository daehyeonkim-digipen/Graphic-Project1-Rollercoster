1. How to build the project:

Unzip the file and double click on Chobo Graphic Project.sln.
Set the project setting to Release, x64, and press Ctrl + Shift + B or Build button.
The project build will be done.

2. How to run the project:

Unzip the file and double click on Chobo Graphic Project.sln.
Set the project setting to Release, x64, and press F5 or Local Windows Debugger.
The project demo will run.

Else Unzip the Pre-BuiltFile.7z included, and double click the binary exe file in the Debug folder.

3. How to interact with the project:

When you open the demo, there will be an ImGui window on the top right.
This helps you to choose the scene.

Camera key is WASD, and Q to go up, E to go down.
To change camera direction, click on ImGui Camera window or press Space.
Then Camera Follow Mouse will be true, and now the camera will follow mouse position.
This applies on all scenes except the curve scene.

The first scene you will see is the Model scene.

This scene shows -
Procedural Geometric Modeling,
Toon / Cell Shading,
Fog,
Grometry and Tessellation Shaders

To rotate mesh, you press the mouse button and move the mouse.
Scrolling will also rotate the mesh too.
Make sure Camera Follow Mouse is off.

The ImGui window on the top left has mesh settings.
You can modify mesh's settings with this.
Grid Mode will make a grid with current mesh, so that fog will be more clear.
Clicking on Meshes button will change the mesh on screen.
Reset button will reset everything about meshes.

The ImGui window on the bottom left has shader settings.
There are various geometry and tessellation shaders examples.

The two ImGui windows on bottom right is color modifiers.
Each change the mesh color and the fog color.

The second scene is the Noise Scene.

This scene shows -
Value noise,
Gradient noise / Perlin noise

The ImGui window on the top left has noise settings.
If you press mapping, the noise will be mapped into a plane.
Turning terrace mode on will give cut-off effects on the map.
Anim button will animate the noise.
Reset button will reset everything about noise.

The third scene is the Curve Scene.

This scene shows - 
Hermite Curves / Catmull-Rom Splines

You can add points by clicking on the screen.
Also you can move points and vectors by dragging them.
You can delete the last point by right clicking.

The ImGui window on the top left has curve settings.
The line count is the count of lines drawn between two lines.
Turning Loop on will make the curve loop.
Reset Values will reset all curve settings.
Delete Points will reset all points.

The ImGui window under setting has curve types.
You can change between hermite curve and catmull-rom splines.

The fourth scene is the Shadow Scene.

This scene shows - 
Shadow Mapping

The ImGui window on the top left has Shadow settings.
Turning Light Mode on will set the camera to light frustum.
The movement key is same with the camera.
Light Map will display the depth map created on the screen.
Show Frustum will display the light camera's frustum.
Reset will reset all shadow settings.

The last scene is the Final Scene.

This scene shows - 
Interactive Experience

Right Clicking will make you move faster.

The ImGui window on the top left has final scene settings.
Fog will turn on the fog effect.
Toon will turn on the toon shading effect.
Stop time will stop the time of the scene except the roller coaster.
Time speed affects the time except the roller coaster.

Turning on Rollercoaster will show the roller coaster on the scene.
Coaster Cam will move you to the roller coaster view.
If you turn on Show Points, you can modify the tracks by clicking on the points.
The points will have their position, tangent, and the angle.
You can turn off the track with Show Track.

Reset will reset all scene settings.
Coaster Reset will only reset roller coaster settings.