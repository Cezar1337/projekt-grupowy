This project requires additional libraries:
apt-get update
apt-get install libgtk-3-dev
apt-get install build-essential gnome-devel

The following project is meant to be imported into Eclipse IDE via EGit, but it containes a makefile in the main directory that allows to build an executable file without an IDE

Using the makefile:
- navigate to projekt-grupowy/projekt-grupowy via CLI
- run a "make" command
- this should generate an executable file "switch_emulator"
- run the executable with "./switch_emulator"
WARNING: Using debian 10, I've encountered an error message "cannot open display :0" while running the executable as root. This problem was resolved by running the script as a standard user

Building with Eclipse:
- make sure that pkg-config is installed in Eclipse IDE (can be found in Eclipse Marketplace in "Help" tab)
- right click the project and select "Properties"
- navigate to "C/C++ Build" -> "Settings" -> "Pkg-config" tab
- scroll down the list of libraries and check "atk", "gdk-3.0", "gtk3+-3.0", press "Apply and Close"
- open project properties once more
- navigate to "C/C++ Build" -> "Settings" -> "Tool Settings" tab -> "GCC C Linker"
- add an "-rdynamic" option to "Command" field
- try rebuilding the project and it should build without errors




