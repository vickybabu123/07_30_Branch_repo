################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../device/system_LPC55S16.c 

C_DEPS += \
./device/system_LPC55S16.d 

OBJS += \
./device/system_LPC55S16.o 


# Each subdirectory must supply rules for building sources it contributes
device/%.o: ../device/%.c device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC55S16JBD100 -DCPU_LPC55S16JBD100_cm33 -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=0 -DPRINTF_FLOAT_ENABLE=1 -DSDK_OS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\01_Vignesh Babu R\04_Workouts\SP1100\02_APR\29-04-2025\AKAS_1100_Battery_section_completed_SP1100_ver_1.00\source" -I"D:\01_Vignesh Babu R\04_Workouts\SP1100\02_APR\29-04-2025\AKAS_1100_Battery_section_completed_SP1100_ver_1.00\board" -I"D:\01_Vignesh Babu R\04_Workouts\SP1100\02_APR\29-04-2025\AKAS_1100_Battery_section_completed_SP1100_ver_1.00\drivers" -I"D:\01_Vignesh Babu R\04_Workouts\SP1100\02_APR\29-04-2025\AKAS_1100_Battery_section_completed_SP1100_ver_1.00\device" -I"D:\01_Vignesh Babu R\04_Workouts\SP1100\02_APR\29-04-2025\AKAS_1100_Battery_section_completed_SP1100_ver_1.00\CMSIS" -O2 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-device

clean-device:
	-$(RM) ./device/system_LPC55S16.d ./device/system_LPC55S16.o

.PHONY: clean-device

