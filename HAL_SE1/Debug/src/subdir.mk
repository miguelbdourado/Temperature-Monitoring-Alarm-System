################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/BMP.c \
../src/FLASH.c \
../src/KEYPAD.c \
../src/LCDText.c \
../src/LED.c \
../src/RTC.c \
../src/SPI.c \
../src/Wait.c 

OBJS += \
./src/BMP.o \
./src/FLASH.o \
./src/KEYPAD.o \
./src/LCDText.o \
./src/LED.o \
./src/RTC.o \
./src/SPI.o \
./src/Wait.o 

C_DEPS += \
./src/BMP.d \
./src/FLASH.d \
./src/KEYPAD.d \
./src/LCDText.d \
./src/LED.d \
./src/RTC.d \
./src/SPI.d \
./src/Wait.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__REDLIB__ -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -I"D:\University\Engineering\Ano 4\se1\HAL_SE1\inc" -I"D:\University\Engineering\Ano 4\se1\CMSIS_CORE_LPC17xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


