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
include src/CMakeFiles/cmocka-static.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/cmocka-static.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/cmocka-static.dir/flags.make

src/CMakeFiles/cmocka-static.dir/cmocka.c.o: src/CMakeFiles/cmocka-static.dir/flags.make
src/CMakeFiles/cmocka-static.dir/cmocka.c.o: /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka/src/cmocka.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/cmocka-static.dir/cmocka.c.o"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/src && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cmocka-static.dir/cmocka.c.o   -c /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka/src/cmocka.c

src/CMakeFiles/cmocka-static.dir/cmocka.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cmocka-static.dir/cmocka.c.i"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/src && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka/src/cmocka.c > CMakeFiles/cmocka-static.dir/cmocka.c.i

src/CMakeFiles/cmocka-static.dir/cmocka.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cmocka-static.dir/cmocka.c.s"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/src && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka/src/cmocka.c -o CMakeFiles/cmocka-static.dir/cmocka.c.s

# Object files for target cmocka-static
cmocka__static_OBJECTS = \
"CMakeFiles/cmocka-static.dir/cmocka.c.o"

# External object files for target cmocka-static
cmocka__static_EXTERNAL_OBJECTS =

src/libcmocka-static.a: src/CMakeFiles/cmocka-static.dir/cmocka.c.o
src/libcmocka-static.a: src/CMakeFiles/cmocka-static.dir/build.make
src/libcmocka-static.a: src/CMakeFiles/cmocka-static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libcmocka-static.a"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/src && $(CMAKE_COMMAND) -P CMakeFiles/cmocka-static.dir/cmake_clean_target.cmake
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cmocka-static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/cmocka-static.dir/build: src/libcmocka-static.a

.PHONY : src/CMakeFiles/cmocka-static.dir/build

src/CMakeFiles/cmocka-static.dir/clean:
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/src && $(CMAKE_COMMAND) -P CMakeFiles/cmocka-static.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/cmocka-static.dir/clean

src/CMakeFiles/cmocka-static.dir/depend:
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka/src /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/src /home/dubois/eclipse-workspace/CYLIB-Speculos/build/cmocka/src/cmocka-build/src/CMakeFiles/cmocka-static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/cmocka-static.dir/depend

