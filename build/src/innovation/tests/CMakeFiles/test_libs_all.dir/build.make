# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build

# Include any dependencies generated for this target.
include src/innovation/tests/CMakeFiles/test_libs_all.dir/depend.make

# Include the progress variables for this target.
include src/innovation/tests/CMakeFiles/test_libs_all.dir/progress.make

# Include the compile flags for this target's objects.
include src/innovation/tests/CMakeFiles/test_libs_all.dir/flags.make

src/innovation/tests/CMakeFiles/test_libs_all.dir/cy_test_libs_all.c.o: src/innovation/tests/CMakeFiles/test_libs_all.dir/flags.make
src/innovation/tests/CMakeFiles/test_libs_all.dir/cy_test_libs_all.c.o: ../src/innovation/tests/cy_test_libs_all.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/innovation/tests/CMakeFiles/test_libs_all.dir/cy_test_libs_all.c.o"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/src/innovation/tests && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_libs_all.dir/cy_test_libs_all.c.o   -c /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/src/innovation/tests/cy_test_libs_all.c

src/innovation/tests/CMakeFiles/test_libs_all.dir/cy_test_libs_all.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_libs_all.dir/cy_test_libs_all.c.i"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/src/innovation/tests && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/src/innovation/tests/cy_test_libs_all.c > CMakeFiles/test_libs_all.dir/cy_test_libs_all.c.i

src/innovation/tests/CMakeFiles/test_libs_all.dir/cy_test_libs_all.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_libs_all.dir/cy_test_libs_all.c.s"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/src/innovation/tests && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/src/innovation/tests/cy_test_libs_all.c -o CMakeFiles/test_libs_all.dir/cy_test_libs_all.c.s

# Object files for target test_libs_all
test_libs_all_OBJECTS = \
"CMakeFiles/test_libs_all.dir/cy_test_libs_all.c.o"

# External object files for target test_libs_all
test_libs_all_EXTERNAL_OBJECTS =

src/innovation/tests/test_libs_all: src/innovation/tests/CMakeFiles/test_libs_all.dir/cy_test_libs_all.c.o
src/innovation/tests/test_libs_all: src/innovation/tests/CMakeFiles/test_libs_all.dir/build.make
src/innovation/tests/test_libs_all: src/libemu.a
src/innovation/tests/test_libs_all: src/innovation/tests/CMakeFiles/test_libs_all.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test_libs_all"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/src/innovation/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_libs_all.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/innovation/tests/CMakeFiles/test_libs_all.dir/build: src/innovation/tests/test_libs_all

.PHONY : src/innovation/tests/CMakeFiles/test_libs_all.dir/build

src/innovation/tests/CMakeFiles/test_libs_all.dir/clean:
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/src/innovation/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_libs_all.dir/cmake_clean.cmake
.PHONY : src/innovation/tests/CMakeFiles/test_libs_all.dir/clean

src/innovation/tests/CMakeFiles/test_libs_all.dir/depend:
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/src/innovation/tests /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/src/innovation/tests /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/src/innovation/tests/CMakeFiles/test_libs_all.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/innovation/tests/CMakeFiles/test_libs_all.dir/depend

