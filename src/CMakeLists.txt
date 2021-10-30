add_compile_definitions(_POSIX_C_SOURCE=200809L _XOPEN_SOURCE=700)

if(APPLE)
    add_definitions(-D_DARWIN_C_SOURCE)
endif()

find_program(LINT "clang-tidy")
IF (LINT)
    set(CMAKE_C_CLANG_TIDY "clang-tidy;-checks=*,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-init-variables,-clang-analyzer-security.insecureAPI.strcpy,-concurrency-mt-unsafe,-android-cloexec-accept,-android-cloexec-dup,-google-readability-todo,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers,-cert-dcl03-c,-hicpp-static-assert,-misc-static-assert,-altera-struct-pack-align,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling;--quiet")
ENDIF ()

# Make an executable
add_executable(ascii2hamming ${COMMON_SOURCE_LIST}  ${ASCII_TO_HAMMING_SOURCE_LIST} ${ASCII_TO_HAMMING_MAIN_SOURCE} ${HEADER_LIST} hamming2ascii.h)
add_executable(hamming2ascii ${COMMON_SOURCE_LIST}  ${HAMMING_TO_ASCII_SOURCE_LIST} ${HAMMING_TO_ASCII_MAIN_SOURCE} ${HEADER_LIST} hamming2ascii.h)

# We need this directory, and users of our library will need it too
target_include_directories(ascii2hamming PRIVATE ../include)
target_include_directories(ascii2hamming PRIVATE /usr/include)
target_include_directories(ascii2hamming PRIVATE /usr/local/include)
target_link_directories(ascii2hamming PRIVATE /usr/lib)
target_link_directories(ascii2hamming PRIVATE /usr/local/lib)
target_include_directories(hamming2ascii PRIVATE ../include)
target_include_directories(hamming2ascii PRIVATE /usr/include)
target_include_directories(hamming2ascii PRIVATE /usr/local/include)
target_link_directories(hamming2ascii PRIVATE /usr/lib)
target_link_directories(hamming2ascii PRIVATE /usr/local/lib)

# All users of this library will need at least C11
target_compile_features(ascii2hamming PUBLIC c_std_11)
target_compile_options(ascii2hamming PRIVATE -g)
target_compile_options(ascii2hamming PRIVATE -fstack-protector-all -ftrapv)
target_compile_options(ascii2hamming PRIVATE -Wpedantic -Wall -Wextra)
target_compile_options(ascii2hamming PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn -Walloca -Wfloat-equal -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)
target_compile_features(hamming2ascii PUBLIC c_std_11)
target_compile_options(hamming2ascii PRIVATE -g)
target_compile_options(hamming2ascii PRIVATE -fstack-protector-all -ftrapv)
target_compile_options(hamming2ascii PRIVATE -Wpedantic -Wall -Wextra)
target_compile_options(hamming2ascii PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn -Walloca -Wfloat-equal -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)

find_library(LIBM m REQUIRED)
find_library(LIBSOCKET socket)
find_library(LIBDC_ERROR dc_error REQUIRED)
find_library(LIBDC_POSIX dc_posix REQUIRED)
find_library(LIBDC_UTIL dc_util REQUIRED)
find_library(LIBDC_FSM dc_fsm REQUIRED)
find_library(LIBDC_APPLICATION dc_application REQUIRED)
target_link_libraries(ascii2hamming PRIVATE ${LIBM})
target_link_libraries(ascii2hamming PRIVATE ${LIBDC_ERROR})
target_link_libraries(ascii2hamming PRIVATE ${LIBDC_POSIX})
target_link_libraries(ascii2hamming PRIVATE ${LIBDC_UTIL})
target_link_libraries(ascii2hamming PRIVATE ${LIBDC_FSM})
target_link_libraries(ascii2hamming PRIVATE ${LIBDC_APPLICATION})
target_link_libraries(hamming2ascii PRIVATE ${LIBM})
target_link_libraries(hamming2ascii PRIVATE ${LIBDC_ERROR})
target_link_libraries(hamming2ascii PRIVATE ${LIBDC_POSIX})
target_link_libraries(hamming2ascii PRIVATE ${LIBDC_UTIL})
target_link_libraries(hamming2ascii PRIVATE ${LIBDC_FSM})
target_link_libraries(hamming2ascii PRIVATE ${LIBDC_APPLICATION})

set_target_properties(ascii2hamming PROPERTIES OUTPUT_NAME "ascii2hamming")
set_target_properties(hamming2ascii PROPERTIES OUTPUT_NAME "hamming2ascii")
install(TARGETS ascii2hamming DESTINATION bin)
install(TARGETS hamming2ascii DESTINATION bin)

# IDEs should put the headers in a nice place
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)

add_custom_target(
        format
        COMMAND clang-format
        -i
        ${HEADER_LIST}
        ${COMMON_SOURCE_LIST}
        ${ASCII_TO_HAMMING_SOURCE_LIST}
        ${HAMMING_TO_ASCII_SOURCE_LIST}
        ${ASCII_TO_HAMMING_MAIN_SOURCE}
        ${HAMMING_TO_ASCII_MAIN_SOURCE}
)
