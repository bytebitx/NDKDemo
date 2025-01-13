
#include <iostream>
#include "libavutil/avutil.h"
#include "libavcodec/avcodec.h"

int sub(int a, int b){
    char version[1024 * 4] = {0};
    strcat(version, "ffmpeg : ");
    strcat(version, av_version_info());
    strcat(version, "\nlibavcodec : ");
    strcat(version, AV_STRINGIFY(LIBAVCODEC_VERSION));
    strcat(version, "\nlibavformat : ");
    strcat(version, AV_STRINGIFY(LIBAVFORMAT_VERSION));
    strcat(version, "\navcodec_configure : \n");
    strcat(version, avcodec_configuration());
    std::cout << "The string is: " << version << std::endl;
    return a - b;
}


