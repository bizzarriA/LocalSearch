# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.2\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Sara\Documents\GitHub\LocalSearch\CreaIstanze

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Sara\Documents\GitHub\LocalSearch\CreaIstanze\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/CreaIstanze.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CreaIstanze.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CreaIstanze.dir/flags.make

CMakeFiles/CreaIstanze.dir/main.c.obj: CMakeFiles/CreaIstanze.dir/flags.make
CMakeFiles/CreaIstanze.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Sara\Documents\GitHub\LocalSearch\CreaIstanze\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/CreaIstanze.dir/main.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\CreaIstanze.dir\main.c.obj   -c C:\Users\Sara\Documents\GitHub\LocalSearch\CreaIstanze\main.c

CMakeFiles/CreaIstanze.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CreaIstanze.dir/main.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Sara\Documents\GitHub\LocalSearch\CreaIstanze\main.c > CMakeFiles\CreaIstanze.dir\main.c.i

CMakeFiles/CreaIstanze.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CreaIstanze.dir/main.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Sara\Documents\GitHub\LocalSearch\CreaIstanze\main.c -o CMakeFiles\CreaIstanze.dir\main.c.s

# Object files for target CreaIstanze
CreaIstanze_OBJECTS = \
"CMakeFiles/CreaIstanze.dir/main.c.obj"

# External object files for target CreaIstanze
CreaIstanze_EXTERNAL_OBJECTS =

CreaIstanze.exe: CMakeFiles/CreaIstanze.dir/main.c.obj
CreaIstanze.exe: CMakeFiles/CreaIstanze.dir/build.make
CreaIstanze.exe: CMakeFiles/CreaIstanze.dir/linklibs.rsp
CreaIstanze.exe: CMakeFiles/CreaIstanze.dir/objects1.rsp
CreaIstanze.exe: CMakeFiles/CreaIstanze.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Sara\Documents\GitHub\LocalSearch\CreaIstanze\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable CreaIstanze.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\CreaIstanze.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CreaIstanze.dir/build: CreaIstanze.exe

.PHONY : CMakeFiles/CreaIstanze.dir/build

CMakeFiles/CreaIstanze.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\CreaIstanze.dir\cmake_clean.cmake
.PHONY : CMakeFiles/CreaIstanze.dir/clean

CMakeFiles/CreaIstanze.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Sara\Documents\GitHub\LocalSearch\CreaIstanze C:\Users\Sara\Documents\GitHub\LocalSearch\CreaIstanze C:\Users\Sara\Documents\GitHub\LocalSearch\CreaIstanze\cmake-build-debug C:\Users\Sara\Documents\GitHub\LocalSearch\CreaIstanze\cmake-build-debug C:\Users\Sara\Documents\GitHub\LocalSearch\CreaIstanze\cmake-build-debug\CMakeFiles\CreaIstanze.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CreaIstanze.dir/depend

