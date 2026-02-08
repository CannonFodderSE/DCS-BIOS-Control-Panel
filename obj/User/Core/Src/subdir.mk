################################################################################
# MRS Version: 2.3.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Core/Src/config.c 

C_DEPS += \
./User/Core/Src/config.d 

OBJS += \
./User/Core/Src/config.o 

DIR_OBJS += \
./User/Core/Src/*.o \

DIR_DEPS += \
./User/Core/Src/*.d \

DIR_EXPANDS += \
./User/Core/Src/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
User/Core/Src/%.o: ../User/Core/Src/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"/home/randy/mounriver-studio-projects/EXAM/ETH/NetLib" -I"/home/randy/mounriver-studio-projects/EXAM/SRC/Core" -I"/home/randy/mounriver-studio-projects/EXAM/SRC/Debug" -I"/home/randy/mounriver-studio-projects/EXAM/SRC/Peripheral/inc" -I"/home/randy/mounriver-studio-projects/EXAM/ETH/UdpServer/User" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

