################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Lab1_partA_Sorting.obj: ../Lab1_partA_Sorting.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"" -v28 -ml -mt --float_support=fpu32 --include_path="/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Lab1_partA_Sorting.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

test.obj: ../test.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"" -v28 -ml -mt --float_support=fpu32 --include_path="/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="test.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


