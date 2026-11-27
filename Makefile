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
# Keep every file the build makes, overlay ELFs and objects included, as pret
# keeps its intermediates: a debugger or a size report can read them.
.SECONDARY:

.DEFAULT_GOAL := all
# Plain make builds and checks all twelve editions, one job a core.
.PHONY: all
all:
	@$(MAKE) -j$(shell getconf _NPROCESSORS_ONLN) compare-editions
.PHONY: help all compare $(EDITIONS) $(addprefix compare-,$(EDITIONS)) compare-tbs compare-tla compare-editions clean

help:
	@printf '%s\n' \
	  'make                    build all twelve editions (the compilers first, once) and check each against rom.sha1' \
	  'make bootstrap          build the pinned compilers and tools into tools/out' \
	  'make tbs-en             build one edition: tbs or tla, then ja en de es fr it' \
	  'make compare-tbs-en     build one edition and check it against rom.sha1' \
	  'make compare-tbs        the six ☀️ editions' \
	  'make compare-editions   all twelve editions' \
	  'make modern             all twelve editions'\'' C through GCC 13.2.1 (MODERN_CC)' \
	  'make test               test the C build tools and the format' \
	  'make test-linking       add a row to the scene, resource and map lists and link: every name follows' \
	  'make SHIFT=4 tbs-en     the shift test: everything after the header moves 4 bytes'

# Each edition builds in its own make, so its variables below are set once;
# the tools and compilers are built first, once. GNU Make 3.81, macOS's make,
# busy-waits when a parallel make waits for a job, so there each edition
# builds serially while the editions still build side by side.
SERIAL := $(if $(filter 3.%,$(MAKE_VERSION)),-j1)
$(EDITIONS): bootstrap lists
	@$(MAKE) $(SERIAL) -f Makefile EDITION=$@ rom

$(addprefix compare-,$(EDITIONS)): compare-%: %
	@grep -F ' out/$*/$*.gba' rom.sha1 | $(SHA1) -

compare-tbs: $(addprefix compare-,$(TBS))
compare-tla: $(addprefix compare-,$(TLA))
compare-editions: compare-tbs compare-tla
compare: compare-tbs-en

# make modern: all twelve editions' C through GCC 13.2.1 (MODERN_CC);
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
# A map's grid is three tables, one number a square: picture, behavior, shape.
%/map.grid.lz: %/grid_picture.tsv %/grid_behavior.tsv %/grid_shape.tsv $$(wildcard $$*/grid_picture_patch.tsv $$*/grid_actors.tsv) $(AGSGFX) ; $(AGSGFX) $< $@
%/map_ja.grid.lz: %/grid_picture_ja.tsv %/grid_behavior_ja.tsv %/grid_shape_ja.tsv $$(wildcard $$*/grid_picture_ja_patch.tsv $$*/grid_actors_ja.tsv) $(AGSGFX) ; $(AGSGFX) $< $@
%/map_en.grid.lz: %/grid_picture_en.tsv %/grid_behavior_en.tsv %/grid_shape_en.tsv $$(wildcard $$*/grid_picture_en_patch.tsv $$*/grid_actors_en.tsv) $(AGSGFX) ; $(AGSGFX) $< $@
%/map_de.grid.lz: %/grid_picture_de.tsv %/grid_behavior_de.tsv %/grid_shape_de.tsv $$(wildcard $$*/grid_picture_de_patch.tsv $$*/grid_actors_de.tsv) $(AGSGFX) ; $(AGSGFX) $< $@
%/map_es.grid.lz: %/grid_picture_es.tsv %/grid_behavior_es.tsv %/grid_shape_es.tsv $$(wildcard $$*/grid_picture_es_patch.tsv $$*/grid_actors_es.tsv) $(AGSGFX) ; $(AGSGFX) $< $@
%/map_fr.grid.lz: %/grid_picture_fr.tsv %/grid_behavior_fr.tsv %/grid_shape_fr.tsv $$(wildcard $$*/grid_picture_fr_patch.tsv $$*/grid_actors_fr.tsv) $(AGSGFX) ; $(AGSGFX) $< $@
%/map_it.grid.lz: %/grid_picture_it.tsv %/grid_behavior_it.tsv %/grid_shape_it.tsv $$(wildcard $$*/grid_picture_it_patch.tsv $$*/grid_actors_it.tsv) $(AGSGFX) ; $(AGSGFX) $< $@
# A map's scene is three tables: its points, entrances and bounds.
%/scene.scene.lz: %/points.tsv %/entrances.tsv %/bounds.tsv $(AGSGFX) ; $(AGSGFX) $< $@
%/scene_ja.scene.lz: %/points_ja.tsv %/entrances_ja.tsv %/bounds_ja.tsv $(AGSGFX) ; $(AGSGFX) $< $@
$(foreach form,$(TABLE_FORMS),$(eval %.$(form): %.tsv $$(AGSGFX) ; $$(AGSGFX) $$< $$@))
# A sprite bank is one table for every edition: an edition's bank, sprites_ja,
# draws NAME_ja.png and reads NAME_ja.tsv wherever a sprite has them.
$(foreach form,sprites spriteblocks,$(foreach lang,ja en de es fr it,$(eval %/sprites_$(lang).$(form): %/sprites.tsv $$(AGSGFX) ; $$(AGSGFX) $$< $$@)))
$(foreach form,$(TABLE_FORMS),$(foreach codec,$(CODECS),$(eval %.$(form).$(codec): %.tsv $(PATCH) $$(AGSGFX) ; $$(AGSGFX) $$< $$@)))

# An animation whose pictures differ in size, so that no one sheet holds them:
# NAME.bitmaps.CODEC from NAME_00.png, NAME_01.png and on, each picture's
# pixels in turn.
PICTURE_RUN := $$$$(wildcard $$$$(basename $$$$(basename $$$$@))_[0-9][0-9].png)
$(foreach codec,$(CODECS),$(eval %.bitmaps.$(codec): %_00.png $(PICTURE_RUN) $(PATCH) $$(AGSGFX) ; $$(AGSGFX) $$< $$@))

# A background's tile map as the screen shows it, a grid of tile numbers with
# its flips and palettes in two tables beside it where it has them, and the
# world map's collision tiles as the game reads them.
%.tilemap.lz: %.tsv $$(wildcard $$*.flip.tsv $$*.palette.tsv $$*_patch.tsv) $(AGSGFX) ; $(AGSGFX) $< $@
%.collision.lz: %.tsv $$(wildcard $$(basename $$(basename $$@))_patch.tsv) $(AGSGFX) ; $(AGSGFX) $< $@

# Sound samples: NAME.pcm8.bin from NAME.pcm8.wav.
%.pcm8.bin: %.pcm8.wav $(WAV2AGS) ; $(WAV2AGS) $< $@

# --- One edition ---------------------------------------------------------

ifdef EDITION
GAME  := $(word 1,$(subst -, ,$(EDITION)))
LANG  := $(word 2,$(subst -, ,$(EDITION)))
# make SHIFT=n tbs-en is the shift test: everything after the header moves n
# bytes (a multiple of 4), built into out/shift-n/ so out/<edition> is untouched.
# make test-linking builds with LINKING=1, into out/linking/, as the shift test
# builds into its own folder.
BUILD := out/$(if $(SHIFT),shift-$(SHIFT)/)$(if $(LINKING),linking/)$(EDITION)
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

# Game C: alchemy-gcc's GCC of July 2000, one option set per game;
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
SOUND_C := games/tbs/src/sdk/sound/audio.c games/tbs/src/sdk/sound/tables.c games/tbs/src/sdk/sound/driver.c \
           games/tla/src/sdk/sound/sound.c games/tla/src/sdk/sound/tables.c games/tla/src/sdk/sound/player_work_copy.c \
           games/tla/src/sdk/sound/driver.c
FLASH_C := $(addprefix games/tbs/src/sdk/flash/,flash.c sst.c atmel.c program_flash_sector.c) \
           $(addprefix games/tla/src/sdk/flash/,flash.c sst.c atmel.c program_flash_sector.c)
ARM_C   := games/tbs/src/window/glyph_arm.c games/tbs/src/field/actor_update_arm.c games/tbs/src/field/world_map_lines.c \
           games/tla/src/window/window_spans.c games/tla/src/window/glyph_arm.c games/tla/src/field/grid_outline.c \
           games/tla/src/field/world_map_lines.c

ASFLAGS := -mcpu=arm7tdmi -mthumb-interwork
EDITION_ASFLAGS := --defsym $(DEFINE)=1 -I$(BUILD) $(ASFLAGS) $(if $(SHIFT),--defsym ROM_SHIFT=$(SHIFT))

OVERLAY_STREAM := games/$(GAME)/src/system/overlay/overlays.list
# The linker script: games/$(GAME)/ld_script.ld, run through the C
# preprocessor for the edition as its C is, once for the main image and once
# for each overlay (-DOVERLAY -DOVERLAY_<name>). The overlays are the names its
# overlay.ld tests; each image links the objects its script names, in order.
# An overlay has no number here: that is its row in the resource directory.
OVERLAY_LD := games/$(GAME)/src/system/overlay/overlay.ld
OVERLAY_NAMES := $(shell sed -n 's/^\#\(el\)*if defined(OVERLAY_\([A-Za-z0-9]*\))$$/\2/p' $(OVERLAY_LD))
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
	new=$(@:.stamp=.ld).$$$$.new; $(CPP) $(LD_CPPFLAGS) -DOVERLAY -DOVERLAY_$* $< $$new && { cmp -s $$new $(@:.stamp=.ld) && rm $$new || mv $$new $(@:.stamp=.ld); }
	@touch $@
.PHONY: FORCE
FORCE:
# The objects each script names, first mention first, as make variables.
object_list = grep -o '\*/games/[^"( ]*\.o' $(2) | sed 's|^\*/||; s|\.o$$||' | \
              awk -v name=$(1) '!seen[$$0]++ { list = list " \\\n\t$$(BUILD)/obj/" $$0 ".o" } END { print name " :=" list }'
$(BUILD)/objects.mk: $(LD_SCRIPT) $(OVERLAY_NAMES:%=$(BUILD)/ld/overlay_%.ld)
	@{ echo '# Written by the Makefile from the linker script: do not edit.'; \
	  $(call object_list,MAIN_OBJS,$(LD_SCRIPT)); \
	  echo 'OVERLAYS := $(OVERLAY_NAMES)'; \
	  $(foreach name,$(OVERLAY_NAMES),$(call object_list,OVERLAY_OBJS_$(name),$(BUILD)/ld/overlay_$(name).ld);) \
	} > $@.new
	cmp -s $@.new $@ && rm $@.new || mv $@.new $@
OVERLAY_DEPS = $(BUILD)/ld/overlay_$*.ld
include $(BUILD)/objects.mk

# The source an object is built from: NAME.c, NAME.s, NAME.S or NAME.mid.
source = $(firstword $(wildcard $(1).c $(1).s $(1).S $(1).mid))
OVERLAY_OBJS := $(foreach name,$(OVERLAYS),$(OVERLAY_OBJS_$(name)))
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

.PHONY: rom elf
rom: $(ROM)
elf: $(ELF)

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

# An overlay's import table: a stub for each main-image routine
# its code calls and does not define, copied from the game's list of them,
# imports.inc, in the list's order. The overlay's script places it as
# "*/imports.o". It is rewritten when the overlay's objects or its script
# change, so an object leaving the overlay takes its calls' stubs with it.
IMPORT_LIST := games/$(GAME)/src/system/overlay/imports.inc
# The stub macros the list includes.
STUB_MACROS_tbs := games/tbs/src/system/stub.inc
STUB_MACROS_tla := games/tla/src/system/stub.s
OVERLAY_IMPORTS = $(BUILD)/overlays/$*/imports.o
.SECONDEXPANSION:
$(BUILD)/overlays/%/imports.s: $$(OVERLAY_OBJS_$$*) $$(OVERLAY_DEPS) $(IMPORT_LIST)
	@mkdir -p $(@D)
	{ $(NM) -g --defined-only $(OVERLAY_OBJS_$*) | awk 'NF == 3 { print "own", $$3 }'; \
	  $(NM) -u $(OVERLAY_OBJS_$*) | awk '{ print "calls", $$NF }'; } | \
	awk 'NR == FNR { seen[$$1 " " $$2] = 1; next } $$1 == "stub" { name = $$2; sub(/,$$/, "", name); \
	     if (!(("calls " name) in seen) || ("own " name) in seen) next } { print }' - $(IMPORT_LIST) > $@
$(BUILD)/overlays/%/imports.o: $(BUILD)/overlays/%/imports.s $(STUB_MACROS_$(GAME))
	$(AS) $(EDITION_ASFLAGS) -o $@ $<

# An overlay links at 0x2008000 against the main image's symbols, less the
# names it defines itself; its image is packed as Camelot's packer did.
$(BUILD)/overlays/%.elf: $$(OVERLAY_OBJS_$$*) $$(OVERLAY_IMPORTS) $$(OVERLAY_DEPS) $(BUILD)/symbols.elf $(LIBGCC)
	@mkdir -p $(@D)
	$(NM) -g --defined-only $(OVERLAY_OBJS_$*) $(OVERLAY_IMPORTS) $(LIBGCC) | awk 'NF == 3 { print $$3 }' > $(@:.elf=.own)
	$(OBJCOPY) --extract-symbol --strip-symbols=$(@:.elf=.own) $(BUILD)/symbols.elf $(@:.elf=.symbols.elf)
	$(LD) --no-warn-mismatch -n $(OVERLAY_SCRIPT) -Map $(@:.elf=.map) -R $(@:.elf=.symbols.elf) -o $@ $(OVERLAY_OBJS_$*) $(OVERLAY_IMPORTS) $(LIBGCC)

$(BUILD)/overlays/%.lz: $(BUILD)/overlays/%.elf $(OVLPACK)
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
# a .s file includes enums/NAME.inc for include/NAME.h, and
# enums/src/NAME.inc for a header kept beside its sources, src/NAME.h.
$(BUILD)/enums/src/%.inc: games/$(GAME)/src/%.h $(ENUMS)
	@mkdir -p $(@D)
	$(ENUMS) $< $@
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

# The linking test (make test-linking) assembles the three lists that number
# the scenes, the resources (the overlays among them) and the maps from copies
# with one row more in the middle: the middle scene's row and the middle map's
# row again under the name LinkingTest, and an unused resource of that name,
# which falls before the overlays' rows. Nothing else differs from the
# edition's build, so an image that links has taken every number from the lists.
MAP_LIST_tbs := games/tbs/src/map/resources
MAP_LIST_tla := games/tla/src/field/map_resources
ifdef LINKING
$(BUILD)/obj/$(MAP_LIST_$(GAME)).o: $(MAP_LIST_$(GAME)).s
	@mkdir -p $(@D)
	awk 'NR == FNR { if ($$1 == "map") rows++; next } { print } \
	     $$1 == "map" && ++row == int(rows / 2) { sub(/map [A-Za-z0-9_]*,/, "map LinkingTest,"); print }' $< $< > $(@:.o=.s)
	$(AS) $(EDITION_ASFLAGS) -o $@ $(@:.o=.s)
$(BUILD)/obj/games/$(GAME)/src/field/scenes.o: games/$(GAME)/src/field/scenes.s
	@mkdir -p $(@D)
	awk 'NR == FNR { if ($$1 == "scene") rows++; next } { print } \
	     $$1 == "scene" && ++row == int(rows / 2) { sub(/scene [A-Za-z0-9_]*,/, "scene LinkingTest,"); print }' $< $< > $(@:.o=.s)
	$(AS) $(EDITION_ASFLAGS) -o $@ $(@:.o=.s)
$(BUILD)/obj/games/$(GAME)/src/system/resource_directory.o: games/$(GAME)/src/system/resource_directory.s
	@mkdir -p $(@D)
	awk 'NR == FNR { if ($$1 ~ /resource$$/) rows++; next } { print } \
	     $$1 ~ /resource$$/ && ++row == int(rows / 2) { print "\tresource LinkingTest, 0" }' $< $< > $(@:.o=.s)
	$(AS) $(EDITION_ASFLAGS) -o $@ $(@:.o=.s)
endif

$(BUILD)/obj/%.o: %.S
	@mkdir -p $(@D)
	$(CPP) -lang-asm -nostdinc -D$(DEFINE)=1 -I$(BUILD) $< $(@:.o=.i)
	$(AS) $(EDITION_ASFLAGS) -o $@ $(@:.o=.i)

$(BUILD)/obj/%.o: %.mid $(MID2AGS)
	@mkdir -p $(@D)
	$(MID2AGS) $< $(@:.o=.s)
	$(AS) $(ASFLAGS) -o $@ $(@:.o=.s)

# --- make modern ----------------------------------------------------------
# The same C through a modern GCC, as pret's `make modern`: every file must
# compile, with no match expected. Errors stop it; warnings are shown. The GCC
# is one release, 13.2.1 (Arm's 13.2.rel1, Ubuntu 24.04's gcc-arm-none-eabi),
# so that its warnings and errors are the same on every machine and in CI.
MODERN_CC ?= arm-none-eabi-gcc
MODERN_GCC_VERSION := 13.2.1
MODERN_CFLAGS := -mthumb-interwork -mcpu=arm7tdmi -mabi=apcs-gnu -O2 -fno-toplevel-reorder -std=gnu89 \
                 -ffreestanding -nostdinc -fno-zero-initialized-in-bss -fno-tree-loop-distribute-patterns \
                 -DMODERN=1 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter \
                 -Wno-missing-field-initializers -Wno-pointer-sign -fdiagnostics-plain-output \
                 -Werror=implicit-int -Werror=missing-parameter-type \
                 $(INCLUDES) -D$(DEFINE)=1 -I$(BUILD)
MODERN_OBJS := $(patsubst %.c,$(BUILD)/modern/%.o,$(filter %.c,$(SRCS)))

.PHONY: modern modern-gcc
modern: $(MODERN_OBJS)
modern-gcc:
	@test "$$($(MODERN_CC) -dumpfullversion 2>/dev/null)" = "$(MODERN_GCC_VERSION)" || \
	  { echo "make modern: $(MODERN_CC) is not GCC $(MODERN_GCC_VERSION)"; exit 1; }

$(BUILD)/modern/%.o: %.c | $(BUILD)/text/msg_ids.h modern-gcc
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
# --- Toolchain -------------------------------------------------------------
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

# libgcc: Thumb, no interworking, each member from the source and the
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

# --- Lists -----------------------------------------------------------------
# Packed files placed in order have a list.tsv or NAME.list.tsv beside them;
# ags-list writes its .s, in every edition the same, before an edition is
# built. A table the code reads is never one of these: it is C, or assembler
# source with its macro above its rows.
MAPS = games/tbs/src/map/maps.s games/tla/src/map/maps.s
LISTS := $(patsubst %.tsv,%.s,$(shell find games -name list.tsv -o -name '*.list.tsv'))
.PHONY: lists
lists: $(LISTS) $(MAPS)
tools/out/ags-list tools/out/ags-maps: | tools
$(LISTS): %.s: %.tsv tools/out/ags-list
	tools/out/ags-list $< $@
# Every map of the field has its tables in its own folder, and maps.tsv
# beside the folders gives their order;
# ags-maps writes maps.s from them.
MAPS := games/tbs/src/map/maps.s games/tla/src/map/maps.s
$(MAPS): %/maps.s: %/maps.tsv $$(wildcard $$*/*/header.tsv $$*/*/layers.tsv $$*/*/contents.tsv $$*/*/spots.tsv $$*/*/tilesets.tsv $$*/*/placed.tsv $$*/../graphics/tileset/*/packed.tsv) tools/out/ags-maps
	tools/out/ags-maps $* $@

# --- Tools and tests -------------------------------------------------------

tools:
	$(MAKE) -f tools/Makefile

test: tools format-check
	$(MAKE) -f tools/Makefile test
	git ls-files games tools | tools/out/ags-check

# make test-linking: the tree links itself. A scene's, a resource's, an
# overlay's or a map's number is its row in games/<game>/src/field/scenes.s,
# src/system/resource_directory.s or the map list, and everything that refers
# to one names it, so the linker fills the number in. The test builds
# LINKING_EDITION again into out/linking/ with one row more in the middle of
# each list and no other change, and reads both images. First their symbols:
# the image links, every SceneId_, ResourceId_, MapResource_, OverlayId_ and
# MapIndex_ name is still there, each one after a new row is one more than it
# was and each one before it is what it was, and every overlay has moved.
# Then their bytes, read back to names through those symbols (linking_rows):
# every scene's row of the scene table still names the overlay and the map it
# named, and every overlay's row of the directory still holds its own image.
# It edits no tracked file, and the ROM comparison does not run it. What it
# cannot see is a number written bare where no row macro pastes a name.
LINKING_EDITION ?= tla-en
LINKING_WAS := out/$(LINKING_EDITION)/$(LINKING_EDITION)
LINKING_NOW := out/linking/$(LINKING_EDITION)/$(LINKING_EDITION)
# The rows of image $(1), an ELF: "scene NAME OVERLAY MAP" for each row of
# gScenes, and "overlay NAME own" for each overlay whose row of the directory
# holds the address of Overlay_NAME. A number no name has reads "?".
linking_rows = $(NM) -t d $(1).elf > $(1).names && $(OBJCOPY) -O binary $(1).elf $(1).image && \
	{ od -An -v -t u2 -j $$(awk '$$3 == "gScenes" { at = $$1 } $$3 == "Rom_Start" { rom = $$1 } END { print at - rom }' $(1).names) \
	     -N $$(awk '$$3 ~ /^SceneId_/ && $$1 >= rows { rows = $$1 + 1 } END { print rows * 8 }' $(1).names) $(1).image | sed 's/^/scenes /'; \
	  od -An -v -t u4 -j $$(awk '$$3 == "Resource_DirectoryTable" { at = $$1 } $$3 == "Rom_Start" { rom = $$1 } END { print at - rom }' $(1).names) \
	     -N $$(awk '$$3 ~ /^(Resource|Overlay)Id_/ && $$1 >= rows { rows = $$1 + 1 } END { print rows * 4 }' $(1).names) $(1).image | sed 's/^/directory /'; } | \
	awk 'NR == FNR { if ($$3 ~ /^SceneId_/) scene[$$1 + 0] = substr($$3, 9); \
	                 else if ($$3 ~ /^OverlayId_/) overlay[$$1 + 0] = substr($$3, 11); \
	                 else if ($$3 ~ /^MapIndex_/) map[$$1 + 0] = substr($$3, 10); \
	                 else if ($$3 ~ /^Overlay_/) image[substr($$3, 9)] = $$1 + 0; next } \
	     function named(names, number) { return number in names ? names[number] : "?" } \
	     $$1 == "scenes" { for (i = 2; i <= NF; i++) cell[cells++] = $$i } \
	     $$1 == "directory" { for (i = 2; i <= NF; i++) word[words++] = $$i } \
	     END { for (row = 0; row * 4 < cells; row++) \
	             print "scene", named(scene, row), named(overlay, cell[row * 4]), named(map, cell[row * 4 + 2]); \
	           for (row = 0; row < words; row++) if (row in overlay) \
	             print "overlay", overlay[row], (word[row] == image[overlay[row]] ? "own" : "?") }' $(1).names - > $(1).rows
.PHONY: test-linking
test-linking: $(LINKING_EDITION)
	@$(MAKE) $(SERIAL) -f Makefile EDITION=$(LINKING_EDITION) LINKING=1 elf
	@{ $(NM) -t d $(LINKING_WAS).elf | sed "s/^/was /"; $(NM) -t d $(LINKING_NOW).elf | sed "s/^/now /"; } | \
	awk '$$3 != "A" || $$4 !~ /^(SceneId|ResourceId|MapResource|OverlayId|MapIndex)_/ { next } \
	     $$1 == "was" { was[$$4] = $$2 + 0; next } { now[$$4] = $$2 + 0 } \
	     END { if (!("SceneId_LinkingTest" in now) || !("ResourceId_LinkingTest" in now) || !("MapIndex_LinkingTest" in now)) { \
	             print "test-linking: the new rows have no numbers"; exit 1 } \
	           scene = now["SceneId_LinkingTest"]; resource = now["ResourceId_LinkingTest"]; map = now["MapIndex_LinkingTest"]; \
	           empty = was["ResourceId_EmptyMap"]; \
	           for (name in was) { \
	             if (name ~ /^SceneId_/) step = was[name] >= scene; \
	             else if (name ~ /^(Resource|Overlay)Id_/) step = was[name] >= resource; \
	             else if (name ~ /^MapIndex_/) step = was[name] >= map; \
	             else step = (was[name] + empty >= resource) - (empty >= resource); \
	             if (!(name in now)) { print "test-linking: " name " is gone"; bad++ } \
	             else if (now[name] != was[name] + step) { bad++; \
	               print "test-linking: " name " was " was[name] ", is " now[name] ", should be " was[name] + step } \
	             else if (name ~ /^OverlayId_/ && !step) { bad++; print "test-linking: the new resource is not before " name } \
	             else if (step) moved++; else stayed++ } \
	           if (bad || !moved) { print "test-linking: FAILED"; exit 1 } \
	           print "test-linking: scene " scene ", resource " resource " and map " map " added, " moved " names moved by one, " stayed " stayed: OK" }'
	@$(call linking_rows,$(LINKING_WAS)) && $(call linking_rows,$(LINKING_NOW)) && \
	grep -v '^scene LinkingTest ' $(LINKING_NOW).rows | cmp -s - $(LINKING_WAS).rows && ! grep -q '?' $(LINKING_WAS).rows || { \
	  grep -v '^scene LinkingTest ' $(LINKING_NOW).rows | diff $(LINKING_WAS).rows - | head -20; grep '?' $(LINKING_WAS).rows | head -20; \
	  echo "test-linking: a scene's overlay or map, or an overlay's image, is not its own: FAILED"; exit 1; }
	@awk '{ rows[$$1]++ } END { print "test-linking: " rows["scene"] " scenes name the overlay and the map they named, " \
	      rows["overlay"] " overlays hold their own images: OK" }' $(LINKING_WAS).rows

# Both games' C is as clang-format 20 writes it: earlier versions read a cast
# such as (s32)&x as a bitwise and, whatever .clang-format's TypeNames say.
# Without clang-format-20 installed the check says so and is skipped.
CLANG_FORMAT ?= clang-format-20
FORMATTED := $(shell git ls-files 'games/*.c' 'games/*.h')
.PHONY: format-check
format-check:
	@if command -v $(CLANG_FORMAT) >/dev/null; then \
	  $(CLANG_FORMAT) --dry-run -Werror $(FORMATTED); \
	else echo "format-check: $(CLANG_FORMAT) not found, skipped"; fi
endif
