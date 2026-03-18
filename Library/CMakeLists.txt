# CMakeLists.txt for VeryGUI library.

set(VERY_GUI_SOURCES
	Source/Window.cpp
	Source/Window.h
	Source/DesktopWindow.cpp
	Source/DesktopWindow.h
)

add_library(VeryGUI STATIC ${VERY_GUI_SOURCES})

LinkWithGAL2DBackend(VeryGUI)

target_include_directories(VeryGUI PUBLIC Source)

target_compile_definitions(VeryGUI PRIVATE
	_USE_MATH_DEFINES
	_CRT_SECURE_NO_WARNINGS
)