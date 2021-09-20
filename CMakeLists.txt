cmake_minimum_required(VERSION 3.17)
project(ex5)

set(CMAKE_CXX_STANDARD 14)

include_directories(ex5)

add_executable(ex5
        ./anomaly_detection_util.cpp
        ./anomaly_detection_util.h
        ./AnomalyDetector.h
        ./SimpleAnomalyDetector.cpp
        ./SimpleAnomalyDetector.h
        ./timeseries.cpp
        ./timeseries.h
        ./minCircle.cpp
        ./Server.cpp
        ./Server.h
        ./HybridAnomalyDetector.cpp
        ./HybridAnomalyDetector.h
        ./CLI.cpp
        ./CLI.h
        ./commands.h
        ./MainTrain.cpp
        ./StandardIO.cpp ./StandardIO.h)



set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(ex5 Threads::Threads)
