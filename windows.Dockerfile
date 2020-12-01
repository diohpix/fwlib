from mcr.microsoft.com/windows/servercore:20H2

workdir /fwlib

copy . .

workdir /fwlib/example/build

run cmake -A Win32 ..

run cmake --build .
