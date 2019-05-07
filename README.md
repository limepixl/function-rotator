# Македонски

Програма која визуелизира функција во 2D и има функционалност да ја ротира околу една оска, генерирајќи 3D објект.

## Симнување и користење на програмата

[Тука](https://github.com/limepixl/function-rotator/releases) може да симнете веќе компајлирана верзија на програмата, под делот Assets. Единствена платформа која е поддржана во моментот е Windows. 

Симнете го zip фајлот кој кореспондира на вашиот систем (built-32bit за 32-битен систем и built-64bit за 64-битен систем). 
Потоа, екстрактирајте го фолдерот од zip архивата и инсталирајте ги редистрибутивните ресурси во фолдерот со вклучување на програмата "vc_redist.exe" и поминување низ процедурата за инсталација.

На крај, .exe фајловите во фолдерот со име "function-rotator" може да ги уклучите. Има четири компајлирани верзии за четири различни функции (e^x, 1/x, sin(x) и x^2).

Видете ги инструкциите за користење на програмата подоле.

## Проблеми

Ако ја добивате пораката "Failed to initialize GLAD" во конзолата, проверете дали ги имате инсталирано најновите драјвери за вашата графичка картичка. Програмата работи преку користење на OpenGL 3.3 (2010).

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
