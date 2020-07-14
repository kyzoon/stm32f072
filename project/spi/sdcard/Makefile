########################################
# Makefile for STM32F072, Ubuntu 16.04
########################################
TARGET=stm32f072

SRC_DIR = $(shell find . -maxdepth 2 -type d)
OBJ_DIR=build

SRC_C = $(foreach dir, ${SRC_DIR}, $(wildcard $(dir)/*.c))
SRC_S = $(foreach dir, ${SRC_DIR}, $(wildcard $(dir)/*.s))
SRC_LD = $(strip $(foreach dir, ${SRC_DIR}, $(wildcard $(dir)/*.ld)))

INC_PATH = $(foreach dir, ${SRC_DIR}, $(join -I, $(dir)))

OBJ_C = $(patsubst %.c, %.o, $(notdir ${SRC_C}))
OBJ_S = $(patsubst %.s, %.o, $(notdir ${SRC_S}))
OBJS = ${OBJ_S} ${OBJ_C}
OBJS_PATH = $(foreach f, ${OBJS}, ${OBJ_DIR}/$(f))
OBJS_DEP = $(patsubst %.o, %.d, ${OBJS})

# Make sure if ${OBJ_DIR} exist, if not then create it
$(shell [ -d ${OBJ_DIR} ] || mkdir -p ${OBJ_DIR})

VPATH := ${SRC_DIR}

PREFIX = arm-none-eabi-
CC = ${PREFIX}gcc
AS = ${PREFIX}gcc -x assembler-with-cpp
CP = ${PREFIX}objcopy
SZ = ${PREFIX}size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

# Solve diff host palatform use diff JLink command
ifeq ($(OS), Windows_NT)
	JLINK = JLink
else
	JLINK = JLinkExe
endif

CPU = -mcpu=cortex-m0
FPU = 
FLOAT-ABI =
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

C_DEFS = \
-DUSE_STDPERIPH_DRIVER \
-DSTM32F072

LIBS = -lc -lm -lnosys
LIBDIR =

DEP_ASFLAGS = -MM -MT "$(subst .s,.o,${notdir $<}) $(subst .s,.d,${notdir $<})" -MF "$(subst .s,.d,${OBJ_DIR}/${notdir $<})"
DEP_CFLAGS = -MM -MT "$(subst .c,.o,${notdir $<}) $(subst .c,.d,${notdir $<})" -MF "$(subst .c,.d,${OBJ_DIR}/${notdir $<})"

ASFLAGS = $(MCU) $(AS_DEFS) -Wall -fdata-sections -ffunction-sections
CFLAGS = $(MCU) -Wall -Og -fsigned-char -fno-builtin -fdata-sections -ffunction-sections -g -gdwarf-2 $(C_DEFS)
LFLAGS = $(MCU) -specs=nano.specs -T$(SRC_LD) $(LIBDIR) $(LIBS) -Wl,-Map=$(OBJ_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -u _printf_float

#############################################################################################
# Main Target
#############################################################################################
.PHONY: all

all: ${OBJ_DIR}/${TARGET}.elf ${OBJ_DIR}/${TARGET}.hex ${OBJ_DIR}/${TARGET}.bin

.PHONY: test
test:
	@echo '###########'

${OBJ_DIR}/%.hex: ${OBJ_DIR}/%.elf
	$(HEX) $< $@

${OBJ_DIR}/%.bin: ${OBJ_DIR}/%.elf
	$(BIN) $< $@

${OBJ_DIR}/${TARGET}.elf: ${OBJS:.o=.d} ${OBJS} 
	${CC} ${OBJS_PATH} -o $@ ${LFLAGS}
	${SZ} $@

%.o: %.c
	${CC} $(CFLAGS) ${INC_PATH} -o ${OBJ_DIR}/$@ -c $<

%.o: %.s
	${AS} $(ASFLAGS) -o ${OBJ_DIR}/$@ -c $<

%.d: %.c
	${CC} $(CFLAGS) $(INC_PATH) $< $(DEP_CFLAGS)

%.d: %.s
	${AS} $(ASFLAGS) $(INC_PATH) $< $(DEP_ASFLAGS)

.PHONY: rebuild
rebuild:
	-rm -rf build
	$(MAKE)

.PHONY: clean
clean:
	-rm -rf build

.PHONY: burn
burn:
	$(JLINK) stm32.jlink
