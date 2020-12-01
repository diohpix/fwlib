arg TAG=ltsc2019
from mcr.microsoft.com/windows/servercore:${TAG}

add https://github.com/Kitware/CMake/releases/download/v3.19.1/cmake-3.19.1-win64-x64.msi /TEMP/cmake.msi

run msiexec /i /TEMP/cmake.msi /quiet /norestart 

workdir /fwlib

copy . .

workdir /fwlib/example/build

run cmake -A Win32 ..

run cmake --build .
