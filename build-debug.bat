@echo off
set root=%~dp0

g++ %root%src\main.cpp -std=c++11 -g3 -Wall -D_CRT_SECURE_NO_WARNINGS -D_DEBUG %root%src\Mesh.cpp %root%src\Shader.cpp %root%thirdparty\glad\src\glad.c -I %root%thirdparty\glad\include\ -I %root%thirdparty\glfw-mingw\include\ -I %root%thirdparty\glm\ -L %root%thirdparty\glfw-mingw\lib -lglfw3 -lopengl32 -lgdi32 -o function-rotator-debug.exe

echo Build complete!
pause