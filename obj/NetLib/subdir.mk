################################################################################
# MRS Version: 2.3.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NetLib/eth_driver_10M.c 

C_DEPS += \
./NetLib/eth_driver_10M.d 

OBJS += \
./NetLib/eth_driver_10M.o 

DIR_OBJS += \
./NetLib/*.o \

DIR_DEPS += \
./NetLib/*.d \

DIR_EXPANDS += \
./NetLib/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
NetLib/%.o: ../NetLib/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"/home/randy/mounriver-studio-projects/UdpServer/NetLib" -I"/home/randy/mounriver-studio-projects/UdpServer/Core" -I"/home/randy/mounriver-studio-projects/UdpServer/Debug" -I"/home/randy/mounriver-studio-projects/UdpServer/Peripheral/inc" -I"/home/randy/mounriver-studio-projects/UdpServer/User" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

