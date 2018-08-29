cmake_minimum_required(VERSION 2.8)

project(pressureGaugeDetection)

find_package( OpenCV 3.3 REQUIRED )

add_executable(${PROJECT_NAME}Circle "circleDetection.cpp")
target_link_libraries( ${PROJECT_NAME}Circle ${OpenCV_LIBS} )

add_executable(${PROJECT_NAME}Edge "edgeDetection.cpp")
target_link_libraries( ${PROJECT_NAME}Edge ${OpenCV_LIBS} )

add_executable(${PROJECT_NAME}PG "pressureGuageDetection.cpp")
target_link_libraries( ${PROJECT_NAME}PG ${OpenCV_LIBS} )

add_executable(${PROJECT_NAME}Line "lineDetection.cpp")
target_link_libraries( ${PROJECT_NAME}Line ${OpenCV_LIBS} )
