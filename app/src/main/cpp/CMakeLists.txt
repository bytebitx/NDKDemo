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

message("-----")
message(${CMAKE_SOURCE_DIR}) # /Users/geely/AndroidStudioProjects/NDKDemo/app/src/main/cpp
message(${CMAKE_PROJECT_NAME}) # ndkdemo
message(${CMAKE_CURRENT_LIST_FILE}) # /Users/geely/AndroidStudioProjects/NDKDemo/app/src/main/cpp/CMakeLists.txt
message(${CMAKE_CURRENT_LIST_DIR}) # /Users/geely/AndroidStudioProjects/NDKDemo/app/src/main/cpp
message(${ANDROID_ABI})


set(jniLibs ${CMAKE_SOURCE_DIR}/../jniLibs)
# 指定在链接目标时找到库文件所需路径
# 调用add_executable()或add_library()创建目标后，再调用link_directories()是没有效果的
link_directories(${jniLibs}/${ANDROID_ABI})

#add_library(main SHARED
#        jni/main.cpp
#        jni/jvm.cpp
#)
# 当cpp源文件越来越多的时候，上面写法就很繁琐，所以可以使用file命令把所有cpp源文件打包，设置在一个变量里面，直接引用变量即可

file(GLOB src-files
        ${CMAKE_SOURCE_DIR}/jni/*.cpp)

add_library(main SHARED
        ${src-files}
)

# 这个命令可以使用<>引入头文件，否则要使用""双引号
include_directories(base/)
include_directories(jni/)
include_directories(ffmpeg/)

set(ffmpeg-libs
        avformat
        avcodec
        avfilter
        swresample
        swscale
        avutil
)



target_link_libraries(
        main
        ${ffmpeg-libs}
        android
        log)


add_library(${CMAKE_PROJECT_NAME} SHARED
        native-lib.cpp)

target_link_libraries(
        ${CMAKE_PROJECT_NAME}
        android
        log)

