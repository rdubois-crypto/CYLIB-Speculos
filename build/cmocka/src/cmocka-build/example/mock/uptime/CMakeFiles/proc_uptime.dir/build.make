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
CMAKE_SOURCE_DIR = /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build

# Include any dependencies generated for this target.
include example/mock/uptime/CMakeFiles/proc_uptime.dir/depend.make

# Include the progress variables for this target.
include example/mock/uptime/CMakeFiles/proc_uptime.dir/progress.make

# Include the compile flags for this target's objects.
include example/mock/uptime/CMakeFiles/proc_uptime.dir/flags.make

example/mock/uptime/CMakeFiles/proc_uptime.dir/proc_uptime.c.o: example/mock/uptime/CMakeFiles/proc_uptime.dir/flags.make
example/mock/uptime/CMakeFiles/proc_uptime.dir/proc_uptime.c.o: /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/mock/uptime/proc_uptime.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object example/mock/uptime/CMakeFiles/proc_uptime.dir/proc_uptime.c.o"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build/example/mock/uptime && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proc_uptime.dir/proc_uptime.c.o   -c /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/mock/uptime/proc_uptime.c

example/mock/uptime/CMakeFiles/proc_uptime.dir/proc_uptime.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proc_uptime.dir/proc_uptime.c.i"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build/example/mock/uptime && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/mock/uptime/proc_uptime.c > CMakeFiles/proc_uptime.dir/proc_uptime.c.i

example/mock/uptime/CMakeFiles/proc_uptime.dir/proc_uptime.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proc_uptime.dir/proc_uptime.c.s"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build/example/mock/uptime && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/mock/uptime/proc_uptime.c -o CMakeFiles/proc_uptime.dir/proc_uptime.c.s

# Object files for target proc_uptime
proc_uptime_OBJECTS = \
"CMakeFiles/proc_uptime.dir/proc_uptime.c.o"

# External object files for target proc_uptime
proc_uptime_EXTERNAL_OBJECTS =

example/mock/uptime/libproc_uptime.a: example/mock/uptime/CMakeFiles/proc_uptime.dir/proc_uptime.c.o
example/mock/uptime/libproc_uptime.a: example/mock/uptime/CMakeFiles/proc_uptime.dir/build.make
example/mock/uptime/libproc_uptime.a: example/mock/uptime/CMakeFiles/proc_uptime.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libproc_uptime.a"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build/example/mock/uptime && $(CMAKE_COMMAND) -P CMakeFiles/proc_uptime.dir/cmake_clean_target.cmake
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build/example/mock/uptime && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/proc_uptime.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/mock/uptime/CMakeFiles/proc_uptime.dir/build: example/mock/uptime/libproc_uptime.a

.PHONY : example/mock/uptime/CMakeFiles/proc_uptime.dir/build

example/mock/uptime/CMakeFiles/proc_uptime.dir/clean:
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build/example/mock/uptime && $(CMAKE_COMMAND) -P CMakeFiles/proc_uptime.dir/cmake_clean.cmake
.PHONY : example/mock/uptime/CMakeFiles/proc_uptime.dir/clean

example/mock/uptime/CMakeFiles/proc_uptime.dir/depend:
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka/example/mock/uptime /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build/example/mock/uptime /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build/example/mock/uptime/CMakeFiles/proc_uptime.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/mock/uptime/CMakeFiles/proc_uptime.dir/depend

