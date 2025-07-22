cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/arm-gcc-toolchain.cmake" CACHE STRING "Toolchain file")
endif()


project(QEMU_an505 C ASM)

set(IMPLIB_PATH ${PROJECT_BINARY_DIR})

# Source files
set(SECURE_SOURCES
    secure/startup_ARMCM33.S
    secure/main.c
    secure/system_ARMCM33.c
    secure/startup_ARMCM33.S
    secure/partition_ARMCM33.h
    common/logPrint.c
    common/uart.c
)

set(NONSECURE_SOURCES
    non_secure/startup_ARMCM33_ns.c
    non_secure/main_ns.c
    non_secure/system_ARMCM33_ns.c
    common/logPrint.c
    common/uart.c
)

# Linker scripts
set(SECURE_LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/secure/gcc_arm.ld")
set(NONSECURE_LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/non_secure/gcc_arm_ns.ld")
set(TZ_VTOR_TABLE_ADDR 0x00200000)

# Include directories as variables
set(SECURE_INCLUDES
    secure
)

set(NONSECURE_INCLUDES
    non_secure
)

set(COMMON_INCLUDES
    common
    CMSIS_5/CMSIS/Core/Include
    CMSIS_5/Device/ARM/ARMCM33/Include
)

add_compile_options(
    -mcpu=cortex-m33 
    -std=gnu23
    -mthumb 
    -ggdb -O0
    --diagnostics-color=always
)

add_link_options(
    --specs=nosys.specs
    --specs=nano.specs
    -ffunction-sections
    -Wl,--gc-sections
    -mcpu=cortex-m33
    -ggdb
    -g3
    -nostartfiles 
    -ffreestanding
    -mthumb
)

# Executables
add_executable(secure.elf ${SECURE_SOURCES})
add_executable(nonsecure.elf ${NONSECURE_SOURCES})


# Assign include directories to targets
target_include_directories(secure.elf PRIVATE ${SECURE_INCLUDES} ${COMMON_INCLUDES})
target_include_directories(nonsecure.elf PRIVATE ${NONSECURE_INCLUDES} ${COMMON_INCLUDES})


target_compile_options(secure.elf PRIVATE -mcmse)

target_link_options(secure.elf PRIVATE
    -Wl,--sort-section=alignment 
    -Wl,--cmse-implib 
    -Wl,--out-implib=${IMPLIB_PATH}/secure-implib.o
    -Wl,-Map=secure.map
    -Wl,-T${SECURE_LINKER_SCRIPT}
)

# Set linker scripts and dependencies
set_target_properties(secure.elf PROPERTIES LINK_DEPENDS "${SECURE_LINKER_SCRIPT}")
set_target_properties(nonsecure.elf PROPERTIES LINK_DEPENDS "${NONSECURE_LINKER_SCRIPT}")

target_link_options(nonsecure.elf PRIVATE 
    ${IMPLIB_PATH}/secure-implib.o
    -Wl,-T${NONSECURE_LINKER_SCRIPT}
)

add_dependencies(nonsecure.elf secure.elf)

target_compile_definitions(nonsecure.elf PRIVATE ARMCM33_DSP_FP_TZ)
target_compile_definitions(secure.elf PRIVATE 
    ARMCM33_DSP_FP_TZ
    TZ_VTOR_TABLE_ADDR=${TZ_VTOR_TABLE_ADDR}
)

# Add a 'run' target to match the Makefile
add_custom_target(run
    COMMAND qemu-system-arm
        -machine mps2-an505
        -cpu cortex-m33 
        -m 16M 
        -nographic 
        -semihosting 
        -monitor null 
        -device loader,file=${CMAKE_BINARY_DIR}/nonsecure.elf 
        -device loader,file=${CMAKE_BINARY_DIR}/secure.elf
    DEPENDS secure.elf nonsecure.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    USES_TERMINAL
    COMMENT "Running QEMU with secure and non-secure images"
)
