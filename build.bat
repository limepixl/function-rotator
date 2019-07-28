@echo off
set mingw = %~dp0thirdparty\ThreadCC\x64\bin
set root = %~dp0

%mingw%g++ .\src\*.cpp .\thirdparty\glad\src\glad.c -I .\thirdparty\glad\include\ -I .\thirdparty\glfw-mingw\include\ -I .\thirdparty\glm\ -L .\thirdparty\glfw-mingw\lib -lglfw3 -lopengl32 -lgdi32 -o function-rotator.exe
