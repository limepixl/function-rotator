@echo off
set vs="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.22.27905\bin\Hostx64\x64\"
set root=%~dp0
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

%vs%cl %root%src\main.cpp /std:c++17 /O2 /MDd /W4 /EHsc /ZI /D_CRT_SECURE_NO_WARNINGS %root%src\Mesh.cpp %root%src\Shader.cpp %root%thirdparty\glad\src\glad.c /I %root%thirdparty\glad\include\ /I %root%thirdparty\glfw-mingw\include\ /I %root%thirdparty\glm\ /link /LIBPATH:%root%thirdparty\glfw\lib glfw3.lib opengl32.lib gdi32.lib winmm.lib Shell32.lib User32.lib /out:function-rotator-debug.exe

echo Build complete!
pause