# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/169/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/169/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/toni/work/assignment2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/toni/work/assignment2/cmake-build-debug

# Include any dependencies generated for this target.
include src/CMakeFiles/ascii2hamming.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/ascii2hamming.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/ascii2hamming.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/ascii2hamming.dir/flags.make

src/CMakeFiles/ascii2hamming.dir/common.c.o: src/CMakeFiles/ascii2hamming.dir/flags.make
src/CMakeFiles/ascii2hamming.dir/common.c.o: ../src/common.c
src/CMakeFiles/ascii2hamming.dir/common.c.o: src/CMakeFiles/ascii2hamming.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/toni/work/assignment2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/ascii2hamming.dir/common.c.o"
	cd /home/toni/work/assignment2/cmake-build-debug/src && $(CMAKE_COMMAND) -E __run_co_compile --tidy="clang-tidy;-checks=*,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-init-variables,-clang-analyzer-security.insecureAPI.strcpy,-concurrency-mt-unsafe,-android-cloexec-accept,-android-cloexec-dup,-google-readability-todo,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers,-cert-dcl03-c,-hicpp-static-assert,-misc-static-assert,-altera-struct-pack-align,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling;--quiet;--extra-arg-before=--driver-mode=gcc" --source=/home/toni/work/assignment2/src/common.c -- /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/ascii2hamming.dir/common.c.o -MF CMakeFiles/ascii2hamming.dir/common.c.o.d -o CMakeFiles/ascii2hamming.dir/common.c.o -c /home/toni/work/assignment2/src/common.c

src/CMakeFiles/ascii2hamming.dir/common.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ascii2hamming.dir/common.c.i"
	cd /home/toni/work/assignment2/cmake-build-debug/src && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/toni/work/assignment2/src/common.c > CMakeFiles/ascii2hamming.dir/common.c.i

src/CMakeFiles/ascii2hamming.dir/common.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ascii2hamming.dir/common.c.s"
	cd /home/toni/work/assignment2/cmake-build-debug/src && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/toni/work/assignment2/src/common.c -o CMakeFiles/ascii2hamming.dir/common.c.s

src/CMakeFiles/ascii2hamming.dir/ascii2hamming.c.o: src/CMakeFiles/ascii2hamming.dir/flags.make
src/CMakeFiles/ascii2hamming.dir/ascii2hamming.c.o: ../src/ascii2hamming.c
src/CMakeFiles/ascii2hamming.dir/ascii2hamming.c.o: src/CMakeFiles/ascii2hamming.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/toni/work/assignment2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/CMakeFiles/ascii2hamming.dir/ascii2hamming.c.o"
	cd /home/toni/work/assignment2/cmake-build-debug/src && $(CMAKE_COMMAND) -E __run_co_compile --tidy="clang-tidy;-checks=*,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-init-variables,-clang-analyzer-security.insecureAPI.strcpy,-concurrency-mt-unsafe,-android-cloexec-accept,-android-cloexec-dup,-google-readability-todo,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers,-cert-dcl03-c,-hicpp-static-assert,-misc-static-assert,-altera-struct-pack-align,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling;--quiet;--extra-arg-before=--driver-mode=gcc" --source=/home/toni/work/assignment2/src/ascii2hamming.c -- /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/ascii2hamming.dir/ascii2hamming.c.o -MF CMakeFiles/ascii2hamming.dir/ascii2hamming.c.o.d -o CMakeFiles/ascii2hamming.dir/ascii2hamming.c.o -c /home/toni/work/assignment2/src/ascii2hamming.c

src/CMakeFiles/ascii2hamming.dir/ascii2hamming.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ascii2hamming.dir/ascii2hamming.c.i"
	cd /home/toni/work/assignment2/cmake-build-debug/src && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/toni/work/assignment2/src/ascii2hamming.c > CMakeFiles/ascii2hamming.dir/ascii2hamming.c.i

src/CMakeFiles/ascii2hamming.dir/ascii2hamming.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ascii2hamming.dir/ascii2hamming.c.s"
	cd /home/toni/work/assignment2/cmake-build-debug/src && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/toni/work/assignment2/src/ascii2hamming.c -o CMakeFiles/ascii2hamming.dir/ascii2hamming.c.s

# Object files for target ascii2hamming
ascii2hamming_OBJECTS = \
"CMakeFiles/ascii2hamming.dir/common.c.o" \
"CMakeFiles/ascii2hamming.dir/ascii2hamming.c.o"

# External object files for target ascii2hamming
ascii2hamming_EXTERNAL_OBJECTS =

src/ascii2hamming: src/CMakeFiles/ascii2hamming.dir/common.c.o
src/ascii2hamming: src/CMakeFiles/ascii2hamming.dir/ascii2hamming.c.o
src/ascii2hamming: src/CMakeFiles/ascii2hamming.dir/build.make
src/ascii2hamming: /usr/lib/x86_64-linux-gnu/libm.so
src/ascii2hamming: /usr/local/lib/libdc_error.so
src/ascii2hamming: /usr/local/lib/libdc_posix.so
src/ascii2hamming: /usr/local/lib/libdc_util.so
src/ascii2hamming: /usr/local/lib/libdc_fsm.so
src/ascii2hamming: /usr/local/lib/libdc_application.so
src/ascii2hamming: src/CMakeFiles/ascii2hamming.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/toni/work/assignment2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ascii2hamming"
	cd /home/toni/work/assignment2/cmake-build-debug/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ascii2hamming.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/ascii2hamming.dir/build: src/ascii2hamming
.PHONY : src/CMakeFiles/ascii2hamming.dir/build

src/CMakeFiles/ascii2hamming.dir/clean:
	cd /home/toni/work/assignment2/cmake-build-debug/src && $(CMAKE_COMMAND) -P CMakeFiles/ascii2hamming.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/ascii2hamming.dir/clean

src/CMakeFiles/ascii2hamming.dir/depend:
	cd /home/toni/work/assignment2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/toni/work/assignment2 /home/toni/work/assignment2/src /home/toni/work/assignment2/cmake-build-debug /home/toni/work/assignment2/cmake-build-debug/src /home/toni/work/assignment2/cmake-build-debug/src/CMakeFiles/ascii2hamming.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/ascii2hamming.dir/depend

