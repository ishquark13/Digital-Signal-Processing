################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Sources/DSP2833x_ADC_cal.obj: ../Sources/DSP2833x_ADC_cal.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/DSP2833x_ADC_cal.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DSP2833x_Adc.obj: ../Sources/DSP2833x_Adc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/DSP2833x_Adc.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DSP2833x_CpuTimers.obj: ../Sources/DSP2833x_CpuTimers.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/DSP2833x_CpuTimers.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DSP2833x_DMA.obj: ../Sources/DSP2833x_DMA.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/DSP2833x_DMA.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DSP2833x_DefaultIsr.obj: ../Sources/DSP2833x_DefaultIsr.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/DSP2833x_DefaultIsr.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DSP2833x_Gpio.obj: ../Sources/DSP2833x_Gpio.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/DSP2833x_Gpio.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DSP2833x_Mcbsp.obj: ../Sources/DSP2833x_Mcbsp.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/DSP2833x_Mcbsp.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DSP2833x_PieCtrl.obj: ../Sources/DSP2833x_PieCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/DSP2833x_PieCtrl.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DSP2833x_PieVect.obj: ../Sources/DSP2833x_PieVect.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/DSP2833x_PieVect.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DSP2833x_Spi.obj: ../Sources/DSP2833x_Spi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/DSP2833x_Spi.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DSP2833x_SysCtrl.obj: ../Sources/DSP2833x_SysCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/DSP2833x_SysCtrl.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DSP2833x_Xintf.obj: ../Sources/DSP2833x_Xintf.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/DSP2833x_Xintf.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/OneToOneI2CDriver.obj: ../Sources/OneToOneI2CDriver.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_16.6.0.STS/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Sources/OneToOneI2CDriver.d" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


