# CMakeLists.txt

# Phiên bản CMake tối thiểu để dùng C++ Modules
cmake_minimum_required(VERSION 3.28)

# Định nghĩa project
project(meow LANGUAGES CXX)

# Cấu hình cho chuẩn C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Tìm các file nguồn .cpp thông thường (nếu có, ví dụ main.cpp)
file(GLOB_RECURSE MEOW_SOURCES
    "src/*.cpp"
)

# Tìm TẤT CẢ các file module (cả interface .ixx và implementation .cppm)
file(GLOB_RECURSE MEOW_MODULE_SOURCES
    "src/*.ixx"
    "src/*.cppm"
)

# Tạo file Executable chính
add_executable(meow)

# Thêm các file nguồn thông thường vào target
target_sources(meow PRIVATE
    ${MEOW_SOURCES}
)

# Thêm TẤT CẢ các file module vào FILE_SET
target_sources(meow PUBLIC FILE_SET CXX_MODULES FILES
    ${MEOW_MODULE_SOURCES}
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Thiết lập đường dẫn Include
target_include_directories(meow PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Biên dịch precompiled headers
target_precompile_headers(meow PRIVATE "src/common/pch.h")

# Đặt vị trí output cho file Executable
set_target_properties(meow PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)