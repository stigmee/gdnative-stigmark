cargo build
cl /EHsc /Zi /Fe:stigmark-client.exe /Iinclude examples\stigmark-client\main.cc examples\stigmark-client\echo.cc examples\stigmark-client\login.cc examples\stigmark-client\collection.cc target\debug\stigmark_client.lib