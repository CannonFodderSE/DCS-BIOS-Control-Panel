################################################################################
# MRS Version: 2.3.0
# Automatically-generated file. Do not edit!
################################################################################
-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include NetLib/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
DCS-Bios.hex \

SECONDARY_LIST += \
DCS-Bios.lst \

SECONDARY_SIZE += \
DCS-Bios.siz \


# All Target
all: 
	$(MAKE) --no-print-directory main-build 

main-build: DCS-Bios.elf secondary-outputs

# Tool invocations
DCS-Bios.elf: $(OBJS) $(USER_OBJS_ESCAPE)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU RISC-V Cross C Linker'
	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -w -Wunused -Wuninitialized -g -T "/home/randy/mounriver-studio-projects/DCS-Bios/Ld/Link.ld" -nostartfiles -Xlinker --gc-sections -L"/home/randy/mounriver-studio-projects/DCS-Bios/NetLib" -Xlinker --print-memory-usage -Wl,-Map,"DCS-Bios.map" --specs=nano.specs --specs=nosys.specs -o "DCS-Bios.elf" $(OBJS) $(LIBS) -lprintf
	@echo 'Finished building target: $@'
	@
DCS-Bios.hex: DCS-Bios.elf
	@echo 'Invoking: GNU RISC-V Cross Create Flash Image'
	@	riscv-none-embed-objcopy -O ihex "DCS-Bios.elf" "DCS-Bios.hex"
	@echo 'Finished building $@'
	@
DCS-Bios.lst: DCS-Bios.elf
	@echo 'Invoking: GNU RISC-V Cross Create Listing'
	@	riscv-none-embed-objdump --source --all-headers --demangle -M xw --line-numbers --wide "DCS-Bios.elf" > "DCS-Bios.lst"
	@echo 'Finished building $@'
	@
DCS-Bios.siz: DCS-Bios.elf
	@echo 'Invoking: GNU RISC-V Cross Print Size'
	riscv-none-embed-size --format=berkeley "DCS-Bios.elf"
	@echo 'Finished building $@'
	@

# Other Targets
clean:
	-$(RM) $(DIR_OBJS) $(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE) DCS-Bios.elf
	-$(RM) $(DIR_EXPANDS) $(CALLGRAPH_DOT)
	-$(RM) $(DIR_DEPS)
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets