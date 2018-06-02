################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Lab9_vibratoProj.obj" \
"./OneToOneI2CDriver.obj" \
"./Sram.obj" \
"./analogToDigitalConverter.obj" \
"./audioCntrl.obj" \
"./digitalToAnalogConverter.obj" \
"./timer.obj" \
"./timer2.obj" \
"./timer3.obj" \
"./Commons/DSP2833x_ADC_cal.obj" \
"./Commons/DSP2833x_Adc.obj" \
"./Commons/DSP2833x_CpuTimers.obj" \
"./Commons/DSP2833x_DMA.obj" \
"./Commons/DSP2833x_DefaultIsr.obj" \
"./Commons/DSP2833x_GlobalVariableDefs.obj" \
"./Commons/DSP2833x_Gpio.obj" \
"./Commons/DSP2833x_Mcbsp.obj" \
"./Commons/DSP2833x_PieCtrl.obj" \
"./Commons/DSP2833x_PieVect.obj" \
"./Commons/DSP2833x_Spi.obj" \
"./Commons/DSP2833x_SysCtrl.obj" \
"./Commons/DSP2833x_Xintf.obj" \
"./Commons/DSP2833x_usDelay.obj" \
"../28335_RAM_lnk.cmd" \
"../Commons/DSP2833x_Headers_nonBIOS.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Commons/subdir_vars.mk
-include subdir_rules.mk
-include Commons/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Lab5_1.out \

EXE_OUTPUTS__QUOTED += \
"Lab5_1.out" \

BIN_OUTPUTS += \
Lab5_1.hex \

BIN_OUTPUTS__QUOTED += \
"Lab5_1.hex" \


# All Target
all: Lab5_1.out

# Tool invocations
Lab5_1.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --fp_mode=strict -g --c99 --display_error_number --diag_warning=225 --diag_wrap=off -z -m"Lab5_1.map" --stack_size=0x300 --warn_sections -i"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/lib" -i"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="Lab5_1_linkInfo.xml" --rom_model -o "Lab5_1.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

Lab5_1.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/hex2000"  -o "Lab5_1.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "Lab9_vibratoProj.d" "OneToOneI2CDriver.d" "Sram.d" "analogToDigitalConverter.d" "audioCntrl.d" "digitalToAnalogConverter.d" "timer.d" "timer2.d" "timer3.d" "Commons\DSP2833x_Adc.d" "Commons\DSP2833x_CpuTimers.d" "Commons\DSP2833x_DMA.d" "Commons\DSP2833x_DefaultIsr.d" "Commons\DSP2833x_GlobalVariableDefs.d" "Commons\DSP2833x_Gpio.d" "Commons\DSP2833x_Mcbsp.d" "Commons\DSP2833x_PieCtrl.d" "Commons\DSP2833x_PieVect.d" "Commons\DSP2833x_Spi.d" "Commons\DSP2833x_SysCtrl.d" "Commons\DSP2833x_Xintf.d" 
	-$(RM) "Lab9_vibratoProj.obj" "OneToOneI2CDriver.obj" "Sram.obj" "analogToDigitalConverter.obj" "audioCntrl.obj" "digitalToAnalogConverter.obj" "timer.obj" "timer2.obj" "timer3.obj" "Commons\DSP2833x_ADC_cal.obj" "Commons\DSP2833x_Adc.obj" "Commons\DSP2833x_CpuTimers.obj" "Commons\DSP2833x_DMA.obj" "Commons\DSP2833x_DefaultIsr.obj" "Commons\DSP2833x_GlobalVariableDefs.obj" "Commons\DSP2833x_Gpio.obj" "Commons\DSP2833x_Mcbsp.obj" "Commons\DSP2833x_PieCtrl.obj" "Commons\DSP2833x_PieVect.obj" "Commons\DSP2833x_Spi.obj" "Commons\DSP2833x_SysCtrl.obj" "Commons\DSP2833x_Xintf.obj" "Commons\DSP2833x_usDelay.obj" 
	-$(RM) "Commons\DSP2833x_ADC_cal.d" "Commons\DSP2833x_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

