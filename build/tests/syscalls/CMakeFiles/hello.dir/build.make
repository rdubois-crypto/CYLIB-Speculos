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
include tests/syscalls/CMakeFiles/hello.dir/depend.make

# Include the progress variables for this target.
include tests/syscalls/CMakeFiles/hello.dir/progress.make

# Include the compile flags for this target's objects.
include tests/syscalls/CMakeFiles/hello.dir/flags.make

tests/syscalls/CMakeFiles/hello.dir/hello.c.o: tests/syscalls/CMakeFiles/hello.dir/flags.make
tests/syscalls/CMakeFiles/hello.dir/hello.c.o: ../tests/syscalls/hello.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/syscalls/CMakeFiles/hello.dir/hello.c.o"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/syscalls && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/hello.dir/hello.c.o   -c /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/tests/syscalls/hello.c

tests/syscalls/CMakeFiles/hello.dir/hello.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hello.dir/hello.c.i"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/syscalls && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/tests/syscalls/hello.c > CMakeFiles/hello.dir/hello.c.i

tests/syscalls/CMakeFiles/hello.dir/hello.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hello.dir/hello.c.s"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/syscalls && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/tests/syscalls/hello.c -o CMakeFiles/hello.dir/hello.c.s

# Object files for target hello
hello_OBJECTS = \
"CMakeFiles/hello.dir/hello.c.o"

# External object files for target hello
hello_EXTERNAL_OBJECTS =

tests/syscalls/hello: tests/syscalls/CMakeFiles/hello.dir/hello.c.o
tests/syscalls/hello: tests/syscalls/CMakeFiles/hello.dir/build.make
tests/syscalls/hello: src/libemu.a
tests/syscalls/hello: tests/syscalls/CMakeFiles/hello.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable hello"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/syscalls && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hello.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/syscalls/CMakeFiles/hello.dir/build: tests/syscalls/hello

.PHONY : tests/syscalls/CMakeFiles/hello.dir/build

tests/syscalls/CMakeFiles/hello.dir/clean:
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/syscalls && $(CMAKE_COMMAND) -P CMakeFiles/hello.dir/cmake_clean.cmake
.PHONY : tests/syscalls/CMakeFiles/hello.dir/clean

tests/syscalls/CMakeFiles/hello.dir/depend:
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/tests/syscalls /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/syscalls /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/syscalls/CMakeFiles/hello.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/syscalls/CMakeFiles/hello.dir/depend

