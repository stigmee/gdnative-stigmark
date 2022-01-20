git clone --recursive -b 3.4 https://github.com/godotengine/godot-cpp
mkdir project
mkdir project\gdnative
scons platform=windows
xcopy /y project\gdnative\windows\libstigmark.dll ..\src-stigmarkapp\bin\win64
