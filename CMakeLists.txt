cmake_minimum_required(VERSION 3.7)
project(HW3)

set(CMAKE_C_STANDARD 99)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic-errors -Werror -DNDEBUG")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(SOURCE_FILES list_test.c list.h list_mtm1.c list_mtm1.h mtm_ex3.h set.h
        company.h room.h escaper.h company.c room.c escaper.c reservation.h
        reservation.c EscapeTechnion.h EscapeTechnion.c utility.c utility.h)
add_executable(HW3 ${SOURCE_FILES})

target_link_libraries(HW3 ${CMAKE_SOURCE_DIR}/libmtm.a)
