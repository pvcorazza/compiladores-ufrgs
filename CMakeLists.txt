cmake_minimum_required(VERSION 3.8)
project(etapa4)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c scanner.l symbol_table.c lex.yy.c y.tab.c parser.y ast.c semantic.c semantic.h)
add_executable(etapa4 ${SOURCE_FILES} semantic.c semantic.h)