# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.2\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\LocalSearch.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\LocalSearch.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\LocalSearch.dir\flags.make

CMakeFiles\LocalSearch.dir\main.c.obj: CMakeFiles\LocalSearch.dir\flags.make
CMakeFiles\LocalSearch.dir\main.c.obj: ..\main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/LocalSearch.dir/main.c.obj"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1420~1.275\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\LocalSearch.dir\main.c.obj /FdCMakeFiles\LocalSearch.dir\ /FS -c C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch\main.c
<<

CMakeFiles\LocalSearch.dir\main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/LocalSearch.dir/main.c.i"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1420~1.275\bin\Hostx86\x86\cl.exe > CMakeFiles\LocalSearch.dir\main.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch\main.c
<<

CMakeFiles\LocalSearch.dir\main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/LocalSearch.dir/main.c.s"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1420~1.275\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\LocalSearch.dir\main.c.s /c C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch\main.c
<<

# Object files for target LocalSearch
LocalSearch_OBJECTS = \
"CMakeFiles\LocalSearch.dir\main.c.obj"

# External object files for target LocalSearch
LocalSearch_EXTERNAL_OBJECTS =

LocalSearch.exe: CMakeFiles\LocalSearch.dir\main.c.obj
LocalSearch.exe: CMakeFiles\LocalSearch.dir\build.make
LocalSearch.exe: CMakeFiles\LocalSearch.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable LocalSearch.exe"
	"C:\Program Files\JetBrains\CLion 2019.2\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\LocalSearch.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100177~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100177~1.0\x86\mt.exe --manifests  -- C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1420~1.275\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\LocalSearch.dir\objects1.rsp @<<
 /out:LocalSearch.exe /implib:LocalSearch.lib /pdb:C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch\cmake-build-debug\LocalSearch.pdb /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\LocalSearch.dir\build: LocalSearch.exe

.PHONY : CMakeFiles\LocalSearch.dir\build

CMakeFiles\LocalSearch.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\LocalSearch.dir\cmake_clean.cmake
.PHONY : CMakeFiles\LocalSearch.dir\clean

CMakeFiles\LocalSearch.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch\cmake-build-debug C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch\cmake-build-debug C:\Users\alice\OneDrive\Documents\GitHub\LocalSearch\LocalSearch\cmake-build-debug\CMakeFiles\LocalSearch.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\LocalSearch.dir\depend

