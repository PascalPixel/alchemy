# Host utilities used by the ROM build. Run from the repository root.
HOST_CC ?= cc
HOST_CFLAGS ?= -O2
HOST_LDFLAGS ?=
PKG_CONFIG ?= pkg-config
OUT := tools/out
OBJ := $(OUT)/obj
NAMES := graphics audio music text objects deps overlays header
BINARIES := $(addprefix $(OUT)/ags-,$(NAMES))
# Shared runtime, codecs, and assembly output are linked only where needed.
COMMON := $(addprefix $(OBJ)/common/,common.o files.o strings.o)
COMPRESS := $(addprefix $(OBJ)/common/,compress.o lz.o arena.o)
ASSEMBLY := $(OBJ)/common/assembly.o
PNG_CFLAGS = $(shell $(PKG_CONFIG) --cflags libpng 2>/dev/null)
PNG_LIBS = $(shell $(PKG_CONFIG) --libs libpng 2>/dev/null)
UTF8_CFLAGS = $(shell $(PKG_CONFIG) --cflags libutf8proc 2>/dev/null)
UTF8_LIBS = $(shell $(PKG_CONFIG) --libs libutf8proc 2>/dev/null)
SOURCES := $(wildcard tools/ags-*/*.c tools/common/*.c tools/tests/*.c)
OBJECTS := $(patsubst tools/%.c,$(OBJ)/%.o,$(SOURCES))

.PHONY: all test clean dependencies
all: dependencies $(BINARIES)

dependencies:
	@command -v $(PKG_CONFIG) >/dev/null || { echo 'Install pkg-config (see INSTALL.md).' >&2; exit 1; }
	@$(PKG_CONFIG) --exists libpng libutf8proc || { echo 'Install the libpng and utf8proc development packages (see INSTALL.md).' >&2; exit 1; }
	@printf '%s\n' -xc -std=c11 $(PNG_CFLAGS) $(UTF8_CFLAGS) > tools/compile_flags.txt

$(OBJECTS): | dependencies

$(OUT)/ags-graphics: $(patsubst tools/%.c,$(OBJ)/%.o,$(wildcard tools/ags-graphics/*.c)) $(COMMON) $(COMPRESS) $(ASSEMBLY)
	$(HOST_CC) $^ $(HOST_LDFLAGS) $(PNG_LIBS) -o $@
$(OUT)/ags-audio: $(OBJ)/ags-audio/ags-audio.o $(COMMON)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -lm -o $@
$(OUT)/ags-music: $(patsubst tools/%.c,$(OBJ)/%.o,$(wildcard tools/ags-music/*.c)) $(COMMON) $(ASSEMBLY)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -o $@
$(OUT)/ags-text: $(patsubst tools/%.c,$(OBJ)/%.o,$(wildcard tools/ags-text/*.c)) $(COMMON) $(ASSEMBLY)
	$(HOST_CC) $^ $(HOST_LDFLAGS) $(UTF8_LIBS) -o $@
$(OUT)/ags-objects: $(OBJ)/ags-objects/ags-objects.o $(COMMON)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -o $@
$(OUT)/ags-deps: $(OBJ)/ags-deps/ags-deps.o $(COMMON)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -o $@
$(OUT)/ags-overlays: $(OBJ)/ags-overlays/ags-overlays.o $(COMMON) $(COMPRESS)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -o $@
$(OUT)/ags-header: $(OBJ)/ags-header/ags-header.o $(COMMON)
	$(HOST_CC) $^ $(HOST_LDFLAGS) -o $@

$(OBJ)/ags-graphics/%.o: CPPFLAGS += $(PNG_CFLAGS)
$(OBJ)/ags-text/%.o: CPPFLAGS += $(UTF8_CFLAGS)
$(OBJ)/%.o: tools/%.c
	@mkdir -p $(@D)
	$(HOST_CC) $(CPPFLAGS) $(HOST_CFLAGS) -std=c11 -Wall -Wextra -Wpedantic -MMD -MP -c $< -o $@

test: all $(OUT)/test-tools
	$(OUT)/test-tools $(OUT)
$(OUT)/test-tools: $(OBJ)/tests/tests.o $(OBJ)/ags-graphics/graphics.o $(OBJ)/ags-graphics/image.o $(COMMON) $(COMPRESS) $(ASSEMBLY)
	$(HOST_CC) $^ $(HOST_LDFLAGS) $(PNG_LIBS) -o $@

clean:
	$(RM) $(BINARIES) $(OUT)/test-tools
	$(RM) -r $(OBJ)

-include $(OBJECTS:.o=.d)
