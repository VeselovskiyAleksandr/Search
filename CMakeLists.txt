# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project ("Search")
set(JSON_BuildTests OFF CACHE INTERNAL "")
# Включите подпроекты.
add_subdirectory ("Search")

#FetchContent(nlohmann.json)

# Включите подпроекты.
add_subdirectory(nlohmann.json)