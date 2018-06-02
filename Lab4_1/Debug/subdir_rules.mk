################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
LCD.obj: ../LCD.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" --advice:performance=all -g --c99 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="LCD.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" --advice:performance=all -g --c99 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


