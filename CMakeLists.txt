# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

if (DEFINED ENV{SdkRootDirPath})
	SET(SdkRootDirPath "$ENV{SdkRootDirPath}")
endif()

if (NOT DEFINED SdkRootDirPath)
	MESSAGE(FATAL_ERROR "SDK path could not be determined. Please update SdkRootDirPath value from your CMake presets file(s).")
endif()

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

# RESET FLAG VALUES
SET(CMAKE_ASM_FLAGS_DEBUG "")
SET(CMAKE_ASM_FLAGS_RELEASE "")
SET(CMAKE_C_FLAGS_DEBUG "")
SET(CMAKE_C_FLAGS_RELEASE "")
SET(CMAKE_CXX_FLAGS_DEBUG "")
SET(CMAKE_CXX_FLAGS_RELEASE "")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "")

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})

set(CMAKE_USER_MAKE_RULES_OVERRIDE "user_override.cmake")

project(AKAS_1100)
set(MCUX_SDK_PROJECT_NAME AKAS_1100.elf)

include(${ProjDirPath}/flags.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} "${ProjDirPath}/CMSIS/tz_context.h"
"${ProjDirPath}/CMSIS/mpu_armv8.h"
"${ProjDirPath}/CMSIS/core_cm33.h"
"${ProjDirPath}/CMSIS/cmsis_version.h"
"${ProjDirPath}/CMSIS/cmsis_gcc.h"
"${ProjDirPath}/CMSIS/cmsis_compiler.h"
"${ProjDirPath}/board/pin_mux.h"
"${ProjDirPath}/board/pin_mux.c"
"${ProjDirPath}/board/peripherals.h"
"${ProjDirPath}/board/peripherals.c"
"${ProjDirPath}/board/clock_config.h"
"${ProjDirPath}/board/clock_config.c"
"${ProjDirPath}/board/board.h"
"${ProjDirPath}/board/board.c"
"${ProjDirPath}/device/system_LPC55S16.h"
"${ProjDirPath}/device/system_LPC55S16.c"
"${ProjDirPath}/device/LPC55S16_features.h"
"${ProjDirPath}/device/LPC55S16.h"
"${ProjDirPath}/device/fsl_device_registers.h"
"${ProjDirPath}/drivers/fsl_rtc.h"
"${ProjDirPath}/drivers/fsl_reset.h"
"${ProjDirPath}/drivers/fsl_reset.c"
"${ProjDirPath}/drivers/fsl_power.h"
"${ProjDirPath}/drivers/fsl_power.c"
"${ProjDirPath}/drivers/fsl_inputmux_connections.h"
"${ProjDirPath}/drivers/fsl_inputmux.h"
"${ProjDirPath}/drivers/fsl_inputmux.c"
"${ProjDirPath}/drivers/fsl_common_arm.h"
"${ProjDirPath}/drivers/fsl_common_arm.c"
"${ProjDirPath}/drivers/fsl_common.h"
"${ProjDirPath}/drivers/fsl_common.c"
"${ProjDirPath}/drivers/fsl_clock.h"
"${ProjDirPath}/drivers/fsl_clock.c"
"${ProjDirPath}/source/year.c"
"${ProjDirPath}/source/Used_hrs.c"
"${ProjDirPath}/source/USART.c"
"${ProjDirPath}/source/UART.h"
"${ProjDirPath}/source/TFT_String.h"
"${ProjDirPath}/source/TFT_String.c"
"${ProjDirPath}/source/TFT_Commands.h"
"${ProjDirPath}/source/TFT_Commands.c"
"${ProjDirPath}/source/Target_fun.h"
"${ProjDirPath}/source/Target_fun.c"
"${ProjDirPath}/source/sys_sett_menu.c"
"${ProjDirPath}/source/SYR_length_frame.c"
"${ProjDirPath}/source/SYR_length_calib.c"
"${ProjDirPath}/source/Syringe_calib_header.h"
"${ProjDirPath}/source/Symbol.h"
"${ProjDirPath}/source/Symbol.c"
"${ProjDirPath}/source/STEPPER.h"
"${ProjDirPath}/source/STEPPER.c"
"${ProjDirPath}/source/standby_fun.c"
"${ProjDirPath}/source/sound.h"
"${ProjDirPath}/source/sound.c"
"${ProjDirPath}/source/Shapes.h"
"${ProjDirPath}/source/Shapes.c"
"${ProjDirPath}/source/serialno_set.c"
"${ProjDirPath}/source/semihost_hardfault.c"
"${ProjDirPath}/source/SD_Calib_frames.c"
"${ProjDirPath}/source/SD_Calib.h"
"${ProjDirPath}/source/SD_Calib.c"
"${ProjDirPath}/source/RTC_driver.h"
"${ProjDirPath}/source/rtc_driver.c"
"${ProjDirPath}/source/RTC.h"
"${ProjDirPath}/source/RTC.c"
"${ProjDirPath}/source/Rec_frame.c"
"${ProjDirPath}/source/Rec_drug_list.c"
"${ProjDirPath}/source/Ramp_mode.c"
"${ProjDirPath}/source/PWM.h"
"${ProjDirPath}/source/Pressure_calib.c"
"${ProjDirPath}/source/Power_off.h"
"${ProjDirPath}/source/Power_off.c"
"${ProjDirPath}/source/plunger_alarm.h"
"${ProjDirPath}/source/plunger_alarm.c"
"${ProjDirPath}/source/patient_para_sett.c"
"${ProjDirPath}/source/Password_screens.c"
"${ProjDirPath}/source/Password.h"
"${ProjDirPath}/source/Password.c"
"${ProjDirPath}/source/occl_menu.c"
"${ProjDirPath}/source/Occlusion.h"
"${ProjDirPath}/source/Occlusion.c"
"${ProjDirPath}/source/near_empty.c"
"${ProjDirPath}/source/Motor.h"
"${ProjDirPath}/source/Motor.c"
"${ProjDirPath}/source/mode_screen.c"
"${ProjDirPath}/source/modes.h"
"${ProjDirPath}/source/Main_screen_functionality.c"
"${ProjDirPath}/source/Main_screen.h"
"${ProjDirPath}/source/Main_Menu.h"
"${ProjDirPath}/source/Main_menu.c"
"${ProjDirPath}/source/Main_frame.c"
"${ProjDirPath}/source/main.c"
"${ProjDirPath}/source/Linear_pot_functionality.c"
"${ProjDirPath}/source/Linear_pot.h"
"${ProjDirPath}/source/Linear_pot.c"
"${ProjDirPath}/source/Linear_Header.h"
"${ProjDirPath}/source/Last_infusion.c"
"${ProjDirPath}/source/kvo_menu.c"
"${ProjDirPath}/source/Keypad.h"
"${ProjDirPath}/source/Keypad.c"
"${ProjDirPath}/source/infused_calculate.h"
"${ProjDirPath}/source/Infused_Calculate.c"
"${ProjDirPath}/source/I2C.c"
"${ProjDirPath}/source/HEADER.h"
"${ProjDirPath}/source/GPIO.h"
"${ProjDirPath}/source/GPIO.c"
"${ProjDirPath}/source/gcov_support.h"
"${ProjDirPath}/source/gcov_support.c"
"${ProjDirPath}/source/gcov.c"
"${ProjDirPath}/source/FR_CALC.h"
"${ProjDirPath}/source/FR_CALC.c"
"${ProjDirPath}/source/Frame_Brand_List.c"
"${ProjDirPath}/source/Font.h"
"${ProjDirPath}/source/Flowrate_Screen.h"
"${ProjDirPath}/source/Flowrate_Screen.c"
"${ProjDirPath}/source/fav_frame.c"
"${ProjDirPath}/source/Fav_drug_list.c"
"${ProjDirPath}/source/Fav_Brand.c"
"${ProjDirPath}/source/factory_menu.c"
"${ProjDirPath}/source/factory.h"
"${ProjDirPath}/source/EXTERNAL_RTC.h"
"${ProjDirPath}/source/EXTERNAL.RTC.c"
"${ProjDirPath}/source/Event_log.c"
"${ProjDirPath}/source/Events_log_frames.c"
"${ProjDirPath}/source/Events_log.h"
"${ProjDirPath}/source/Dynamic_Pressure.h"
"${ProjDirPath}/source/Dynamic_pressure.c"
"${ProjDirPath}/source/Drug_name_search.c"
"${ProjDirPath}/source/Drug_List.h"
"${ProjDirPath}/source/Drug_List.c"
"${ProjDirPath}/source/Drug_frame.c"
"${ProjDirPath}/source/Drug_category.c"
"${ProjDirPath}/source/Driver_Timer.h"
"${ProjDirPath}/source/Driver_Timer.c"
"${ProjDirPath}/source/Driver_SPI.h"
"${ProjDirPath}/source/Driver_SPI.c"
"${ProjDirPath}/source/Driver_PWM.c"
"${ProjDirPath}/source/Driver_PINT.h"
"${ProjDirPath}/source/Driver_PINT.c"
"${ProjDirPath}/source/Driver_I2C.h"
"${ProjDirPath}/source/Driver_HX711.h"
"${ProjDirPath}/source/Driver_HX711.c"
"${ProjDirPath}/source/Driver_ADC.h"
"${ProjDirPath}/source/Driver_ADC.c"
"${ProjDirPath}/source/Dose_mode_fun.c"
"${ProjDirPath}/source/Dose_frame.c"
"${ProjDirPath}/source/Dosage_mode.h"
"${ProjDirPath}/source/Dosage_mode.c"
"${ProjDirPath}/source/Device_main_scrn.c"
"${ProjDirPath}/source/Data_Sample.c"
"${ProjDirPath}/source/custom_drug_list.c"
"${ProjDirPath}/source/Consumed_hrs.h"
"${ProjDirPath}/source/Color.h"
"${ProjDirPath}/source/Calibration_Screen.h"
"${ProjDirPath}/source/Calibration_Screen.c"
"${ProjDirPath}/source/Brand.h"
"${ProjDirPath}/source/Brand.c"
"${ProjDirPath}/source/Boot_up.h"
"${ProjDirPath}/source/Boot_up.c"
"${ProjDirPath}/source/Bolus_Screen.c"
"${ProjDirPath}/source/bolus_menu_scrn.c"
"${ProjDirPath}/source/bolus_limit.c"
"${ProjDirPath}/source/Bolus_Header.h"
"${ProjDirPath}/source/Bolus_Calculate..c"
"${ProjDirPath}/source/Bebas_60.c"
"${ProjDirPath}/source/battery_frames.c"
"${ProjDirPath}/source/Battery.h"
"${ProjDirPath}/source/Battery.c"
"${ProjDirPath}/source/Alarm.h"
"${ProjDirPath}/source/Alarm.c"
"${ProjDirPath}/source/AKAS_LOGO.c"
"${ProjDirPath}/source/Add_Drug.c"
"${ProjDirPath}/source/9_Arial.c"
"${ProjDirPath}/source/43_Bebas.c"
"${ProjDirPath}/source/36_bebas.c"
"${ProjDirPath}/source/36_Arial.c"
"${ProjDirPath}/source/20_Arial.c"
"${ProjDirPath}/source/14_Arial_Bold.c"
"${ProjDirPath}/source/14_Arial.c"
"${ProjDirPath}/source/12_Arial_Bold.c"
"${ProjDirPath}/source/12_Arial.c"
"${ProjDirPath}/startup/startup_lpc55s16.c"
)




if(${LIBRARY_TYPE} STREQUAL "REDLIB")
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE $ENV{ARMGCC_DIR}/redlib/include)
endif()

set(c_include_directories "${ProjDirPath}/source"
"${ProjDirPath}/gcov_support"
"${ProjDirPath}/board"
"${ProjDirPath}/freertos/freertos-kernel/include"
"${ProjDirPath}/drivers"
"${ProjDirPath}/device"
"${ProjDirPath}/CMSIS"
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:C>:${c_include_directories}>)

set(cpp_include_directories "")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${cpp_include_directories}>)

set(include_files "")


target_precompile_headers(${MCUX_SDK_PROJECT_NAME} PRIVATE ${include_files})

set(link_directories "")


set(link_libraries "")


target_link_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE ${link_directories})
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES} ${link_libraries})
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)





