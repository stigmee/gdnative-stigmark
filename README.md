### stigmark-client-rs

This repo contains 2 things :

- a statically / dynamically build (written in) Rust library that provides an API to access remote stigmark server (https://github.com/stigmee/stigmark-rocket-rs).
- a command line example written in C++11 that shows how to use that library.

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

C++ interface :
```C++
std::string stigmark_login(const std::string &mail, const std::string &pass)
```

todo: change it to int stigmark_login(const std::string &mail, const std::string &pass, const std::string &token) ?

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

C++ interface :
```C++
int stigmark_add_collection(const std::string &token,
    const std::vector<std::string> &urls, const std::vector<std::string> &keywords)
```
