################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: LAB_3.axf

# Tool invocations
LAB_3.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\University\Engineering\Ano 4\se1\CMSIS_CORE_LPC17xx\Debug" -L"D:\University\Engineering\Ano 4\se1\HAL_SE1\Debug" -Xlinker -Map="LAB_3.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "LAB_2_Debug.ld" -o "LAB_3.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) LAB_3.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "LAB_3.axf"; # arm-none-eabi-objcopy -v -O binary "LAB_3.axf" "LAB_3.bin" ; # checksum -p LPC1769 -d "LAB_3.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
