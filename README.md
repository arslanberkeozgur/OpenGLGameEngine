# OpenGLGameEngine 

This is a crude game engine in development, being created for educational purposes. It is probably mildly -- if not horribly -- inefficient at some places, and it does not do much *yet*, but it is a learning experience focused mostly on creating a minimal working example.

##  Features
- Basic 3-D shape creation and management, with customizable positions, velocities, rotations and angular velocities.
- Multiple cameras in one scene, either free or following an object.

## Use
In order to use the engine, the user needs to satisfy the usual requirements for using OpenGL. This means that the user needs to include GLEW and GLFW to his/her path. For help, see https://www.youtube.com/watch?v=vGptI11wRxE. After this,the user #includes GameEngine.h and creates a class inheriting publicly from the GLManager. In this class,the user needs to overwrite two public functions :
- **bool OnUserCreate()**
- **bool OnUserUpdate(GLdouble deltaTime)**

**OnUserCreate()** is called once before the first loop of the game. This function is used to generate resources and add inital conditions to variables by the user.  **Any data  that will be used in each loop should be defined in the private data member section of the child class, since this data will be available everywhere in the class.** 

**OnUserCreate(GLdouble deltaTime)** is called on each loop. The user uses this function for per loop calculations. Use deltaTime variable in velocity calculations to achieve system independent movement.

In the main function,the user creates an default instant of the child class, and then calls the Run(width, height) function with desired window width and height to initialize the game.

**Source.cpp in the repo gives a demonstration of implementation of the engine.**

## Game Loop Flowchart
``` mermaid
flowchart TD
st([Start])
op1[[Create shader programs and get IDs for uniform variables]]
op2[[Call OnUserCreate]]
cond1{OnUserCreate returns true}
op3[[Create projection matrix]]
cond2{Window should close}
cond3{Free camera active}
op4[[Update free camera using input]]
op5[[Call OnUserUpdate]]
cond4{OnUserUpdate returns true}
op6[[Render all objects and refresh window.]]
e([Terminate])

st-->op1
op1-->op2
op2-->cond1
cond1-- YES -->op3
cond1-- NO -->e
op3-->cond2
cond2-- NO -->cond3
cond2-- YES -->e
cond3-- YES -->op4
cond3-- NO -->op5
op4-->op5
op5-->cond4
cond4-- YES -->op6
cond4-- NO -->e
op6-->cond2
```

## Files
### GLManager.h
This file defines the engine : everything the user will interact with when making his/her game. Inside, it defines the class GLManager, which the user needs to inherit from when making his/her game.

#### Functions

##### General Functions

- ###### void Run(GLint width, GLint height, GLfloat R, GLfloat G, GLfloat B) : 
The user calls this function with desired window width and height to run the game loop.The user also sets the background colour using **R**, **G**, **B** values.

##### Camera Management Functions
- ###### GLuint CreateCamera(glm::vec3 pos, glm::vec3 up, GLfloat yaw, GLfloat pitch, bool active) :
Create a camera in position **pos**, facing the direction designated by parameters **up**, **yaw** and **pitch**. Set boolean parameter **active** to true if you want this camera to be the active camera right from the beginning. Returns the integer index of the generated camera in the list of cameras of the manager. This index now can be used with GetCamWithIndex (below) to get a handle for the camera.

- ###### void SetActiveCam(Camera* cam) : 
Sets the camera which is being pointed at by parameter **cam** to be active, and other cameras inactive. **Only one camera at a time can be the active camera.**

- ###### Camera* GetCamWithIndex(GLuint index) :
Returns a pointer to the Camera object with an **index** given by the parameter.

- ###### GLuint GetCamCount() : 
Returns the number of Camera objects in the list of cameras of the manager.

##### Object Management Functions 

- ###### GLuint CreateObject(Geometry geometry) :
Creates an object in the position (0.0, 0.0, 0.0), facing towards z direction with up vector pointing at y direction. *(These will be customizable in later patches.)*  Object position, velocity, rotation, angular velocity and scale can only be changed through GLObject functions described below. Returns the index of the object in the list of objects of the manager.

- ###### GLObject* GetObjectWithIndex(GLuint index) : 
Returns the GLObject pointer with **index** given by the parameter.

- ###### void KillObject(GLObject* obj) : 
Kill and remove the object from the list of objects. Since list of objects is a list of unique pointers, this will remove the object completely from memory, therefore **the user doesn't need to worry about garbage collection as long as he/she kills the object after he/she is done with it.**

- ###### GLuint GetObjectCount() : 
Returns the number of objects which exist in the list of objects of the manager.

##### Input Management Functions 

- ###### bool GetKey(GLint key) : 
Returns true if the key described by enum **key** is pressed, false otherwise. Input can be given in the form of GL enumeration : https://www.glfw.org/docs/3.3/group__keys.html

### GLObject.h

This file defines the physical object class which can be manipulated in the game. *In the future, this will probably become a more abstract class from which the GLPhysicalObject class will derive from. Furthermore, all objects are currently being drawn under the same shader, which will definitely change in future versions. Also, objects will have textures and multiple meshes. Lastly, collision detection will be added. This is a minimal (kind of) working example.*

#### Functions

##### Spatial Manipulation Functions

- glm::vec3 **GetPosition**();
- void **SetPosition**(GLfloat x, GLfloat y, GLfloat z);
- glm::vec3 **GetRotation**() { return rotation; };
- void **SetRotation**(GLfloat yaw, GLfloat pitch, GLfloat roll) { rotation = glm::vec3(yaw, pitch, roll); };
- glm::vec3 **GetScale**() { return scale; };
- void **SetScale**(GLfloat x, GLfloat y, GLfloat z) { scale = glm::vec3(x, y, z); };
- glm::vec3 **GetVelocity**() { return velocity; };
- void **SetVelocity**(GLfloat x, GLfloat y, GLfloat z) { velocity = glm::vec3(x, y, z); };
- glm::vec3 **GetAngularVelocity**() { return angularVelocity; };
- void **SetAngularVelocity**(GLfloat x, GLfloat y, GLfloat z) { angularVelocity = glm::vec3(x, y, z); };

All of which I believe to be self-explanatory.

##### Utility Functions 

- ###### Mesh* GetMesh() and void SetMesh(Mesh* mesh) : 
First one returns the mesh of the object, second one sets the mesh of the object using Mesh object pointer **mesh**.

- ###### glm::vec3 GetUp() and glm::vec3 GetRight() : 
Returns the up and right vectors of the object.

##### Functions used by GLManager

- ###### glm::mat4 GetModelMatrix() : 
Returns the model matrix of the object for rendering. 

- ###### void Update() : 
 Every loop, updates objects velocity and angular velocity. 

- ###### void Render() : 
 Renders the object's mesh for now.

### Camera.h

Defines two camera classes: Camera and ActiveCamera, latter of which derives from the former. Basically, an active camera can be manipulated by key strokes and mouse movement, while a Camera object is usually passive. A Camera object can follow an GLObject object.

#### Camera class

A Camera object is initialized with given parameters in the CreateCamera function in GLManager, which talks to the constructor of this class. 

- ###### void Follow(GLObject* obj, GLfloat radius, GLfloat height, GLfloat angle) : 
Follows the GLObject object pointed at by **obj**, from the shell of a cylinder of **radius**, **height** and **angle** given by these parameters. Camera always looks at the object.

- ###### void updateCameraVectors():
When something changes the yaw or pitch or roll of the camera, this function is called. *Probably does not need to be public, will think about this one.*

- ###### glm::mat4 Camera::CalculateViewMatrix() : 
Is used by GLManager to create view matrix. *Probably does not need to be public.*

#### ActiveCamera class

Derives from Camera.

- ###### void keyControl(bool* keys, GLfloat deltaTime)
Called by GLManager.  Can be customized by the the user.  Pressing WASD updates camera position . Holding down left shift speeds the camera to 2 times its base speed. 

- ###### void MouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime, GLuint smoothness) : 
Called by GLManager. Can be customized by the the user.  Calculates the change in camera orientation based on changes in cursor position x and y. 
*There is a smoothness feature, which is not really stable at all among different systems, which basically delays the mouse input over several loops to create a more smooth mouse look. Generally smoothness of about 2 - 4 works, but when the screen is graphic intensive, best is putting it to 1 (which is the default value anyway).*
