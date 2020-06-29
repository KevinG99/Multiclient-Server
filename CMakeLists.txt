cmake_minimum_required(VERSION 3.10)
project(BS_CODE_besser C)

set(CMAKE_C_STANDARD 99)

add_executable(BS_CODE_besser main.c shmmemory.c shmmemory.h shmDatenhaltung.c semaphoren.c semaphoren.h Bedingungen.c main.h socket_erstellen.c signal_handler.c)