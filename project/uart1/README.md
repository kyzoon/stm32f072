# STM32F103 ARM-GCC Template
**Info: All about this are test under Windows 10**

This is a template project for stm32f103 microcontroller, and try to driving an LED.

## Environment requires:
- [GNU Tools for ARM Embedded Processors toolchain](https://launchpad.net/gcc-arm-embedded) (compiler, objcopy)
- [SEGGER JLink](https://www.segger.com/products/debug-probes/j-link/) (uploading fireware to STM32)
- [STM32 Standard Peripheral Libraries](http://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32-standard-peripheral-libraries/stsw-stm32054.html) (template use V3.5.0)

## Directories and Files
- **.vscode** - VSCode config file. In tasks.json，there're three task: build/burn/Start GDB Server. Use 'Ctrl+Alt+T' to call it.
- **app** - Application Layer Code
- **bsp** - BSP/Hardware Layer Code
- **build** - Compiler Files
- **core** - About Cortex-M3 Code
- **middle** - Middle Layer Code
- **stdlib** - STD Peripheral Lib代码，V3.5.0
- **stm32.jlink** - JLink Config file，use SEGGER JLink CL burn fireware to STM32, Require JLink.

## Usage
`arm-none-eabi-gcc`, `arm-none-eabi-objcopy`, `make`, `jlink` and `jlinkgdbservercl` should be in `${PATH}`.
(Make sure JLink connect to the target board, and power on it.)
```
# clean up
make clean

# compile and link the project
make all

# upload the code to stm32f103
make burn
# burn fireware build/STM32_Template.hex to STM32 Address 0x08000000
# JLink Burnning Commnader in stm32.jlink file.
```

Debug:
```
# Press 'Ctrl+Alt+T' and select 'Start gdb server'
# Check GDB Server startup

# Prese 'F5' to Debug
# Use arm-none-eabi-gdb, debug fireware build/STM32_Template.elf

# JLink GDB Server CL Commander
jlinkgdbservercl -device STM32F103ZE -if SWD
# other parameters use default is ok

# arm-none-eabi-gdb Commander
target remote localhost:2331
file /path/STM32_Template.elf
monitor halt
monitor reset
load
c
q
```
