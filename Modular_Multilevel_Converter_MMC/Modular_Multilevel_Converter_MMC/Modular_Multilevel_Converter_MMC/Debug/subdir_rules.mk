################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccs1010/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O1 --fp_mode=relaxed --include_path="/opt/ti/ccs1010/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS/include" --include_path="/home/waner/workspace_v10/MMC_F28379D" --include_path="/opt/ti/c2000/F2837xD/v200/F2837xD_common/include" --include_path="/opt/ti/c2000/F2837xD/v200/F2837xD_headers/include" --include_path="/opt/ti/CLAmath/v4_01_00_00/include" --advice:performance=all --define=_LAUNCHXL_F28379D --define=CPU1 --define=CLA_C -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccs1010/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O1 --fp_mode=relaxed --include_path="/opt/ti/ccs1010/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS/include" --include_path="/home/waner/workspace_v10/MMC_F28379D" --include_path="/opt/ti/c2000/F2837xD/v200/F2837xD_common/include" --include_path="/opt/ti/c2000/F2837xD/v200/F2837xD_headers/include" --include_path="/opt/ti/CLAmath/v4_01_00_00/include" --advice:performance=all --define=_LAUNCHXL_F28379D --define=CPU1 --define=CLA_C -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


