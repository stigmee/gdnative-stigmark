git clone --recursive -b 3.4 https://github.com/godotengine/godot-cpp
mkdir -p project/gdnative
scons platform=osx arch=arm64
