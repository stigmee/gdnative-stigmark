#!/bin/bash -e

OPENSSL_STATIC=1 cargo build
c++ -std=c++11 -Wall -Wextra -o stigmark-client \
    -I../../include \
    main.cc \
    search.cc \
    echo.cc \
    login.cc \
    collection.cc \
    ../../target/debug/libstigmark_client.a \
    -pthread -ldl -lssl -lcrypto

