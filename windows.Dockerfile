arg TAG=ltsc2019
from mcr.microsoft.com/windows/servercore:${TAG}

workdir /fwlib

copy . .

workdir /fwlib/example/build

run cmake -A Win32 ..

run cmake --build .
