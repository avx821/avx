cmake_minimum_required(VERSION 2.8)
project( Vision )
SET(COMPILE_DEFINITIONS -Werror)


set(CMAKE_BUILD_TYPE Release)

include_directories(/usr/include)
include_directories(/opt/vc/include)
include_directories(/opt/vc/userland-master/interface/vcos)
include_directories(/opt/vc/userland-master)
include_directories(/opt/vc/userland-master/interface/vcos/pthreads)
include_directories(/opt/vc/userland-master/interface/vmcs_host/linux)
include_directories(/opt/vc/userland-master/host_applications/linux/libs/bcm_host/include)

include_directories(~/Navio/C++/Navio)
include_directories(/usr/local/include)
include_directories(/usr/local/include/opencv2)
include_directories(/usr/local/include/opencv)
include_directories(../gpu_test/)
include_directories(~/Raspi_Home/16.82Raspi_Code-master/Navio/C++/Navio/)
include_directories(/usr/include/GL)
include_directories(/opt/vc/userland-master/host_applications/linux/libs/bcm_host/include)


link_directories(/opt/vc/lib)
link_directories(/usr/local/lib/)
link_directories(/usr/lib/)

add_executable (motor_controler motor_control.cpp)


target_link_libraries(motor_controler liblcm.so)
