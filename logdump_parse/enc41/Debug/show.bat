@echo on

set PATH="C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE"
set PATH=%PATH%;"C:\Program Files\Microsoft Visual Studio 10.0\VC\bin\"

rem dumpbin.exe /exports /SYMBOLS /IMPORTS enc41.dll

dumpbin.exe /exports /SYMBOLS enc41.dll

