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
CMAKE_SOURCE_DIR = /home/dubois/eclipse-workspace/CYLIB-Speculos

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dubois/eclipse-workspace/CYLIB-Speculos/build

# Include any dependencies generated for this target.
include tests/syscalls/CMakeFiles/test_mpi_rng.dir/depend.make

# Include the progress variables for this target.
include tests/syscalls/CMakeFiles/test_mpi_rng.dir/progress.make

# Include the compile flags for this target's objects.
include tests/syscalls/CMakeFiles/test_mpi_rng.dir/flags.make

tests/syscalls/CMakeFiles/test_mpi_rng.dir/test_mpi_rng.c.o: tests/syscalls/CMakeFiles/test_mpi_rng.dir/flags.make
tests/syscalls/CMakeFiles/test_mpi_rng.dir/test_mpi_rng.c.o: ../tests/syscalls/test_mpi_rng.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dubois/eclipse-workspace/CYLIB-Speculos/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/syscalls/CMakeFiles/test_mpi_rng.dir/test_mpi_rng.c.o"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/tests/syscalls && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_mpi_rng.dir/test_mpi_rng.c.o   -c /home/dubois/eclipse-workspace/CYLIB-Speculos/tests/syscalls/test_mpi_rng.c

tests/syscalls/CMakeFiles/test_mpi_rng.dir/test_mpi_rng.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_mpi_rng.dir/test_mpi_rng.c.i"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/tests/syscalls && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dubois/eclipse-workspace/CYLIB-Speculos/tests/syscalls/test_mpi_rng.c > CMakeFiles/test_mpi_rng.dir/test_mpi_rng.c.i

tests/syscalls/CMakeFiles/test_mpi_rng.dir/test_mpi_rng.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_mpi_rng.dir/test_mpi_rng.c.s"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/tests/syscalls && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dubois/eclipse-workspace/CYLIB-Speculos/tests/syscalls/test_mpi_rng.c -o CMakeFiles/test_mpi_rng.dir/test_mpi_rng.c.s

tests/syscalls/CMakeFiles/test_mpi_rng.dir/utils.c.o: tests/syscalls/CMakeFiles/test_mpi_rng.dir/flags.make
tests/syscalls/CMakeFiles/test_mpi_rng.dir/utils.c.o: ../tests/syscalls/utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dubois/eclipse-workspace/CYLIB-Speculos/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object tests/syscalls/CMakeFiles/test_mpi_rng.dir/utils.c.o"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/tests/syscalls && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_mpi_rng.dir/utils.c.o   -c /home/dubois/eclipse-workspace/CYLIB-Speculos/tests/syscalls/utils.c

tests/syscalls/CMakeFiles/test_mpi_rng.dir/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_mpi_rng.dir/utils.c.i"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/tests/syscalls && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dubois/eclipse-workspace/CYLIB-Speculos/tests/syscalls/utils.c > CMakeFiles/test_mpi_rng.dir/utils.c.i

tests/syscalls/CMakeFiles/test_mpi_rng.dir/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_mpi_rng.dir/utils.c.s"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/tests/syscalls && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dubois/eclipse-workspace/CYLIB-Speculos/tests/syscalls/utils.c -o CMakeFiles/test_mpi_rng.dir/utils.c.s

# Object files for target test_mpi_rng
test_mpi_rng_OBJECTS = \
"CMakeFiles/test_mpi_rng.dir/test_mpi_rng.c.o" \
"CMakeFiles/test_mpi_rng.dir/utils.c.o"

# External object files for target test_mpi_rng
test_mpi_rng_EXTERNAL_OBJECTS =

tests/syscalls/test_mpi_rng: tests/syscalls/CMakeFiles/test_mpi_rng.dir/test_mpi_rng.c.o
tests/syscalls/test_mpi_rng: tests/syscalls/CMakeFiles/test_mpi_rng.dir/utils.c.o
tests/syscalls/test_mpi_rng: tests/syscalls/CMakeFiles/test_mpi_rng.dir/build.make
tests/syscalls/test_mpi_rng: src/libemu.a
tests/syscalls/test_mpi_rng: tests/syscalls/CMakeFiles/test_mpi_rng.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dubois/eclipse-workspace/CYLIB-Speculos/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable test_mpi_rng"
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/tests/syscalls && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_mpi_rng.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/syscalls/CMakeFiles/test_mpi_rng.dir/build: tests/syscalls/test_mpi_rng

.PHONY : tests/syscalls/CMakeFiles/test_mpi_rng.dir/build

tests/syscalls/CMakeFiles/test_mpi_rng.dir/clean:
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build/tests/syscalls && $(CMAKE_COMMAND) -P CMakeFiles/test_mpi_rng.dir/cmake_clean.cmake
.PHONY : tests/syscalls/CMakeFiles/test_mpi_rng.dir/clean

tests/syscalls/CMakeFiles/test_mpi_rng.dir/depend:
	cd /home/dubois/eclipse-workspace/CYLIB-Speculos/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dubois/eclipse-workspace/CYLIB-Speculos /home/dubois/eclipse-workspace/CYLIB-Speculos/tests/syscalls /home/dubois/eclipse-workspace/CYLIB-Speculos/build /home/dubois/eclipse-workspace/CYLIB-Speculos/build/tests/syscalls /home/dubois/eclipse-workspace/CYLIB-Speculos/build/tests/syscalls/CMakeFiles/test_mpi_rng.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/syscalls/CMakeFiles/test_mpi_rng.dir/depend

