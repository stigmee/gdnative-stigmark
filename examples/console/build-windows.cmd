cargo build
cl /EHsc /Zi /Fe:stigmark-client.exe /I..\..\include main.cc echo.cc login.cc search.cc collection.cc ..\..\target\debug\stigmark_client.lib
