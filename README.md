# FFMPEGTimeoutStreamController
FFMPEGTimeoutStreamController is a simple controller for FFMPEG live streaming process. It can check availability of incoming live stream and status of current execution of FFMPEG process. If incoming would be timed out or disconnected, Controller wait until incoming stream be alive again, Controller run FFMPEG again with defined conditions.


Linux Compile & Build Steps:
 - sudo apt-get install curl
 - sudo apt-get install ffmpeg
 - sudo apt-get install libcurl4-openssl-dev
 - g++ -Wall -o ffmpegcontroller ffmpegcontroller.cpp -std=c++11 -lcurl ./ffmpegcontroller

Usage Steps:
  - Insert your FFMPEG command in runffmpeg file.
  - Run ffmpegController with incoming live stream url argument. (For example: ffmpegController rtmp://xyz)
