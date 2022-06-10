################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Cli/Src/console.c \
../Core/Cli/Src/consoleCommands.c \
../Core/Cli/Src/consoleIo.c 

OBJS += \
./Core/Cli/Src/console.o \
./Core/Cli/Src/consoleCommands.o \
./Core/Cli/Src/consoleIo.o 

C_DEPS += \
./Core/Cli/Src/console.d \
./Core/Cli/Src/consoleCommands.d \
./Core/Cli/Src/consoleIo.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Cli/Src/%.o Core/Cli/Src/%.su: ../Core/Cli/Src/%.c Core/Cli/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/CBuffer/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/Cli/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/GY521/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/Helpers/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/WS2812/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/LC709203F/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/LedController/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/Orinentation/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/EE/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/SystemConfig/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/Dodeca/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/Colours/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/StateContoller/Inc" -I"D:/atp/OneDrive - A Thousand Projects/Study/Making Embedded Systems/making-embedded-projects/final-project/Core/RtcController/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Cli-2f-Src

clean-Core-2f-Cli-2f-Src:
	-$(RM) ./Core/Cli/Src/console.d ./Core/Cli/Src/console.o ./Core/Cli/Src/console.su ./Core/Cli/Src/consoleCommands.d ./Core/Cli/Src/consoleCommands.o ./Core/Cli/Src/consoleCommands.su ./Core/Cli/Src/consoleIo.d ./Core/Cli/Src/consoleIo.o ./Core/Cli/Src/consoleIo.su

.PHONY: clean-Core-2f-Cli-2f-Src

