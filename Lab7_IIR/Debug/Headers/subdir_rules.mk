################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Headers/DSP2833x_GlobalVariableDefs.obj: ../Headers/DSP2833x_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"" -v28 -ml -mt --float_support=fpu32 --include_path="\"${CG_TOOL_ROOT}/include\"
\"C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include\"
\"C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include\"" --preinclude="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" --preinclude="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --preproc_with_compile --preproc_dependency="Headers/DSP2833x_GlobalVariableDefs.d" --obj_directory="Headers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


