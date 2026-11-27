/* The Broken Seal, all six editions: the one linker script, run through the
   C preprocessor with -DTBS_EDITION_<LANG>=1 as the C is. edition.h gives it
   LANGUAGE and the edition's features, so an edition's differences here are the
   same tests the C makes.

   The build preprocesses it once for the main image, and once for each
   overlay with OVERLAY and OVERLAY_<its name> defined; it reads each
   image's objects from the statements below, in order, and links them in
   that order.

   The main image lists its objects in the order the Game Pak holds them. In
   a module of ordinary code every object's .text comes first, then every
   object's .rodata, then its .data, as pret writes its scripts; an object
   that needs other sections names them. */
#include "edition.h"

/* The shift test (make SHIFT=n) moves everything after the header n bytes,
   so every module starts n bytes later; 0 in every other build. */
#ifndef ROM_SHIFT
#define ROM_SHIFT 0
#endif
#if ROM_SHIFT % 4 != 0
#error "SHIFT must be a multiple of 4: words and Thumb literal pools in byte-aligned sections would misalign"
#endif

/* Where each stub call module starts, and the resource directory after them,
   in each edition. English and Italian share every address, and Spanish
   and French; Japanese shares English's first two. Most of German's fall
   on 256-byte steps, the other editions' on 2 or 4 KB ones. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define FIELD_BANK_START          0x08009000
#define WINDOW_BANK_START         0x08015000
#define GAME_BANK_START           0x0806d000
#define FIELD_MODE_BANK_START 0x08081000
#define MENU_BANK_START           0x08098000
#define SHOP_BANK_START           0x080a7000
#define BATTLE_BANK_START         0x080ac000
#define BATTLE_EFFECT_BANK_START  0x080c0000
#define CREDITS_BANK_START        0x080e7000
#define TITLE_BANK_START          0x080e9000
#define LUCKY_DICE_BANK_START     0x080eb000
#define LUCKY_WHEELS_BANK_START   0x080ed000
#define SOUND_BANK_START          0x080f0000
#define SPRITE_BANK_START         0x0817c000
#define RESOURCE_DIRECTORY_START  0x08317000
#elif LANGUAGE == LANGUAGE_GERMAN
#define FIELD_BANK_START          0x08008800
#define WINDOW_BANK_START         0x08013d00
#define GAME_BANK_START           0x0807ae00
#define FIELD_MODE_BANK_START 0x0808d600
#define MENU_BANK_START           0x080a3e00
#define SHOP_BANK_START           0x080b2600
#define BATTLE_BANK_START         0x080b7600
#define BATTLE_EFFECT_BANK_START  0x080cae00
#define CREDITS_BANK_START        0x080f1600
#define TITLE_BANK_START          0x080f3600
#define LUCKY_DICE_BANK_START     0x080f5600
#define LUCKY_WHEELS_BANK_START   0x080f7600
#define SOUND_BANK_START          0x080f9e00
#define SPRITE_BANK_START         0x08185e00
#define RESOURCE_DIRECTORY_START  0x0831fe00
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
#define FIELD_BANK_START          0x08008800
#define WINDOW_BANK_START         0x08014000
#define GAME_BANK_START           0x0807c000
#define FIELD_MODE_BANK_START 0x0808f000
#define MENU_BANK_START           0x080a5800
#define SHOP_BANK_START           0x080b4000
#define BATTLE_BANK_START         0x080b9000
#define BATTLE_EFFECT_BANK_START  0x080cc800
#define CREDITS_BANK_START        0x080f3000
#define TITLE_BANK_START          0x080f5000
#define LUCKY_DICE_BANK_START     0x080f7000
#define LUCKY_WHEELS_BANK_START   0x080f9000
#define SOUND_BANK_START          0x080fb800
#define SPRITE_BANK_START         0x08187800
#define RESOURCE_DIRECTORY_START  0x08321800
#else
#define FIELD_BANK_START          0x08009000
#define WINDOW_BANK_START         0x08015000
#define GAME_BANK_START           0x08077000
#define FIELD_MODE_BANK_START 0x0808a000
#define MENU_BANK_START           0x080a1000
#define SHOP_BANK_START           0x080b0000
#define BATTLE_BANK_START         0x080b5000
#define BATTLE_EFFECT_BANK_START  0x080c9000
#define CREDITS_BANK_START        0x080f0000
#define TITLE_BANK_START          0x080f2000
#define LUCKY_DICE_BANK_START     0x080f4000
#define LUCKY_WHEELS_BANK_START   0x080f6000
#define SOUND_BANK_START          0x080f9000
#define SPRITE_BANK_START         0x08185000
#define RESOURCE_DIRECTORY_START  0x08320000
#endif

#ifdef OVERLAY
#include "src/system/overlay/overlay.ld"
#else
OUTPUT_ARCH(arm)
ENTRY(syEntry)
DecompressMoveToFrontLzCommandsCodeSize = syResourceExecuteTransfer - syDecompressMoveToFrontLZCommands;
RenderDecodePictureCodeSize = fdTileMapDrawRows - fdRenderDecodePicture;
TileDecompress4bppCodeSize = mnTileExpandMasked - mnTileDecompress4bpp;
TileExpandMaskedCodeSize = mnTileExpandOpaque - mnTileExpandMasked;
TileExpandOpaqueCodeSize = mnWindowSprAcquireFree - mnTileExpandOpaque;
CreditsDecodeBackgroundCodeSize = mnCreditsClearBuffer - mnCreditsDecodeBackground;
fdActorUpdateAllCodeSize = fdDecompressLZ - fdActorUpdateAll;
mnScanlineWindowBuildEdgesCodeSize = mnTileDecompress4bpp - mnScanlineWindowBuildEdges;
fdWorldMapUpdateAffineLinesCodeSize = fdTileConvertMapGrid - fdWorldMapUpdateAffineLines;
mnGlyphDrawCodeSize = mnScanlineWindowBuildEdges - mnGlyphDrawArm;
BitDecoderSize = btDebugParmSetLoad - btBitDecoderDecodeImage;

MEMORY
{
    EWRAM (rwx) : ORIGIN = 0x2000000, LENGTH = 256K
    IWRAM (rwx) : ORIGIN = 0x3000000, LENGTH = 32K
    ROM    (rx) : ORIGIN = 0x8000000, LENGTH = 32M
}

SECTIONS
{
    ewram (NOLOAD) :
    {
        "*/games/tbs/src/system/ewram.o"(.sym_ewram)
    } > EWRAM
    .rom_start :
    {
        "*/games/tbs/src/system/rom_header.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/stubs.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/entry.o"(.text .padding .text.resampler_kernels_1 .padding.resampler_kernels_pad_1 .text.resampler_kernels_2)
        "*/games/tbs/src/system/channel_update_entry.o"(.text .rodata .data .padding)
    } > ROM
    .iwram_resident :
    {
        "*/games/tbs/src/system/irq.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/irq_table.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/iwram_routines.o"(.text .data .text.iwram_divide)
        "*/games/tbs/src/system/resident_mixer.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/resident_mixer_interpolate.o"(.text .data .text.sound_mixer_channel)
        "*/games/tbs/src/system/mixer_render.o"(.text .data .text.iwram_copy_words)
    } > IWRAM AT>ROM
    iwram (NOLOAD) :
    {
        "*/games/tbs/src/system/iwram.o"(.sym_iwram)
    } > IWRAM
    .rom_main :
    {
        "*/games/tbs/src/system/decompress_byte_lz.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/iwram_entry.o"(.text)
        "*/games/tbs/src/system/trig.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/decompress_tagged_lz.o"(.text .rodata .text.decode_2_loop)
        "*/games/tbs/src/system/heap_release.o"(.text .text.* .rodata .data .padding)
        "*/games/tbs/src/system/system.o"(.text)
        "*/games/tbs/src/system/bios.o"(.text .padding .text.sound_vsync_on)
        "*/games/common/sdk/flash/flash.o"(.text)
        "*/games/common/sdk/flash/sst.o"(.text)
        "*/games/common/sdk/flash/program_flash_sector.o"(.text)
        "*/games/common/sdk/flash/atmel.o"(.text)
        "*libgcc.a:_call_via_rX.o"(.text)
        "*/games/tbs/src/system/irq_table.o"(.rodata.irq_head .rodata.irq_middle .text.irq_halt .rodata.irq_tail)
        "*/games/tbs/src/system/math_table.o"(.rodata)
        "*/games/tbs/src/graphics/palettes/palettes.list.o"(.rodata)
        "*/games/tbs/src/system/number_text.o"(.rodata)
        "*/games/tbs/src/system/inner_product.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/bupdata.o"(.rodata)
        "*/games/common/sdk/flash/flashtab.o"(.rodata)
        "*/games/common/sdk/flash/flash.o"(.rodata)
        "*/games/common/sdk/flash/flashsst.o"(.rodata)
        "*/games/common/sdk/flash/sst.o"(.rodata)
        "*/games/common/sdk/flash/flashmx.o"(.rodata)
        "*/games/common/sdk/flash/program_flash_sector.o"(.rodata)
        "*/games/common/sdk/flash/flashat.o"(.rodata)
        "*/games/common/sdk/flash/atmel.o"(.rodata)
        . = ABSOLUTE(ROM_SHIFT + FIELD_BANK_START);
        "*/games/tbs/src/field/stubs.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/sprite_color_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/sprite_color_remap.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/picture_decode.o"(.text .data .text.draw_map)
        "*/games/tbs/src/field/world_map_lines.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/map_grid_convert.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/actor_update_arm.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/actor_update_far.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/field.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/actor_shadow.list.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/sprite_sources.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/facing.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/world_map_resources.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/terrain_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/map_grid_data.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/sprite_viewer_scale.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/actor_script.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/actor_script_operands.o"(.text .rodata .data .padding)
        "*/games/tbs/src/map/resources.o"(.text .rodata .data .padding)
        . = ABSOLUTE(ROM_SHIFT + WINDOW_BANK_START);
        "*/games/tbs/src/window/stubs.o"(.text .rodata .data .padding)
        "*/games/tbs/src/window/message_decode.o"(.text .rodata .data .padding)
        "*/games/tbs/src/window/glyph_arm.o"(.text)
        "*/games/tbs/src/window/decompress_4bpp.o"(.text .rodata .text.decompress_4bpp_loop)
        "*/games/tbs/src/window/expand_opaque.o"(.text .rodata .data .padding)
        "*/games/tbs/src/window/window.o"(.text)
        "*/games/tbs/src/window/screens.o"(.text)
        "*/games/common/graphics/ui/slider.list.o"(.rodata)
        "*/games/tbs/src/graphics/ui/marks.o"(.rodata)
        "*/games/tbs/src/graphics/ui/menu_icons/list.o"(.rodata)
        "*/games/tbs/src/graphics/ui/pointing_hand.o"(.rodata)
        "*/games/tbs/src/graphics/ui/menu_titles.o"(.rodata)
        "*/games/tbs/src/graphics/font/text.o"(.rodata)
        "*/games/tbs/src/window/articles.o"(.rodata)
        "*/games/common/graphics/ui/sprite_cursors.list.o"(.rodata)
        "*/games/tbs/src/window/scales.o"(.rodata)
        "*/games/tbs/src/window/window_tables.o"(.rodata)
        "*/games/tbs/src/window/menu_title_tables.o"(.rodata)
        "*/games/tbs/src/window/shift_prompt.list.o"(.rodata)
        "*/games/tbs/src/window/expand_nibble_table.o"(.rodata)
        "*/games/common/window/djinn_blink.list.o"(.rodata)
        "*/games/tbs/src/window/battle_status_cursors.o"(.rodata)
        "*/games/tbs/src/window/battle_status_strings.o"(.rodata)
        "*/games/tbs/src/window/menu_positions.o"(.rodata)
        "*/games/tbs/src/window/debug_labels.o"(.rodata)
        "*/games/common/text/messages.o"(.rodata)
        /* The item genders: only German and Italian give them entries, and the
           Japanese edition does not build them. */
        "*/games/tbs/src/window/item_genders.o"(.rodata)
        "*/games/tbs/src/window/message_speed.o"(.rodata)
        /* English and German link the counted nouns; only German gives them
           entries. */
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_GERMAN
        "*/games/tbs/src/window/counted_nouns.o"(.rodata)
#endif
        "*/games/common/graphics/ui/slider_palette.list.o"(.rodata)
        "*/games/tbs/src/window/keyboard.o"(.rodata)
        . = ABSOLUTE(ROM_SHIFT + GAME_BANK_START);
        "*/games/tbs/src/game/stubs.o"(.text .padding)
        "*/games/tbs/src/game/game.o"(.text .padding)
        "*/games/tbs/src/game/experience_table.o"(.text .padding)
        "*/games/tbs/src/game/rare_item_stock.o"(.text .padding)
        "*/games/tbs/src/game/starting_equipment.o"(.text .padding)
        "*/games/tbs/src/game/item_table.o"(.text .padding)
        "*/games/tbs/src/game/ability_table.o"(.text .padding)
        "*/games/tbs/src/game/enemy_table.o"(.text .padding)
        "*/games/tbs/src/game/character_table.o"(.text .padding)
        "*/games/tbs/src/game/summon_table.o"(.text .padding)
        "*/games/tbs/src/game/class_table.o"(.text .padding)
        "*/games/tbs/src/game/element_table.o"(.text .padding)
        "*/games/tbs/src/game/djinn_table.o"(.text .padding)
        "*/games/tbs/src/game/stubs.o"(.rodata)
        "*/games/tbs/src/game/game.o"(.rodata)
        "*/games/tbs/src/game/experience_table.o"(.rodata)
        "*/games/tbs/src/game/rare_item_stock.o"(.rodata)
        "*/games/tbs/src/game/starting_equipment.o"(.rodata)
        "*/games/tbs/src/game/item_table.o"(.rodata)
        "*/games/tbs/src/game/ability_table.o"(.rodata)
        "*/games/tbs/src/game/enemy_table.o"(.rodata)
        "*/games/tbs/src/game/character_table.o"(.rodata)
        "*/games/tbs/src/game/summon_table.o"(.rodata)
        "*/games/tbs/src/game/class_table.o"(.rodata)
        "*/games/tbs/src/game/element_table.o"(.rodata)
        "*/games/tbs/src/game/djinn_table.o"(.rodata)
        "*/games/tbs/src/game/stubs.o"(.data .overlays)
        "*/games/tbs/src/game/game.o"(.data .overlays)
        "*/games/tbs/src/game/experience_table.o"(.data .overlays)
        "*/games/tbs/src/game/rare_item_stock.o"(.data .overlays)
        "*/games/tbs/src/game/starting_equipment.o"(.data .overlays)
        "*/games/tbs/src/game/item_table.o"(.data .overlays)
        "*/games/tbs/src/game/ability_table.o"(.data .overlays)
        "*/games/tbs/src/game/enemy_table.o"(.data .overlays)
        "*/games/tbs/src/game/character_table.o"(.data .overlays)
        "*/games/tbs/src/game/summon_table.o"(.data .overlays)
        "*/games/tbs/src/game/class_table.o"(.data .overlays)
        "*/games/tbs/src/game/element_table.o"(.data .overlays)
        "*/games/tbs/src/game/djinn_table.o"(.data .overlays)
        . = ABSOLUTE(ROM_SHIFT + FIELD_MODE_BANK_START);
        "*/games/tbs/src/field/mode_stubs.o"(.text .rodata .data .padding)
        "*/games/tbs/src/game/main_routine.o"(.text)
        "*/games/tbs/src/field/field_mode.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/script_commands.o"(.text .rodata .data .padding)
        "*/games/tbs/src/graphics/field_effects/spark_tiles.list.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/encounter_zones.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/encounter_rules.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/world_map_encounter_rules.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/music_rules.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/area_name_rules.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/restart_rules.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/battle_background_rules.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/battle_music.o"(.text .rodata .data .padding)
        "*/games/tbs/src/graphics/palette_editor/swatch.list.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/field_mode_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/scenes.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/leader_placement.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/screen_transition_shapes.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/movement_scripts.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/field_effect_tiles.list.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/field_effect_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/field/map_screen_markers.o"(.text .rodata .data .padding)
        . = ABSOLUTE(ROM_SHIFT + MENU_BANK_START);
        "*/games/tbs/src/menu/stubs.o"(.text .padding)
        "*/games/tbs/src/menu/menu.o"(.text .padding)
        "*/games/tbs/src/graphics/ui/menu_sprites.list.o"(.text .padding)
        "*/games/tbs/src/menu/menu_tables.o"(.text .padding)
        "*/games/tbs/src/menu/stubs.o"(.rodata)
        "*/games/tbs/src/menu/menu.o"(.rodata)
        "*/games/tbs/src/graphics/ui/menu_sprites.list.o"(.rodata)
        "*/games/tbs/src/menu/menu_strings.o"(.rodata)
        "*/games/tbs/src/menu/menu_tiles.o"(.rodata)
        "*/games/tbs/src/menu/djinn_menu_strings.o"(.rodata)
        "*/games/tbs/src/menu/menu_tables.o"(.rodata)
        "*/games/tbs/src/menu/stubs.o"(.data .overlays)
        "*/games/tbs/src/menu/menu.o"(.data .overlays)
        "*/games/tbs/src/graphics/ui/menu_sprites.list.o"(.data .overlays)
        "*/games/tbs/src/menu/menu_tables.o"(.data .overlays)
        . = ABSOLUTE(ROM_SHIFT + SHOP_BANK_START);
        "*/games/tbs/src/shop/stubs.o"(.text .padding)
        "*/games/tbs/src/graphics/ui/shop_sprites.list.o"(.text .padding)
        "*/games/tbs/src/shop/shop.o"(.text .padding)
        "*/games/tbs/src/shop/stubs.o"(.rodata)
        "*/games/tbs/src/graphics/ui/shop_sprites.list.o"(.rodata)
        "*/games/tbs/src/shop/shop.o"(.rodata)
        "*/games/tbs/src/shop/stubs.o"(.data .overlays)
        "*/games/tbs/src/graphics/ui/shop_sprites.list.o"(.data .overlays)
        "*/games/tbs/src/shop/shop.o"(.data .overlays)
        . = ABSOLUTE(ROM_SHIFT + BATTLE_BANK_START);
        "*/games/tbs/src/battle/stubs.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/background_decode.o"(.text .rodata .text.bit_dispatch)
        "*/games/tbs/src/battle/background_decode.o"(.data .text.bit_commands)
        "*/games/tbs/src/battle/battle.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/battle_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/ability_staging.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/turn_end_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/element_glow_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/unused_portrait_pairs.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/advance_arrow.list.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/debug_parameter_sets.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/battle_unit_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/knockback_tables.o"(.text .rodata .data .padding)
        "*/games/common/battle/transition/curtain.list.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/highlight_tables.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/formations.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle/battle_sprites.o"(.text .rodata .data .padding)
        . = ABSOLUTE(ROM_SHIFT + BATTLE_EFFECT_BANK_START);
        "*/games/tbs/src/battle_effect/stubs.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle_effect/battle_effect.o"(.text .data)
        "*/games/tbs/src/battle_effect/blitter_fragments.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle_effect/battle_effect_table.o"(.data)
        "*/games/tbs/src/battle_effect/blitter_color_fragments.o"(.text .rodata .data .padding)
        "*/games/tbs/src/battle_effect/blitter_operands.o"(.data)
        . = ABSOLUTE(ROM_SHIFT + CREDITS_BANK_START);
        "*/games/tbs/src/menu/credits_stubs.o"(.text .rodata .data .padding)
        "*/games/tbs/src/menu/credits_background_decode.o"(.text .rodata .data .padding)
        "*/games/tbs/src/menu/credits_background_expand.o"(.rodata)
        "*/games/tbs/src/menu/credits_background_expand.o"(.text .data .text.credits_background_expand_commands)
        "*/games/tbs/src/menu/credits.o"(.text)
        "*/games/tbs/src/menu/credits.o"(.rodata)
        "*/games/tbs/src/menu/credits.o"(.data)
        "*/games/tbs/src/graphics/credits/font.list.o"(.rodata)
        . = ABSOLUTE(ROM_SHIFT + TITLE_BANK_START);
        "*/games/tbs/src/menu/title_stubs.o"(.text .padding)
        "*/games/tbs/src/graphics/title/prompt.list.o"(.text .padding)
        "*/games/tbs/src/menu/title_intro.o"(.text .padding)
        "*/games/tbs/src/menu/title_stubs.o"(.rodata)
        "*/games/tbs/src/graphics/title/prompt.list.o"(.rodata)
        "*/games/tbs/src/menu/title_intro.o"(.rodata)
        "*/games/tbs/src/menu/title_stubs.o"(.data .overlays)
        "*/games/tbs/src/graphics/title/prompt.list.o"(.data .overlays)
        "*/games/tbs/src/menu/title_intro.o"(.data .overlays)
        . = ABSOLUTE(ROM_SHIFT + LUCKY_DICE_BANK_START);
        "*/games/tbs/src/field/lucky_dice_stubs.o"(.text .padding)
        "*/games/tbs/src/field/lucky_dice.o"(.text .padding)
        "*/games/tbs/src/field/lucky_dice_stubs.o"(.rodata)
        "*/games/tbs/src/field/lucky_dice.o"(.rodata)
        "*/games/tbs/src/field/lucky_dice_stubs.o"(.data .overlays)
        "*/games/tbs/src/field/lucky_dice.o"(.data .overlays)
        . = ABSOLUTE(ROM_SHIFT + LUCKY_WHEELS_BANK_START);
        "*/games/tbs/src/field/lucky_wheels_stubs.o"(.text .padding)
        "*/games/tbs/src/field/lucky_wheels.o"(.text .padding)
        "*/games/tbs/src/field/lucky_wheels_stubs.o"(.rodata)
        "*/games/tbs/src/field/lucky_wheels.o"(.rodata)
        "*/games/tbs/src/field/lucky_wheels_stubs.o"(.data .overlays)
        "*/games/tbs/src/field/lucky_wheels.o"(.data .overlays)
        . = ABSOLUTE(ROM_SHIFT + SOUND_BANK_START);
        "*/games/tbs/src/sound/stubs.o"(.text .rodata .data .padding)
        "*/games/tbs/src/sound/sound.o"(.text)
        "*/games/common/sdk/sound/umul_high.o"(.text .padding)
    } > ROM
    .iwram_mixer 0x3007000 :
    {
        "*/games/common/sdk/sound/mixer.o"(.text .rodata .data .padding)
    } > IWRAM AT>ROM
    iwram_high (NOLOAD) :
    {
        "*/games/tbs/src/system/iwram_high.o"(.sym_iwram_high)
    } > IWRAM
    .rom_tail :
    {
        "*/games/common/sdk/sound/clear_buffer.o"(.text .text.command_byte_reader .text.command_byte_setters)
        "*/games/common/sdk/sound/sound.o"(.text)
        "*/games/common/sdk/sound/sound_get_jump_list.o"(.text .rodata .data .padding)
        "*/games/common/sdk/sound/driver.o"(.text)
        "*/games/common/sdk/sound/empty_callback.o"(.text)
        "*/games/tbs/src/sound/sound.o"(.rodata)
        "*/games/common/sdk/sound/tables.o"(.rodata)
        "*/games/tbs/sound/voice_groups.o"(.rodata)
        "*/games/common/sound/programmable_wave_samples.o"(.rodata)
        "*/games/tbs/sound/music_player_table.o"(.rodata)
        "*/games/tbs/sound/song_table.o"(.rodata)
        "*/games/tbs/sound/direct_sound_samples.o"(.rodata)
        "*/games/tbs/sound/songs/mus_village_theme.o"(.rodata)
        "*/games/tbs/sound/songs/mus_town_theme.o"(.rodata)
        "*/games/tbs/sound/songs/mus_an_unsettling_feeling.o"(.rodata)
        "*/games/tbs/sound/songs/mus_palace_theme.o"(.rodata)
        "*/games/tbs/sound/songs/mus_sinking_into_sorrow.o"(.rodata)
        "*/games/tbs/sound/songs/mus_temple_theme.o"(.rodata)
        "*/games/tbs/sound/songs/mus_port_town_theme.o"(.rodata)
        "*/games/tbs/sound/songs/mus_trial_road.o"(.rodata)
        "*/games/tbs/sound/songs/mus_colosseum.o"(.rodata)
        "*/games/tbs/sound/songs/mus_try_your_luck.o"(.rodata)
        "*/games/tbs/sound/songs/mus_tolbi.o"(.rodata)
        "*/games/common/sound/songs/mus_silence.o"(.rodata)
        "*/games/tbs/sound/songs/mus_sol_sanctum.o"(.rodata)
        "*/games/tbs/sound/songs/mus_forest_theme.o"(.rodata)
        "*/games/tbs/sound/songs/mus_elemental_stars.o"(.rodata)
        "*/games/tbs/sound/songs/mus_dungeon_theme.o"(.rodata)
        "*/games/tbs/sound/songs/mus_cave_theme.o"(.rodata)
        "*/games/tbs/sound/songs/mus_desert_theme.o"(.rodata)
        "*/games/tbs/sound/songs/mus_remote_village.o"(.rodata)
        "*/games/tbs/sound/songs/mus_mercury_lighthouse.o"(.rodata)
        "*/games/tbs/sound/songs/mus_dungeon.o"(.rodata)
        "*/games/tbs/sound/songs/mus_the_sleeping_treasure_of_altmiller_cave.o"(.rodata)
        "*/games/tbs/sound/songs/mus_babis_theme.o"(.rodata)
        "*/games/tbs/sound/songs/mus_unused.o"(.rodata)
        "*/games/tbs/sound/songs/mus_set_sail_on_the_karagol_sea.o"(.rodata)
        "*/games/tbs/sound/songs/mus_a_long_journey.o"(.rodata)
        "*/games/tbs/sound/songs/mus_venus_lighthouse.o"(.rodata)
        "*/games/tbs/sound/songs/mus_battle.o"(.rodata)
        "*/games/tbs/sound/songs/mus_sworn_enemies_stand_in_your_way.o"(.rodata)
        "*/games/tbs/sound/songs/mus_battling_a_powerful_foe.o"(.rodata)
        "*/games/tbs/sound/songs/mus_battle_with_saturos_and_menardi.o"(.rodata)
        "*/games/tbs/sound/songs/mus_battle_with_the_fusion_dragon.o"(.rodata)
        "*/games/tbs/sound/songs/mus_test_your_skills.o"(.rodata)
        "*/games/tbs/sound/songs/mus_victory.o"(.rodata)
        "*/games/tbs/sound/songs/mus_defeated.o"(.rodata)
        "*/games/tbs/sound/songs/mus_wind_adepts.o"(.rodata)
        "*/games/tbs/sound/songs/mus_chance_meeting_with_sworn_enemies.o"(.rodata)
        "*/games/tbs/sound/songs/mus_sheba_hangs_on.o"(.rodata)
        "*/games/tbs/sound/songs/mus_mount_aleph_erupts.o"(.rodata)
        "*/games/tbs/sound/songs/mus_the_passage_of_time.o"(.rodata)
        "*/games/tbs/sound/songs/mus_the_broken_seal_ending.o"(.rodata)
        "*/games/tbs/sound/songs/mus_main_theme_title.o"(.rodata)
        "*/games/tbs/sound/songs/mus_felixs_resolve.o"(.rodata)
        "*/games/tbs/sound/songs/mus_setting_off.o"(.rodata)
        "*/games/common/sound/songs/mus_to_be_continued.o"(.rodata)
        "*/games/tbs/sound/songs/mus_colosseum_finals.o"(.rodata)
        "*/games/tbs/sound/songs/mus_puzzle_solved.o"(.rodata)
        "*/games/tbs/sound/songs/mus_a_new_ally.o"(.rodata)
        "*/games/tbs/sound/songs/mus_recovering_after_combat.o"(.rodata)
        "*/games/tbs/sound/songs/mus_treasure_found.o"(.rodata)
        "*/games/tbs/sound/songs/mus_poison_cured.o"(.rodata)
        "*/games/tbs/sound/songs/mus_saved.o"(.rodata)
        "*/games/tbs/sound/songs/mus_overnight_at_the_inn.o"(.rodata)
        "*/games/tbs/sound/songs/mus_curse_lifted.o"(.rodata)
        "*/games/tbs/sound/songs/mus_exorcised.o"(.rodata)
        "*/games/tbs/sound/songs/mus_level_up.o"(.rodata)
        "*/games/tbs/sound/songs/mus_lucky.o"(.rodata)
        "*/games/tbs/sound/songs/mus_very_lucky.o"(.rodata)
        "*/games/tbs/sound/songs/mus_super_lucky.o"(.rodata)
        "*/games/common/sound/songs/mus_unused_silence_jingle.o"(.rodata)
        "*/games/common/sound/songs/se_bell.o"(.rodata)
        "*/games/common/sound/songs/se_shop_purchase.o"(.rodata)
        "*/games/common/sound/songs/se_shop_sell.o"(.rodata)
        "*/games/common/sound/songs/se_curse.o"(.rodata)
        "*/games/common/sound/songs/se_launch.o"(.rodata)
        "*/games/common/sound/songs/se_coin_payout.o"(.rodata)
        "*/games/common/sound/songs/se_drip.o"(.rodata)
        "*/games/common/sound/songs/se_tremor.o"(.rodata)
        "*/games/common/sound/songs/se_triple_tone_low.o"(.rodata)
        "*/games/common/sound/songs/se_triple_tone_high.o"(.rodata)
        "*/games/common/sound/songs/se_chime.o"(.rodata)
        "*/games/common/sound/songs/se_menu_cursor_move.o"(.rodata)
        "*/games/common/sound/songs/se_menu_confirm.o"(.rodata)
        "*/games/common/sound/songs/se_menu_cancel.o"(.rodata)
        "*/games/common/sound/songs/se_menu_error.o"(.rodata)
        "*/games/common/sound/songs/se_ice.o"(.rodata)
        "*/games/common/sound/songs/se_tolbi_spring_toss_prompt.o"(.rodata)
        "*/games/common/sound/songs/se_item_remove.o"(.rodata)
        "*/games/common/sound/songs/se_footstep.o"(.rodata)
        "*/games/common/sound/songs/se_unused_footstep_low.o"(.rodata)
        "*/games/common/sound/songs/se_unused_static_crackle.o"(.rodata)
        "*/games/common/sound/songs/se_scuffle.o"(.rodata)
        "*/games/common/sound/songs/se_djinni_flees.o"(.rodata)
        "*/games/common/sound/songs/se_map_exit.o"(.rodata)
        "*/games/common/sound/songs/se_chest_open.o"(.rodata)
        "*/games/common/sound/songs/se_chest_close.o"(.rodata)
        "*/games/common/sound/songs/se_recovery.o"(.rodata)
        "*/games/common/sound/songs/se_landing_thud.o"(.rodata)
        "*/games/common/sound/songs/se_stairs_up.o"(.rodata)
        "*/games/common/sound/songs/se_stairs_down.o"(.rodata)
        "*/games/common/sound/songs/se_psynergy_start.o"(.rodata)
        "*/games/common/sound/songs/se_psynergy_sparkle.o"(.rodata)
        "*/games/common/sound/songs/se_water.o"(.rodata)
        "*/games/common/sound/songs/se_impact.o"(.rodata)
        "*/games/common/sound/songs/se_heavy_impact.o"(.rodata)
        "*/games/common/sound/songs/se_click.o"(.rodata)
        "*/games/common/sound/songs/se_whoosh.o"(.rodata)
        "*/games/tbs/sound/songs/se_kirin.o"(.rodata)
        "*/games/common/sound/songs/se_item_break.o"(.rodata)
        "*/games/tbs/sound/songs/se_djinni_set.o"(.rodata)
        "*/games/common/sound/songs/se_spin.o"(.rodata)
        "*/games/common/sound/songs/se_rumble.o"(.rodata)
        "*/games/common/sound/songs/se_swirl.o"(.rodata)
        "*/games/common/sound/songs/se_crack.o"(.rodata)
        "*/games/common/sound/songs/se_clatter.o"(.rodata)
        "*/games/common/sound/songs/se_crash.o"(.rodata)
        "*/games/common/sound/songs/se_enemy_cry.o"(.rodata)
        "*/games/common/sound/songs/se_beast_cry.o"(.rodata)
        "*/games/common/sound/songs/se_boss_roar.o"(.rodata)
        "*/games/common/sound/songs/se_monster_roar.o"(.rodata)
        "*/games/common/sound/songs/se_unused_defeat_cry.o"(.rodata)
        "*/games/common/sound/songs/se_light_appear.o"(.rodata)
        "*/games/common/sound/songs/se_jump.o"(.rodata)
        "*/games/common/sound/songs/se_high_jump.o"(.rodata)
        "*/games/common/sound/songs/se_cast.o"(.rodata)
        "*/games/common/sound/songs/se_switch.o"(.rodata)
        "*/games/common/sound/songs/se_switch_mechanism.o"(.rodata)
        "*/games/common/sound/songs/se_gate_move.o"(.rodata)
        "*/games/common/sound/songs/se_door_open.o"(.rodata)
        "*/games/common/sound/songs/se_door_close.o"(.rodata)
        "*/games/common/sound/songs/se_unused_noise_swell.o"(.rodata)
        "*/games/common/sound/songs/se_rattle.o"(.rodata)
        "*/games/tbs/sound/songs/se_wave.o"(.rodata)
        "*/games/common/sound/songs/se_rush.o"(.rodata)
        "*/games/common/sound/songs/se_scatter.o"(.rodata)
        "*/games/common/sound/songs/se_bird_wings.o"(.rodata)
        "*/games/common/sound/songs/se_unused_ambient_wind_low.o"(.rodata)
        "*/games/common/sound/songs/se_map_transition.o"(.rodata)
        "*/games/common/sound/songs/se_unlock.o"(.rodata)
        "*/games/tbs/sound/songs/se_ambient_wind.o"(.rodata)
        "*/games/tbs/sound/songs/se_ambient_water.o"(.rodata)
        "*/games/common/sound/songs/se_thunder.o"(.rodata)
        "*/games/common/sound/songs/se_thunder_loud.o"(.rodata)
        "*/games/common/sound/songs/se_note_low.o"(.rodata)
        "*/games/common/sound/songs/se_note_mid.o"(.rodata)
        "*/games/common/sound/songs/se_note_high.o"(.rodata)
        "*/games/common/sound/songs/se_kolima_bridge_hop.o"(.rodata)
        "*/games/common/sound/songs/se_thunder_card.o"(.rodata)
        "*/games/common/sound/songs/se_gathering_power.o"(.rodata)
        "*/games/common/sound/songs/se_wind_funnel.o"(.rodata)
        "*/games/common/sound/songs/se_settle.o"(.rodata)
        "*/games/common/sound/songs/se_boom.o"(.rodata)
        "*/games/common/sound/songs/se_gem_set.o"(.rodata)
        "*/games/common/sound/songs/se_stone_rumble.o"(.rodata)
        "*/games/tbs/sound/songs/se_cart_turn.o"(.rodata)
        "*/games/common/sound/songs/se_push.o"(.rodata)
        "*/games/common/sound/songs/se_sol_sanctum_light_up.o"(.rodata)
        "*/games/common/sound/songs/se_falling_light.o"(.rodata)
        "*/games/common/sound/songs/se_thump.o"(.rodata)
        "*/games/common/sound/songs/se_stone_grind.o"(.rodata)
        "*/games/common/sound/songs/se_warp.o"(.rodata)
        "*/games/common/sound/songs/se_tablet_rise_high.o"(.rodata)
        "*/games/common/sound/songs/se_shriek.o"(.rodata)
        "*/games/common/sound/songs/se_doll_placed.o"(.rodata)
        "*/games/common/sound/songs/se_tablet_rise.o"(.rodata)
        "*/games/common/sound/songs/se_sparks.o"(.rodata)
        "*/games/common/sound/songs/se_wise_one_quest.o"(.rodata)
        "*/games/common/sound/songs/se_gabomba_voice.o"(.rodata)
        "*/games/common/sound/songs/se_kolima_bridge_post.o"(.rodata)
        "*/games/common/sound/songs/se_water_column.o"(.rodata)
        "*/games/common/sound/songs/se_sparkle_fall.o"(.rodata)
        "*/games/common/sound/songs/se_ascend.o"(.rodata)
        "*/games/common/sound/songs/se_flame_flare.o"(.rodata)
        "*/games/common/sound/songs/se_unused_noise_puff.o"(.rodata)
        "*/games/common/sound/songs/se_dust_puff.o"(.rodata)
        "*/games/common/sound/songs/se_unused_ambient_stream_high.o"(.rodata)
        "*/games/common/sound/songs/se_curse_fall.o"(.rodata)
        "*/games/common/sound/songs/se_bane_escape.o"(.rodata)
        "*/games/common/sound/songs/se_mercury_lighthouse_door_open.o"(.rodata)
        "*/games/common/sound/songs/se_hidden_passage_open.o"(.rodata)
        "*/games/common/sound/songs/se_mercury_lighthouse_mechanism.o"(.rodata)
        "*/games/common/sound/songs/se_power_up.o"(.rodata)
        "*/games/common/sound/songs/se_heavy_stop.o"(.rodata)
        "*/games/common/sound/songs/se_rising_chimes.o"(.rodata)
        "*/games/common/sound/songs/se_splash_small.o"(.rodata)
        "*/games/common/sound/songs/se_burst.o"(.rodata)
        "*/games/common/sound/songs/se_floor_switch.o"(.rodata)
        "*/games/common/sound/songs/se_unused_static_burst.o"(.rodata)
        "*/games/common/sound/songs/se_gush.o"(.rodata)
        "*/games/common/sound/songs/se_shimmer.o"(.rodata)
        "*/games/common/sound/songs/se_rise_with_sparks.o"(.rodata)
        "*/games/common/sound/songs/se_whirlwind.o"(.rodata)
        "*/games/common/sound/songs/se_lift.o"(.rodata)
        "*/games/common/sound/songs/se_unused_warble_tone.o"(.rodata)
        "*/games/common/sound/songs/se_cloud_billows.o"(.rodata)
        "*/games/common/sound/songs/se_log_push.o"(.rodata)
        "*/games/common/sound/songs/se_topple.o"(.rodata)
        "*/games/common/sound/songs/se_sparkling_passage.o"(.rodata)
        "*/games/common/sound/songs/se_splash.o"(.rodata)
        "*/games/tbs/sound/songs/se_water_drain.o"(.rodata)
        "*/games/common/sound/songs/se_water_level.o"(.rodata)
        "*/games/common/sound/songs/se_crunch.o"(.rodata)
        "*/games/common/sound/songs/se_cart_roll.o"(.rodata)
        "*/games/common/sound/songs/se_run_back.o"(.rodata)
        "*/games/common/sound/songs/se_gust.o"(.rodata)
        "*/games/common/sound/songs/se_colosso_countdown.o"(.rodata)
        "*/games/common/sound/songs/se_colosso_go.o"(.rodata)
        "*/games/common/sound/songs/se_push_step.o"(.rodata)
        "*/games/common/sound/songs/se_heavy_move.o"(.rodata)
        "*/games/common/sound/songs/se_log_fall.o"(.rodata)
        "*/games/common/sound/songs/se_water_move.o"(.rodata)
        "*/games/common/sound/songs/se_unused_curse_rumble.o"(.rodata)
        "*/games/common/sound/songs/se_lemurian_ship_lift.o"(.rodata)
        "*/games/common/sound/songs/se_sandstorm.o"(.rodata)
        "*/games/tbs/sound/songs/se_ambient_stream.o"(.rodata)
        "*/games/common/sound/songs/se_glitter.o"(.rodata)
        "*/games/common/sound/songs/se_colosso_lettering.o"(.rodata)
        "*/games/common/sound/songs/se_gabomba_statue_arrow.o"(.rodata)
        "*/games/common/sound/songs/se_unused_falling_hiss.o"(.rodata)
        "*/games/common/sound/songs/se_unused_black_crystal_set_low.o"(.rodata)
        "*/games/common/sound/songs/se_black_crystal_set.o"(.rodata)
        "*/games/common/sound/songs/se_colosso_warrior_appear.o"(.rodata)
        "*/games/common/sound/songs/se_unused_colosso_warrior_roar.o"(.rodata)
        "*/games/common/sound/songs/se_voice_animal_a.o"(.rodata)
        "*/games/common/sound/songs/se_voice_animal_b.o"(.rodata)
        "*/games/common/sound/songs/se_voice_animal_c.o"(.rodata)
        "*/games/common/sound/songs/se_voice_animal_d.o"(.rodata)
        "*/games/common/sound/songs/se_voice_girl_a.o"(.rodata)
        "*/games/common/sound/songs/se_voice_girl_b.o"(.rodata)
        "*/games/common/sound/songs/se_voice_girl_c.o"(.rodata)
        "*/games/common/sound/songs/se_voice_girl_d.o"(.rodata)
        "*/games/common/sound/songs/se_voice_boy_a.o"(.rodata)
        "*/games/common/sound/songs/se_voice_boy_b.o"(.rodata)
        "*/games/common/sound/songs/se_voice_boy_c.o"(.rodata)
        "*/games/common/sound/songs/se_voice_boy_d.o"(.rodata)
        "*/games/common/sound/songs/se_voice_young_woman_a.o"(.rodata)
        "*/games/common/sound/songs/se_voice_young_woman_b.o"(.rodata)
        "*/games/common/sound/songs/se_voice_young_woman_c.o"(.rodata)
        "*/games/common/sound/songs/se_voice_young_woman_d.o"(.rodata)
        "*/games/common/sound/songs/se_voice_young_man_a.o"(.rodata)
        "*/games/common/sound/songs/se_voice_young_man_b.o"(.rodata)
        "*/games/common/sound/songs/se_voice_young_man_c.o"(.rodata)
        "*/games/common/sound/songs/se_voice_young_man_d.o"(.rodata)
        "*/games/common/sound/songs/se_voice_woman_a.o"(.rodata)
        "*/games/common/sound/songs/se_voice_woman_b.o"(.rodata)
        "*/games/common/sound/songs/se_voice_woman_c.o"(.rodata)
        "*/games/common/sound/songs/se_voice_woman_d.o"(.rodata)
        "*/games/common/sound/songs/se_voice_old_man_a.o"(.rodata)
        "*/games/common/sound/songs/se_voice_old_man_b.o"(.rodata)
        "*/games/common/sound/songs/se_voice_old_man_c.o"(.rodata)
        "*/games/common/sound/songs/se_voice_old_man_d.o"(.rodata)
        "*/games/common/sound/songs/se_voice_man_a.o"(.rodata)
        "*/games/common/sound/songs/se_voice_man_b.o"(.rodata)
        "*/games/common/sound/songs/se_voice_man_c.o"(.rodata)
        "*/games/common/sound/songs/se_voice_man_d.o"(.rodata)
        "*/games/common/sound/songs/se_stop_action_se.o"(.rodata)
        "*/games/tbs/sound/songs/sound_fade_out_action_se.o"(.rodata)
        "*/games/common/sound/songs/se_unused_take_flight_high.o"(.rodata)
        "*/games/common/sound/songs/se_take_flight.o"(.rodata)
        "*/games/common/sound/songs/se_eruption_columns.o"(.rodata)
        "*/games/common/sound/songs/se_unused_eruption_columns_low.o"(.rodata)
        "*/games/common/sound/songs/se_stop_exit_se.o"(.rodata)
        "*/games/common/sound/songs/se_tolbi_spring_toss.o"(.rodata)
        "*/games/common/sound/songs/se_tolbi_spring_obstacle_hit.o"(.rodata)
        "*/games/common/sound/songs/se_lucky_dice_throw.o"(.rodata)
        "*/games/common/sound/songs/se_lucky_dice_number.o"(.rodata)
        "*/games/common/sound/songs/se_lucky_wheels_spin.o"(.rodata)
        "*/games/common/sound/songs/se_lucky_wheels_hold.o"(.rodata)
        "*/games/common/sound/songs/se_lucky_wheels_stop_prompt.o"(.rodata)
        "*/games/common/sound/songs/se_lucky_wheels_stop.o"(.rodata)
        "*/games/common/sound/songs/se_materialize.o"(.rodata)
        "*/games/common/sound/songs/se_star_bounce.o"(.rodata)
        "*/games/common/sound/songs/se_fall.o"(.rodata)
        "*/games/common/sound/songs/se_unused_sparkling_lettering.o"(.rodata)
        . = ABSOLUTE(ROM_SHIFT + SPRITE_BANK_START);
        "*/games/tbs/src/system/sprite_stubs.o"(.text .rodata .data .padding)
        "*/games/tbs/src/system/sprite_metadata.o"(.text)
        "*/games/tbs/src/graphics/sprite/sprites.o"(.rodata)
        "*/games/tbs/src/graphics/sprite/self_pointers_after_first_block.o"(.rodata)
        "*/games/tbs/src/graphics/sprite/sprites.o"(.rodata.a)
        "*/games/tbs/src/graphics/sprite/self_pointers_after_second_block.o"(.rodata)
        "*/games/tbs/src/graphics/sprite/sprites.o"(.rodata.b)
        . = ABSOLUTE(ROM_SHIFT + RESOURCE_DIRECTORY_START);
        "*/games/tbs/src/system/resource_directory.o"(.text .padding)
        "*/games/tbs/src/graphics/ui/ui.list.o"(.text .padding)
        "*/games/tbs/src/graphics/title/title.list.o"(.text .padding)
        "*/games/tbs/src/graphics/battle_backgrounds/battle_backgrounds.list.o"(.text .padding)
        "*/games/tbs/src/graphics/lucky_wheels/lucky_wheels.list.o"(.text .padding)
        "*/games/tbs/src/graphics/lucky_dice/lucky_dice.list.o"(.text .padding)
        "*/games/tbs/src/graphics/battle_effects/battle_effects.list.o"(.text .padding)
        "*/games/tbs/src/graphics/battle_effects/paths.o"(.text .padding)
        "*/games/tbs/src/map/world_map.list.o"(.text .padding)
        "*/games/tbs/src/graphics/world_map/world_map.list.o"(.text .padding)
        "*/games/tbs/src/map/colosso/colosso.list.o"(.text .padding)
        "*/games/tbs/src/graphics/ui/menu_cursor.list.o"(.text .padding)
        "*/games/tbs/src/graphics/ui/icons.list.o"(.text .padding)
        "*/games/tbs/src/graphics/monster/monster.list.o"(.text .padding)
        "*/games/tbs/src/map/maps.o"(.text .padding)
        "*/games/tbs/src/system/overlay/overlays.list.o"(.text .padding)
        "*/games/tbs/src/system/resource_directory.o"(.rodata)
        "*/games/tbs/src/graphics/ui/ui.list.o"(.rodata)
        "*/games/tbs/src/graphics/title/title.list.o"(.rodata)
        "*/games/tbs/src/graphics/battle_backgrounds/battle_backgrounds.list.o"(.rodata)
        "*/games/tbs/src/graphics/lucky_wheels/lucky_wheels.list.o"(.rodata)
        "*/games/tbs/src/graphics/lucky_dice/lucky_dice.list.o"(.rodata)
        "*/games/tbs/src/graphics/battle_effects/battle_effects.list.o"(.rodata)
        "*/games/tbs/src/graphics/battle_effects/paths.o"(.rodata)
        "*/games/tbs/src/map/world_map.list.o"(.rodata)
        "*/games/tbs/src/graphics/world_map/world_map.list.o"(.rodata)
        "*/games/tbs/src/map/colosso/colosso.list.o"(.rodata)
        "*/games/tbs/src/graphics/ui/menu_cursor.list.o"(.rodata)
        "*/games/tbs/src/graphics/ui/icons.list.o"(.rodata)
        "*/games/tbs/src/graphics/monster/monster.list.o"(.rodata)
        "*/games/tbs/src/map/maps.o"(.rodata)
        "*/games/tbs/src/system/overlay/overlays.list.o"(.rodata)
        "*/games/tbs/src/system/resource_directory.o"(.data .overlays)
        "*/games/tbs/src/graphics/ui/ui.list.o"(.data .overlays)
        "*/games/tbs/src/graphics/title/title.list.o"(.data .overlays)
        "*/games/tbs/src/graphics/battle_backgrounds/battle_backgrounds.list.o"(.data .overlays)
        "*/games/tbs/src/graphics/lucky_wheels/lucky_wheels.list.o"(.data .overlays)
        "*/games/tbs/src/graphics/lucky_dice/lucky_dice.list.o"(.data .overlays)
        "*/games/tbs/src/graphics/battle_effects/battle_effects.list.o"(.data .overlays)
        "*/games/tbs/src/graphics/battle_effects/paths.o"(.data .overlays)
        "*/games/tbs/src/map/world_map.list.o"(.data .overlays)
        "*/games/tbs/src/graphics/world_map/world_map.list.o"(.data .overlays)
        "*/games/tbs/src/map/colosso/colosso.list.o"(.data .overlays)
        "*/games/tbs/src/graphics/ui/menu_cursor.list.o"(.data .overlays)
        "*/games/tbs/src/graphics/ui/icons.list.o"(.data .overlays)
        "*/games/tbs/src/graphics/monster/monster.list.o"(.data .overlays)
        "*/games/tbs/src/map/maps.o"(.data .overlays)
        "*/games/tbs/src/system/overlay/overlays.list.o"(.data .overlays)
    } > ROM
    IwramResidentRom = LOADADDR(.iwram_resident);
    sySndMainRAMRom = LOADADDR(.iwram_mixer) + 1;
    MixerCallViaR3Rom = LOADADDR(.iwram_mixer) + (MixerCallViaR3 - ADDR(.iwram_mixer));
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
#endif
