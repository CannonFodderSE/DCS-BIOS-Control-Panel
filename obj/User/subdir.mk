################################################################################
# MRS Version: 2.3.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v30x_it.c \
../User/config.c \
../User/gpio_mgr.c \
../User/ini_parser.c \
../User/main.c \
../User/system_ch32v30x.c \
../User/timebase.c 

C_DEPS += \
./User/ch32v30x_it.d \
./User/config.d \
./User/gpio_mgr.d \
./User/ini_parser.d \
./User/main.d \
./User/system_ch32v30x.d \
./User/timebase.d 

OBJS += \
./User/ch32v30x_it.o \
./User/config.o \
./User/gpio_mgr.o \
./User/ini_parser.o \
./User/main.o \
./User/system_ch32v30x.o \
./User/timebase.o 

DIR_OBJS += \
./User/*.o \

DIR_DEPS += \
./User/*.d \

DIR_EXPANDS += \
./User/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -w -Wunused -Wuninitialized -g -I"/home/randy/mounriver-studio-projects/DCS-Bios/NetLib" -I"/home/randy/mounriver-studio-projects/DCS-Bios/Core" -I"/home/randy/mounriver-studio-projects/DCS-Bios/Debug" -I"/home/randy/mounriver-studio-projects/DCS-Bios/Peripheral/inc" -I"/home/randy/mounriver-studio-projects/DCS-Bios/User" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@

