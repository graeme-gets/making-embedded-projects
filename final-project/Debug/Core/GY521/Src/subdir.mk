################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/GY521/Src/mpu6050.c 

OBJS += \
./Core/GY521/Src/mpu6050.o 

C_DEPS += \
./Core/GY521/Src/mpu6050.d 


# Each subdirectory must supply rules for building sources it contributes
Core/GY521/Src/%.o Core/GY521/Src/%.su: ../Core/GY521/Src/%.c Core/GY521/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/CBuffer/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/Cli/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/GY521/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/Helpers/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/WS2812/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/LC709203F/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/LedController/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/Orinentation/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/EE/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/SystemConfig/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/Dodeca/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/Colours/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/StateContoller/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-GY521-2f-Src

clean-Core-2f-GY521-2f-Src:
	-$(RM) ./Core/GY521/Src/mpu6050.d ./Core/GY521/Src/mpu6050.o ./Core/GY521/Src/mpu6050.su

.PHONY: clean-Core-2f-GY521-2f-Src

