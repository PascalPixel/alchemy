# Golden Sun: The Broken Seal ☀️ (tbs) and The Lost Age ⚓️ (tla), in six
# languages each: ja en de es fr it. `make tbs-en` builds one edition into
# out/tbs-en/tbs-en.gba; `make compare-tbs-en` also checks it against
# rom.sha1. Every rule the ROM is built by is in this file.

GAMES    := tbs tla
LANGS    := ja en de es fr it
EDITIONS := $(foreach game,$(GAMES),$(addprefix $(game)-,$(LANGS)))
TBS      := $(filter tbs-%,$(EDITIONS))
TLA      := $(filter tla-%,$(EDITIONS))

CPUS := $(shell sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)
JOBS ?= $(CPUS)
SHA1 := $(shell { command -v sha1sum || command -v shasum; } 2>/dev/null) -c
MAKEFLAGS += --no-print-directory --no-builtin-rules

.DEFAULT_GOAL := help
.PHONY: help all compare $(EDITIONS) $(addprefix compare-,$(EDITIONS)) compare-tbs compare-tla compare-editions clean

help:
	@printf '%s\n' \
	  'make bootstrap          build the pinned compilers and tools into tools/out' \
	  'make tbs-en             build one edition: tbs or tla, then ja en de es fr it' \
	  'make compare-tbs-en     build one edition and check it against rom.sha1' \
	  'make compare-tbs        the six ☀️ editions, which need no ROM' \
	  'make compare-editions   all twelve editions; ⚓️ needs your own ROMs in roms/' \
	  'make test               the tools'\'' tests'

# Each edition builds in its own make, so its variables below are set once.
$(EDITIONS):
	@$(MAKE) -f Makefile EDITION=$@ rom

$(addprefix compare-,$(EDITIONS)): compare-%: %
	@grep -F ' out/$*/$*.gba' rom.sha1 | $(SHA1) -

compare-tbs: $(addprefix compare-,$(TBS))
compare-tla: $(addprefix compare-,$(TLA))
compare-editions: compare-tbs compare-tla
compare: compare-tbs-ja

clean:
	rm -rf out
	find games recon -type f \( $(foreach suffix,$(ASSET_SUFFIXES),-name '*.$(suffix)' -o) -false \) -delete

# --- Tools ---------------------------------------------------------------

TOOLS    := tools/out/cargo-target/release
COMPILER := tools/out/compilers
export PATH := $(CURDIR)/tools/out/binutils/bin:$(PATH)

XGCC   := $(COMPILER)/xgcc -B$(COMPILER)/
CPP    := $(COMPILER)/cpp0
AGBCC  := $(COMPILER)/agbcc/old_agbcc
ARMCC  := $(COMPILER)/gcc2952_arm
CCAS   := $(COMPILER)/as
PREFIX := arm-none-eabi-
AS      := $(PREFIX)as
LD      := $(PREFIX)ld
OBJCOPY := $(PREFIX)objcopy
NM      := $(PREFIX)nm
LIBGCC  := tools/out/compiler-runtime/libgcc.a

AGSGFX  := $(TOOLS)/agsgfx
WAV2AGS := $(TOOLS)/wav2ags
MID2AGS := $(TOOLS)/mid2ags
PO2AGS  := $(TOOLS)/po2ags
OBJLIST := $(TOOLS)/objlist
SCANINC := $(TOOLS)/scaninc
OVLPACK := $(TOOLS)/ovlpack
GBAFIX  := $(TOOLS)/gbafix

# --- Assets (A1) ---------------------------------------------------------
# A picture or table is built beside its source, in every edition the same:
# games/.../NAME.FORM[.CODEC] from NAME.png, or from NAME.tsv for a table
# form. agsgfx reads the form and codec from the file name it writes.

PICTURE_FORMS := 1bpp 4bpp 4bpp16x16 4bpp16x32 4bpp32x8 4bpp32x16 4bpp32x32 4bpp64x64 \
                 8bpp 8bpp8x16 8bpp16x16 8bpp64x64 bitmap font frames gbapal glyphs icon4 icons strips
TABLE_FORMS   := blocks delta0 delta1 delta2 icons4 parts plane script spriteblocks sprites table
CODECS        := lz plz mtf d7 hlz
FORMS         := $(PICTURE_FORMS) $(TABLE_FORMS)
ASSET_SUFFIXES := $(FORMS) $(foreach form,$(FORMS),$(addprefix $(form).,$(CODECS))) bin

$(foreach form,$(PICTURE_FORMS),$(eval %.$(form): %.png $$(AGSGFX) ; $$(AGSGFX) $$< $$@))
$(foreach form,$(PICTURE_FORMS),$(foreach codec,$(CODECS),$(eval %.$(form).$(codec): %.png $$(AGSGFX) ; $$(AGSGFX) $$< $$@)))
$(foreach form,$(TABLE_FORMS),$(eval %.$(form): %.tsv $$(AGSGFX) ; $$(AGSGFX) $$< $$@))
$(foreach form,$(TABLE_FORMS),$(foreach codec,$(CODECS),$(eval %.$(form).$(codec): %.tsv $$(AGSGFX) ; $$(AGSGFX) $$< $$@)))

# Sound samples: NAME.pcm8.bin from NAME.pcm8.wav, NAME.bin from NAME.pcm4.
%.pcm8.bin: %.pcm8.wav $(WAV2AGS) ; $(WAV2AGS) $< $@
%.bin: %.pcm4 $(WAV2AGS) ; $(WAV2AGS) $< $@

# --- One edition ---------------------------------------------------------

ifdef EDITION
GAME  := $(word 1,$(subst -, ,$(EDITION)))
LANG  := $(word 2,$(subst -, ,$(EDITION)))
BUILD := out/$(EDITION)
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

# The cartridge's last address: objcopy fills the image to it with zeros.
ROM_END_tbs := 0x08800000
ROM_END_tla := 0x09000000

INCLUDES := -Igames/$(GAME)/include \
            $(addprefix -Igames/common/include/,battle field game graphics lib menu sound system)

# Game C (K1, K2): agscc's GCC of July 2000, one option set per game.
CFLAGS_tbs := -O2 -mthumb -mthumb-interwork -mcpu=arm7tdmi -nostdinc -fcall-used-r4
CFLAGS_tla := -O2 -mthumb -mcpu=arm7tdmi -nostdinc -fcall-used-r4 \
              -mtune=arm9tdmi -mthumb-split-constants -mthumb-call-via-lr
CFLAGS := $(CFLAGS_$(GAME)) $(INCLUDES) -D$(DEFINE)=1 -I$(BUILD)

# The sound and flash libraries: pret's old_agbcc. ARM C: GCC 2.95.2.
CPPFLAGS_ARM := -lang-c -nostdinc '-Acpu(arm)' '-Amachine(arm)' -D__CHAR_UNSIGNED__ -D__OPTIMIZE__ \
                -D__ARM_ARCH_4T__ -D__APCS_32__ -D__ARMEL__ -Darm_elf -D__ELF__
CPPFLAGS_AGBCC := $(CPPFLAGS_ARM) -Igames/$(GAME)/include -D__GNUC__=2 -D__GNUC_MINOR__=9 \
                  -D__GNUC_PATCHLEVEL__=0 -D__THUMBEL__ -Dthumb -D__thumb__ -D$(DEFINE)=1 -I$(BUILD)
CPPFLAGS_ARMCC := $(CPPFLAGS_ARM) -Igames/$(GAME)/include $(filter-out -Igames/$(GAME)/include,$(INCLUDES)) \
                  -D__GNUC__=2 -D__GNUC_MINOR__=95 -D__GNUC_PATCHLEVEL__=2 -Darm -D__arm__ -D$(DEFINE)=1 -I$(BUILD)
SOUND_C := games/common/src/sound/sound.c games/common/src/sound/tables.c games/tbs/src/sound/engine.c \
           $(wildcard games/tla/src/sound/engine.c games/tla/src/sound/tail.c games/tla/src/sound/music_track_*.c)
FLASH_C := $(addprefix games/common/src/system/save/,flash.c sst.c atmel.c program_flash_sector.c)
ARM_C   := games/tbs/src/window/moji.c games/tbs/src/field/update.c games/tbs/src/field/vertices.c \
           games/tla/src/graphics/tile/metatile.c

ASFLAGS := -mcpu=arm7tdmi -mthumb-interwork -meabi=gnu -mfpu=fpa -mfloat-abi=soft
EDITION_ASFLAGS := --defsym $(DEFINE)=1 -I$(BUILD) $(ASFLAGS)

# The object list: ☀️'s objects.tsv gives each edition its linker fragments
# under $(BUILD)/ld; ⚓️'s scripts name their objects themselves.
ifeq ($(GAME),tbs)
LD_SCRIPT := games/tbs/main.ld
LDFLAGS := --defsym $(DEFINE)=1 -L $(BUILD)/ld -T $(LD_SCRIPT)
OVERLAY_SCRIPT = -L $(BUILD)/ld/$* -T games/tbs/src/system/overlay/overlay.ld
OVERLAY_SOURCE = $(wildcard games/tbs/src/system/overlay/ovl_$*.s)
$(BUILD)/objects.mk: games/tbs/objects.tsv games/tbs/overlays.tsv $(OBJLIST)
	$(OBJLIST) games/tbs/objects.tsv games/tbs/overlays.tsv $(LANG) $(BUILD)
else
LD_SCRIPT := $(if $(filter ja,$(LANG)),games/tla/main.ld,recon/tla/$(LANG)/main.ld)
LDFLAGS := -T $(LD_SCRIPT)
OVERLAY_SCRIPT = -T recon/tla/raw/overlays/resource_$*.ld
OVERLAY_SOURCE = recon/tla/raw/overlays/resource_$*_overlay.s
OVERLAY_ROM := $(BUILD)/baserom.gba
$(BUILD)/objects.mk: $(LD_SCRIPT) $(wildcard recon/tla/raw/overlays/*.ld) $(OBJLIST)
	$(OBJLIST) --scripts $(LD_SCRIPT) recon/tla/raw/overlays $(BUILD)
endif
include $(BUILD)/objects.mk

# The source an object is built from: NAME.c, NAME.s, NAME.S or NAME.mid.
source = $(firstword $(wildcard $(1).c $(1).s $(1).S $(1).mid))
OVERLAY_OBJS := $(foreach id,$(OVERLAYS),$(OVERLAY_OBJS_$(id)))
OBJS := $(sort $(MAIN_OBJS) $(OVERLAY_OBJS))
SRCS := $(foreach object,$(OBJS:$(BUILD)/obj/%.o=%),$(call source,$(object)))

# The overlays' packed streams go into the main image through one source,
# so it is assembled once the overlays are linked against the main image.
STREAM := $(BUILD)/obj/games/$(GAME)/src/system/overlay/overlays.o
LINKED := $(filter-out $(STREAM),$(MAIN_OBJS))

$(BUILD)/deps.mk: $(BUILD)/objects.mk $(SCANINC)
	$(SCANINC) $@ $(BUILD) $(BUILD)/obj $(INCLUDES) -I$(BUILD) -G text/ -G overlays/ -G baserom.gba -- $(SRCS)
# Read once the object list is: the scan needs its sources.
ifneq ($(wildcard $(BUILD)/objects.mk),)
include $(BUILD)/deps.mk
endif

.PHONY: rom
rom: $(ROM)

$(ROM): $(ELF) $(GBAFIX)
	$(OBJCOPY) -O binary --pad-to $(ROM_END_$(GAME)) $< $@
	$(GBAFIX) $@ --silent

$(ELF): $(LINKED) $(STREAM) $(LIBGCC) $(LD_SCRIPT)
	$(LD) -Map $(BUILD)/$(EDITION).map --no-warn-mismatch $(LDFLAGS) -o $@ $(LINKED) $(STREAM) $(LIBGCC)

# The main image without its overlays: the addresses they link against.
$(BUILD)/symbols.elf: $(LINKED) $(LIBGCC) $(LD_SCRIPT)
	$(LD) --unresolved-symbols=ignore-all --no-warn-mismatch $(LDFLAGS) -o $@ $(LINKED) $(LIBGCC)

# An overlay links at 0x2008000 against the main image's symbols, less the
# names it defines itself; its image is packed as Camelot's packer did.
.SECONDEXPANSION:
$(BUILD)/overlays/resource_%.elf: $(BUILD)/overlays/resource_%_overlay.o $$(OVERLAY_OBJS_$$*) $(BUILD)/symbols.elf $(LIBGCC)
	$(NM) -g --defined-only $< $(OVERLAY_OBJS_$*) $(LIBGCC) | awk 'NF == 3 { print $$3 }' > $(@:.elf=.own)
	$(OBJCOPY) --extract-symbol --strip-symbols=$(@:.elf=.own) $(BUILD)/symbols.elf $(@:.elf=.symbols.elf)
	$(LD) --no-warn-mismatch -n $(OVERLAY_SCRIPT) -Map $(@:.elf=.map) -R $(@:.elf=.symbols.elf) -o $@ $< $(OVERLAY_OBJS_$*) $(LIBGCC)

$(BUILD)/overlays/resource_%.lz: $(BUILD)/overlays/resource_%.elf $(OVLPACK)
	$(OBJCOPY) -O binary $< $(@:.lz=.bin)
	$(OVLPACK) $(@:.lz=.bin) $@

$(BUILD)/overlays/resource_%_overlay.o: $$(OVERLAY_SOURCE) $(OVERLAY_ROM)
	@mkdir -p $(@D)
	$(AS) $(EDITION_ASFLAGS) -o $@ $(or $(filter %.s,$^),/dev/null)

# The edition's messages (A6).
$(BUILD)/text/messages.inc: games/$(GAME)/text/$(LANG).po $(PO2AGS)
	@mkdir -p $(@D)
	$(PO2AGS) $< $@
$(BUILD)/text/msg_ids.h: games/$(GAME)/text/$(LANG).po $(PO2AGS)
	@mkdir -p $(@D)
	$(PO2AGS) $< $@ --header $(if $(filter tbs,$(GAME)),--as-value)

# ⚓️'s scaffolds read the edition's own ROM (O1), checked against rom.sha1.
$(BUILD)/baserom.gba: roms/$(EDITION).gba
	@mkdir -p $(@D)
	grep -F ' out/$(EDITION)/$(EDITION).gba' rom.sha1 | sed 's| out/.*| $<|' | $(SHA1) - > /dev/null
	ln -sf $(CURDIR)/$< $@

# --- Objects -------------------------------------------------------------

$(BUILD)/obj/%.o: %.c
	@mkdir -p $(@D)
	$(XGCC) $(CFLAGS) -S -o $(@:.o=.s) $<
	$(CCAS) -marm7tdmi -mthumb -mthumb-interwork -o $@ $(@:.o=.s)

$(addprefix $(BUILD)/obj/,$(SOUND_C:.c=.o) $(FLASH_C:.c=.o)): $(BUILD)/obj/%.o: %.c
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS_AGBCC) $< $(@:.o=.i)
	$(AGBCC) $(@:.o=.i) -dumpbase $(<F) -mthumb-interwork $(if $(filter $(FLASH_C),$<),-O,-O2) -o $(@:.o=.s)
	$(CCAS) -marm7tdmi -mthumb -mthumb-interwork -o $@ $(@:.o=.s)

$(addprefix $(BUILD)/obj/,$(ARM_C:.c=.o)): $(BUILD)/obj/%.o: %.c
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS_ARMCC) $< $(@:.o=.i)
	$(ARMCC) $(@:.o=.i) -dumpbase $(<F) -O2 -mthumb-interwork -fomit-frame-pointer -fcall-used-r4 -o $(@:.o=.s)
	$(CCAS) -marm7tdmi -mthumb-interwork -o $@ $(@:.o=.s)

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
endif

# --- Toolchain and tests ---------------------------------------------------

CARGO ?= cargo
export CARGO_TARGET_DIR := $(CURDIR)/tools/out/cargo-target
.PHONY: bootstrap tools test

bootstrap: tools
	$(TOOLS)/alchemy bootstrap $(if $(BUNDLE),--from "$(BUNDLE)")
	$(TOOLS)/alchemy build runtime --output $(LIBGCC) \
	    _call_via_rX=gcc/config/arm/lib1funcs.asm _addsub_df=dp-bit.c _si_to_df=dp-bit.c \
	    _df_to_si=dp-bit.c _pack_df=dp-bit.c _unpack_df=dp-bit.c _thenan_df=dp-bit.c _lshrdi3=gcc/libgcc2.c

tools:
	$(CARGO) build --release --workspace --manifest-path tools/Cargo.toml

$(AGSGFX) $(WAV2AGS) $(MID2AGS) $(PO2AGS) $(OBJLIST) $(SCANINC) $(OVLPACK) $(GBAFIX):
	$(CARGO) build --release -p ags --manifest-path tools/Cargo.toml

test:
	$(CARGO) test --offline --quiet --release --workspace --manifest-path tools/Cargo.toml
