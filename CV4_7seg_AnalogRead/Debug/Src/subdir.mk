################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/max7219.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f4xx.c 

OBJS += \
./Src/main.o \
./Src/max7219.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f4xx.o 

C_DEPS += \
./Src/main.d \
./Src/max7219.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F429xx -DSTM32F429ZITx -DSTM32F4 -DDEBUG -c -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/Inc" -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/max7219.o: ../Src/max7219.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F429xx -DSTM32F429ZITx -DSTM32F4 -DDEBUG -c -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/Inc" -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/max7219.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F429xx -DSTM32F429ZITx -DSTM32F4 -DDEBUG -c -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/Inc" -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F429xx -DSTM32F429ZITx -DSTM32F4 -DDEBUG -c -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/Inc" -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32f4xx.o: ../Src/system_stm32f4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F429xx -DSTM32F429ZITx -DSTM32F4 -DDEBUG -c -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/Inc" -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

