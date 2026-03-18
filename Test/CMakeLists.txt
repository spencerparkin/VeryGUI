# CMakeLists.txt for VeryGUI test program.

set(VERY_GUI_TEST_SOURCES
	Source/Main.cpp
	Source/Main.h
)

add_executable(VeryGUI_Test WIN32 ${VERY_GUI_TEST_SOURCES})

target_link_libraries(VeryGUI_Test PRIVATE VeryGUI)