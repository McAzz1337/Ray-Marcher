# Description
This is a small template for a ray marcher running on the gpu with opengl (using glfw and glew)

# Modifying the ray marcher
To modify the ray marcher you can edit the shader.frag file inside the shaders directory
You can also pass additional uniform variables to the shader by defifnin a variable inside shader.frag and from the cpu side calling shader.uniformXX(char* name, ...);
Note: not all functions to upload all kinds of uniforms are implemented, but it is easy to implement them yourself if you wish to
To do this check the dacumentation on: https://registry.khronos.org/OpenGL-Refpages/gl4/
You can call the povided Shader::uniformLocation(char\* name) to cache the uniforms location like the already implemented functions do

# Dependencies
This project uses glfw, glew and glm.
So make sure you have these libraries

# Seup
On Windows with Visual Studio open the directory in VS and open the properties of you solution in the soution explorer.
Under C++ add the includes for glfw, glew and glm.
Under C++/preprocessor add  GLEW_STATIC if you wish for staic linking (the glew dll has to be in the project directory otherwise), GLM_FORCE_RADIANS if you want to use radians with glm functions, and USE_MATH_DEFINES to use M_PI.
Under linker add the library diretories of glfw and glew (glm not needed as it is a header only library).
Then under linker add opoengl32, glfw.lib and glew.lib to the additional libraries.

On Linux everything should work with make.
Install make and in the projects directory run: make run or make build and then ./main
