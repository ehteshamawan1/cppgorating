# Projekt: cpp gorating

CC=gcc
CFLAGS= -std=c++20 -pedantic -Wall -Wextra -g

make:
	cc -I/src -Ilib/crow -Ilib/mysql/include/jdbc src/main.cpp -o gorating
