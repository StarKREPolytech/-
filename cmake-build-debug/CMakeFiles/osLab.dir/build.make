# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /home/ihb/Downloads/CLion-2018.2.3/clion-2018.2.3/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/ihb/Downloads/CLion-2018.2.3/clion-2018.2.3/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ihb/IgorProjects/projects/osLab

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ihb/IgorProjects/projects/osLab/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/osLab.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/osLab.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/osLab.dir/flags.make

CMakeFiles/osLab.dir/main.cpp.o: CMakeFiles/osLab.dir/flags.make
CMakeFiles/osLab.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ihb/IgorProjects/projects/osLab/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/osLab.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/osLab.dir/main.cpp.o -c /home/ihb/IgorProjects/projects/osLab/main.cpp

CMakeFiles/osLab.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/osLab.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ihb/IgorProjects/projects/osLab/main.cpp > CMakeFiles/osLab.dir/main.cpp.i

CMakeFiles/osLab.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/osLab.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ihb/IgorProjects/projects/osLab/main.cpp -o CMakeFiles/osLab.dir/main.cpp.s

# Object files for target osLab
osLab_OBJECTS = \
"CMakeFiles/osLab.dir/main.cpp.o"

# External object files for target osLab
osLab_EXTERNAL_OBJECTS =

osLab: CMakeFiles/osLab.dir/main.cpp.o
osLab: CMakeFiles/osLab.dir/build.make
osLab: CMakeFiles/osLab.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ihb/IgorProjects/projects/osLab/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable osLab"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/osLab.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/osLab.dir/build: osLab

.PHONY : CMakeFiles/osLab.dir/build

CMakeFiles/osLab.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/osLab.dir/cmake_clean.cmake
.PHONY : CMakeFiles/osLab.dir/clean

CMakeFiles/osLab.dir/depend:
	cd /home/ihb/IgorProjects/projects/osLab/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ihb/IgorProjects/projects/osLab /home/ihb/IgorProjects/projects/osLab /home/ihb/IgorProjects/projects/osLab/cmake-build-debug /home/ihb/IgorProjects/projects/osLab/cmake-build-debug /home/ihb/IgorProjects/projects/osLab/cmake-build-debug/CMakeFiles/osLab.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/osLab.dir/depend

