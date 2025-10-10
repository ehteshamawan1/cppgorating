# Projekt: cpp gorating

CC=gcc
CFLAGS= -std=c++20 -pedantic -Wall -Wextra -g

make:
	cc -I/src -I/lib/crow -I/lib/mysql/include/jdbc src/main.cpp -o gorating
