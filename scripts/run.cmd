@echo off
cd ..
cd source
gcc -std=c99 -Wall -Werror -Wpedantic -Wextra -Wfloat-conversion main.c -o app.exe -lcsfml-graphics -lcsfml-window -lcsfml-system
app.exe
del app.exe