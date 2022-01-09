#!/bin/bash
cargo build

c++ -std=c++11 -Wall \
    -framework CoreServices \
    -framework Security \
    -o stigmark-client \
    -Iinclude \
    examples/stigmark-client/main.cc \
    examples/stigmark-client/echo.cc \
    examples/stigmark-client/login.cc \
    examples/stigmark-client/collection.cc \
    target/debug/libstigmark_client.a
