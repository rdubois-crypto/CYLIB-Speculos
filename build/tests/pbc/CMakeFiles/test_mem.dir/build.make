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
include tests/pbc/CMakeFiles/test_mem.dir/depend.make

# Include the progress variables for this target.
include tests/pbc/CMakeFiles/test_mem.dir/progress.make

# Include the compile flags for this target's objects.
include tests/pbc/CMakeFiles/test_mem.dir/flags.make

tests/pbc/CMakeFiles/test_mem.dir/inno/cy_test_mem_unit.c.o: tests/pbc/CMakeFiles/test_mem.dir/flags.make
tests/pbc/CMakeFiles/test_mem.dir/inno/cy_test_mem_unit.c.o: ../tests/pbc/inno/cy_test_mem_unit.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/pbc/CMakeFiles/test_mem.dir/inno/cy_test_mem_unit.c.o"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/pbc && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_mem.dir/inno/cy_test_mem_unit.c.o   -c /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/tests/pbc/inno/cy_test_mem_unit.c

tests/pbc/CMakeFiles/test_mem.dir/inno/cy_test_mem_unit.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_mem.dir/inno/cy_test_mem_unit.c.i"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/pbc && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/tests/pbc/inno/cy_test_mem_unit.c > CMakeFiles/test_mem.dir/inno/cy_test_mem_unit.c.i

tests/pbc/CMakeFiles/test_mem.dir/inno/cy_test_mem_unit.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_mem.dir/inno/cy_test_mem_unit.c.s"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/pbc && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/tests/pbc/inno/cy_test_mem_unit.c -o CMakeFiles/test_mem.dir/inno/cy_test_mem_unit.c.s

# Object files for target test_mem
test_mem_OBJECTS = \
"CMakeFiles/test_mem.dir/inno/cy_test_mem_unit.c.o"

# External object files for target test_mem
test_mem_EXTERNAL_OBJECTS =

tests/pbc/test_mem: tests/pbc/CMakeFiles/test_mem.dir/inno/cy_test_mem_unit.c.o
tests/pbc/test_mem: tests/pbc/CMakeFiles/test_mem.dir/build.make
tests/pbc/test_mem: src/libemu.a
tests/pbc/test_mem: tests/pbc/CMakeFiles/test_mem.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test_mem"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/pbc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_mem.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/pbc/CMakeFiles/test_mem.dir/build: tests/pbc/test_mem

.PHONY : tests/pbc/CMakeFiles/test_mem.dir/build

tests/pbc/CMakeFiles/test_mem.dir/clean:
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/pbc && $(CMAKE_COMMAND) -P CMakeFiles/test_mem.dir/cmake_clean.cmake
.PHONY : tests/pbc/CMakeFiles/test_mem.dir/clean

tests/pbc/CMakeFiles/test_mem.dir/depend:
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/tests/pbc /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/pbc /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/tests/pbc/CMakeFiles/test_mem.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/pbc/CMakeFiles/test_mem.dir/depend

