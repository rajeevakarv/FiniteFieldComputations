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
include bench/CMakeFiles/bench_eb.dir/depend.make

# Include the progress variables for this target.
include bench/CMakeFiles/bench_eb.dir/progress.make

# Include the compile flags for this target's objects.
include bench/CMakeFiles/bench_eb.dir/flags.make

bench/CMakeFiles/bench_eb.dir/bench_eb.c.o: bench/CMakeFiles/bench_eb.dir/flags.make
bench/CMakeFiles/bench_eb.dir/bench_eb.c.o: /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench/bench_eb.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linaro/rv_final/NEON/254-target_proj/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object bench/CMakeFiles/bench_eb.dir/bench_eb.c.o"
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/bench_eb.dir/bench_eb.c.o   -c /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench/bench_eb.c

bench/CMakeFiles/bench_eb.dir/bench_eb.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bench_eb.dir/bench_eb.c.i"
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench/bench_eb.c > CMakeFiles/bench_eb.dir/bench_eb.c.i

bench/CMakeFiles/bench_eb.dir/bench_eb.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bench_eb.dir/bench_eb.c.s"
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench/bench_eb.c -o CMakeFiles/bench_eb.dir/bench_eb.c.s

bench/CMakeFiles/bench_eb.dir/bench_eb.c.o.requires:
.PHONY : bench/CMakeFiles/bench_eb.dir/bench_eb.c.o.requires

bench/CMakeFiles/bench_eb.dir/bench_eb.c.o.provides: bench/CMakeFiles/bench_eb.dir/bench_eb.c.o.requires
	$(MAKE) -f bench/CMakeFiles/bench_eb.dir/build.make bench/CMakeFiles/bench_eb.dir/bench_eb.c.o.provides.build
.PHONY : bench/CMakeFiles/bench_eb.dir/bench_eb.c.o.provides

bench/CMakeFiles/bench_eb.dir/bench_eb.c.o.provides.build: bench/CMakeFiles/bench_eb.dir/bench_eb.c.o

# Object files for target bench_eb
bench_eb_OBJECTS = \
"CMakeFiles/bench_eb.dir/bench_eb.c.o"

# External object files for target bench_eb
bench_eb_EXTERNAL_OBJECTS =

bin/bench_eb: bench/CMakeFiles/bench_eb.dir/bench_eb.c.o
bin/bench_eb: bench/CMakeFiles/bench_eb.dir/build.make
bin/bench_eb: lib/librelic_s.a
bin/bench_eb: bench/CMakeFiles/bench_eb.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ../bin/bench_eb"
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bench_eb.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bench/CMakeFiles/bench_eb.dir/build: bin/bench_eb
.PHONY : bench/CMakeFiles/bench_eb.dir/build

bench/CMakeFiles/bench_eb.dir/requires: bench/CMakeFiles/bench_eb.dir/bench_eb.c.o.requires
.PHONY : bench/CMakeFiles/bench_eb.dir/requires

bench/CMakeFiles/bench_eb.dir/clean:
	cd /home/linaro/rv_final/NEON/254-target_proj/bench && $(CMAKE_COMMAND) -P CMakeFiles/bench_eb.dir/cmake_clean.cmake
.PHONY : bench/CMakeFiles/bench_eb.dir/clean

bench/CMakeFiles/bench_eb.dir/depend:
	cd /home/linaro/rv_final/NEON/254-target_proj && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linaro/rv_final/NEON/relic-NEON-proj-254-src /home/linaro/rv_final/NEON/relic-NEON-proj-254-src/bench /home/linaro/rv_final/NEON/254-target_proj /home/linaro/rv_final/NEON/254-target_proj/bench /home/linaro/rv_final/NEON/254-target_proj/bench/CMakeFiles/bench_eb.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bench/CMakeFiles/bench_eb.dir/depend

