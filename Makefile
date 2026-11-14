# Golden Sun: The Broken Seal ☀️ (tbs) and The Lost Age ⚓️ (tla), in six
# languages each: ja en de es fr it. `make tbs-en` builds one edition into
# out/tbs-en/tbs-en.gba; `make compare-tbs-en` also checks it against
# rom.sha1. Every rule the ROM is built by is in this file.

ifneq ($(words $(CURDIR)),1)
$(error The checkout path "$(CURDIR)" contains a space, which make cannot build from: move or rename it so the path has none)
endif

GAMES    := tbs tla
LANGS    := ja en de es fr it
EDITIONS := $(foreach game,$(GAMES),$(addprefix $(game)-,$(LANGS)))
TBS      := $(filter tbs-%,$(EDITIONS))
TLA      := $(filter tla-%,$(EDITIONS))

# make SHIFT=n builds the shift test (below), which never matches rom.sha1.
ifneq ($(and $(SHIFT),$(filter compare%,$(MAKECMDGOALS))),)
$(error SHIFT builds the shift test, which never matches rom.sha1: compare takes no SHIFT)
endif

CPUS := $(shell sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)
JOBS ?= $(CPUS)
SHA1 := $(shell { command -v sha1sum || command -v shasum; } 2>/dev/null) -c
MAKEFLAGS += --no-print-directory
.SUFFIXES:
# Keep every file the build makes, overlay ELFs and objects included: the
# matching aids read them (as pret keeps its intermediates).
.SECONDARY:

# Plain `make` builds the English Broken Seal, the compilers first if they are
# missing; `make compare` checks it against rom.sha1.
.DEFAULT_GOAL := tbs-en
.PHONY: help all compare $(EDITIONS) $(addprefix compare-,$(EDITIONS)) compare-tbs compare-tla compare-editions clean

help:
	@printf '%s\n' \
	  'make                    build tbs-en (the compilers first, once); make compare checks it' \
	  'make bootstrap          build the pinned compilers and tools into tools/out' \
	  'make tbs-en             build one edition: tbs or tla, then ja en de es fr it' \
	  'make compare-tbs-en     build one edition and check it against rom.sha1' \
	  'make compare-tbs        the six ☀️ editions' \
	  'make compare-editions   all twelve editions' \
	  'make modern             all twelve editions'\'' C through a current GCC (MODERN_CC)' \
	  'make test               test the C build tools, the format and the fake-match devices' \
	  'make SHIFT=4 tbs-en     the shift test: everything after the header moves 4 bytes'

# Each edition builds in its own make, so its variables below are set once;
# the tools and compilers are built first, once. GNU Make 3.81, macOS's make,
# busy-waits when a parallel make waits for a job, so there each edition
# builds serially while the editions still build side by side.
SERIAL := $(if $(filter 3.%,$(MAKE_VERSION)),-j1)
$(EDITIONS): bootstrap
	@$(MAKE) $(SERIAL) -f Makefile EDITION=$@ rom

$(addprefix compare-,$(EDITIONS)): compare-%: %
	@grep -F ' out/$*/$*.gba' rom.sha1 | $(SHA1) -

compare-tbs: $(addprefix compare-,$(TBS))
compare-tla: $(addprefix compare-,$(TLA))
compare-editions: compare-tbs compare-tla
compare: compare-tbs-en

# make modern: all twelve editions' C through a current GCC (MODERN_CC);
# modern-tbs and modern-tla for one game's six.
ifndef EDITION
.PHONY: modern modern-tbs modern-tla
modern-tbs: $(addprefix modern-,$(TBS))
modern-tla: $(addprefix modern-,$(TLA))
modern: modern-tbs modern-tla
modern-%: tools
	@$(MAKE) $(SERIAL) -f Makefile EDITION=$* modern
endif

clean:
	rm -rf out
	find games -type f \( $(foreach suffix,$(ASSET_SUFFIXES),-name '*.$(suffix)' -o -name '*.$(suffix).d' -o) -false \) -delete

# --- Tools ---------------------------------------------------------------

TOOLS    := tools/out
COMPILER := tools/out/compilers

XGCC   := $(COMPILER)/xgcc -B$(COMPILER)/
CPP    := $(COMPILER)/cpp0
AGBCC  := $(COMPILER)/agbcc/old_agbcc
ARMCC  := $(COMPILER)/gcc2952_arm
CCAS   := $(COMPILER)/as
CCASFLAGS := -marm7tdmi
PREFIX := arm-none-eabi-
AS      := $(PREFIX)as
LD      := $(PREFIX)ld
OBJCOPY := $(PREFIX)objcopy
NM      := $(PREFIX)nm
LIBGCC  := tools/out/compiler-runtime/libgcc.a

AGSGFX  := $(TOOLS)/ags-graphics
WAV2AGS := $(TOOLS)/ags-audio
MID2AGS := $(TOOLS)/ags-music
PO2AGS  := $(TOOLS)/ags-text
SCANINC := $(TOOLS)/ags-deps
OVLPACK := $(TOOLS)/ags-overlays
GBAFIX  := $(TOOLS)/ags-header
ENUMS   := $(TOOLS)/ags-enums

# --- Assets (A1) ---------------------------------------------------------
# A picture or table is built beside its source, in every edition the same:
# games/.../NAME.FORM[.CODEC] from NAME.png, or from NAME.tsv for a table
# form. ags-graphics reads the form and codec from the file name it writes.

PICTURE_FORMS := 1bpp 4bpp 4bpp16x16 4bpp16x32 4bpp32x8 4bpp32x16 4bpp32x32 4bpp64x64 \
                 8bpp 8bpp8x16 8bpp16x16 8bpp64x64 bitmap font gbapal glyphs icon4 icons pictures strips
TABLE_FORMS   := blocks delta0 delta1 delta2 icons4 parts plane scene script spriteblocks sprites table
CODECS        := lz plz mtf d7 hlz
FORMS         := $(PICTURE_FORMS) $(TABLE_FORMS)
ASSET_SUFFIXES := $(FORMS) $(foreach form,$(FORMS),$(addprefix $(form).,$(CODECS))) \
                  $(addprefix bitmaps.,$(CODECS)) bin grid.lz tilemap.lz collision.lz

# Every file an encoder reads is a prerequisite: a compressed asset's packer
# patch (NAME_patch.tsv, W3) where it has one, and whatever else a recipe
# reads (a part list's parts, a sprite bank's sprites), which ags-graphics
# lists in NAME.FORM[.CODEC].d beside what it writes, as a compiler's -MMD.
.SECONDEXPANSION:
PATCH := $$$$(wildcard $$$$(basename $$$$(basename $$$$@))_patch.tsv)

$(foreach form,$(PICTURE_FORMS),$(eval %.$(form): %.png $$(AGSGFX) ; $$(AGSGFX) $$< $$@))
$(foreach form,$(PICTURE_FORMS),$(foreach codec,$(CODECS),$(eval %.$(form).$(codec): %.png $(PATCH) $$(AGSGFX) ; $$(AGSGFX) $$< $$@)))
$(foreach form,$(TABLE_FORMS),$(eval %.$(form): %.tsv $$(AGSGFX) ; $$(AGSGFX) $$< $$@))
$(foreach form,$(TABLE_FORMS),$(foreach codec,$(CODECS),$(eval %.$(form).$(codec): %.tsv $(PATCH) $$(AGSGFX) ; $$(AGSGFX) $$< $$@)))

# An animation whose pictures differ in size, so that no one sheet holds them:
# NAME.bitmaps.CODEC from NAME_00.png, NAME_01.png and on, each picture's
# pixels in turn.
PICTURE_RUN := $$$$(wildcard $$$$(basename $$$$(basename $$$$@))_[0-9][0-9].png)
$(foreach codec,$(CODECS),$(eval %.bitmaps.$(codec): %_00.png $(PICTURE_RUN) $(PATCH) $$(AGSGFX) ; $$(AGSGFX) $$< $$@))

# A map's grid: map.txt, one cell per square, builds the three planes the
# game reads.
%.grid.lz: %.txt $$(wildcard $$(basename $$(basename $$@))_patch.tsv) $(AGSGFX) ; $(AGSGFX) $< $@

# A background's tile map as the screen shows it, and the world map's
# collision tiles as the game reads them: text grids, like map.txt.
%.tilemap.lz: %.txt $$(wildcard $$(basename $$(basename $$@))_patch.tsv) $(AGSGFX) ; $(AGSGFX) $< $@
%.collision.lz: %.txt $$(wildcard $$(basename $$(basename $$@))_patch.tsv) $(AGSGFX) ; $(AGSGFX) $< $@

# Sound samples: NAME.pcm8.bin from NAME.pcm8.wav, NAME.bin from NAME.pcm4.
%.pcm8.bin: %.pcm8.wav $(WAV2AGS) ; $(WAV2AGS) $< $@
%.bin: %.pcm4 $(WAV2AGS) ; $(WAV2AGS) $< $@

# --- One edition ---------------------------------------------------------

ifdef EDITION
GAME  := $(word 1,$(subst -, ,$(EDITION)))
LANG  := $(word 2,$(subst -, ,$(EDITION)))
# make SHIFT=n tbs-en is the shift test: everything after the header moves n
# bytes (a multiple of 4), built into out/shift-n/ so out/<edition> is untouched.
BUILD := out/$(if $(SHIFT),shift-$(SHIFT)/)$(EDITION)
ROM   := $(BUILD)/$(EDITION).gba
ELF   := $(BUILD)/$(EDITION).elf

UPPER_tbs := TBS
UPPER_tla := TLA
UPPER_ja := JA
UPPER_en := EN
UPPER_de := DE
UPPER_es := ES
UPPER_fr := FR
UPPER_it := IT
DEFINE := $(UPPER_$(GAME))_EDITION_$(UPPER_$(LANG))

# The Game Pak's last address: objcopy fills the image to it with zeros.
ROM_END_tbs := 0x08800000
ROM_END_tla := 0x09000000

# One include path, as pret has: a header names its bank folder in its
# path (#include "field/actor.h").
INCLUDES := -Igames/$(GAME)/include

# Game C (K1, K2): alchemy-gcc's GCC of July 2000, one option set per game;
# ⚓️'s overlays swap -O2 for -Os below, by file and place in the build.
CFLAGS_tbs := -O2 -mthumb -mthumb-interwork -nostdinc -fcall-used-r4
CFLAGS_tla := -O2 -mthumb -nostdinc -fcall-used-r4 -mtune=gba
CFLAGS := $(CFLAGS_$(GAME)) $(INCLUDES) -D$(DEFINE)=1 -I$(BUILD)

# The sound and flash libraries: pret's old_agbcc. ARM C: GCC 2.95.2.
CPPFLAGS_ARM := -lang-c -nostdinc '-Acpu(arm)' '-Amachine(arm)' -D__CHAR_UNSIGNED__ -D__OPTIMIZE__ \
                -D__ARM_ARCH_4T__ -D__APCS_32__ -D__ARMEL__ -Darm_elf -D__ELF__
CPPFLAGS_AGBCC := $(CPPFLAGS_ARM) -Igames/$(GAME)/include -D__GNUC__=2 -D__GNUC_MINOR__=9 \
                  -D__GNUC_PATCHLEVEL__=0 -D__THUMBEL__ -Dthumb -D__thumb__ -D$(DEFINE)=1 -I$(BUILD)
CPPFLAGS_ARMCC := $(CPPFLAGS_ARM) $(INCLUDES) \
                  -D__GNUC__=2 -D__GNUC_MINOR__=95 -D__GNUC_PATCHLEVEL__=2 -Darm -D__arm__ -D$(DEFINE)=1 -I$(BUILD)
SOUND_C := games/tbs/src/sound/audio.c games/tbs/src/sound/tables.c games/tbs/src/sound/driver.c \
           games/tla/src/sound/sound.c games/tla/src/sound/tables.c games/tla/src/sound/player_work_copy.c \
           games/tla/src/sound/driver.c
FLASH_C := $(addprefix games/tbs/src/gamepak/flash/,flash.c sst.c atmel.c program_flash_sector.c \
                         flashtab.c flashsst.c flashmx.c flashat.c) \
           $(addprefix games/tla/src/gamepak/flash/,flash.c sst.c atmel.c program_flash_sector.c)
ARM_C   := games/tbs/src/window/glyph_arm.c games/tbs/src/field/actor_update_arm.c games/tbs/src/field/world_map_lines.c \
           games/tla/src/window/cell_picture.c games/tla/src/window/glyph_arm.c games/tla/src/field/grid_outline.c \
           games/tla/src/field/world_map_lines.c

ASFLAGS := -mcpu=arm7tdmi -mthumb-interwork
EDITION_ASFLAGS := --defsym $(DEFINE)=1 -I$(BUILD) $(ASFLAGS) $(if $(SHIFT),--defsym ROM_SHIFT=$(SHIFT))

OVERLAY_STREAM := games/$(GAME)/src/system/overlay/overlays
# The linker script: games/$(GAME)/ld_script.ld, run through the C
# preprocessor for the edition as its C is, once for the main image and once
# for each overlay (-DOVERLAY=0x<number>). The overlays are the numbers its
# overlay.ld tests; each image links the objects its script names, in order.
OVERLAY_LD := games/$(GAME)/src/system/overlay/overlay.ld
OVERLAY_IDS := $(shell sed -n 's/^\#\(el\)*if OVERLAY == 0x\([0-9a-f]*\)$$/\2/p' $(OVERLAY_LD))
LD_CPPFLAGS := -P -lang-asm -nostdinc -Igames/$(GAME)/include -D$(DEFINE)=1 $(if $(SHIFT),-DROM_SHIFT=$(SHIFT))
LD_HEADERS := games/$(GAME)/include/edition.h games/$(GAME)/include/version.h
LD_SCRIPT := $(BUILD)/ld/main.ld
LDFLAGS := -T $(LD_SCRIPT)
OVERLAY_SCRIPT = -T $(BUILD)/ld/overlay_$*.ld
# A script is rewritten only when its text changes, so an edit that leaves an
# image's script as it was relinks nothing; its stamp records when it last ran,
# and a script that has gone missing runs its stamp's recipe again. Two makes of
# one edition may run a recipe at once (make compare-editions modern builds the
# ROM and the modern objects together), so each writes a new file of its own.
if_missing = $(if $(wildcard $(1)),,FORCE)
$(BUILD)/ld/main.ld: $(BUILD)/ld/main.stamp ;
$(BUILD)/ld/main.stamp: games/$(GAME)/ld_script.ld $(LD_HEADERS) $(call if_missing,$(BUILD)/ld/main.ld)
	@mkdir -p $(@D)
	new=$(@:.stamp=.ld).$$$$.new; $(CPP) $(LD_CPPFLAGS) $< $$new && { cmp -s $$new $(@:.stamp=.ld) && rm $$new || mv $$new $(@:.stamp=.ld); }
	@touch $@
$(BUILD)/ld/overlay_%.ld: $(BUILD)/ld/overlay_%.stamp ;
$(BUILD)/ld/overlay_%.stamp: games/$(GAME)/ld_script.ld $(OVERLAY_LD) $(LD_HEADERS) $$(call if_missing,$(BUILD)/ld/overlay_$$*.ld)
	@mkdir -p $(@D)
	new=$(@:.stamp=.ld).$$$$.new; $(CPP) $(LD_CPPFLAGS) -DOVERLAY=0x$* $< $$new && { cmp -s $$new $(@:.stamp=.ld) && rm $$new || mv $$new $(@:.stamp=.ld); }
	@touch $@
.PHONY: FORCE
FORCE:
# The objects each script names, first mention first, as make variables.
object_list = grep -o '\*/games/[^"( ]*\.o' $(2) | sed 's|^\*/||; s|\.o$$||' | \
              awk -v name=$(1) '!seen[$$0]++ { list = list " \\\n\t$$(BUILD)/obj/" $$0 ".o" } END { print name " :=" list }'
$(BUILD)/objects.mk: $(LD_SCRIPT) $(OVERLAY_IDS:%=$(BUILD)/ld/overlay_%.ld)
	@{ echo '# Written by the Makefile from the linker script: do not edit.'; \
	  $(call object_list,MAIN_OBJS,$(LD_SCRIPT)); \
	  echo 'OVERLAYS := $(OVERLAY_IDS)'; \
	  $(foreach id,$(OVERLAY_IDS),$(call object_list,OVERLAY_OBJS_$(id),$(BUILD)/ld/overlay_$(id).ld);) \
	} > $@.new
	cmp -s $@.new $@ && rm $@.new || mv $@.new $@
OVERLAY_DEPS = $(BUILD)/ld/overlay_$*.ld
include $(BUILD)/objects.mk

# The source an object is built from: NAME.c, NAME.s, NAME.S or NAME.mid.
source = $(firstword $(wildcard $(1).c $(1).s $(1).S $(1).mid))
OVERLAY_OBJS := $(foreach id,$(OVERLAYS),$(OVERLAY_OBJS_$(id)))
OBJS := $(sort $(MAIN_OBJS) $(OVERLAY_OBJS))
# ⚓️'s overlays are built for size: C linked only into overlays takes -Os.
ifeq ($(GAME),tla)
$(filter-out $(MAIN_OBJS),$(OVERLAY_OBJS)): CFLAGS := $(subst -O2,-Os,$(CFLAGS))
endif
SRCS := $(foreach object,$(OBJS:$(BUILD)/obj/%.o=%),$(call source,$(object)))

# The overlays' packed streams go into the main image through one source,
# so it is assembled once the overlays are linked against the main image.
STREAM := $(BUILD)/obj/$(OVERLAY_STREAM).o
LINKED := $(filter-out $(STREAM),$(MAIN_OBJS))

$(BUILD)/deps.mk: $(BUILD)/objects.mk $(SCANINC) $(SRCS)
	$(SCANINC) $@ $(BUILD) $(BUILD)/obj $(INCLUDES) -I$(BUILD) -G enums/ -G text/ -G overlays/ -- $(SRCS)
# Read once the object list is: the scan needs its sources.
ifneq ($(wildcard $(BUILD)/objects.mk),)
include $(BUILD)/deps.mk
-include $(wildcard $(addsuffix .d,$(filter $(addprefix %.,$(ASSET_SUFFIXES)),$(DEPENDENCIES))))
endif

.PHONY: rom
rom: $(ROM)

$(ROM): $(ELF) $(GBAFIX)
	$(OBJCOPY) -O binary --pad-to $(ROM_END_$(GAME)) $< $@
	$(GBAFIX) $@ --silent

# objects.mk is a prerequisite because it holds the link order: an object list
# change that only moves objects must still relink.
$(ELF): $(LINKED) $(STREAM) $(LIBGCC) $(LD_SCRIPT) $(BUILD)/objects.mk
	$(LD) -Map $(BUILD)/$(EDITION).map --no-warn-mismatch $(LDFLAGS) -o $@ $(LINKED) $(STREAM) $(LIBGCC)

# The main image without its overlays: the addresses they link against.
$(BUILD)/symbols.elf: $(LINKED) $(LIBGCC) $(LD_SCRIPT) $(BUILD)/objects.mk
	$(LD) --unresolved-symbols=ignore-all --no-warn-mismatch $(LDFLAGS) -o $@ $(LINKED) $(LIBGCC)

# An overlay's import table: a far-call stub for each main-image routine
# its code calls and does not define, copied from the game's list of them,
# imports.inc, in the list's order. The overlay's script places it as
# "*/imports.o".
IMPORT_LIST := games/$(GAME)/src/system/overlay/imports.inc
# The far-call macros the list includes.
VENEER_tbs := games/tbs/src/gamepak/far_call/veneer.inc
VENEER_tla := games/tla/src/gamepak/far_call/veneer.s
OVERLAY_IMPORTS = $(BUILD)/overlays/$*/imports.o
.SECONDEXPANSION:
$(BUILD)/overlays/%/imports.s: $$(OVERLAY_OBJS_$$*) $(IMPORT_LIST)
	@mkdir -p $(@D)
	{ $(NM) -g --defined-only $(OVERLAY_OBJS_$*) | awk 'NF == 3 { print "own", $$3 }'; \
	  $(NM) -u $(OVERLAY_OBJS_$*) | awk '{ print "calls", $$NF }'; } | \
	awk 'NR == FNR { seen[$$1 " " $$2] = 1; next } $$1 == "export" { name = $$2; sub(/,$$/, "", name); \
	     if (!(("calls " name) in seen) || ("own " name) in seen) next } { print }' - $(IMPORT_LIST) > $@
$(BUILD)/overlays/%/imports.o: $(BUILD)/overlays/%/imports.s $(VENEER_$(GAME))
	$(AS) $(EDITION_ASFLAGS) -o $@ $<

# An overlay links at 0x2008000 against the main image's symbols, less the
# names it defines itself; its image is packed as Camelot's packer did.
$(BUILD)/overlays/resource_%.elf: $$(OVERLAY_OBJS_$$*) $$(OVERLAY_IMPORTS) $$(OVERLAY_DEPS) $(BUILD)/symbols.elf $(LIBGCC)
	@mkdir -p $(@D)
	$(NM) -g --defined-only $(OVERLAY_OBJS_$*) $(OVERLAY_IMPORTS) $(LIBGCC) | awk 'NF == 3 { print $$3 }' > $(@:.elf=.own)
	$(OBJCOPY) --extract-symbol --strip-symbols=$(@:.elf=.own) $(BUILD)/symbols.elf $(@:.elf=.symbols.elf)
	$(LD) --no-warn-mismatch -n $(OVERLAY_SCRIPT) -Map $(@:.elf=.map) -R $(@:.elf=.symbols.elf) -o $@ $(OVERLAY_OBJS_$*) $(OVERLAY_IMPORTS) $(LIBGCC)

$(BUILD)/overlays/resource_%.lz: $(BUILD)/overlays/resource_%.elf $(OVLPACK)
	$(OBJCOPY) -O binary $< $(@:.lz=.bin)
	$(OVLPACK) $(@:.lz=.bin) $@

# The edition's messages (A6). Japanese glyphs follow the editable font map.
CHARMAP_tbs := games/tbs/src/graphics/font/charmap_ja.tsv
CHARMAP_tla := games/tla/src/graphics/font/charmap_tla_ja.tsv
CHARMAP := $(if $(filter ja,$(LANG)),$(CHARMAP_$(GAME)))
# ags-text rewrites a file only when its contents change, so a .po edit
# recompiles only what it affects; the stamp records when it last ran.
$(BUILD)/text/messages.inc $(BUILD)/text/msg_ids.h: $(BUILD)/text/stamp ;
$(BUILD)/text/stamp: games/$(GAME)/text/$(LANG).po $(PO2AGS) $(CHARMAP)
	@mkdir -p $(@D)
	$(PO2AGS) $< $(@D)/messages.inc $(if $(CHARMAP),--charmap $(CHARMAP))
	$(PO2AGS) $< $(@D)/msg_ids.h --header $(if $(filter tbs,$(GAME)),--as-value)
	@touch $@

# A header's enum constants for the assembly tables that name its ids:
# a .s file includes enums/NAME.inc for include/NAME.h.
$(BUILD)/enums/%.inc: games/$(GAME)/include/%.h $(ENUMS)
	@mkdir -p $(@D)
	$(ENUMS) $< $@

# --- Objects -------------------------------------------------------------

$(BUILD)/obj/%.o: %.c
	@mkdir -p $(@D)
	$(XGCC) $(CFLAGS) -S -o $(@:.o=.s) $<
	$(CCAS) $(CCASFLAGS) -mthumb -mthumb-interwork -o $@ $(@:.o=.s)

$(addprefix $(BUILD)/obj/,$(SOUND_C:.c=.o) $(FLASH_C:.c=.o)): $(BUILD)/obj/%.o: %.c
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS_AGBCC) $< $(@:.o=.i)
	$(AGBCC) $(@:.o=.i) -dumpbase $(<F) -mthumb-interwork $(if $(filter $(FLASH_C),$<),-O,-O2) -o $(@:.o=.s)
	$(CCAS) $(CCASFLAGS) -mthumb -mthumb-interwork -o $@ $(@:.o=.s)

$(addprefix $(BUILD)/obj/,$(ARM_C:.c=.o)): $(BUILD)/obj/%.o: %.c
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS_ARMCC) $< $(@:.o=.i)
	$(ARMCC) $(@:.o=.i) -dumpbase $(<F) -O2 -mthumb-interwork -fomit-frame-pointer -fcall-used-r4 -o $(@:.o=.s)
	$(CCAS) $(CCASFLAGS) -mthumb-interwork -o $@ $(@:.o=.s)

$(BUILD)/obj/%.o: %.s
	@mkdir -p $(@D)
	$(AS) $(EDITION_ASFLAGS) -o $@ $<

$(BUILD)/obj/%.o: %.S
	@mkdir -p $(@D)
	$(CPP) -lang-asm -nostdinc -D$(DEFINE)=1 -I$(BUILD) $< $(@:.o=.i)
	$(AS) $(EDITION_ASFLAGS) -o $@ $(@:.o=.i)

$(BUILD)/obj/%.o: %.mid $(MID2AGS)
	@mkdir -p $(@D)
	$(MID2AGS) $< $(@:.o=.s)
	$(AS) $(ASFLAGS) -o $@ $(@:.o=.s)

# --- make modern (Goal, point 5) -------------------------------------------
# The same C through a current GCC, as pret's `make modern`: every file
# must compile, with no match expected. Errors stop it; warnings are the
# humanizing pass's to read.

MODERN_CC ?= arm-none-eabi-gcc
# Older current GCC releases do not know this separate diagnostic's name.
MODERN_DECLARATION_TYPE_ERROR := $(shell $(MODERN_CC) -Werror -Werror=declaration-missing-parameter-type \
                               -x c -fsyntax-only /dev/null >/dev/null 2>&1 && \
                               printf '%s' '-Werror=declaration-missing-parameter-type')
MODERN_CFLAGS := -mthumb-interwork -mcpu=arm7tdmi -mabi=apcs-gnu -O2 -fno-toplevel-reorder -std=gnu89 \
                 -ffreestanding -nostdinc -fno-zero-initialized-in-bss -fno-tree-loop-distribute-patterns \
                 -DMODERN=1 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter \
                 -Wno-missing-field-initializers -Wno-pointer-sign -fdiagnostics-plain-output \
                 -Werror=implicit-int -Werror=missing-parameter-type $(MODERN_DECLARATION_TYPE_ERROR) \
                 $(INCLUDES) -D$(DEFINE)=1 -I$(BUILD)
MODERN_OBJS := $(patsubst %.c,$(BUILD)/modern/%.o,$(filter %.c,$(SRCS)))

.PHONY: modern
modern: $(MODERN_OBJS)

$(BUILD)/modern/%.o: %.c | $(BUILD)/text/msg_ids.h
	@mkdir -p $(@D)
	$(MODERN_CC) $(if $(filter $(ARM_C),$<),-marm,-mthumb) $(MODERN_CFLAGS) \
	    $(if $(filter $(SOUND_C) $(FLASH_C),$<),,-fcall-used-r4) -MMD -MP -c -o $@ $<
# Read only when building modern: as included makefiles, make would check
# every one before any other build, which slowed a ROM build by minutes.
ifneq ($(filter modern,$(MAKECMDGOALS)),)
-include $(MODERN_OBJS:.o=.d)
endif
endif

ifndef EDITION
# --- Toolchain (K1) --------------------------------------------------------
# `make bootstrap` builds the pinned compilers from source into
# tools/out/compilers, as pret installs agbcc: alchemy-gcc's three C generators,
# GAS 2.10 from the GNU release vendored in alchemy-gcc (it assembles their
# output), and the eight libgcc members the games link, from alchemy-gcc's
# own sources.

TOOLCHAIN := tools/out/toolchain
-include alchemy-gcc/config/host.mk
GAS_CFLAGS := -O2 -std=gnu89 -D_POSIX_C_SOURCE=200809L -DNSIG=32 -Wno-implicit-int \
              -Wno-implicit-function-declaration -Wno-int-conversion -Wno-incompatible-pointer-types -fcommon

.PHONY: bootstrap compilers tools test
bootstrap: tools compilers $(CCAS) $(LIBGCC)
compilers: $(COMPILER)/xgcc

$(COMPILER)/xgcc:
	@test -f alchemy-gcc/Makefile || { echo 'alchemy-gcc is empty: run git submodule update --init --recursive (see README.md).' >&2; exit 1; }
	$(MAKE) -C alchemy-gcc
	$(MAKE) -C alchemy-gcc generated-check
	$(MAKE) -C alchemy-gcc/arm probe
	@mkdir -p $(COMPILER)/agbcc
	cp alchemy-gcc/build/gcc/xgcc alchemy-gcc/build/gcc/cpp0 alchemy-gcc/build/gcc/tradcpp0 alchemy-gcc/build/gcc/cc1 $(COMPILER)/
	cp alchemy-gcc/arm/build/gcc/cc1 $(COMPILER)/gcc2952_arm
	cp alchemy-gcc/agbcc/gcc/old_agbcc $(COMPILER)/agbcc/old_agbcc

# tc-arm.c's rotate_left shifts a 32-bit value by 32 when n is 0, which is
# undefined; Apple clang at -O2 then rejects every ARM immediate. Masking the
# count gives the rotation the macro means and changes no output.
# Git keeps no file times, so every file of the copy is given the release's
# date: with no file newer than another, make regenerates nothing.
# The arm-elf assembler builds no lexer, but its configure stops unless
# running lex leaves a lex.yy.c: `touch` is the stand-in its own Makefile uses.
$(CCAS):
	$(MAKE) -C alchemy-gcc source-check
	rm -rf $(TOOLCHAIN)/binutils-2.10 $(TOOLCHAIN)/gas
	@mkdir -p $(TOOLCHAIN)
	cp -R alchemy-gcc/binutils-2.10 $(TOOLCHAIN)/
	find $(TOOLCHAIN)/binutils-2.10 -exec touch -t 200006130000 {} +
	sed -i.orig 's/v >> (32 - n))/v >> ((32 - n) \& 31))/' $(TOOLCHAIN)/binutils-2.10/gas/config/tc-arm.c
	grep -q 'v >> ((32 - n) & 31))' $(TOOLCHAIN)/binutils-2.10/gas/config/tc-arm.c
	mkdir -p $(TOOLCHAIN)/gas
	cd $(TOOLCHAIN)/gas && LEX='touch lex.yy.c' CC='$(CC) $(HOST_MODEL)' CFLAGS='$(GAS_CFLAGS)' \
	    ../binutils-2.10/configure --target=arm-elf --build=i686-unknown-linux-gnu \
	    --host=i686-unknown-linux-gnu --disable-nls --disable-shared
	env -u MAKEFLAGS -u MFLAGS make -C $(TOOLCHAIN)/gas MAKEINFO=true all-gas
	@mkdir -p $(@D)
	cp $(TOOLCHAIN)/gas/gas/as-new $@

# libgcc: Thumb, no interworking (K1), each member from the source and the
# -DL<member> switch GCC's own library rules (gcc/config/arm/t-arm-elf) give it.
LIBGCC_WORK := $(TOOLCHAIN)/libgcc
LIBGCC_MEMBERS := _call_via_rX _addsub_df _si_to_df _df_to_si _pack_df _unpack_df _thenan_df _lshrdi3
LIBGCC_CFLAGS := -O2 -mthumb -mcpu=arm7tdmi -nostdinc -DCROSS_COMPILE -DIN_GCC -DIN_LIBGCC2 \
                 -D__GCC_FLOAT_NOT_NEEDED -Dinhibit_libc -fno-inline -isystem $(CURDIR)/alchemy-gcc/gcc/ginclude
LIBGCC_INCLUDES := -I$(CURDIR)/alchemy-gcc/gcc -I$(CURDIR)/alchemy-gcc/gcc/config -I$(CURDIR)/alchemy-gcc/include
LIBGCC_CC := $(CURDIR)/$(COMPILER)/xgcc -B$(CURDIR)/$(COMPILER)/ $(LIBGCC_CFLAGS)
LIBGCC_TOOLS := | $(COMPILER)/xgcc $(CCAS)

$(LIBGCC): $(addprefix $(LIBGCC_WORK)/,$(addsuffix .o,$(LIBGCC_MEMBERS)))
	@mkdir -p $(@D)
	rm -f $@
	cd $(LIBGCC_WORK) && $(PREFIX)ar crsD $(CURDIR)/$@ $(notdir $^)

$(LIBGCC_WORK)/_call_via_rX.o: alchemy-gcc/gcc/config/arm/lib1funcs.asm $(LIBGCC_TOOLS)
	@mkdir -p $(@D)
	cp $< $(@D)/lib1funcs.asm
	cd $(@D) && $(LIBGCC_CC) $(LIBGCC_INCLUDES) -DL_call_via_rX -xassembler-with-cpp lib1funcs.asm -c -o $(@F)

$(LIBGCC_WORK)/dp-bit.c: alchemy-gcc/gcc/config/fp-bit.c
	@mkdir -p $(@D)
	printf '#ifndef __ARMEB__\n#define FLOAT_BIT_ORDER_MISMATCH\n#define FLOAT_WORD_ORDER_MISMATCH\n#endif\n' > $@
	cat $< >> $@

LIBGCC_DPBIT := _addsub_df _si_to_df _df_to_si _pack_df _unpack_df _thenan_df
$(addprefix $(LIBGCC_WORK)/,$(addsuffix .o,$(LIBGCC_DPBIT))): $(LIBGCC_WORK)/%.o: $(LIBGCC_WORK)/dp-bit.c $(LIBGCC_TOOLS)
	cd $(@D) && $(LIBGCC_CC) $(LIBGCC_INCLUDES) -DFINE_GRAINED_LIBRARIES -DL$* $(CURDIR)/$< -c -o $(@F)

$(LIBGCC_WORK)/_lshrdi3.o: alchemy-gcc/gcc/libgcc2.c $(LIBGCC_TOOLS)
	@mkdir -p $(@D)
	cp alchemy-gcc/config/arm/tconfig.h alchemy-gcc/config/arm/tm.h $(@D)/
	cd $(@D) && $(LIBGCC_CC) -I. $(LIBGCC_INCLUDES) -DL_lshrdi3 $(CURDIR)/$< -c -o $(@F)

# --- Tools and tests -------------------------------------------------------

tools:
	$(MAKE) -f tools/Makefile

test: tools format-check device-check
	$(MAKE) -f tools/Makefile test

# ☀️ takes no register pins or signature devices (S2, in alchemy-devtools'
# AGENTS.md): only the proven CAMELOT_ASM helpers, Dma_Set and MulQ16, fix a
# variable to a register.
.PHONY: device-check
device-check:
	@if git grep -n -E 'FAKEMATCH\((pin|signature)\)|register[^;]*(__asm__|asm) *\(' -- 'games/tbs/*.c' 'games/tbs/*.h' \
	    ':!games/tbs/include/system/dma.h' ':!games/tbs/include/system/iwram_call.h'; then \
	  echo "device-check: The Broken Seal takes no pin or signature devices (S2, in alchemy-devtools/AGENTS.md)" >&2; exit 1; fi

# Both games' C is as clang-format writes it (AGENTS.md S3). Without
# clang-format installed the check says so
# and is skipped.
CLANG_FORMAT ?= clang-format
FORMATTED := $(shell git ls-files 'games/*.c' 'games/*.h')
.PHONY: format-check
format-check:
	@if command -v $(CLANG_FORMAT) >/dev/null; then \
	  $(CLANG_FORMAT) --dry-run -Werror $(FORMATTED); \
	else echo "format-check: $(CLANG_FORMAT) not found, skipped"; fi
endif
