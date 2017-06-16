# CMake generated Testfile for 
# Source directory: /home/mike/Dropbox/repos/UpdateArch
# Build directory: /home/mike/Dropbox/repos/UpdateArch/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(appstreamtest "/usr/bin/cmake" "-DAPPSTREAMCLI=/usr/bin/appstreamcli" "-DINSTALL_FILES=/home/mike/Dropbox/repos/UpdateArch/build/install_manifest.txt" "-P" "/usr/share/ECM/kde-modules/appstreamtest.cmake")
subdirs("plugin")
