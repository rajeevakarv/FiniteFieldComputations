# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/linaro/rv_final/NEON/relic-NEON-proj-254-src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/linaro/rv_final/NEON/254-target_proj

# Include any dependencies generated for this target.
include bench/CMakeFiles/bench_fp.dir/depend.make

# Include the progress variables for this target.
include bench/CMakeFiles/bench_fp.dir/progress.make

# Include the compile flags for this target's objects.
include bench/CMakeFiles/bench_fp.dir/flags.make

bench/CMakeFiles/bench_fp.dir/bench_fp.c.o: bench/CMakeFiles/bench_fp.dir/flags.make
bench/CMakeFiles/bench_fp.dir/bench_fp.c.o: /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench/bench_fp.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linaro/rv_final/NEON/254-target_proj/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object bench/CMakeFiles/bench_fp.dir/bench_fp.c.o"
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/bench_fp.dir/bench_fp.c.o   -c /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench/bench_fp.c

bench/CMakeFiles/bench_fp.dir/bench_fp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bench_fp.dir/bench_fp.c.i"
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench/bench_fp.c > CMakeFiles/bench_fp.dir/bench_fp.c.i

bench/CMakeFiles/bench_fp.dir/bench_fp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bench_fp.dir/bench_fp.c.s"
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench/bench_fp.c -o CMakeFiles/bench_fp.dir/bench_fp.c.s

bench/CMakeFiles/bench_fp.dir/bench_fp.c.o.requires:
.PHONY : bench/CMakeFiles/bench_fp.dir/bench_fp.c.o.requires

bench/CMakeFiles/bench_fp.dir/bench_fp.c.o.provides: bench/CMakeFiles/bench_fp.dir/bench_fp.c.o.requires
	$(MAKE) -f bench/CMakeFiles/bench_fp.dir/build.make bench/CMakeFiles/bench_fp.dir/bench_fp.c.o.provides.build
.PHONY : bench/CMakeFiles/bench_fp.dir/bench_fp.c.o.provides

bench/CMakeFiles/bench_fp.dir/bench_fp.c.o.provides.build: bench/CMakeFiles/bench_fp.dir/bench_fp.c.o

# Object files for target bench_fp
bench_fp_OBJECTS = \
"CMakeFiles/bench_fp.dir/bench_fp.c.o"

# External object files for target bench_fp
bench_fp_EXTERNAL_OBJECTS =

bin/bench_fp: bench/CMakeFiles/bench_fp.dir/bench_fp.c.o
bin/bench_fp: bench/CMakeFiles/bench_fp.dir/build.make
bin/bench_fp: lib/librelic_s.a
bin/bench_fp: bench/CMakeFiles/bench_fp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ../bin/bench_fp"
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bench_fp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bench/CMakeFiles/bench_fp.dir/build: bin/bench_fp
.PHONY : bench/CMakeFiles/bench_fp.dir/build

bench/CMakeFiles/bench_fp.dir/requires: bench/CMakeFiles/bench_fp.dir/bench_fp.c.o.requires
.PHONY : bench/CMakeFiles/bench_fp.dir/requires

bench/CMakeFiles/bench_fp.dir/clean:
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && $(CMAKE_COMMAND) -P CMakeFiles/bench_fp.dir/cmake_clean.cmake
.PHONY : bench/CMakeFiles/bench_fp.dir/clean

bench/CMakeFiles/bench_fp.dir/depend:
	cd /home/linaro/rv_final/NEON/254-target_proj && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linaro/rv_final/NEON/relic-NEON-proj-254-src /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench /home/linaro/rv_final/NEON/254-target_proj /home/linaro/rv_final/NEON/254-target_proj/bench /home/linaro/rv_final/NEON/254-target_proj/bench/CMakeFiles/bench_fp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bench/CMakeFiles/bench_fp.dir/depend

