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
include bench/CMakeFiles/bench_dv.dir/depend.make

# Include the progress variables for this target.
include bench/CMakeFiles/bench_dv.dir/progress.make

# Include the compile flags for this target's objects.
include bench/CMakeFiles/bench_dv.dir/flags.make

bench/CMakeFiles/bench_dv.dir/bench_dv.c.o: bench/CMakeFiles/bench_dv.dir/flags.make
bench/CMakeFiles/bench_dv.dir/bench_dv.c.o: /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench/bench_dv.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linaro/rv_final/NEON/254-target_proj/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object bench/CMakeFiles/bench_dv.dir/bench_dv.c.o"
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/bench_dv.dir/bench_dv.c.o   -c /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench/bench_dv.c

bench/CMakeFiles/bench_dv.dir/bench_dv.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bench_dv.dir/bench_dv.c.i"
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench/bench_dv.c > CMakeFiles/bench_dv.dir/bench_dv.c.i

bench/CMakeFiles/bench_dv.dir/bench_dv.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bench_dv.dir/bench_dv.c.s"
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench/bench_dv.c -o CMakeFiles/bench_dv.dir/bench_dv.c.s

bench/CMakeFiles/bench_dv.dir/bench_dv.c.o.requires:
.PHONY : bench/CMakeFiles/bench_dv.dir/bench_dv.c.o.requires

bench/CMakeFiles/bench_dv.dir/bench_dv.c.o.provides: bench/CMakeFiles/bench_dv.dir/bench_dv.c.o.requires
	$(MAKE) -f bench/CMakeFiles/bench_dv.dir/build.make bench/CMakeFiles/bench_dv.dir/bench_dv.c.o.provides.build
.PHONY : bench/CMakeFiles/bench_dv.dir/bench_dv.c.o.provides

bench/CMakeFiles/bench_dv.dir/bench_dv.c.o.provides.build: bench/CMakeFiles/bench_dv.dir/bench_dv.c.o

# Object files for target bench_dv
bench_dv_OBJECTS = \
"CMakeFiles/bench_dv.dir/bench_dv.c.o"

# External object files for target bench_dv
bench_dv_EXTERNAL_OBJECTS =

bin/bench_dv: bench/CMakeFiles/bench_dv.dir/bench_dv.c.o
bin/bench_dv: bench/CMakeFiles/bench_dv.dir/build.make
bin/bench_dv: lib/librelic_s.a
bin/bench_dv: bench/CMakeFiles/bench_dv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ../bin/bench_dv"
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bench_dv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bench/CMakeFiles/bench_dv.dir/build: bin/bench_dv
.PHONY : bench/CMakeFiles/bench_dv.dir/build

bench/CMakeFiles/bench_dv.dir/requires: bench/CMakeFiles/bench_dv.dir/bench_dv.c.o.requires
.PHONY : bench/CMakeFiles/bench_dv.dir/requires

bench/CMakeFiles/bench_dv.dir/clean:
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && $(CMAKE_COMMAND) -P CMakeFiles/bench_dv.dir/cmake_clean.cmake
.PHONY : bench/CMakeFiles/bench_dv.dir/clean

bench/CMakeFiles/bench_dv.dir/depend:
	cd /home/linaro/rv_final/NEON/254-target_proj && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linaro/rv_final/NEON/relic-NEON-proj-254-src /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench /home/linaro/rv_final/NEON/254-target_proj /home/linaro/rv_final/NEON/254-target_proj/bench /home/linaro/rv_final/NEON/254-target_proj/bench/CMakeFiles/bench_dv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bench/CMakeFiles/bench_dv.dir/depend

