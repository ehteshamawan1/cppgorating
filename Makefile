# Projekt: cpp gorating

CC=gcc
CFLAGS= -std=c++20 -pedantic -Wall -Wextra -g

cppgorating: src/main.cpp
    cc src/main.cpp -o gorating
