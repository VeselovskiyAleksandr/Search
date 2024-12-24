# CMakeList.txt: проект CMake для Search; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Search "src/Search.cpp" "include/Search.h" "src/ConverterJSON.cpp" "include/ConverterJSON.h" "include/Entry.h" "src/Entry.cpp" "src/ConfigurationCounter.cpp" "include/ConfigurationCounter.h" "src/Configuration.cpp" "include/Configuration.h"         )

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
target_link_libraries(Search PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(Search PUBLIC include)