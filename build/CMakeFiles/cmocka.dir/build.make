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

# Utility rule file for cmocka.

# Include the progress variables for this target.
include CMakeFiles/cmocka.dir/progress.make

CMakeFiles/cmocka: CMakeFiles/cmocka-complete


CMakeFiles/cmocka-complete: cmocka/src/cmocka-stamp/cmocka-install
CMakeFiles/cmocka-complete: cmocka/src/cmocka-stamp/cmocka-mkdir
CMakeFiles/cmocka-complete: cmocka/src/cmocka-stamp/cmocka-download
CMakeFiles/cmocka-complete: cmocka/src/cmocka-stamp/cmocka-update
CMakeFiles/cmocka-complete: cmocka/src/cmocka-stamp/cmocka-patch
CMakeFiles/cmocka-complete: cmocka/src/cmocka-stamp/cmocka-configure
CMakeFiles/cmocka-complete: cmocka/src/cmocka-stamp/cmocka-build
CMakeFiles/cmocka-complete: cmocka/src/cmocka-stamp/cmocka-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'cmocka'"
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles
	/usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles/cmocka-complete
	/usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp/cmocka-done

cmocka/src/cmocka-stamp/cmocka-install: cmocka/src/cmocka-stamp/cmocka-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing install step for 'cmocka'"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build && $(MAKE) install
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build && /usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp/cmocka-install

cmocka/src/cmocka-stamp/cmocka-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'cmocka'"
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/tmp
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp
	/usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp/cmocka-mkdir

cmocka/src/cmocka-stamp/cmocka-download: cmocka/src/cmocka-stamp/cmocka-urlinfo.txt
cmocka/src/cmocka-stamp/cmocka-download: cmocka/src/cmocka-stamp/cmocka-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (download, verify and extract) for 'cmocka'"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src && /usr/bin/cmake -P /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp/download-cmocka.cmake
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src && /usr/bin/cmake -P /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp/verify-cmocka.cmake
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src && /usr/bin/cmake -P /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp/extract-cmocka.cmake
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src && /usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp/cmocka-download

cmocka/src/cmocka-stamp/cmocka-update: cmocka/src/cmocka-stamp/cmocka-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No update step for 'cmocka'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp/cmocka-update

cmocka/src/cmocka-stamp/cmocka-patch: cmocka/src/cmocka-stamp/cmocka-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'cmocka'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp/cmocka-patch

cmocka/src/cmocka-stamp/cmocka-configure: cmocka/tmp/cmocka-cfgcmd.txt
cmocka/src/cmocka-stamp/cmocka-configure: cmocka/src/cmocka-stamp/cmocka-update
cmocka/src/cmocka-stamp/cmocka-configure: cmocka/src/cmocka-stamp/cmocka-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Performing configure step for 'cmocka'"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build && /usr/bin/cmake -DWITH_STATIC_LIB=true -DCMAKE_INSTALL_PREFIX=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/install -DCMAKE_C_COMPILER=arm-linux-gnueabihf-gcc -DCMAKE_C_FLAGS= "-GUnix Makefiles" /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build && /usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp/cmocka-configure

cmocka/src/cmocka-stamp/cmocka-build: cmocka/src/cmocka-stamp/cmocka-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing build step for 'cmocka'"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build && $(MAKE)
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-build && /usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/cmocka/src/cmocka-stamp/cmocka-build

cmocka: CMakeFiles/cmocka
cmocka: CMakeFiles/cmocka-complete
cmocka: cmocka/src/cmocka-stamp/cmocka-install
cmocka: cmocka/src/cmocka-stamp/cmocka-mkdir
cmocka: cmocka/src/cmocka-stamp/cmocka-download
cmocka: cmocka/src/cmocka-stamp/cmocka-update
cmocka: cmocka/src/cmocka-stamp/cmocka-patch
cmocka: cmocka/src/cmocka-stamp/cmocka-configure
cmocka: cmocka/src/cmocka-stamp/cmocka-build
cmocka: CMakeFiles/cmocka.dir/build.make

.PHONY : cmocka

# Rule to build all files generated by this target.
CMakeFiles/cmocka.dir/build: cmocka

.PHONY : CMakeFiles/cmocka.dir/build

CMakeFiles/cmocka.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cmocka.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cmocka.dir/clean

CMakeFiles/cmocka.dir/depend:
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles/cmocka.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cmocka.dir/depend

