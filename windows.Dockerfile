arg TAG
from mcr.microsoft.com/windows:${TAG}

workdir /fwlib

copy . .

workdir /fwlib/example/build

run cmake -A Win32 ..

run cmake --build .
