@echo off
setlocal enabledelayedexpansion

SET expected_dir="TI Files"

for %%I in (.) do (
    if "%%~nxI" NEQ %expected_dir% (
        echo Script must remain in "TI Files" Directory
        set /p temp="Hit enter to exit script: "
        EXIT
    )
)

SET ZIP_NAME=compressed_resources.zip
SET ZIP_PATH="%CD%\%ZIP_NAME%"

if EXIST %ZIP_NAME% (
    If EXIST "ALL TI Resources" goto :FOUND
    call :UNZIP "%CD%" %ZIP_PATH%
    goto :FOUND
)
echo Could not find %ZIP_NAME%
set /p temp="Hit enter to exit script: "

:FOUND

REM Create temporary shortcut script ----------------------------------------------------------------------------------------
REM set SCRIPT="%TEMP%\%RANDOM%-%RANDOM%-%RANDOM%-%RANDOM%.vbs"
SET SC_SCRIPT="%CD%\sc.vbs"

echo Set oWS = WScript.CreateObject("WScript.Shell") >> %SC_SCRIPT%

echo args = WScript.Arguments.Count >> %SC_SCRIPT%

echo If args ^< 3 then >> %SC_SCRIPT% 

echo     WScript.Echo "usage: args.vbs argument [file_location] [shortcut_location] [current_loc]" >> %SC_SCRIPT%
echo     WScript.Quit >> %SC_SCRIPT%
echo end If >> %SC_SCRIPT%

echo sLinkFile = WScript.Arguments.Item(1) ^& ".lnk" >> %SC_SCRIPT%
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> %SC_SCRIPT%
echo oLink.TargetPath = WScript.Arguments.Item(0) >> %SC_SCRIPT%
echo oLink.WorkingDirectory = WScript.Arguments.Item(2) >> %SC_SCRIPT%
echo oLink.Save >> %SC_SCRIPT%

REM ------------------------------------------------------------------------------------------------------------------

SET short_path=%CD%\Useful Examples
if "%short_path%" NEQ "" (
    if EXIST "%short_path%" (
        rmdir "%short_path%" /s /q
    )
)

SET PATH_1="%CD%\ALL TI Resources\examples\nortos\MSP_EXP432P401R\driverlib"
SET PATH_2="%CD%\ALL TI Resources\examples\nortos\MSP_EXP432P401R\drivers"

call :SHORT_FROM_PATH %PATH_1%
call :SHORT_FROM_PATH %PATH_2%

goto :SCRIPT_END

:SHORT_FROM_PATH
SET orig_path=%~1

SET ADC=%short_path%\ADC
SET OBFS=%short_path%\File Systems\Onboard Flash
SET SDFS=%short_path%\File Systems\SD Card
SET GPIO_INT=%short_path%\GPIO Interrupts
SET I2C=%short_path%\I2C
SET PL=%short_path%\Power Levels
SET PWM=%short_path%\PWM
SET TIMER=%short_path%\Timer
SET UART=%short_path%\UART

SET ALL_TYPES[0]=%ADC%
SET ALL_TYPES[1]=%OBFS%
SET ALL_TYPES[2]=%SDFS%
SET ALL_TYPES[3]=%GPIO_INT%
SET ALL_TYPES[4]=%I2C%
SET ALL_TYPES[5]=%PL%
SET ALL_TYPES[6]=%PWM%
SET ALL_TYPES[7]=%TIMER%
SET ALL_TYPES[8]=%UART%

SET ALL_TYPES="%ADC%" "%OBFS%" "%SDFS%" "%GPIO_INT%" "%I2C%" "%PL%" "%PWM%" "%TIMER%" "%UART%"

for %%a in (%ALL_TYPES%) do (
    if EXIST %%a goto :SKIP
    MKDIR %%a
)
:SKIP

SET ADC_LIST=adc14_multiple_channel_no_repeat adc14_single_channel_external_reference adc14_single_channel_temperature_sensor adc14_single_comp_window_interrupt adc14_single_conversion_repeat adc14_single_conversion_repeat_timera_source adc14_single_differntial_2s_compliment adcbufcontinuous
SET OBFS_LIST=flash_mass_erase flash_nonblocking_erase flash_program_memory nvsinternal spiffsinternal
SET SDFS_LIST=fatsd fatsdraw sdraw
SET GPIO_INT_LIST=gpio_input_interrupt gpio_toggle_output gpiointerrupt
SET I2C_LIST=i2c_master_rw_repeated_start_single_byte_no_interrupt-master_code i2c_master_rw_repeated_start_single_byte-master_code i2c_master_rw_repeated_start_single_byte-slave_code i2c_master_rw_repeated_start-master_code i2c_master_rw_repeated_start-slave_code i2c_master_rw_timeout-master_code i2c_master_rw_timeout-slave_code i2c_master_w_multibyte_multislave-master_code i2c_master_w_multibyte_multislave-slave_code i2c_master_w_multibyte-master_code i2c_master_w_multibyte-slave_code i2c_slave_initiated_read-master_code i2c_slave_initiated_read-slave_code i2ctmp007
SET PL_LIST=
SET PWM_LIST=pwmled2
SET TIMER_LIST=timer_a_continuous_overflow_interrupt timer_a_continuous_vlo_period_capture timer_a_pwm_mode timer_a_updown_compare_multiple_pwm timer_a_upmode_gpio_toggle timer32_one_shot_free_run timer32_periodic_mode_led_toggle
SET UART_LIST=uart_loopback_24mhz_brclk uart_pc_echo_12mhz_brclk uartecho

SET ALL_FILES[0]=%ADC_LIST%
SET ALL_FILES[1]=%OBFS_LIST% 
SET ALL_FILES[2]=%SDFS_LIST% 
SET ALL_FILES[3]=%GPIO_INT_LIST% 
SET ALL_FILES[4]=%I2C_LIST% 
SET ALL_FILES[5]=%PL_LIST% 
SET ALL_FILES[6]=%PWM_LIST% 
SET ALL_FILES[7]=%TIMER_LIST% 
SET ALL_FILES[8]=%UART_LIST%

SET ALL_FILES=%ADC_LIST% %OBFS_LIST% %SDFS_LIST% %GPIO_INT_LIST% %I2C_LIST% %PL_LIST% %PWM_LIST% %TIMER_LIST% %UART_LIST%

for %%a in (%ALL_FILES%) do (
    call :GETTYPE %%a TYPE
    if "!TYPE!" NEQ "" (
        if EXIST "%orig_path%\%%a" (
            cscript /nologo %SC_SCRIPT% "%orig_path%\%%a" "!TYPE!\%%a" "%CD%"
        )
    )
)
goto :EOF

:GETTYPE
SET %2=
for /l %%a in (0, 1, 8) do (
    for %%b in (!ALL_FILES[%%a]!) do (
        if %%b==%1 (
            SET %2=!ALL_TYPES[%%a]!
            GOTO :EOF
        )
    )
)
echo none found
GOTO :EOF

:UNZIP
REM <ExtractTo> <newzipfile>
set UZ_SCRIPT="%CD%\uz.vbs" 
if exist %UZ_SCRIPT% del /f /q %UZ_SCRIPT%

echo Set fso = CreateObject("Scripting.FileSystemObject") >> %UZ_SCRIPT% 
echo ^If ^NOT fso.FolderExists(%1) ^Then >> %UZ_SCRIPT% 
echo fso.CreateFolder(%1) >> %UZ_SCRIPT% 
echo End If >> %UZ_SCRIPT% 
echo set objShell = CreateObject("Shell.Application") >> %UZ_SCRIPT% 
echo set FilesInZip=objShell.NameSpace(%2).items >> %UZ_SCRIPT% 
echo objShell.NameSpace(%1).CopyHere(FilesInZip) >> %UZ_SCRIPT% 
echo Set fso = Nothing >> %UZ_SCRIPT% 
echo Set objShell = Nothing >> %UZ_SCRIPT%
cscript //nologo %UZ_SCRIPT%
if exist %UZ_SCRIPT% del /f /q %UZ_SCRIPT% 
GOTO :EOF



:SCRIPT_END

REM DELETE TEMPORARY SCRIPT
del %SC_SCRIPT%
GOTO :EOF