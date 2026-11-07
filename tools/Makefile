# Host utilities used by the ROM build. Run from the repository root.
HOST_CC ?= cc
HOST_CFLAGS ?= -O2
HOST_LDFLAGS ?=
LIBPNG_CONFIG ?= libpng-config
OUT := tools/out
OBJ := $(OUT)/obj
NAMES := graphics audio music text objects deps overlays header enums
BINARIES := $(addprefix $(OUT)/ags-,$(NAMES))
# Shared runtime, codecs, and assembly output are linked only where needed.
SHARED := $(addprefix $(OBJ)/shared/,shared.o files.o strings.o)
COMPRESS := $(addprefix $(OBJ)/shared/,compress.o lz.o arena.o)
ASSEMBLY := $(OBJ)/shared/assembly.o
# libpng-config ships with libpng's development package on every system.
PNG_CFLAGS = $(shell $(LIBPNG_CONFIG) --cflags 2>/dev/null)
PNG_LIBS = $(shell $(LIBPNG_CONFIG) --ldflags 2>/dev/null)
SOURCES := $(wildcard tools/ags-*/*.c tools/shared/*.c tools/tests/*.c)
OBJECTS := $(patsubst tools/%.c,$(OBJ)/%.o,$(SOURCES))

.PHONY: all test clean dependencies
all: dependencies $(BINARIES)

dependencies:
	@command -v $(LIBPNG_CONFIG) >/dev/null || { echo 'Install the libpng development package (see INSTALL.md).' >&2; exit 1; }
	@printf '%s\n' -xc -std=c11 $(PNG_CFLAGS) > tools/compile_flags.txt

$(OBJECTS): | dependencies

$(OUT)/ags-graphics: $(patsubst tools/%.c,$(OBJ)/%.o,$(wildcard tools/ags-graphics/*.c)) $(SHARED) $(COMPRESS) $(ASSEMBLY)
	$(HOST_CC) $^ $(HOST_LDFLAGS) $(PNG_LIBS) -o $@
$(OUT)/ags-audio: $(OBJ)/ags-audio/ags-audio.o $(SHARED)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -lm -o $@
$(OUT)/ags-music: $(patsubst tools/%.c,$(OBJ)/%.o,$(wildcard tools/ags-music/*.c)) $(SHARED) $(ASSEMBLY)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -o $@
$(OUT)/ags-text: $(patsubst tools/%.c,$(OBJ)/%.o,$(wildcard tools/ags-text/*.c)) $(SHARED) $(ASSEMBLY)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -o $@
$(OUT)/ags-objects: $(OBJ)/ags-objects/ags-objects.o $(SHARED)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -o $@
$(OUT)/ags-deps: $(OBJ)/ags-deps/ags-deps.o $(SHARED)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -o $@
$(OUT)/ags-overlays: $(OBJ)/ags-overlays/ags-overlays.o $(SHARED) $(COMPRESS)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -o $@
$(OUT)/ags-header: $(OBJ)/ags-header/ags-header.o $(SHARED)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -o $@
$(OUT)/ags-enums: $(OBJ)/ags-enums/ags-enums.o $(SHARED)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -o $@

$(OBJ)/ags-graphics/%.o: CPPFLAGS += $(PNG_CFLAGS)
$(OBJ)/%.o: tools/%.c
	@mkdir -p $(@D)
	$(HOST_CC) $(CPPFLAGS) $(HOST_CFLAGS) -std=c11 -Wall -Wextra -Wpedantic -MMD -MP -c $< -o $@

test: all $(OUT)/test-tools
	$(OUT)/test-tools $(OUT)
$(OUT)/test-tools: $(OBJ)/tests/tests.o $(OBJ)/ags-graphics/graphics.o $(OBJ)/ags-graphics/image.o $(SHARED) $(COMPRESS) $(ASSEMBLY)
	$(HOST_CC) $^ $(HOST_LDFLAGS) $(PNG_LIBS) -o $@

clean:
	$(RM) $(BINARIES) $(OUT)/test-tools
	$(RM) -r $(OBJ)

-include $(OBJECTS:.o=.d)
