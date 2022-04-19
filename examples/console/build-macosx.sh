#!/bin/bash -e

cargo build

c++ -std=c++11 -Wall -Wextra \
    -framework CoreServices \
    -framework Security \
    -o stigmark-client \
    -I../../include \
    main.cc \
    echo.cc \
    login.cc \
    collection.cc \
    ../../target/debug/libstigmark_client.a
