#!/bin/bash
OPENSSL_STATIC=1 cargo build
c++ -std=c++11 -Wall -o stigmark-client \
    -Iinclude \
    examples/stigmark-client/main.cc \
    examples/stigmark-client/search.cc \
    examples/stigmark-client/echo.cc \
    examples/stigmark-client/login.cc \
    examples/stigmark-client/collection.cc \
    target/debug/libstigmark_client.a \
    -pthread -ldl -lssl -lcrypto

