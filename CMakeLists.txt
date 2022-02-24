cmake_minimum_required(VERSION 3.12...3.23)

project(
    CoolCC
    LANGUAGES C
    VERSION 0.1.0
    DESCRIPTION "Some kind of C compiler"
    HOMEPAGE_URL "https://github.com/Phytolizer/CoolCC"
)

add_executable(character_count character_count.c)
