LM = ../lite-manager/source/lm.exe


TARGET    := sgl_demo
BUILD_DIR := build


# include configuration file for makefile
.PHONY: check_lmmk
ifneq ($(wildcard .lm.mk),)
-include .lm.mk
else
check_lmmk:
	@echo "Please run 'make config'"
endif


# disable print directory for make
MAKEFLAGS += --no-print-directory


# toolchain
CC_PREFIX ?= 
CC = $(CC_PREFIX)gcc
AS = $(CC_PREFIX)gcc -x assembler-with-cpp
CP = $(CC_PREFIX)objcopy
SZ = $(CC_PREFIX)size
OD = $(CC_PREFIX)objdump
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


CFLAGS    := $(MC_FLAG) $(C_PATH) $(C_DEFINE) $(C_FLAG) $(CPP_FLAG)
ASFLAGS   := $(MC_FLAG) $(C_PATH) $(C_DEFINE) $(AS_FLAG) 
LDFLAGS   := $(MC_FLAG) $(LD_FLAG) $(LIB_PATH) $(LIB_NAME) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map


.PHONY: all
all: $(BUILD_DIR)/$(TARGET).exe elf_info


# list of c and c++ program objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(C_SOURCE)))))
vpath %.c $(sort $(dir $(C_SOURCE)))
vpath %.cpp $(sort $(dir $(C_SOURCE)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCE:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCE)))


$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo "CC   $<"
	@$(CC) -c $(CFLAGS) -MMD -MP \
		-MF  $(BUILD_DIR)/$(notdir $(<:.c=.d)) \
		-Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	@echo "CC   $<"
	@$(CC) -c $(CFLAGS) -MMD -MP \
		-MF  $(BUILD_DIR)/$(notdir $(<:.cpp=.d)) \
		-Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	@echo "AS   $<"
	@$(AS) -c $(ASFLAGS) -MMD -MP  \
		-MF $(BUILD_DIR)/$(notdir $(<:.S=.d)) $< -o $@


$(BUILD_DIR)/$(TARGET).exe: $(OBJECTS) Makefile
	@echo "LD   $@"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(OD) $(BUILD_DIR)/$(TARGET).exe -xS > $(BUILD_DIR)/$(TARGET).s $@
	@echo ''
	@echo "Build Successful!"
	@echo "ELF   $@"


elf_info: $(BUILD_DIR)/$(TARGET).exe
	@echo "=================================================================="
	@$(SZ) $<
	@echo "=================================================================="


$(BUILD_DIR):
	@mkdir $@


# Pseudo command
.PHONY: config clean run


run: $(BUILD_DIR)/$(TARGET).exe
	@$(LM) --cp sdl/bin/SDL2.dll $(BUILD_DIR)/SDL2.dll
	@$(BUILD_DIR)/$(TARGET).exe


# Check if the proj.cfg file exists
config: $(LM) .config
	@$(LM) --projcfg .config --lmcfg lm.cfg --out ../sgl/source/sgl_config.h --mk .lm.mk --mem 50
	@$(MAKE) clean

# clean command, delete build directory
clean:
	@$(LM) --rm $(BUILD_DIR)


# lite-manager tool build
$(LM):
	@$(MAKE) -C $(dir $(LM)) -j4
