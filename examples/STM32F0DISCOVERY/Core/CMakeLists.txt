cmake_minimum_required(VERSION 3.27)

target_sources(${PROJECT_NAME} PRIVATE
    Src/gpio.c
    Src/main.c
    Src/stm32f0xx_hal_msp.c
    Src/stm32f0xx_it.c
    Src/tim.c)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Inc)
