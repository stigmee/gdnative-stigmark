### stigmark-client-rs

This repo contains 4 projects :

- a statically / dynamically build (written in) Rust library that provides an API to access remote stigmark server (https://github.com/stigmee/stigmark-rocket-rs). See [src](src) folder.
- a GDNative module providing API access to Godot Scripts needed for the Stigmee project. See [gdstigmark/](gdstigmark/) folder.
- a command line example written in C++11 that shows how to use that library. See [examples/console/](examples/console/) folder.
- a GDNative application showing how to use the GDNative module. See [examples/godot/](examples/godot/) folder.

### Dependencies

In order to build this repo, you will need :

- [rust/cargo](https://www.rust-lang.org) compiler,
- a C/C++ compiler: gcc/clang,
- the python3 [scons](https://scons.org/) build tool.

### Dependency: Rust

In order to compile the Rust library, you'll have to install the Rust compiler thanks to rustup:

[https://rustup.rs/](https://rustup.rs/)

### Build

On Windows 10, you'll need the **x64 native tools command prompt** (from either Visual Studio or Build tools with Windows SDK).

1- Open the command prompt. If ok, the command `cl` should work :

```text
C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools>cl
Microsoft (R) C/C++ Optimizing Compiler Version 19.29.30040 for x64
Copyright (C) Microsoft Corporation.  All rights reserved.

usage: cl [ option... ] filename... [ /link linkoption... ]
```

2- Change directory to `${workspace}\godot\gdnative\stigmark`:

```text
d:
cd \sm\workspace\godot\gdnative\stigmark
```

3- Compile the Rust library:

```text
cargo build
```

```text
D:\sm\workspace\godot\gdnative\stigmark>cargo build
   Compiling winapi v0.3.9
   Compiling cfg-if v1.0.0
   Compiling log v0.4.14
   Compiling pin-project-lite v0.2.8
   Compiling ntapi v0.3.6
   Compiling memchr v2.4.1
   Compiling lazy_static v1.4.0
   Compiling futures-core v0.3.19
   Compiling bytes v1.1.0
   Compiling proc-macro2 v1.0.36
   Compiling unicode-xid v0.2.2
   Compiling itoa v1.0.1
   Compiling futures-task v0.3.19
   Compiling autocfg v1.0.1
   Compiling num_cpus v1.13.1
   Compiling syn v1.0.85
   Compiling fnv v1.0.7
   Compiling futures-util v0.3.19
   Compiling hashbrown v0.11.2
   Compiling serde_derive v1.0.133
   Compiling httparse v1.5.1
   Compiling futures-channel v0.3.19
   Compiling pin-utils v0.1.0
   Compiling futures-sink v0.3.19
   Compiling native-tls v0.2.8
   Compiling serde v1.0.133
   Compiling slab v0.4.5
   Compiling try-lock v0.2.3
   Compiling libc v0.2.112
   Compiling itoa v0.4.8
   Compiling tower-service v0.3.1
   Compiling httpdate v1.0.2
   Compiling serde_json v1.0.74
   Compiling ryu v1.0.9
   Compiling urlencoding v2.1.0
   Compiling tracing-core v0.1.21
   Compiling http v0.2.6
   Compiling indexmap v1.8.0
   Compiling tracing v0.1.29
   Compiling want v0.3.0
   Compiling quote v1.0.14
   Compiling http-body v0.4.4
   Compiling miow v0.3.7
   Compiling schannel v0.1.19
   Compiling socket2 v0.4.2
   Compiling mio v0.7.14
   Compiling tokio v1.15.0
   Compiling tokio-util v0.6.9
   Compiling tokio-native-tls v0.3.0
   Compiling h2 v0.3.10
   Compiling hyper v0.14.16
   Compiling hyper-tls v0.5.0
   Compiling stigmark-client-rs v0.1.0 (D:\sm\workspace\godot\gdnative\stigmark)
    Finished dev [unoptimized + debuginfo] target(s) in 50.29s
```

4- Now, compile the GDNative module:

```text
cd src-stigmarkmod
build-win64.cmd
```

```text
D:\sm\workspace\godot\gdnative\stigmark\src-stigmarkmod>build-win64.cmd

D:\sm\workspace\godot\gdnative\stigmark\src-stigmarkmod>git clone --recursive -b 3.4 https://github.com/godotengine/godot-cpp
fatal: destination path 'godot-cpp' already exists and is not an empty directory.

D:\sm\workspace\godot\gdnative\stigmark\src-stigmarkmod>mkdir project
A subdirectory or file project already exists.

D:\sm\workspace\godot\gdnative\stigmark\src-stigmarkmod>mkdir project\gdnative
A subdirectory or file project\gdnative already exists.

D:\sm\workspace\godot\gdnative\stigmark\src-stigmarkmod>scons platform=windows
scons: Reading SConscript files ...
scons: done reading SConscript files.
scons: Building targets ...
scons: `godot-cpp\bin\libgodot-cpp.windows.debug.64.lib' is up to date.
scons: `project\gdnative\windows\libstigmark.dll' is up to date.
scons: done building targets.

D:\sm\workspace\godot\gdnative\stigmark\src-stigmarkmod>xcopy /y project\gdnative\windows\libstigmark.dll ..\src-stigmarkapp\bin\win64
project\gdnative\windows\libstigmark.dll
1 File(s) copied
```

5- Now, compile the Godot test app by opening `${workspace}\godot\gdnative\stigmark\src-stigmarkapp\project.godot`.

### TODO

Add error enum shared between C/C++ and Rust code.

### Warning

Please be cautious with structure modification

### API

The Rust code provides a C low-level API. But in .cc files you can find a higher interface. However, for this first version, the C++ disables asynchronous calls.

## Login 

C Rust-compatible exported function :
```C
typedef void (*stigmark_client_login_callback_t)(void *data, int status, const char *token);
int stigmark_client_login(const char *mail, const char *pass,
                          stigmark_client_login_callback_t callback, void *data,
                          int waitsync);
```

|name|type|description|note|
|---|---|---|---|
|data|void *|private data passed to the callback, because there is no closure in C|not read/written by the Rust code|
|callback|stigmark_client_login_callback_t|function called by the Rust code when operation done| |
|mail|const char *|user email for authentication| |
|pass|const char *|user password for authentication| |
|status|int|0 or http status returned by server|todo: fix with enum|
|token|const char *|login token (Jwt)|This is a parameter required by some othe functions|
|waitsync|int|if != 0, this blocks the call into the current thread. If 0, the call is totally asynchronous. You thus have to take care, because it means the callback is called in another thread.|this is really a bool: 0 => false, else => true|

C++ interface :
```C++
int stigmark_login(const std::string &mail, const std::string &pass, std::string &token)
```

## Add Collection 

C Rust-compatible exported function :
```C
typedef void (*stigmark_add_collection_callback_t)(void *data, int status);
int stigmark_client_add_collection(const char *token,
                                   const char **urls_ptr, int urls_len,
                                   const char **keywords_ptr, int keywords_len,
                                   stigmark_add_collection_callback_t callback, void *data,
                                   int waitsync);
```

|name|type|description|note|
|---|---|---|---|
|data|void *|private data passed to the callback, because there is no closure in C|not read/written by the Rust code|
|callback|stigmark_client_login_callback_t|function called by the Rust code when operation done| |
|token|const char *|user token obtained with stigmark_client_login| |
|urls_ptr|const char **|array of urls||
|urls_len|int|number of url in urls array||
|keywords_ptr|const char **|array of keywords||
|keywords_len|int|number of keyword in keywords array||
|status|int|0 or http status returned by server|todo: fix with enum|
|waitsync|int|if != 0, this blocks the call into the current thread. If 0, the call is totally asynchronous. You thus have to take care, because it means the callback is called in another thread.|this is really a bool: 0 => false, else => true|

C++ interface :
```C++
int stigmark_add_collection(const std::string &token,
    const std::vector<std::string> &urls, const std::vector<std::string> &keywords)
```
