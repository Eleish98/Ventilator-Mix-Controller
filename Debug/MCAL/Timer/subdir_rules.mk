################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
MCAL/Timer/%.obj: ../MCAL/Timer/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/G3fry/workspace_v10/MIX Controller" --include_path="C:/Users/G3fry/workspace_v10/MIX Controller/MCAL/driverlib" --include_path="C:/Users/G3fry/workspace_v10/MIX Controller/MCAL" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/include" --include_path="C:/Users/G3fry/workspace_v10/MIX Controller/OS/FreeRTOS/Source/include" --include_path="C:/Users/G3fry/workspace_v10/MIX Controller/OS/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="C:/Users/G3fry/workspace_v10/MIX Controller/OS" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="MCAL/Timer/$(basename $(<F)).d_raw" --obj_directory="MCAL/Timer" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


