# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/vladimir/.local/share/JetBrains/Toolbox/apps/CLion/ch-1/192.7142.39/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/vladimir/.local/share/JetBrains/Toolbox/apps/CLion/ch-1/192.7142.39/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/VoronoiDiagram.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/VoronoiDiagram.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/VoronoiDiagram.dir/flags.make

CMakeFiles/VoronoiDiagram.dir/main.cpp.o: CMakeFiles/VoronoiDiagram.dir/flags.make
CMakeFiles/VoronoiDiagram.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/VoronoiDiagram.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VoronoiDiagram.dir/main.cpp.o -c "/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram/main.cpp"

CMakeFiles/VoronoiDiagram.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VoronoiDiagram.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram/main.cpp" > CMakeFiles/VoronoiDiagram.dir/main.cpp.i

CMakeFiles/VoronoiDiagram.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VoronoiDiagram.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram/main.cpp" -o CMakeFiles/VoronoiDiagram.dir/main.cpp.s

# Object files for target VoronoiDiagram
VoronoiDiagram_OBJECTS = \
"CMakeFiles/VoronoiDiagram.dir/main.cpp.o"

# External object files for target VoronoiDiagram
VoronoiDiagram_EXTERNAL_OBJECTS =

VoronoiDiagram: CMakeFiles/VoronoiDiagram.dir/main.cpp.o
VoronoiDiagram: CMakeFiles/VoronoiDiagram.dir/build.make
VoronoiDiagram: CMakeFiles/VoronoiDiagram.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable VoronoiDiagram"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/VoronoiDiagram.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/VoronoiDiagram.dir/build: VoronoiDiagram

.PHONY : CMakeFiles/VoronoiDiagram.dir/build

CMakeFiles/VoronoiDiagram.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/VoronoiDiagram.dir/cmake_clean.cmake
.PHONY : CMakeFiles/VoronoiDiagram.dir/clean

CMakeFiles/VoronoiDiagram.dir/depend:
	cd "/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram" "/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram" "/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram/cmake-build-debug" "/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram/cmake-build-debug" "/home/vladimir/CLionProjects/Algorithms course/Algorithms/VoronoiDiagram/cmake-build-debug/CMakeFiles/VoronoiDiagram.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/VoronoiDiagram.dir/depend

