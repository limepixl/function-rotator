@echo off
set mingw=%~dp0thirdparty\ThreadCC\x64\bin\
set root=%~dp0

%mingw%g++ %root%src\main.cpp -std=c++11 -O2 %root%src\Mesh.cpp %root%src\Shader.cpp %root%thirdparty\glad\src\glad.c -I %root%thirdparty\glad\include\ -I %root%thirdparty\glfw-mingw\include\ -I %root%thirdparty\glm\ -L %root%thirdparty\glfw-mingw\lib -lglfw3 -lopengl32 -lgdi32 -o function-rotator.exe

echo Build complete!
pause