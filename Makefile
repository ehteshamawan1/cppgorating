# Projekt: cpp gorating

CC=gcc
CFLAGS= -std=c++20 -pedantic -Wall -Wextra -g

make:
	gcc -I/src -Ilib/crow -Ilib/mysql/include/jdbc -Ilib/asio/include src/main.cpp -o gorating
