# You can use this script to build the executable in a Windows environment using MSYS2 along with MinGW.
# If you're using MSYS2 MINGW32 instead of MSYS2 MINGW64,
# then change "C:/msys64/mingw64/lib/libpaho-mqtt3c.dll.a" to "C:/msys64/mingw32/lib/libpaho-mqtt3c.dll.a"
# (Make sure to have installed the proper version of the PAHO library)

g++ \
\
MQTT.hpp \
MQTT.cpp \
Rover.hpp \
Rover.cpp \
Command.hpp \
Command.cpp \
main.cpp \
C:/msys64/mingw64/lib/libpaho-mqtt3c.dll.a \
\
-o program