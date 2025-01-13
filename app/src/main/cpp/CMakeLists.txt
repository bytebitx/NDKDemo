# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("ndkdemo")

# message 打印信息
message("-----")
message(STATUS ${PROJECT_SOURCE_DIR}) # 项目c源文件所在绝对路径目录
message(${CMAKE_SOURCE_DIR}) # 项目c源文件所在绝对路径目录
message(${CMAKE_CURRENT_SOURCE_DIR}) # 项目当前目录c源文件所在绝对路径目录
message(${CMAKE_PROJECT_NAME}) # 项目名称 project指令里面的名称
message(${CMAKE_CURRENT_LIST_FILE}) # 当前CMakeLists.txt文件所在的绝对路径
message(${CMAKE_CURRENT_LIST_DIR}) # 当前CMakeLists.txt文件所在的绝对路径目录
message(${ANDROID_ABI})

# set(arg value) 前面是变量名，后面是变量值
set(FFMPEG_LIBS ${CMAKE_SOURCE_DIR}/ffmpeg/libs)
message(${FFMPEG_LIBS}/${ANDROID_ABI})
# 指定在链接目标时找到库文件所需路径
# 调用add_executable()或add_library()创建目标后，再调用link_directories()是没有效果的
link_directories(${FFMPEG_LIBS}/${ANDROID_ABI})
# 在这里，jniLibs目录可以在任意位置，只要link的时候，路径位置正确即可找到对应的

# 设置头文件路径变量
set(FFMPEG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ffmpeg/include)
set(JVM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/jvm/include)
set(ADD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/add/include)

# 项目中引入需要使用的头文件
include_directories(
        ${FFMPEG_INCLUDE_DIR}
        ${JVM_INCLUDE_DIR}
        ${ADD_INCLUDE_DIR}
)

# 设置ffmpeg库变量
set(ffmpeg-libs
        avformat
        avcodec
        avfilter
        swresample
        swscale
        avutil
)

add_subdirectory(${CMAKE_SOURCE_DIR}/jvm)
add_subdirectory(${CMAKE_SOURCE_DIR}/add)

#add_library(main SHARED
#        jni/main.cpp
#)
# 当cpp源文件越来越多的时候，上面写法就很繁琐，所以可以使用file命令把所有cpp源文件打包，设置在一个变量里面，直接引用变量即可
file(GLOB SRCS
        ${CMAKE_SOURCE_DIR}/jni/*.cpp)

add_library(main SHARED
        ${SRCS}
)

target_link_libraries(
        main
        ${ffmpeg-libs}
        Add
        jvm
        android
        log)


add_library(
        ${CMAKE_PROJECT_NAME} SHARED
        native-lib.cpp)

target_link_libraries(
        ${CMAKE_PROJECT_NAME}
        main
        android
        log)

