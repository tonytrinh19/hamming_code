add_compile_definitions(_POSIX_C_SOURCE=200809L _XOPEN_SOURCE=700)

if (APPLE)
    add_definitions(-D_DARWIN_C_SOURCE)
endif ()

set(TEST_HEADER_LIST
        tests.h
        )

set(TEST_SOURCE_LIST
        main.c
        )

include_directories(${CGREEN_PUBLIC_INCLUDE_DIRS} ${PROJECT_BINARY_DIR})
add_executable(template2_test
        ${TEST_SOURCE_LIST} ${TEST_HEADER_LIST} ${COMMON_SOURCE_LIST} ${ASCII_TO_HAMMING_SOURCE_LIST} ${HAMMING_TO_ASCII_SOURCE_LIST} ${HEADER_LIST})

target_compile_features(template2_test PRIVATE c_std_11)
target_compile_options(template2_test PRIVATE -g)
target_compile_options(template2_test PRIVATE -fstack-protector-all -ftrapv)
target_compile_options(template2_test PRIVATE -Wpedantic -Wall -Wextra)
target_compile_options(template2_test PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn -Walloca -Wfloat-equal -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)

target_include_directories(template2_test PRIVATE ../include)
target_include_directories(template2_test PRIVATE /usr/include)
target_include_directories(template2_test PRIVATE /usr/local/include)

find_library(LIBCGREEN cgreen REQUIRED)
find_library(LIBDC_ERROR dc_error REQUIRED)
find_library(LIBDC_POSIX dc_posix REQUIRED)
target_link_libraries(template2_test PRIVATE ${LIBCGREEN})
target_link_libraries(template2_test PRIVATE ${LIBDC_ERROR})
target_link_libraries(template2_test PRIVATE ${LIBDC_POSIX})

add_test(NAME template2_test COMMAND template2_test)
