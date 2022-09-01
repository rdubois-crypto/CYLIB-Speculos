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

# Utility rule file for openssl.

# Include the progress variables for this target.
include CMakeFiles/openssl.dir/progress.make

CMakeFiles/openssl: CMakeFiles/openssl-complete


CMakeFiles/openssl-complete: openssl-prefix/src/openssl-stamp/openssl-install
CMakeFiles/openssl-complete: openssl-prefix/src/openssl-stamp/openssl-mkdir
CMakeFiles/openssl-complete: openssl-prefix/src/openssl-stamp/openssl-download
CMakeFiles/openssl-complete: openssl-prefix/src/openssl-stamp/openssl-update
CMakeFiles/openssl-complete: openssl-prefix/src/openssl-stamp/openssl-patch
CMakeFiles/openssl-complete: openssl-prefix/src/openssl-stamp/openssl-configure
CMakeFiles/openssl-complete: openssl-prefix/src/openssl-stamp/openssl-build
CMakeFiles/openssl-complete: openssl-prefix/src/openssl-stamp/openssl-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'openssl'"
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles
	/usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles/openssl-complete
	/usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp/openssl-done

openssl-prefix/src/openssl-stamp/openssl-install: openssl-prefix/src/openssl-stamp/openssl-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing install step for 'openssl'"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl && make install_sw
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl && /usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp/openssl-install

openssl-prefix/src/openssl-stamp/openssl-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'openssl'"
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/tmp
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src
	/usr/bin/cmake -E make_directory /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp
	/usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp/openssl-mkdir

openssl-prefix/src/openssl-stamp/openssl-download: openssl-prefix/src/openssl-stamp/openssl-urlinfo.txt
openssl-prefix/src/openssl-stamp/openssl-download: openssl-prefix/src/openssl-stamp/openssl-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (download, verify and extract) for 'openssl'"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src && /usr/bin/cmake -P /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp/download-openssl.cmake
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src && /usr/bin/cmake -P /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp/verify-openssl.cmake
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src && /usr/bin/cmake -P /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp/extract-openssl.cmake
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src && /usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp/openssl-download

openssl-prefix/src/openssl-stamp/openssl-update: openssl-prefix/src/openssl-stamp/openssl-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No update step for 'openssl'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp/openssl-update

openssl-prefix/src/openssl-stamp/openssl-patch: openssl-prefix/src/openssl-stamp/openssl-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'openssl'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp/openssl-patch

openssl-prefix/src/openssl-stamp/openssl-configure: openssl-prefix/tmp/openssl-cfgcmd.txt
openssl-prefix/src/openssl-stamp/openssl-configure: openssl-prefix/src/openssl-stamp/openssl-update
openssl-prefix/src/openssl-stamp/openssl-configure: openssl-prefix/src/openssl-stamp/openssl-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Performing configure step for 'openssl'"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl && ./Configure CC=arm-linux-gnueabihf-gcc "CFLAGS= -mthumb -W -Wall -fPIC -Wno-unused-parameter -Wno-missing-field-initializers" no-afalgeng no-aria no-asan no-asm no-async no-autoalginit no-autoerrinit no-autoload-config no-bf no-buildtest-c++ no-camellia no-capieng no-cast no-chacha no-cmac no-cms no-comp no-crypto-mdebug no-crypto-mdebug-backtrace no-ct no-deprecated no-des no-devcryptoeng no-dgram no-dh no-dsa no-dso no-dtls no-ec2m no-ecdh no-egd no-engine no-err no-external-tests no-filenames no-fuzz-afl no-fuzz-libfuzzer no-gost no-heartbeats no-hw no-idea no-makedepend no-md2 no-md4 no-mdc2 no-msan no-multiblock no-nextprotoneg no-ocb no-ocsp no-pinshared no-poly1305 no-posix-io no-psk no-rc2 no-rc4 no-rc5 no-rdrand no-rfc3779 no-scrypt no-sctp no-seed no-shared no-siphash no-sm2 no-sm3 no-sm4 no-sock no-srp no-srtp no-sse2 no-ssl no-ssl3-method no-ssl-trace no-stdio no-tests no-threads no-tls no-ts no-ubsan no-ui-console no-unit-test no-whirlpool no-zlib no-zlib-dynamic linux-armv4 --prefix=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/install
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl && /usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp/openssl-configure

openssl-prefix/src/openssl-stamp/openssl-build: openssl-prefix/src/openssl-stamp/openssl-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing build step for 'openssl'"
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl && make
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl && /usr/bin/cmake -E touch /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/openssl-prefix/src/openssl-stamp/openssl-build

openssl: CMakeFiles/openssl
openssl: CMakeFiles/openssl-complete
openssl: openssl-prefix/src/openssl-stamp/openssl-install
openssl: openssl-prefix/src/openssl-stamp/openssl-mkdir
openssl: openssl-prefix/src/openssl-stamp/openssl-download
openssl: openssl-prefix/src/openssl-stamp/openssl-update
openssl: openssl-prefix/src/openssl-stamp/openssl-patch
openssl: openssl-prefix/src/openssl-stamp/openssl-configure
openssl: openssl-prefix/src/openssl-stamp/openssl-build
openssl: CMakeFiles/openssl.dir/build.make

.PHONY : openssl

# Rule to build all files generated by this target.
CMakeFiles/openssl.dir/build: openssl

.PHONY : CMakeFiles/openssl.dir/build

CMakeFiles/openssl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/openssl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/openssl.dir/clean

CMakeFiles/openssl.dir/depend:
	cd /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build /home/dubois/eclipse-workspace/Speculos-blst/speculos/speculos-master/build/CMakeFiles/openssl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/openssl.dir/depend

