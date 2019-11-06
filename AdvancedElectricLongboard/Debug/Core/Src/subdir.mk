################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/freertos.c \
../Core/Src/main.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_hal_timebase_tim.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/system_stm32f1xx.c 

OBJS += \
./Core/Src/freertos.o \
./Core/Src/main.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_hal_timebase_tim.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/system_stm32f1xx.o 

C_DEPS += \
./Core/Src/freertos.d \
./Core/Src/main.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_hal_timebase_tim.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/data/STM32FW/AdvancedElectricLongboard/Core/Inc" -I"C:/data/STM32FW/AdvancedElectricLongboard/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/data/STM32FW/AdvancedElectricLongboard/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/data/STM32FW/AdvancedElectricLongboard/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/data/STM32FW/AdvancedElectricLongboard/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2" -I"C:/data/STM32FW/AdvancedElectricLongboard/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"C:/data/STM32FW/AdvancedElectricLongboard/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/data/STM32FW/AdvancedElectricLongboard/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


