TARGET    := sgl_simulator
BUILD_DIR := build


# toolchain
CC_PREFIX ?= 
CC = $(CC_PREFIX)gcc
AS = $(CC_PREFIX)gcc -x assembler-with-cpp
CP = $(CC_PREFIX)objcopy
SZ = $(CC_PREFIX)size
OD = $(CC_PREFIX)objdump
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

CPATH     := -Isdl/include/SDL2      \
			 -I../sgl/source         \
			 -I../sgl/source/include

CFLAGS    := $(CPATH) -O2 -Wl,-Bstatic -ffunction-sections -fdata-sections -nostdlib -ffreestanding -Wunused-function -Wall -Wextra -Werror -std=c99  -g
LDFLAGS   := -lmingw32 -lSDL2main -lSDL2.dll -Lsdl/lib -Wl,-Map=$(BUILD_DIR)/$(TARGET).map


SOURCE    := main.c sgl_port_sdl2.c  test.c bg.c  \
			../sgl/source/core/sgl_core.c    \
			../sgl/source/core/sgl_log.c     \
			../sgl/source/core/sgl_math.c    \
			../sgl/source/core/sgl_event.c   \
			../sgl/source/core/sgl_anim.c    \
			../sgl/source/mm/lwmem/lwmem.c   \
			../sgl/source/mm/lwmem/sgl_mm.c  \
			../sgl/source/draw/sgl_draw_line.c  \
			../sgl/source/draw/sgl_draw_rect.c  \
			../sgl/source/draw/sgl_draw_bar.c    \
			../sgl/source/draw/sgl_draw_circle.c  \
			../sgl/source/draw/sgl_draw_arc.c     \
			../sgl/source/draw/sgl_draw_text.c \
            ../sgl/source/draw/sgl_draw_ring.c  \
			../sgl/source/draw/sgl_draw_icon.c  \
			../sgl/source/widgets/line/sgl_line.c  \
			../sgl/source/widgets/rectangle/sgl_rectangle.c  \
			../sgl/source/widgets/circle/sgl_circle.c  \
			../sgl/source/widgets/ring/sgl_ring.c  \
			../sgl/source/widgets/arc/sgl_arc.c   \
			../sgl/source/widgets/button/sgl_button.c   \
			../sgl/source/widgets/slider/sgl_slider.c   \
			../sgl/source/widgets/label/sgl_label.c   \
			../sgl/source/widgets/switch/sgl_switch.c   \
			../sgl/source/widgets/msgbox/sgl_msgbox.c   \
			../sgl/source/widgets/textline/sgl_textline.c  \
			../sgl/source/widgets/textbox/sgl_textbox.c    \
			../sgl/source/widgets/checkbox/sgl_checkbox.c   \
			../sgl/source/widgets/listview/sgl_listview.c   \
			../sgl/source/widgets/icon/sgl_icon.c     \
			../sgl/source/widgets/numberkbd/sgl_numberkbd.c  \
			../sgl/source/widgets/keyboard/sgl_keyboard.c   \
			../sgl/source/fonts/sgl_ascii_song23.c         \
			../sgl/source/fonts/sgl_ascii_consolas23.c     \
			../sgl/source/fonts/sgl_ascii_consolas14.c     \
			../sgl/source/fonts/sgl_ascii_kai33.c

.PHONY: config all
all: config $(BUILD_DIR)/$(TARGET).exe elf_info

config:
	@echo "copy sgl_config.h..."
	@cp sgl_config.h ../sgl/source/sgl_config.h


# list of c and c++ program objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(patsubst %.c, %.o, $(SOURCE))))
vpath %.c $(sort $(dir $(SOURCE)))


$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo "CC   $<"
	@$(CC) -c $(CFLAGS) -MMD -MP \
		-MF  $(BUILD_DIR)/$(notdir $(<:.c=.d)) \
		-Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@


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
.PHONY: clean run


run: $(BUILD_DIR)/$(TARGET).exe
	@cp sdl/bin/SDL2.dll $(BUILD_DIR)/SDL2.dll
	@$(BUILD_DIR)/$(TARGET).exe


# clean command, delete build directory
clean:
	rm -rf $(BUILD_DIR)
