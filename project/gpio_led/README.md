# STM32F072 GPIO LED 实例

## Environment:
- Ubuntu 16.04
- arm-none-eabi-gcc 8.3
- JLinkExe 

## Notes
- notes\[STM32F072]Note1-GPIO.md

## Usage
```
# clean up
make clean

# compile and link the project
make

# upload the code to stm32f103
make burn
# burn fireware build/STM32_Template.hex to STM32 Address 0x08000000
# JLink Burnning Commnader in stm32.jlink file.
```

