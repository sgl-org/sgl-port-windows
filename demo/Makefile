TARGET    := sgl_simulator
BUILD_DIR := build

# Detect OS
ifeq ($(OS),Windows_NT)
    CONFIG = cmd /c copy /Y sgl_config.h ..\sgl\source\sgl_config.h
    SDL_COPY = cmd /c copy /Y sdl\bin\SDL2.dll $(BUILD_DIR)\SDL2.dll
    CLEAN = cmd /c if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)
    MKDIR = cmd /c if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
else
    CONFIG = cp sgl_config.h ../sgl/source/sgl_config.h
    SDL_COPY = cp sdl/bin/SDL2.dll $(BUILD_DIR)/SDL2.dll
    CLEAN = rm -rf $(BUILD_DIR)
    MKDIR = mkdir -p $(BUILD_DIR)
endif


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

CFLAGS    := $(CPATH) -O2 -ffunction-sections -fdata-sections -Wunused-function -Wall -Wextra -std=c99 -g
LDFLAGS   := -Lsdl/lib -lmingw32 -lSDL2main -lSDL2 -mwindows -lm -ldinput8 -ldxguid -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -Wl,-Map=$(BUILD_DIR)/$(TARGET).map


SOURCE    := main.c sgl_port_sdl2.c  test.c bg.c \
			../sgl/source/core/sgl_core.c    \
			../sgl/source/core/sgl_log.c     \
			../sgl/source/core/sgl_math.c    \
			../sgl/source/core/sgl_event.c   \
			../sgl/source/core/sgl_misc.c   \
			../sgl/source/core/sgl_anim.c    \
			../sgl/source/core/sgl_snprintf.c \
			../sgl/source/mm/lwmem/lwmem.c   \
			../sgl/source/mm/lwmem/sgl_mm.c  \
			../sgl/source/draw/sgl_draw_line.c  \
			../sgl/source/draw/sgl_draw_rect.c  \
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
			../sgl/source/widgets/icon/sgl_icon.c     \
			../sgl/source/widgets/numberkbd/sgl_numberkbd.c  \
			../sgl/source/widgets/keyboard/sgl_keyboard.c   \
			../sgl/source/widgets/2dball/sgl_2dball.c      \
			../sgl/source/widgets/led/sgl_led.c      \
			../sgl/source/fonts/sgl_ascii_song23.c         \
			../sgl/source/fonts/sgl_ascii_consolas23.c     \
			../sgl/source/fonts/sgl_ascii_consolas24.c     \
			../sgl/source/fonts/sgl_ascii_consolas14.c     


.PHONY: config all
all: config $(BUILD_DIR)/$(TARGET).exe elf_info

config:
	@echo "copy sgl_config.h..."
	@$(CONFIG)


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
	@$(OD) $(BUILD_DIR)/$(TARGET).exe -xS > $(BUILD_DIR)/$(TARGET).s
	@echo "Build Successful!"


elf_info: $(BUILD_DIR)/$(TARGET).exe
	@echo "=================================================================="
	@$(SZ) $<
	@echo "=================================================================="


$(BUILD_DIR):
	@$(MKDIR)


# Pseudo command
.PHONY: clean run


run: $(BUILD_DIR)/$(TARGET).exe
	@$(SDL_COPY)
	@$(BUILD_DIR)/$(TARGET).exe


# clean command, delete build directory
clean:
	@$(CLEAN)
