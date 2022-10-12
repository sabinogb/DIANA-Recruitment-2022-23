cmake_minimum_required(VERSION 3.0)
project(Recruitment_VirtualRover)

set(CMAKE_CXX_STANDARD 17)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(PahoMqttCpp CONFIG REQUIRED)


add_executable(Recruitment_VirtualRover src/main.cpp lib/MqttClient.h lib/Obstacle.h lib/Target.h lib/Wheel.h lib/Game.hpp lib/MqttClient.h)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ImGui-SFML::ImGui-SFML
        sfml-graphics
        sfml-window
        sfml-audio
        sfml-system
        )

target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE implot::implot)

if (UNIX)
target_link_libraries(${PROJECT_NAME} PRIVATE PahoMqttCpp::paho-mqttpp3-static)
endif()
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE PahoMqttCpp::paho-mqttpp3)
endif()

#Copy assets into build
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)

