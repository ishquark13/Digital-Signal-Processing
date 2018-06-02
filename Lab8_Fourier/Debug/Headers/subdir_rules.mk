################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Headers/DSP2833x_GlobalVariableDefs.obj: ../Headers/DSP2833x_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/libs/dsp/FPU/v1_50_00_00/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Headers/DSP2833x_GlobalVariableDefs.d" --obj_directory="Headers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


