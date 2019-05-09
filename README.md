# Македонски

Програма која визуелизира функција во 2D и има функционалност да ја ротира околу една оска, генерирајќи 3D објект.

## Симнување и користење на програмата

**[Ова](https://youtu.be/sURqtwp5NLU) е видео-туторијал кој може да го гледате ако имате тешкотии при инсталација на програмата.**

[Тука](https://github.com/limepixl/function-rotator/releases) може да симнете веќе компајлирана верзија на програмата, под делот Assets. Единствена платформа која е поддржана во моментот е Windows. 

Симнете го zip фајлот кој кореспондира на вашиот систем (built-32bit за 32-битен систем и built-64bit за 64-битен систем). 
За да ја дознаете архитектурата на вашиот систем, побарајте ја апликацијата "System Information" во Windows Search и архитектурата ќе биде дадена под името System Type.

Потоа, екстрактирајте го фолдерот од zip архивата на друго место. Програмите не би имале точен визуелен преглед ако не е екстрактирана архивата. 
На крај, .exe фајловите во фолдерот со име "function-rotator" може да ги уклучите. Има четири компајлирани верзии за четири различни функции (e^x, 1/x, sin(x) и x^2).

Ако програмите не се вклучуваат и се појавува прозорец со error, инсталирајте ги редистрибутивните материјали, односно програмата со име "vc_redist.exe" во фолдерот.

Видете ги инструкциите за користење на програмата подоле.

## Проблеми

Ако ја добивате пораката "Failed to initialize GLAD" во конзолата, проверете дали ги имате инсталирано најновите драјвери за вашата графичка картичка. Програмата работи преку користење на OpenGL 3.0 и GLSL 1.30.

За да ги инсталирате најновите верзии на драјвери за вашата графичка картичка, прочитајте го објаснувањето на [оваа страница.](https://www.howtogeek.com/135976/how-to-update-your-graphics-drivers-for-maximum-gaming-performance/)

## Контроли

- **Space** - Вклучување/исклучување на тридимензионалната фигура
- **R** - Вклучување/исклучување на автоматска ротација
- **L** - Вклучување/исклучување на светлина и сенки (experimental)
- **M** - Вклучување/исклучување на прегледот на сите поединечни полигони
- **Left Mouse Button** - Кликнете и движете го курсорот на глувчето за ротација
- **Scroll Wheel** - Приближување/оддалечување од телото

## Креирање

### Visual Studio

Наједноставен начин за креирање на програмата е преку користење на Visual Studio 2017/2019 и користејќи ја .sln датотеката. Изворниот код ги содржи сите потребни библиотеки во себе за да може да се компајлира проектот веднаш по симнување.

1) Симнете Visual Studio Community Edition заедно со "Desktop Development with C++" модулот, кој може да се селектира во процесот на инсталација.
2) Отворете ја "function-rotator.sln" датотеката.
3) Во опциите за платформа во новоотворениот прозорец изберете x86 (за 32-битен систем) или x64 (за 64-битен систем).
4) Одберете ја Release конфигурацијата наместо Debug конфигурацијата.
5) Стиснете го копчето "Local Windows Debugger".
6) За менување на функцијата која ќе се визуелизира, може да ја смените линијата код во фајлот "main.cpp" под коментарот "Insert function here". По сите измени, стиснете го копчето "Local Windows Debugger" повторно.

## Искористени библиотеки

* [GLEW](https://github.com/nigels-com/glew)
* [GLFW](https://github.com/glfw/glfw)
* [glm](https://github.com/g-truc/glm)

# English

A program that visualizes a given function in 2D and has the ability to rotate it along an axis, to generate a 3D mesh.

## Controls

- **Space** - Toggle the 3D mesh view
- **R** - Toggle automatic rotation
- **L** - Toggle lighting (experimental)
- **M** - Toggle polygon mode / see all polygons drawn transparently
- **Left Mouse Button** - Click and drag to rotate manually
- **Scroll Wheel** - Zoom in and out

## Dependencies:

* [GLEW](https://github.com/nigels-com/glew)
* [GLFW](https://github.com/glfw/glfw)
* [glm](https://github.com/g-truc/glm)

## Images

![1](https://github.com/limepixl/function-rotator/blob/master/img/1.png)
![2](https://github.com/limepixl/function-rotator/blob/master/img/2.png)
![3](https://github.com/limepixl/function-rotator/blob/master/img/3.png)
