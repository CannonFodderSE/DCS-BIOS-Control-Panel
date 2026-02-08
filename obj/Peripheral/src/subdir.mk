################################################################################
# MRS Version: 2.3.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Peripheral/src/ch32v30x_dbgmcu.c \
../Peripheral/src/ch32v30x_eth.c \
../Peripheral/src/ch32v30x_exti.c \
../Peripheral/src/ch32v30x_gpio.c \
../Peripheral/src/ch32v30x_misc.c \
../Peripheral/src/ch32v30x_rcc.c \
../Peripheral/src/ch32v30x_sdio.c \
../Peripheral/src/ch32v30x_tim.c \
../Peripheral/src/ch32v30x_usart.c 

C_DEPS += \
./Peripheral/src/ch32v30x_dbgmcu.d \
./Peripheral/src/ch32v30x_eth.d \
./Peripheral/src/ch32v30x_exti.d \
./Peripheral/src/ch32v30x_gpio.d \
./Peripheral/src/ch32v30x_misc.d \
./Peripheral/src/ch32v30x_rcc.d \
./Peripheral/src/ch32v30x_sdio.d \
./Peripheral/src/ch32v30x_tim.d \
./Peripheral/src/ch32v30x_usart.d 

OBJS += \
./Peripheral/src/ch32v30x_dbgmcu.o \
./Peripheral/src/ch32v30x_eth.o \
./Peripheral/src/ch32v30x_exti.o \
./Peripheral/src/ch32v30x_gpio.o \
./Peripheral/src/ch32v30x_misc.o \
./Peripheral/src/ch32v30x_rcc.o \
./Peripheral/src/ch32v30x_sdio.o \
./Peripheral/src/ch32v30x_tim.o \
./Peripheral/src/ch32v30x_usart.o 

DIR_OBJS += \
./Peripheral/src/*.o \

DIR_DEPS += \
./Peripheral/src/*.d \

DIR_EXPANDS += \
./Peripheral/src/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
Peripheral/src/%.o: ../Peripheral/src/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"/home/randy/mounriver-studio-projects/UdpServer/NetLib" -I"/home/randy/mounriver-studio-projects/UdpServer/Core" -I"/home/randy/mounriver-studio-projects/UdpServer/Debug" -I"/home/randy/mounriver-studio-projects/UdpServer/Peripheral/inc" -I"/home/randy/mounriver-studio-projects/UdpServer/User" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

