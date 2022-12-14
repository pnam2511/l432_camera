################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fatfs/diskio.c \
../fatfs/ff.c \
../fatfs/ffsystem.c \
../fatfs/ffunicode.c 

OBJS += \
./fatfs/diskio.o \
./fatfs/ff.o \
./fatfs/ffsystem.o \
./fatfs/ffunicode.o 

C_DEPS += \
./fatfs/diskio.d \
./fatfs/ff.d \
./fatfs/ffsystem.d \
./fatfs/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
fatfs/%.o fatfs/%.su: ../fatfs/%.c fatfs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-fatfs

clean-fatfs:
	-$(RM) ./fatfs/diskio.d ./fatfs/diskio.o ./fatfs/diskio.su ./fatfs/ff.d ./fatfs/ff.o ./fatfs/ff.su ./fatfs/ffsystem.d ./fatfs/ffsystem.o ./fatfs/ffsystem.su ./fatfs/ffunicode.d ./fatfs/ffunicode.o ./fatfs/ffunicode.su

.PHONY: clean-fatfs

