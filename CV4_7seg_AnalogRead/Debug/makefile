################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
CV4_7seg_AnalogRead.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
CV4_7seg_AnalogRead.list \

OBJCOPY_BIN += \
CV4_7seg_AnalogRead.bin \


# All Target
all: CV4_7seg_AnalogRead.elf secondary-outputs

# Tool invocations
CV4_7seg_AnalogRead.elf: $(OBJS) $(USER_OBJS) /home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/STM32F429ZITX_FLASH.ld
	arm-none-eabi-gcc -o "CV4_7seg_AnalogRead.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/martin/DocumentsNS/MKShomeworks/CV4_7seg_AnalogRead/STM32F429ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="CV4_7seg_AnalogRead.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

CV4_7seg_AnalogRead.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "CV4_7seg_AnalogRead.list"
	@echo 'Finished building: $@'
	@echo ' '

CV4_7seg_AnalogRead.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "CV4_7seg_AnalogRead.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
