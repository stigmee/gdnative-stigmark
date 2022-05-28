cargo build

set here=%cd%
cd ..\..\target\debug
ren stigmark_client.lib libstigmark_client.lib
ren stigmark_client.dll libstigmark_client.dll
cd %here%

cl /EHsc /Zi /Fe:stigmark-client.exe /I..\..\include main.cc echo.cc login.cc search.cc collection.cc ..\..\target\debug\libstigmark_client.lib
