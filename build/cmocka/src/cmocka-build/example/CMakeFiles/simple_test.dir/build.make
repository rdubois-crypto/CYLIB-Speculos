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
CMAKE_SOURCE_DIR = /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build

# Include any dependencies generated for this target.
include example/CMakeFiles/simple_test.dir/depend.make

# Include the progress variables for this target.
include example/CMakeFiles/simple_test.dir/progress.make

# Include the compile flags for this target's objects.
include example/CMakeFiles/simple_test.dir/flags.make

example/CMakeFiles/simple_test.dir/simple_test.c.o: example/CMakeFiles/simple_test.dir/flags.make
example/CMakeFiles/simple_test.dir/simple_test.c.o: /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka/example/simple_test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object example/CMakeFiles/simple_test.dir/simple_test.c.o"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/example && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/simple_test.dir/simple_test.c.o   -c /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka/example/simple_test.c

example/CMakeFiles/simple_test.dir/simple_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/simple_test.dir/simple_test.c.i"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/example && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka/example/simple_test.c > CMakeFiles/simple_test.dir/simple_test.c.i

example/CMakeFiles/simple_test.dir/simple_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/simple_test.dir/simple_test.c.s"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/example && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka/example/simple_test.c -o CMakeFiles/simple_test.dir/simple_test.c.s

# Object files for target simple_test
simple_test_OBJECTS = \
"CMakeFiles/simple_test.dir/simple_test.c.o"

# External object files for target simple_test
simple_test_EXTERNAL_OBJECTS =

example/simple_test: example/CMakeFiles/simple_test.dir/simple_test.c.o
example/simple_test: example/CMakeFiles/simple_test.dir/build.make
example/simple_test: src/libcmocka.so.0.7.0
example/simple_test: example/CMakeFiles/simple_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable simple_test"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/example && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/CMakeFiles/simple_test.dir/build: example/simple_test

.PHONY : example/CMakeFiles/simple_test.dir/build

example/CMakeFiles/simple_test.dir/clean:
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/example && $(CMAKE_COMMAND) -P CMakeFiles/simple_test.dir/cmake_clean.cmake
.PHONY : example/CMakeFiles/simple_test.dir/clean

example/CMakeFiles/simple_test.dir/depend:
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka/example /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/example /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/example/CMakeFiles/simple_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/CMakeFiles/simple_test.dir/depend

