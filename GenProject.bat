@echo off

mkdir workspace

cd workspace

cmake .. -G "Visual Studio 15 Win64"

cmake --build . --target ALL_BUILD --config Release

Pause 