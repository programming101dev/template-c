set(PROJECT_NAME "template-c")
set(PROJECT_VERSION "1.0.0")
set(PROJECT_DESCRIPTION "Template C Project")
set(PROJECT_LANGUAGE "C")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Define targets
set(EXECUTABLE_TARGETS main)
set(LIBRARY_TARGETS "")

set(main_SOURCES
        src/main.c
        src/display.c
)

set(main_HEADERS
        include/display.h
)

set(main_LINK_LIBRARIES "")

