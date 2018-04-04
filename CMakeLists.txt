cmake_minimum_required(VERSION 3.8)
project(teste)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c scanner.l symbol_table.c lex.yy.c y.tab.c parser.y)
add_executable(teste ${SOURCE_FILES})