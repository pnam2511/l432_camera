################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../jpegdecode/tjpgd.c 

OBJS += \
./jpegdecode/tjpgd.o 

C_DEPS += \
./jpegdecode/tjpgd.d 


# Each subdirectory must supply rules for building sources it contributes
jpegdecode/%.o jpegdecode/%.su: ../jpegdecode/%.c jpegdecode/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-jpegdecode

clean-jpegdecode:
	-$(RM) ./jpegdecode/tjpgd.d ./jpegdecode/tjpgd.o ./jpegdecode/tjpgd.su

.PHONY: clean-jpegdecode

