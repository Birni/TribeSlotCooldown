@echo off

mkdir workspace_test

cd workspace_test

cmake .. -DTEST=ON -G "Visual Studio 15 Win64"

REM cmake --build . --target ALL_BUILD --config Release

Pause 